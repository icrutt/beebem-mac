#include "plist.h"
#include "BeebEmLog.hpp"

//void WriteMyPropertyListToFile( CFPropertyListRef propertyList,
//            CFURLRef fileURL )
//{
//   CFDataRef xmlData;
//   Boolean status;
//   SInt32 errorCode;
//   // Convert the property list into XML data.
//   xmlData = CFPropertyListCreateXMLData( kCFAllocatorDefault, propertyList );
//   // Write the XML data to the file.
//   status = CFURLWriteDataAndPropertiesToResource (
//               fileURL,                  // URL to use
//               xmlData,                  // data to write
//               NULL,
//               &errorCode);
//   CFRelease(xmlData);
//}
//**CARBON****CARBON****CARBON****CARBON****CARBON****CARBON****CARBON**
//CFPropertyListRef CreateMyPropertyListFromFile( CFURLRef fileURL )
//{
//   CFPropertyListRef propertyList;
//   CFStringRef       errorString;
//   CFDataRef         resourceData;
//   Boolean           status;
//   SInt32            errorCode;
//    BeebEmLog::writeLog("Debug plist 01\n");
//   // Read the XML file.
//   status = CFURLCreateDataAndPropertiesFromResource(
//               kCFAllocatorDefault,
//               fileURL,
//               &resourceData,            // place to put file data
//               NULL,
//               NULL,
//               &errorCode);
//    BeebEmLog::writeLog("Debug plist 02\n");
//
//	if (status == false) return NULL;
//    BeebEmLog::writeLog("Debug plist 03\n");
//
//   // Reconstitute the dictionary using the XML data.
//   propertyList = CFPropertyListCreateFromXMLData( kCFAllocatorDefault,
//               resourceData,
//               kCFPropertyListImmutable,
//               &errorString);
//    BeebEmLog::writeLog("Debug plist 04\n");
//
//   CFRelease( resourceData );
//   BeebEmLog::writeLog("Debug plist 05\n");
//return propertyList;
//}

//void AddDictNum(CFMutableDictionaryRef dict, CFStringRef key, int value)
//{
//CFNumberRef            num;
//
//	num = CFNumberCreate( kCFAllocatorDefault,
//            kCFNumberIntType,
//            &value );
//	CFDictionarySetValue( dict, key, num );
//	CFRelease( num );
//}
//
//void AddDictString(CFMutableDictionaryRef dict, CFStringRef key, char *value)
//{
//CFStringRef pVal;
//
//	pVal = CFStringCreateWithCString (kCFAllocatorDefault, value, kCFStringEncodingASCII);
//
//	CFDictionarySetValue( dict, key, pVal );
//
//	CFRelease(pVal);
//
//}

//int GetDictNum(CFMutableDictionaryRef dict, CFStringRef key, int Default)
//{
//CFNumberRef pVal;
//int value;
//
//	pVal = (CFNumberRef) CFDictionaryGetValue( dict, key );
//	if (pVal)
//	{
//		CFNumberGetValue(pVal, kCFNumberSInt32Type, &value);
//		return value;
//	}
//	else
//	{
//		return Default;
//	}
//}
//
//void GetDictString(CFMutableDictionaryRef dict, CFStringRef key, char *value, char *Default)
//{
//CFStringRef pVal;
//
//	pVal = (CFStringRef) CFDictionaryGetValue( dict, key );
//	if (pVal)
//	{
//		CFStringGetCString (pVal, value, 256, kCFStringEncodingASCII);
//	}
//	else
//	{
//		strcpy(value, Default);
//	}
//}
