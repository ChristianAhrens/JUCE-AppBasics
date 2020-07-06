#pragma once

#include <JuceHeader.h>

namespace JUCEAppBasics
{
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
    iPhone5c,
    iPhone5s,
    iPhone6,
    iPhone6Plus,
    iPhone6s,
    iPhone6sPlus,
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
    GenericiPad,
    iPad,
    iPad2,
    iPad3,
    iPad4,
    iPadAir,
    iPadAir2,
    iPad5,
    iPad6,
    iPadAir3,
    iPad7,
    iPadPro,
    iPadPro2,
    iPadPro3,
    iPadPro4,
    iPadmini,
    iPadmini2,
    iPadmini3,
    iPadmini4,
    iPadmini5,

};

static KnownDevices getDeviceType()
{
    auto deviceDescription = SystemStats::getDeviceDescription();

    if (deviceDescription == "iPhone")
        return KnownDevices::iPhone;
    if (deviceDescription == "iPhone 3G")
        return KnownDevices::iPhone3G;
    if (deviceDescription == "iPhone 3GS")
        return KnownDevices::iPhone3GS;
    if (deviceDescription == "iPhone 4")
        return KnownDevices::iPhone4;
    if (deviceDescription == "iPhone 4S")
        return KnownDevices::iPhone4S;
    if (deviceDescription == "iPhone 5")
        return KnownDevices::iPhone5;
    if (deviceDescription == "iPhone 5c")
        return KnownDevices::iPhone5c;
    if (deviceDescription == "iPhone 5s")
        return KnownDevices::iPhone5s;
    if (deviceDescription == "iPhone 6")
        return KnownDevices::iPhone6;
    if (deviceDescription == "iPhone 6 Plus")
        return KnownDevices::iPhone6Plus;
    if (deviceDescription == "iPhone 6s")
        return KnownDevices::iPhone6s;
    if (deviceDescription == "iPhone 6s Plus")
        return KnownDevices::iPhone6sPlus;
    if (deviceDescription == "iPhone SE")
        return KnownDevices::iPhoneSE;
    if (deviceDescription == "iPhone SE (2nd generation)")
        return KnownDevices::iPhoneSE2;
    if (deviceDescription == "iPhone8,4")
        return KnownDevices::iPhoneSE;
    if (deviceDescription == "iPhone 7")
        return KnownDevices::iPhone7;
    if (deviceDescription == "iPhone 7 Plus")
        return KnownDevices::iPhone7Plus;
    if (deviceDescription == "iPhone 8")
        return KnownDevices::iPhone8;
    if (deviceDescription == "iPhone 8 Plus")
        return KnownDevices::iPhone8Plus;
    if (deviceDescription == "iPhone X")
        return KnownDevices::iPhoneX;
    if (deviceDescription == "iPhone XR")
        return KnownDevices::iPhoneXR;
    if (deviceDescription == "iPhone XS")
        return KnownDevices::iPhoneXS;
    if (deviceDescription == "iPhone XS Max")
        return KnownDevices::iPhoneXSMax;
    if (deviceDescription == "iPhone 11")
        return KnownDevices::iPhone11;
    if (deviceDescription == "iPhone 11 Pro")
        return KnownDevices::iPhone11Pro;
    if (deviceDescription == "iPhone 11 Pro Max")
        return KnownDevices::iPhone11ProMax;
    if (deviceDescription.contains("iPhone"))
        return KnownDevices::GenericiPhone;
    if (deviceDescription == "iPad")
        return KnownDevices::iPad;
    if (deviceDescription == "iPad 2")
        return KnownDevices::iPad2;
    if (deviceDescription == "iPad 3")
        return KnownDevices::iPad3;
    if (deviceDescription == "iPad 4")
        return KnownDevices::iPad4;
    if (deviceDescription == "iPad Air")
        return KnownDevices::iPadAir;
    if (deviceDescription == "iPad Air 2")
        return KnownDevices::iPadAir2;
    if (deviceDescription == "iPad 5")
        return KnownDevices::iPad5;
    if (deviceDescription == "iPad 6")
        return KnownDevices::iPad6;
    if (deviceDescription == "iPad Air 3")
        return KnownDevices::iPadAir3;
    if (deviceDescription == "iPad 7")
        return KnownDevices::iPad7;
    if (deviceDescription == "iPad Pro")
        return KnownDevices::iPadPro;
    if (deviceDescription == "iPad Pro 2")
        return KnownDevices::iPadPro2;
    if (deviceDescription == "iPad Pro 3")
        return KnownDevices::iPadPro3;
    if (deviceDescription == "iPad Pro 4")
        return KnownDevices::iPadPro4;
    if (deviceDescription == "iPad mini")
        return KnownDevices::iPadmini;
    if (deviceDescription == "iPad mini 2")
        return KnownDevices::iPadmini2;
    if (deviceDescription == "iPad mini 3")
        return KnownDevices::iPadmini3;
    if (deviceDescription == "iPad mini 4")
        return KnownDevices::iPadmini4;
    if (deviceDescription == "iPad mini 5")
        return KnownDevices::iPadmini5;
    if (deviceDescription.contains("iPad"))
        return KnownDevices::GenericiPad;
    if (deviceDescription.contains("Mac"))
        return KnownDevices::GenericMac;

    return InvalidDevice;
};

static KnownDevices getGenericDeviceType()
{
    switch (getDeviceType())
    {
    case GenericMac:
        return GenericMac;
    case GenericPC:
        return GenericPC;
    case GenericAndroidPhone:
        return GenericAndroidPhone;
    case GenericiPhone:
    case iPhone:
    case iPhone3G:
    case iPhone3GS:
    case iPhone4:
    case iPhone4S:
    case iPhone5:
    case iPhone5c:
    case iPhone5s:
    case iPhone6:
    case iPhone6Plus:
    case iPhone6s:
    case iPhone6sPlus:
    case iPhoneSE:
    case iPhone7:
    case iPhone7Plus:
    case iPhone8:
    case iPhone8Plus:
    case iPhoneX:
    case iPhoneXR:
    case iPhoneXS:
    case iPhoneXSMax:
    case iPhone11:
    case iPhone11Pro:
    case iPhone11ProMax:
        return GenericiPhone;
    case GenericiPad:
    case iPad:
    case iPad2:
    case iPad3:
    case iPad4:
    case iPadAir:
    case iPadAir2:
    case iPad5:
    case iPad6:
    case iPadAir3:
    case iPad7:
    case iPadPro:
    case iPadPro2:
    case iPadPro3:
    case iPadPro4:
    case iPadmini:
    case iPadmini2:
    case iPadmini3:
    case iPadmini4:
    case iPadmini5:
        return GenericiPad;
    default:
        return InvalidDevice;
    }
};

static int const getDeviceDisplayNotchIndent()
{
    int displayNotchIndent{};
    switch (getDeviceType())
    {
    case iPhone:
    case iPhone3G:
    case iPhone3GS:
    case iPhone4:
    case iPhone4S:
    case iPhone5:
    case iPhone5c:
    case iPhone5s:
    case iPhone6:
    case iPhone6Plus:
    case iPhone6s:
    case iPhone6sPlus:
    case iPhoneSE:
    case iPhoneSE2:
    case iPhone7:
    case iPhone7Plus:
    case iPhone8:
    case iPhone8Plus:
        displayNotchIndent = 0;
        break;
    case iPhoneX:
    case iPhoneXR:
    case iPhoneXS:
    case iPhoneXSMax:
    case iPhone11:
    case iPhone11Pro:
    case iPhone11ProMax:
    case GenericiPhone:
        displayNotchIndent = 35;
        break;
    case iPad:
    case iPad2:
    case iPad3:
    case iPad4:
    case iPadAir:
    case iPadAir2:
    case iPad5:
    case iPad6:
    case iPadAir3:
    case iPad7:
    case iPadPro:
    case iPadPro2:
    case iPadPro3:
    case iPadPro4:
    case iPadmini:
    case iPadmini2:
    case iPadmini3:
    case iPadmini4:
    case iPadmini5:
    case GenericiPad:
    case GenericMac:
    case GenericPC:
    case GenericAndroidPhone:
    case InvalidDevice:
    default:
        displayNotchIndent = 0;
        break;
    };

    return displayNotchIndent;
};

static int const getDeviceDisplaySlideBarIndent()
{
    int slideBarIndent{};
    switch (getDeviceType())
    {
    case iPhone:
    case iPhone3G:
    case iPhone3GS:
    case iPhone4:
    case iPhone4S:
    case iPhone5:
    case iPhone5c:
    case iPhone5s:
    case iPhone6:
    case iPhone6Plus:
    case iPhone6s:
    case iPhone6sPlus:
    case iPhoneSE:
    case iPhoneSE2:
    case iPhone7:
    case iPhone7Plus:
    case iPhone8:
    case iPhone8Plus:
        slideBarIndent = 0;
        break;
    case iPhoneX:
    case iPhoneXR:
    case iPhoneXS:
    case iPhoneXSMax:
    case iPhone11:
    case iPhone11Pro:
    case iPhone11ProMax:
        slideBarIndent = 20;
        break;
    case iPad:
    case iPad2:
    case iPad3:
    case iPad4:
    case iPadAir:
    case iPadAir2:
    case iPad5:
    case iPad6:
    case iPadAir3:
    case iPad7:
    case iPadPro:
    case iPadPro2:
    case iPadPro3:
    case iPadPro4:
    case iPadmini:
    case iPadmini2:
    case iPadmini3:
    case iPadmini4:
    case iPadmini5:
    case GenericiPad:
    case GenericMac:
    case GenericPC:
    case GenericAndroidPhone:
    case InvalidDevice:
    default:
        slideBarIndent = 0;
        break;
    };

    return slideBarIndent;
};

static SafetyMargin const getDeviceSafetyMargins()
{
    SafetyMargin safety{};
    auto displayNotch = getDeviceDisplayNotchIndent();
    auto displaySlideBar = getDeviceDisplaySlideBarIndent();
    switch (SystemStats::getOperatingSystemType())
    {
    case SystemStats::OperatingSystemType::Android:
    case SystemStats::OperatingSystemType::iOS:
    {
        auto orientation = Desktop::getInstance().getCurrentOrientation();
        switch (orientation)
        {
        case Desktop::upright:
            safety._top = displayNotch;
            safety._bottom = displaySlideBar;
            break;
        case Desktop::upsideDown:
            safety._top = displaySlideBar;
            safety._bottom = displayNotch;
            break;
        case Desktop::rotatedAntiClockwise:
            safety._left = displayNotch;
            safety._right = displaySlideBar;
            break;
        case Desktop::rotatedClockwise:
            safety._left = displaySlideBar;
            safety._right = displayNotch;
            break;
        default:
            break;
        }
    }
    break;
    case SystemStats::OperatingSystemType::MacOSX:
    case SystemStats::OperatingSystemType::Windows:
    case SystemStats::OperatingSystemType::Linux:
    default:
        safety._top = displayNotch;
        safety._bottom = displaySlideBar;
        break;
    }

    return safety;
};
     
}; // namespace iOS_utils
}; // namespace JUCE-AppBasics