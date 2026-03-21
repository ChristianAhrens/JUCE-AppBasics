/* Copyright (c) 2020-2026, Christian Ahrens
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
#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{

/**
 * @namespace JUCEAppBasics::iOS_utils
 * @brief iOS/iPadOS platform utilities for JUCE applications.
 *
 * Provides two groups of functionality, both compiled only for iOS/iPadOS targets
 * (guarded by `#if JUCE_IOS` or `#if TARGET_OS_IPHONE` as appropriate).
 *
 * ### Safe-area insets
 * Queries UIKit for the *current* safe-area insets (status bar, home indicator,
 * Dynamic Island, notch, Stage Manager windows, split-screen) and notifies the
 * application whenever they may have changed.
 *
 * Typical usage in a JUCE application:
 * ```cpp
 * // In MainComponent constructor or initialise():
 * JUCEAppBasics::iOS_utils::initialise([this] { resized(); });
 *
 * // In MainComponent destructor or shutdown():
 * JUCEAppBasics::iOS_utils::deinitialise();
 *
 * // In MainComponent::resized():
 * auto safety = JUCEAppBasics::iOS_utils::getDeviceSafetyMargins();
 * auto usable = getLocalBounds().withTrimmedTop(safety._top)
 *                               .withTrimmedBottom(safety._bottom);
 * ```
 *
 * On non-iOS platforms `initialise()` and `deinitialise()` are no-ops and
 * `getDeviceSafetyMargins()` falls back to the static device-model lookup table.
 *
 * ### Native pinch gesture
 * Attaches / detaches a `UIPinchGestureRecognizer` to any UIKit view identified by
 * a raw `void*` handle (obtained from `Component::getPeer()->getNativeHandle()`).
 * The recognizer fires an incremental-scale callback on each gesture update.
 *
 * This is required in JUCE 8 because the iOS peer routes each finger touch to
 * whichever JUCE component passes `hitTest()` at that touch position.  Both fingers
 * of a pinch gesture therefore rarely land on the same JUCE component, so
 * `mouseMagnify()` is never called.  A `UIPinchGestureRecognizer` operates at the
 * UIKit gesture layer, before JUCE's per-component routing, and fires regardless of
 * which components the individual fingers hit.
 *
 * Typical usage (see `UmsciControlComponent::parentHierarchyChanged()`):
 * ```cpp
 * JUCEAppBasics::iOS_utils::registerNativePinchOnView(
 *     getPeer()->getNativeHandle(),
 *     [this](float scale, float cx, float cy) {
 *         auto local = myComponent->getLocalPoint(nullptr, { cx, cy });
 *         myComponent->simulatePinchZoom(scale, local);
 *     });
 * ```
 *
 * ### Implementation files
 * - Platform-independent helpers (device model table, safety margins):
 *   `iOS_utils.cpp`
 * - iOS-only ObjC++ code (UIKit API calls, ObjC observers, gesture recognizers):
 *   `iOS_utils_native.mm`
 */
namespace iOS_utils
{

struct SafetyMargin
{
    int _top;
    int _bottom;
    int _left;
    int _right;
};

enum KnownDevices
{
    InvalidDevice,
    GenericMac,
    GenericPC,
    GenericAndroidPhone,
    GenericiPhone,
    iPhone,
    iPhone3G,
    iPhone3GS,
    iPhone4,
    iPhone4S,
    iPhone5,
    iPhone5C,
    iPhone5S,
    iPhone6,
    iPhone6Plus,
    iPhone6S,
    iPhone6SPlus,
    iPhoneSE,
    iPhoneSE2,
    iPhone7,
    iPhone7Plus,
    iPhone8,
    iPhone8Plus,
    iPhoneX,
    iPhoneXR,
    iPhoneXS,
    iPhoneXSMax,
    iPhone11,
    iPhone11Pro,
    iPhone11ProMax,
    iPhone12,
    iPhone12Mini,
    iPhone12Pro,
    iPhone12ProMax,
    iPhone13,
    iPhone13Mini,
    iPhone13Pro,
    iPhone13ProMax,
    iPhoneSE3,
    iPhone14,
    iPhone14Pro,
    iPhone14ProMax,
    iPhone14Plus,
    iPhone15,
    iPhone15Pro,
    iPhone15ProMax,
    iPhone15Plus,
    iPhone16Pro,
    iPhone16ProMax,
    iPhone16,
    iPhone16Plus,
    iPhone16e,
    iPhone17Pro,
    iPhone17ProMax,
    iPhone17,
    iPhoneAir,
    iPhoneSim_i386,
    iPhoneSim_x86_64,
    GenericiPad,
    iPad,
    iPad3G,
    iPad2,
    iPad3,
    iPad4,
    iPadAir,
    iPadAir2,
    iPad5,
    iPad6,
    iPad7,
    iPad10,
    iPadAir3,
    iPadPro,
    iPadPro2,
    iPadPro3,
    iPadPro4,
    iPadPro970,
    iPadPro1050,
    iPadPro1120,
    iPadPro1290,
    iPad1027,
    iPadPro1101,
    iPadPro1293,
    iPadPro1102,
    iPadPro1104,
    iPadPro1294,
    iPadPro1296,
    iPadAir811,
    iPadAir813,
    iPad11,
    iPadmini,
    iPadmini2,
    iPadmini3,
    iPadmini4,
    iPadmini5,
    iPadmini6,
    iPad2017,
    iPadAir4,
    iPadAir5,
    iPod1,
    iPod2,
    iPod3,
    iPod4,
    iPod5,
    iPod6,
    iPod7,
    GenericiPod,
    AppleWatch,
    AppleWatch1,
    AppleWatch2,
    AppleWatch3,
    AppleWatch4,
    AppleWatch5,
    AppleWatchSE,
    AppleWatch6,
    AppleWatch7,
    GenericAppleWatch,

};

JUCEAppBasics::iOS_utils::KnownDevices getDeviceTypeIPhone(const juce::String& deviceDescription);
JUCEAppBasics::iOS_utils::KnownDevices getDeviceTypeIPad(const juce::String& deviceDescription);
JUCEAppBasics::iOS_utils::KnownDevices getDeviceTypeMac(const juce::String& deviceDescription);
JUCEAppBasics::iOS_utils::KnownDevices getDeviceTypeIPod(const juce::String& deviceDescription);
JUCEAppBasics::iOS_utils::KnownDevices getDeviceTypeAppleWatch(const juce::String& deviceDescription);
JUCEAppBasics::iOS_utils::KnownDevices getDeviceTypeOther(const juce::String& deviceDescription);
JUCEAppBasics::iOS_utils::KnownDevices getDeviceType();
JUCEAppBasics::iOS_utils::KnownDevices getGenericDeviceType();
int getDeviceDisplayNotchIndent();
int getDeviceDisplaySlideBarIndent();
JUCEAppBasics::iOS_utils::SafetyMargin getDeviceSafetyMargins();

/** Initialises platform-specific safe-area monitoring.
 *  On iOS/iPadOS, registers @p safeAreaChangeCallback to be invoked
 *  whenever safe area insets may have changed (app becomes active,
 *  scene activates, Stage Manager / split-screen geometry updates).
 *  Intended use: call resized() on the root component so the layout
 *  picks up the correct insets after UIKit's first layout pass.
 *  On all other platforms this is a no-op. */
void initialise(std::function<void()> safeAreaChangeCallback);

/** Tears down any resources allocated by initialise(). No-op on non-iOS. */
void deinitialise();

#if JUCE_IOS
/** Queries UIWindow.safeAreaInsets via UIKit.
 *  Returns exact OS-reported margins in points for the current window,
 *  handling notch, Dynamic Island, home indicator, iPadOS status bar,
 *  Stage Manager, split-screen, and all future hardware automatically.
 *  Implemented in iOS_utils_native.mm. */
JUCEAppBasics::iOS_utils::SafetyMargin getNativeSafeAreaInsets();

/** iOS-internal: used by initialise(). Prefer initialise() in application code. */
void registerSafeAreaChangeCallback(std::function<void()> callback);
/** iOS-internal: used by deinitialise(). Prefer deinitialise() in application code. */
void unregisterSafeAreaChangeCallback();

/** Attaches a native UIPinchGestureRecognizer to the UIView identified by @p nativeViewHandle.
 *  On each gesture update, @p callback is invoked with (incrementalScale, centreX, centreY)
 *  where incrementalScale is the scale change since the previous callback (not the total
 *  gesture scale), and centreX/centreY are in the UIView's coordinate space (logical points,
 *  same as JUCE component pixel coordinates).
 *  Implemented in iOS_utils_native.mm. */
void registerNativePinchOnView(void* nativeViewHandle, std::function<void(float, float, float)> callback);

/** Removes the UIPinchGestureRecognizer previously attached by registerNativePinchOnView().
 *  Implemented in iOS_utils_native.mm. */
void unregisterNativePinchOnView(void* nativeViewHandle);
#endif
     
}; // namespace iOS_utils
}; // namespace JUCE-AppBasics
