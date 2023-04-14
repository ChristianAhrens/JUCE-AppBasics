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
    iPadAir3,
    iPad7,
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
    iPadPro1294,
    iPadmini,
    iPadmini2,
    iPadmini3,
    iPadmini4,
    iPadmini5,
    iPadmini6,
    iPad2017,
    iPadAir4,
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
     
}; // namespace iOS_utils
}; // namespace JUCE-AppBasics
