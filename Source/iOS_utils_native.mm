/* Copyright (c) 2026, Christian Ahrens
 *
 * This file is part of JUCE-AppBasics <https://github.com/ChristianAhrens/JUCE-AppBasics>
 *
 * This module is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This tool is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this tool; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
// TARGET_OS_IPHONE is an Apple SDK builtin defined before any includes,
// unlike JUCE_IOS which is only available after JuceHeader.h is included.
#import <TargetConditionals.h>
#if TARGET_OS_IPHONE

#import <UIKit/UIKit.h>
#include "iOS_utils.h"
#include <cmath>
#include <functional>

// ---------------------------------------------------------------------------
// ObjC observer — bridges UIKit notifications into the C++ callback.
// ---------------------------------------------------------------------------
@interface JUCEAppBasics_SafeAreaObserver : NSObject
@property (nonatomic, copy) void (^callback)();
- (void)safeAreaDidChange:(NSNotification*)note;
@end

@implementation JUCEAppBasics_SafeAreaObserver
- (void)safeAreaDidChange:(NSNotification*)note
{
    if (self.callback)
        self.callback();
}
@end

// Singleton observer instance, kept alive for the app's lifetime once set.
static JUCEAppBasics_SafeAreaObserver* g_safeAreaObserver = nil;

// ---------------------------------------------------------------------------
// ObjC pinch handler — wraps UIPinchGestureRecognizer with an incremental callback.
// ---------------------------------------------------------------------------
@interface JUCEAppBasics_PinchHandler : NSObject
@property (nonatomic, copy)   void (^callback)(float, float, float);
@property (nonatomic, assign) CGFloat lastScale;
@property (nonatomic, strong) UIPinchGestureRecognizer* recognizer;
- (void)pinchAction:(UIPinchGestureRecognizer*)gesture;
@end

@implementation JUCEAppBasics_PinchHandler
- (void)pinchAction:(UIPinchGestureRecognizer*)gesture
{
    if (gesture.state == UIGestureRecognizerStateBegan)
    {
        self.lastScale = 1.0f;
    }
    else if (gesture.state == UIGestureRecognizerStateChanged)
    {
        CGFloat currentScale = gesture.scale;
        if (self.lastScale > 0.0f)
        {
            float incrementalScale = (float)(currentScale / self.lastScale);
            self.lastScale = currentScale;
            CGPoint centre = [gesture locationInView:gesture.view];
            if (self.callback)
                self.callback(incrementalScale, (float)centre.x, (float)centre.y);
        }
    }
}
@end

// Map from UIView* (weak, opaque key) to pinch handler (strong value).
// Weak keys: when a UIView is deallocated the slot is zeroed, so objectForKey: on a
// dying view returns nil without sending any ObjC message to the view.
// OpaquePersonality: uses raw-pointer hash/equality — no [key hash] or [key isEqual:]
// calls, so it is safe to query with a dangling pointer during teardown.
static NSMapTable<UIView*, JUCEAppBasics_PinchHandler*>* g_pinchHandlers = nil;

// ---------------------------------------------------------------------------

namespace JUCEAppBasics
{
namespace iOS_utils
{

SafetyMargin getNativeSafeAreaInsets()
{
    SafetyMargin result{};

    // Deployment target is iOS 15+, so UIWindowScene API is always available.
    // Iterate connected scenes to find the foreground-active one — this is the correct path
    // for Stage Manager, Split View, and Slide Over, where each app window belongs to a
    // distinct UIWindowScene.
    // Use the first (lowest z-order) window in the active scene — this is always the main
    // application window.  Do NOT use isKeyWindow: JUCE popup/overlay windows temporarily
    // become the key window and carry different (often zero) safe area insets, which would
    // corrupt the main layout while a dialog is open.
    UIWindow* keyWindow = nil;
    for (UIScene* scene in [UIApplication sharedApplication].connectedScenes)
    {
        if ([scene isKindOfClass:[UIWindowScene class]]
            && scene.activationState == UISceneActivationStateForegroundActive)
        {
            UIWindowScene* windowScene = (UIWindowScene*)scene;
            keyWindow = windowScene.windows.firstObject;
            if (keyWindow)
                break;
        }
    }

    if (keyWindow)
    {
        // safeAreaInsets are in logical points, which matches JUCE's
        // coordinate space on iOS (JUCE renders at 1 pt = 1 JUCE unit).
        // Values already account for current device orientation.
        UIEdgeInsets insets = keyWindow.safeAreaInsets;
        result._top    = (int)std::ceil(insets.top);
        result._bottom = (int)std::ceil(insets.bottom);
        result._left   = (int)std::ceil(insets.left);
        result._right  = (int)std::ceil(insets.right);
    }

    return result;
}

void registerSafeAreaChangeCallback(std::function<void()> callback)
{
    // Remove any previous observer before replacing it.
    unregisterSafeAreaChangeCallback();

    g_safeAreaObserver = [[JUCEAppBasics_SafeAreaObserver alloc] init];
    g_safeAreaObserver.callback = ^{ callback(); };

    NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];

    // UIApplicationDidBecomeActiveNotification: fires on launch completion and whenever the
    // app returns from the background — ensures insets are valid after UIKit's first layout pass.
    [nc addObserver:g_safeAreaObserver
           selector:@selector(safeAreaDidChange:)
               name:UIApplicationDidBecomeActiveNotification
             object:nil];

    // UIWindowDidBecomeKeyNotification: fires whenever any window becomes key, including when
    // the main window regains key status after a JUCE popup/overlay is dismissed.  This causes
    // an immediate layout update rather than waiting for the next app-active event.
    [nc addObserver:g_safeAreaObserver
           selector:@selector(safeAreaDidChange:)
               name:UIWindowDidBecomeKeyNotification
             object:nil];
}

void unregisterSafeAreaChangeCallback()
{
    if (g_safeAreaObserver)
    {
        [[NSNotificationCenter defaultCenter] removeObserver:g_safeAreaObserver];
        g_safeAreaObserver = nil;
    }
}

void registerNativePinchOnView(void* nativeViewHandle, std::function<void(float, float, float)> callback)
{
    UIView* view = (__bridge UIView*)nativeViewHandle;
    if (!view)
        return;

    if (!g_pinchHandlers)
    {
        NSPointerFunctions* keyFns = [NSPointerFunctions pointerFunctionsWithOptions:
            NSPointerFunctionsWeakMemory | NSPointerFunctionsOpaquePersonality];
        NSPointerFunctions* valFns = [NSPointerFunctions pointerFunctionsWithOptions:
            NSPointerFunctionsStrongMemory | NSPointerFunctionsObjectPersonality];
        g_pinchHandlers = [[NSMapTable alloc] initWithKeyPointerFunctions:keyFns
                                                    valuePointerFunctions:valFns
                                                                 capacity:4];
    }

    // Remove any existing handler for this view before adding a new one.
    unregisterNativePinchOnView(nativeViewHandle);

    auto* handler      = [[JUCEAppBasics_PinchHandler alloc] init];
    handler.lastScale  = 1.0f;
    handler.callback   = ^(float scale, float cx, float cy) { callback(scale, cx, cy); };

    auto* recognizer   = [[UIPinchGestureRecognizer alloc]
                              initWithTarget:handler
                                      action:@selector(pinchAction:)];
    handler.recognizer = recognizer;

    [g_pinchHandlers setObject:handler forKey:view];
    [view addGestureRecognizer:recognizer];
}

void unregisterNativePinchOnView(void* nativeViewHandle)
{
    UIView* view = (__bridge UIView*)nativeViewHandle;
    if (!view || !g_pinchHandlers)
        return;

    // objectForKey: uses raw-pointer equality (OpaquePersonality), so it is safe even
    // if view is a dangling pointer: the stored weak slot will have been zeroed to nil
    // when the UIView was deallocated, and nil != dangling_ptr → returns nil.
    JUCEAppBasics_PinchHandler* handler = [g_pinchHandlers objectForKey:view];
    if (!handler)
        return;

    [view removeGestureRecognizer:handler.recognizer];
    [g_pinchHandlers removeObjectForKey:view];
}

} // namespace iOS_utils
} // namespace JUCEAppBasics

#endif // TARGET_OS_IPHONE
