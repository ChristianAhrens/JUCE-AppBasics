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

namespace JUCEAppBasics
{
namespace iOS_utils
{

SafetyMargin getNativeSafeAreaInsets()
{
    SafetyMargin result{};

    // Deployment target is iOS 15+, so UIWindowScene API is always available.
    // Iterate connected scenes to find the foreground-active window — this is
    // the correct path for Stage Manager, Split View, and Slide Over, where
    // each app window belongs to a distinct UIWindowScene.
    UIWindow* keyWindow = nil;
    for (UIScene* scene in [UIApplication sharedApplication].connectedScenes)
    {
        if ([scene isKindOfClass:[UIWindowScene class]]
            && scene.activationState == UISceneActivationStateForegroundActive)
        {
            UIWindowScene* windowScene = (UIWindowScene*)scene;
            for (UIWindow* window in windowScene.windows)
            {
                if (window.isKeyWindow) { keyWindow = window; break; }
            }
            // Accept first window of the active scene if no key window yet
            // (can happen briefly during startup).
            if (!keyWindow)
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

    // UIApplicationDidBecomeActiveNotification fires after UIKit's first layout
    // pass on launch (when safeAreaInsets become valid) and whenever the app
    // returns to the foreground. Stage Manager window resizes are handled by
    // JUCE's own resized() call when the component bounds change.
    [[NSNotificationCenter defaultCenter]
        addObserver:g_safeAreaObserver
           selector:@selector(safeAreaDidChange:)
               name:UIApplicationDidBecomeActiveNotification
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

} // namespace iOS_utils
} // namespace JUCEAppBasics

#endif // TARGET_OS_IPHONE
