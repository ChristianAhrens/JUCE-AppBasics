#include "iOS_utils.h"

namespace JUCEAppBasics
{
namespace iOS_utils
{

JUCEAppBasics::iOS_utils::KnownDevices getDeviceTypeIPhone(const juce::String& deviceDescription)
{
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
        return KnownDevices::iPhone5C;
    if (deviceDescription == "iPhone 5s")
        return KnownDevices::iPhone5S;
    if (deviceDescription == "iPhone 6")
        return KnownDevices::iPhone6;
    if (deviceDescription == "iPhone 6 Plus")
        return KnownDevices::iPhone6Plus;
    if (deviceDescription == "iPhone 6s")
        return KnownDevices::iPhone6S;
    if (deviceDescription == "iPhone 6s Plus")
        return KnownDevices::iPhone6SPlus;
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
    if (deviceDescription == "iPhone 12")
        return KnownDevices::iPhone12;
    if (deviceDescription == "iPhone 12 mini")
        return KnownDevices::iPhone12Mini;
    if (deviceDescription == "iPhone 12 Pro")
        return KnownDevices::iPhone12Pro;
    if (deviceDescription == "iPhone 12 Pro Max")
        return KnownDevices::iPhone12ProMax;
    if (deviceDescription == "iPhone 13")
        return KnownDevices::iPhone13;
    if (deviceDescription == "iPhone 13 mini")
        return KnownDevices::iPhone13Mini;
    if (deviceDescription == "iPhone 13 Pro")
        return KnownDevices::iPhone13Pro;
    if (deviceDescription == "iPhone 13 Pro Max")
        return KnownDevices::iPhone13ProMax;
    if (deviceDescription == "iPhone1,1") // iPhone
        return KnownDevices::iPhone;
    if (deviceDescription == "iPhone1,2") // iPhone 3G
        return KnownDevices::iPhone3G;
    if (deviceDescription == "iPhone2,1") // iPhone 3GS
        return KnownDevices::iPhone3GS;
    if (deviceDescription == "iPhone3,1") // iPhone 4
        return KnownDevices::iPhone4;
    if (deviceDescription == "iPhone3,2") // iPhone 4 GSM Rev A
        return KnownDevices::iPhone4;
    if (deviceDescription == "iPhone3,3") // iPhone 4 CDMA
        return KnownDevices::iPhone4;
    if (deviceDescription == "iPhone4,1") // iPhone 4S
        return KnownDevices::iPhone4S;
    if (deviceDescription == "iPhone5,1") // iPhone 5 (GSM)
        return KnownDevices::iPhone5;
    if (deviceDescription == "iPhone5,2") // iPhone 5 (GSM+CDMA)
        return KnownDevices::iPhone5;
    if (deviceDescription == "iPhone5,3") // iPhone 5C (GSM)
        return KnownDevices::iPhone5C;
    if (deviceDescription == "iPhone5,4") // iPhone 5C (Global)
        return KnownDevices::iPhone5C;
    if (deviceDescription == "iPhone6,1") // iPhone 5S (GSM)
        return KnownDevices::iPhone5S;
    if (deviceDescription == "iPhone6,2") // iPhone 5S (Global)
        return KnownDevices::iPhone5S;
    if (deviceDescription == "iPhone7,1") // iPhone 6 Plus
        return KnownDevices::iPhone6Plus;
    if (deviceDescription == "iPhone7,2") // iPhone 6
        return KnownDevices::iPhone6;
    if (deviceDescription == "iPhone8,1") // iPhone 6s
        return KnownDevices::iPhone6S;
    if (deviceDescription == "iPhone8,2") // iPhone 6s Plus
        return KnownDevices::iPhone6SPlus;
    if (deviceDescription == "iPhone8,4") // iPhone SE (GSM)
        return KnownDevices::iPhoneSE;
    if (deviceDescription == "iPhone9,1") // iPhone 7
        return KnownDevices::iPhone7;
    if (deviceDescription == "iPhone9,2") // iPhone 7 Plus
        return KnownDevices::iPhone7Plus;
    if (deviceDescription == "iPhone9,3") // iPhone 7
        return KnownDevices::iPhone7;
    if (deviceDescription == "iPhone9,4") // iPhone 7 Plus
        return KnownDevices::iPhone7Plus;
    if (deviceDescription == "iPhone10,1") // iPhone 8
        return KnownDevices::iPhone8;
    if (deviceDescription == "iPhone10,2") // iPhone 8 Plus
        return KnownDevices::iPhone8Plus;
    if (deviceDescription == "iPhone10,3") // iPhone X Global
        return KnownDevices::iPhoneX;
    if (deviceDescription == "iPhone10,4") // iPhone 8
        return KnownDevices::iPhone8;
    if (deviceDescription == "iPhone10,5") // iPhone 8 Plus
        return KnownDevices::iPhone8Plus;
    if (deviceDescription == "iPhone10,6") // iPhone X GSM
        return KnownDevices::iPhoneX;
    if (deviceDescription == "iPhone11,2") // iPhone XS
        return KnownDevices::iPhoneXS;
    if (deviceDescription == "iPhone11,4") // iPhone XS Max
        return KnownDevices::iPhoneXSMax;
    if (deviceDescription == "iPhone11,6") // iPhone XS Max Global
        return KnownDevices::iPhoneXSMax;
    if (deviceDescription == "iPhone11,8") // iPhone XR
        return KnownDevices::iPhoneXR;
    if (deviceDescription == "iPhone12,1") // iPhone 11
        return KnownDevices::iPhone11;
    if (deviceDescription == "iPhone12,3") // iPhone 11 Pro
        return KnownDevices::iPhone11Pro;
    if (deviceDescription == "iPhone12,5") // iPhone 11 Pro Max
        return KnownDevices::iPhone11ProMax;
    if (deviceDescription == "iPhone12,8") // iPhone SE 2nd Gen
        return KnownDevices::iPhoneSE2;
    if (deviceDescription == "iPhone13,1") // iPhone 12 Mini
        return KnownDevices::iPhone12Mini;
    if (deviceDescription == "iPhone13,2") // iPhone 12
        return KnownDevices::iPhone12;
    if (deviceDescription == "iPhone13,3") // iPhone 12 Pro
        return KnownDevices::iPhone12Pro;
    if (deviceDescription == "iPhone13,4") // iPhone 12 Pro Max
        return KnownDevices::iPhone12ProMax;
    if (deviceDescription == "iPhone14,4") // iPhone 13 Mini
        return KnownDevices::iPhone13Mini;
    if (deviceDescription == "iPhone14,5") // iPhone 13
        return KnownDevices::iPhone13;
    if (deviceDescription == "iPhone14,2") // iPhone 13 Pro
        return KnownDevices::iPhone13Pro;
    if (deviceDescription == "iPhone14,3") // iPhone 13 Pro Max
        return KnownDevices::iPhone13ProMax;
    if (deviceDescription == "iPhone14,6") // iPhone SE 3nd Gen
        return KnownDevices::iPhoneSE3;
    if (deviceDescription == "iPhone14,7") // iPhone 14
        return KnownDevices::iPhone14;
    if (deviceDescription == "iPhone15,2") // iPhone 14 Pro
        return KnownDevices::iPhone14Pro;
    if (deviceDescription == "iPhone14,8") // iPhone 14 Plus
        return KnownDevices::iPhone14Plus;
    if (deviceDescription == "iPhone15,3") // iPhone 14 Pro Max
        return KnownDevices::iPhone14ProMax;
    if (deviceDescription.contains("iPhone"))
        return KnownDevices::GenericiPhone;
    
    return {};
}

JUCEAppBasics::iOS_utils::KnownDevices getDeviceTypeIPad(const juce::String& deviceDescription)
{
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
    if (deviceDescription == "iPad mini 6")
        return KnownDevices::iPadmini6;
    if (deviceDescription == "iPad1,1") // iPad
        return KnownDevices::iPad;
    if (deviceDescription == "iPad1,2") // iPad 3G
        return KnownDevices::iPad3G;
    if (deviceDescription == "iPad2,1") // 2nd Gen iPad
        return KnownDevices::iPad2;
    if (deviceDescription == "iPad2,2") // 2nd Gen iPad GSM
        return KnownDevices::iPad2;
    if (deviceDescription == "iPad2,3") // 2nd Gen iPad CDMA
        return KnownDevices::iPad2;
    if (deviceDescription == "iPad2,4") // 2nd Gen iPad New Revision
        return KnownDevices::iPad2;
    if (deviceDescription == "iPad3,1") // 3rd Gen iPad
        return KnownDevices::iPad3;
    if (deviceDescription == "iPad3,2") // 3rd Gen iPad CDMA
        return KnownDevices::iPad3;
    if (deviceDescription == "iPad3,3") // 3rd Gen iPad GSM
        return KnownDevices::iPad3;
    if (deviceDescription == "iPad2,5") // iPad mini
        return KnownDevices::iPadmini;
    if (deviceDescription == "iPad2,6") // iPad mini GSM+LTE
        return KnownDevices::iPadmini;
    if (deviceDescription == "iPad2,7") // iPad mini CDMA+LTE
        return KnownDevices::iPadmini;
    if (deviceDescription == "iPad3,4") // 4th Gen iPad
        return KnownDevices::iPad4;
    if (deviceDescription == "iPad3,5") // 4th Gen iPad GSM+LTE
        return KnownDevices::iPad4;
    if (deviceDescription == "iPad3,6") // 4th Gen iPad CDMA+LTE
        return KnownDevices::iPad4;
    if (deviceDescription == "iPad4,1") // iPad Air (WiFi)
        return KnownDevices::iPadAir;
    if (deviceDescription == "iPad4,2") // iPad Air (GSM+CDMA)
        return KnownDevices::iPadAir;
    if (deviceDescription == "iPad4,3") // 1st Gen iPad Air (China)
        return KnownDevices::iPadAir;
    if (deviceDescription == "iPad4,4") // iPad mini Retina (WiFi)
        return KnownDevices::iPadmini;
    if (deviceDescription == "iPad4,5") // iPad mini Retina (GSM+CDMA)
        return KnownDevices::iPadmini;
    if (deviceDescription == "iPad4,6") // iPad mini Retina (China)
        return KnownDevices::iPadmini;
    if (deviceDescription == "iPad4,7") // iPad mini 3 (WiFi)
        return KnownDevices::iPadmini3;
    if (deviceDescription == "iPad4,8") // iPad mini 3 (GSM+CDMA)
        return KnownDevices::iPadmini3;
    if (deviceDescription == "iPad4,9") // iPad Mini 3 (China)
        return KnownDevices::iPadmini3;
    if (deviceDescription == "iPad5,1") // iPad mini 4 (WiFi)
        return KnownDevices::iPadmini4;
    if (deviceDescription == "iPad5,2") // 4th Gen iPad mini (WiFi+Cellular)
        return KnownDevices::iPadmini4;
    if (deviceDescription == "iPad5,3") // iPad Air 2 (WiFi)
        return KnownDevices::iPadAir2;
    if (deviceDescription == "iPad5,4") // iPad Air 2 (Cellular)
        return KnownDevices::iPadAir2;
    if (deviceDescription == "iPad6,3") // iPad Pro (9.7 inch, WiFi)
        return KnownDevices::iPadPro970;
    if (deviceDescription == "iPad6,4") // iPad Pro (9.7 inch, WiFi+LTE)
        return KnownDevices::iPadPro970;
    if (deviceDescription == "iPad6,7") // iPad Pro (12.9 inch, WiFi)
        return KnownDevices::iPadPro1290;
    if (deviceDescription == "iPad6,8") // iPad Pro (12.9 inch, WiFi+LTE)
        return KnownDevices::iPadPro1290;
    if (deviceDescription == "iPad6,11") // iPad (2017)
        return KnownDevices::iPad2017;
    if (deviceDescription == "iPad6,12") // iPad (2017)
        return KnownDevices::iPad2017;
    if (deviceDescription == "iPad7,1") // iPad Pro 2nd Gen (WiFi)
        return KnownDevices::iPadPro2;
    if (deviceDescription == "iPad7,2") // iPad Pro 2nd Gen (WiFi+Cellular)
        return KnownDevices::iPadPro2;
    if (deviceDescription == "iPad7,3") // iPad Pro 10.5-inch
        return KnownDevices::iPadPro1050;
    if (deviceDescription == "iPad7,4") // iPad Pro 10.5-inch
        return KnownDevices::iPadPro1050;
    if (deviceDescription == "iPad7,5") // iPad 6th Gen (WiFi)
        return KnownDevices::iPad6;
    if (deviceDescription == "iPad7,6") // iPad 6th Gen (WiFi+Cellular)
        return KnownDevices::iPad6;
    if (deviceDescription == "iPad7,11") // iPad 7th Gen 10.2-inch (WiFi)
        return KnownDevices::iPad1027;
    if (deviceDescription == "iPad7,12") // iPad 7th Gen 10.2-inch (WiFi+Cellular)
        return KnownDevices::iPad1027;
    if (deviceDescription == "iPad8,1") // iPad Pro 11 inch (WiFi)
        return KnownDevices::iPadPro1101;
    if (deviceDescription == "iPad8,2") // iPad Pro 11 inch (1TB, WiFi)
        return KnownDevices::iPadPro1101;
    if (deviceDescription == "iPad8,3") // iPad Pro 11 inch (WiFi+Cellular)
        return KnownDevices::iPadPro1101;
    if (deviceDescription == "iPad8,4") // iPad Pro 11 inch (1TB, WiFi+Cellular)
        return KnownDevices::iPadPro1101;
    if (deviceDescription == "iPad8,5") // iPad Pro 12.9 inch 3rd Gen (WiFi)
        return KnownDevices::iPadPro1293;
    if (deviceDescription == "iPad8,6") // iPad Pro 12.9 inch 3rd Gen (1TB, WiFi)
        return KnownDevices::iPadPro1293;
    if (deviceDescription == "iPad8,7") // iPad Pro 12.9 inch 3rd Gen (WiFi+Cellular)
        return KnownDevices::iPadPro1293;
    if (deviceDescription == "iPad8,8") // iPad Pro 12.9 inch 3rd Gen (1TB, WiFi+Cellular)
        return KnownDevices::iPadPro1293;
    if (deviceDescription == "iPad8,9") // iPad Pro 11 inch 2nd Gen (WiFi)
        return KnownDevices::iPadPro1102;
    if (deviceDescription == "iPad8,10") // iPad Pro 11 inch 2nd Gen (WiFi+Cellular)
        return KnownDevices::iPadPro1102;
    if (deviceDescription == "iPad8,11") // iPad Pro 12.9 inch 4th Gen (WiFi)
        return KnownDevices::iPadPro1294;
    if (deviceDescription == "iPad8,12") // iPad Pro 12.9 inch 4th Gen (WiFi+Cellular)
        return KnownDevices::iPadPro1294;
    if (deviceDescription == "iPad11,1") // iPad mini 5th Gen (WiFi)
        return KnownDevices::iPadmini5;
    if (deviceDescription == "iPad11,2") // iPad mini 5th Gen
        return KnownDevices::iPadmini5;
    if (deviceDescription == "iPad11,3") // iPad Air 3rd Gen (WiFi)
        return KnownDevices::iPadAir3;
    if (deviceDescription == "iPad11,4") // iPad Air 3rd Gen
        return KnownDevices::iPadAir3; 
    if (deviceDescription == "iPad13,1") // iPad air 4th Gen(WiFi)
        return KnownDevices::iPadAir4;
    if (deviceDescription == "iPad13,2") // iPad air 4th Gen(WiFi + Celular)
        return KnownDevices::iPadAir4;
    if (deviceDescription == "iPad13,17") // iPad air 5th Gen
        return KnownDevices::iPadAir5;
    if (deviceDescription == "iPad13,18") // iPad 10th Gen
        return KnownDevices::iPad10;
    if (deviceDescription == "iPad14,1") // iPad mini 6th Gen (WiFi)
        return KnownDevices::iPadmini6;
    if (deviceDescription == "iPad14,2") // iPad mini 6th Gen
        return KnownDevices::iPadmini6;
    if (deviceDescription == "iPad14,3") // iPad Pro 11 inch 4nd Gen
        return KnownDevices::iPadPro1104;
    if (deviceDescription == "iPad14,5") // iPad Pro 12.9 inch 6th Gen
        return KnownDevices::iPadPro1296;
    if (deviceDescription.contains("iPad"))
        return KnownDevices::GenericiPad;

    return {};
}

JUCEAppBasics::iOS_utils::KnownDevices getDeviceTypeMac(const juce::String& deviceDescription)
{
    ignoreUnused(deviceDescription);
    return KnownDevices::GenericMac;
}

JUCEAppBasics::iOS_utils::KnownDevices getDeviceTypeIPod(const juce::String& deviceDescription)
{
    if (deviceDescription == "iPod1,1") // 1st Gen iPod
        return KnownDevices::iPod1;
    if (deviceDescription == "iPod2,1") // 2nd Gen iPod
        return KnownDevices::iPod2;
    if (deviceDescription == "iPod3,1") // 3rd Gen iPod
        return KnownDevices::iPod3;
    if (deviceDescription == "iPod4,1") // 4th Gen iPod
        return KnownDevices::iPod4;
    if (deviceDescription == "iPod5,1") // 5th Gen iPod
        return KnownDevices::iPod5;
    if (deviceDescription == "iPod7,1") // 6th Gen iPod
        return KnownDevices::iPod6;
    if (deviceDescription == "iPod9,1") // 7th Gen iPod
        return KnownDevices::iPod7;

    return {};
}

JUCEAppBasics::iOS_utils::KnownDevices getDeviceTypeAppleWatch(const juce::String& deviceDescription)
{
    if (deviceDescription == "Watch1,1") // Apple Watch 38mm case
        return KnownDevices::AppleWatch;
    if (deviceDescription == "Watch1,2") // Apple Watch 42mm case
        return KnownDevices::AppleWatch;
    if (deviceDescription == "Watch2,6") // Apple Watch Series 1 38mm case
        return KnownDevices::AppleWatch1;
    if (deviceDescription == "Watch2,7") // Apple Watch Series 1 42mm case
        return KnownDevices::AppleWatch1;
    if (deviceDescription == "Watch2,3") // Apple Watch Series 2 38mm case
        return KnownDevices::AppleWatch2;
    if (deviceDescription == "Watch2,4") // Apple Watch Series 2 42mm case
        return KnownDevices::AppleWatch2;
    if (deviceDescription == "Watch3,1") // Apple Watch Series 3 38mm case (GPS+Cellular)
        return KnownDevices::AppleWatch3;
    if (deviceDescription == "Watch3,2") // Apple Watch Series 3 42mm case (GPS+Cellular)
        return KnownDevices::AppleWatch3;
    if (deviceDescription == "Watch3,3") // Apple Watch Series 3 38mm case (GPS)
        return KnownDevices::AppleWatch3;
    if (deviceDescription == "Watch3,4") // Apple Watch Series 3 42mm case (GPS)
        return KnownDevices::AppleWatch3;
    if (deviceDescription == "Watch4,1") // Apple Watch Series 4 40mm case (GPS)
        return KnownDevices::AppleWatch4;
    if (deviceDescription == "Watch4,2") // Apple Watch Series 4 44mm case (GPS)
        return KnownDevices::AppleWatch4;
    if (deviceDescription == "Watch4,3") // Apple Watch Series 4 40mm case (GPS+Cellular)
        return KnownDevices::AppleWatch4;
    if (deviceDescription == "Watch4,4") // Apple Watch Series 4 44mm case (GPS+Cellular)
        return KnownDevices::AppleWatch4;
    if (deviceDescription == "Watch5,1") // Apple Watch Series 5 40mm case (GPS)
        return KnownDevices::AppleWatch5;
    if (deviceDescription == "Watch5,2") // Apple Watch Series 5 44mm case (GPS)
        return KnownDevices::AppleWatch5;
    if (deviceDescription == "Watch5,3") // Apple Watch Series 5 40mm case (GPS+Cellular)
        return KnownDevices::AppleWatch5;
    if (deviceDescription == "Watch5,4") // Apple Watch Series 5 44mm case (GPS+Cellular)
        return KnownDevices::AppleWatch5;
    if (deviceDescription == "Watch5,9") // Apple Watch SE 40mm case (GPS)
        return KnownDevices::AppleWatchSE;
    if (deviceDescription == "Watch5,10") // Apple Watch SE 44mm case (GPS)
        return KnownDevices::AppleWatchSE;
    if (deviceDescription == "Watch5,11") // Apple Watch SE 40mm case (GPS+Cellular)
        return KnownDevices::AppleWatchSE;
    if (deviceDescription == "Watch5,12") // Apple Watch SE 44mm case (GPS+Cellular)
        return KnownDevices::AppleWatchSE;
    if (deviceDescription == "Watch6,1") // Apple Watch Series 6 40mm case (GPS)
        return KnownDevices::AppleWatch6;
    if (deviceDescription == "Watch6,2") // Apple Watch Series 6 44mm case (GPS)
        return KnownDevices::AppleWatch6;
    if (deviceDescription == "Watch6,3") // Apple Watch Series 6 40mm case (GPS+Cellular)
        return KnownDevices::AppleWatch6;
    if (deviceDescription == "Watch6,4") // Apple Watch Series 6 44mm case (GPS+Cellular)
        return KnownDevices::AppleWatch6;
    if (deviceDescription == "Watch6,6") // Apple Watch Series 7 41mm case (GPS)
        return KnownDevices::AppleWatch7;
    if (deviceDescription == "Watch6,7") // Apple Watch Series 7 45mm case (GPS)
        return KnownDevices::AppleWatch7;
    if (deviceDescription == "Watch6,8") // Apple Watch Series 7 41mm case (GPS+Cellular)
        return KnownDevices::AppleWatch7;
    if (deviceDescription == "Watch6,9") // Apple Watch Series 7 45mm case (GPS+Cellular)
        return KnownDevices::AppleWatch7;

    return {};
}

JUCEAppBasics::iOS_utils::KnownDevices getDeviceTypeOther(const juce::String& deviceDescription)
{
    if (deviceDescription == "i386") // iPhone Simulator
        return KnownDevices::iPhoneSim_i386;
    if (deviceDescription == "x86_64") // iPhone Simulator
        return KnownDevices::iPhoneSim_x86_64;
    else
        return InvalidDevice;
}

JUCEAppBasics::iOS_utils::KnownDevices getDeviceType()
{
    auto deviceDescription = SystemStats::getDeviceDescription();

    if (deviceDescription.contains("iPhone"))
        return getDeviceTypeIPhone(deviceDescription);
    else if (deviceDescription.contains("iPad"))
        return getDeviceTypeIPad(deviceDescription);
    else if (deviceDescription.contains("Mac"))
        return getDeviceTypeMac(deviceDescription);
    else if (deviceDescription.contains("iPod"))
        return getDeviceTypeIPod(deviceDescription);
    else if (deviceDescription.contains("AppleWatch"))
        return getDeviceTypeAppleWatch(deviceDescription);
    else
        return getDeviceTypeOther(deviceDescription);
};

JUCEAppBasics::iOS_utils::KnownDevices getGenericDeviceType()
{
    switch (getDeviceType())
    {
    case GenericMac:
        return GenericMac;
    case GenericPC:
        return GenericPC;
    case GenericAndroidPhone:
        return GenericAndroidPhone;
    case GenericiPod:
    case iPod1:
    case iPod2:
    case iPod3:
    case iPod4:
    case iPod5:
    case iPod6:
    case iPod7:
        return GenericiPod;
    case GenericiPhone:
    case iPhone:
    case iPhone3G:
    case iPhone3GS:
    case iPhone4:
    case iPhone4S:
    case iPhone5:
    case iPhone5C:
    case iPhone5S:
    case iPhone6:
    case iPhone6Plus:
    case iPhone6S:
    case iPhone6SPlus:
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
    case iPhoneSE2:
    case iPhone12:
    case iPhone12Mini:
    case iPhone12Pro:
    case iPhone12ProMax:
    case iPhone13:
    case iPhone13Mini:
    case iPhone13Pro:
    case iPhone13ProMax:
    case iPhoneSE3:
    case iPhone14:
    case iPhone14Plus:
    case iPhone14Pro:
    case iPhone14ProMax:
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
    case iPadPro970:
    case iPadPro1050:
    case iPadPro1120:
    case iPadPro1290:
    case iPad1027:
    case iPadPro1101:
    case iPadPro1293:
    case iPadPro1102:
    case iPadPro1294:
    case iPadPro1296:
    case iPadmini:
    case iPadmini2:
    case iPadmini3:
    case iPadmini4:
    case iPadmini5:
    case iPadmini6:
    case iPad2017:
        return GenericiPad;
    case AppleWatch:
    case AppleWatch1:
    case AppleWatch2:
    case AppleWatch3:
    case AppleWatch4:
    case AppleWatch5:
    case AppleWatchSE:
    case AppleWatch6:
    case AppleWatch7:
        return GenericAppleWatch;
    default:
        return InvalidDevice;
    }
};

int getDeviceDisplayNotchIndent()
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
    case iPhone5C:
    case iPhone5S:
    case iPhone6:
    case iPhone6Plus:
    case iPhone6S:
    case iPhone6SPlus:
    case iPhoneSE:
    case iPhoneSE2:
    case iPhoneSE3:
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
    case iPhone12:
    case iPhone12Mini:
    case iPhone12Pro:
    case iPhone12ProMax:
    case iPhone13:
    case iPhone13Mini:
    case iPhone13Pro:
    case iPhone14:
    case iPhone14Plus:
    case GenericiPhone:
        displayNotchIndent = 35;
        break;
    case iPhone14Pro:
    case iPhone14ProMax:
        displayNotchIndent = 50;
        break;
    case iPad:
    case iPad2:
    case iPad3:
    case iPad4:
    case iPad5:
    case iPad6:
    case iPad7:
    case iPad10:
    case iPadAir:
    case iPadAir2:
    case iPadAir3:
    case iPadAir4:
    case iPadAir5:
    case iPadPro:
    case iPadPro2:
    case iPadPro3:
    case iPadPro4:
    case iPadPro1296:
    case iPadPro1104:
    case iPadmini:
    case iPadmini2:
    case iPadmini3:
    case iPadmini4:
    case iPadmini5:
    case iPadmini6:
    case GenericiPad:
        displayNotchIndent = 0;
        break;
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

int getDeviceDisplaySlideBarIndent()
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
    case iPhone5C:
    case iPhone5S:
    case iPhone6:
    case iPhone6Plus:
    case iPhone6S:
    case iPhone6SPlus:
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
    case iPhone12:
    case iPhone12Mini:
    case iPhone12Pro:
    case iPhone12ProMax:
    case iPhone13:
    case iPhone13Mini:
    case iPhone13Pro:
    case iPhone13ProMax:
    case iPhone14:
    case iPhone14Plus:
    case iPhone14Pro:
    case iPhone14ProMax:
        slideBarIndent = 20;
        break;
    case iPad:
    case iPad2:
    case iPad3:
    case iPad4:
    case iPad5:
    case iPad6:
    case iPad7:
    case iPad10:
    case iPadAir:
    case iPadAir2:
    case iPadAir3:
    case iPadAir4:
    case iPadAir5:
    case iPadPro:
    case iPadPro2:
    case iPadPro3:
    case iPadPro4:
    case iPadPro1296:
    case iPadPro1104:
    case iPadmini:
    case iPadmini2:
    case iPadmini3:
    case iPadmini4:
    case iPadmini5:
    case iPadmini6:
    case GenericiPad:
        slideBarIndent = 0;
        break;
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

JUCEAppBasics::iOS_utils::SafetyMargin getDeviceSafetyMargins()
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
