/*
* Copyright (c) 2019-2025 Allwinner Technology Co., Ltd. ALL rights reserved.
*
* Allwinner is a trademark of Allwinner Technology Co.,Ltd., registered in
* the the People's Republic of China and other countries.
* All Allwinner Technology Co.,Ltd. trademarks are used with permission.
*
* DISCLAIMER
* THIRD PARTY LICENCES MAY BE REQUIRED TO IMPLEMENT THE SOLUTION/PRODUCT.
* IF YOU NEED TO INTEGRATE THIRD PARTY’S TECHNOLOGY (SONY, DTS, DOLBY, AVS OR MPEGLA, ETC.)
* IN ALLWINNERS’SDK OR PRODUCTS, YOU SHALL BE SOLELY RESPONSIBLE TO OBTAIN
* ALL APPROPRIATELY REQUIRED THIRD PARTY LICENCES.
* ALLWINNER SHALL HAVE NO WARRANTY, INDEMNITY OR OTHER OBLIGATIONS WITH RESPECT TO MATTERS
* COVERED UNDER ANY REQUIRED THIRD PARTY LICENSE.
* YOU ARE SOLELY RESPONSIBLE FOR YOUR USAGE OF THIRD PARTY’S TECHNOLOGY.
*
*
* THIS SOFTWARE IS PROVIDED BY ALLWINNER"AS IS" AND TO THE MAXIMUM EXTENT
* PERMITTED BY LAW, ALLWINNER EXPRESSLY DISCLAIMS ALL WARRANTIES OF ANY KIND,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING WITHOUT LIMITATION REGARDING
* THE TITLE, NON-INFRINGEMENT, ACCURACY, CONDITION, COMPLETENESS, PERFORMANCE
* OR MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL ALLWINNER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS, OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef _MTPDEBUG_H
#define _MTPDEBUG_H
//#ifdef DEBUG

#include <stdint.h>
#include "mtp.h"

typedef struct
{
    const char *name;
    uint16_t code;
} CodeEntry;

static const CodeEntry sOperationCodes[] =
{
    { "MTP_OPERATION_GET_DEVICE_INFO",              0x1001 },
    { "MTP_OPERATION_OPEN_SESSION",                 0x1002 },
    { "MTP_OPERATION_CLOSE_SESSION",                0x1003 },
    { "MTP_OPERATION_GET_STORAGE_IDS",              0x1004 },
    { "MTP_OPERATION_GET_STORAGE_INFO",             0x1005 },
    { "MTP_OPERATION_GET_NUM_OBJECTS",              0x1006 },
    { "MTP_OPERATION_GET_OBJECT_HANDLES",           0x1007 },
    { "MTP_OPERATION_GET_OBJECT_INFO",              0x1008 },
    { "MTP_OPERATION_GET_OBJECT",                   0x1009 },
    { "MTP_OPERATION_GET_THUMB",                    0x100A },
    { "MTP_OPERATION_DELETE_OBJECT",                0x100B },
    { "MTP_OPERATION_SEND_OBJECT_INFO",             0x100C },
    { "MTP_OPERATION_SEND_OBJECT",                  0x100D },
    { "MTP_OPERATION_INITIATE_CAPTURE",             0x100E },
    { "MTP_OPERATION_FORMAT_STORE",                 0x100F },
    { "MTP_OPERATION_RESET_DEVICE",                 0x1010 },
    { "MTP_OPERATION_SELF_TEST",                    0x1011 },
    { "MTP_OPERATION_SET_OBJECT_PROTECTION",        0x1012 },
    { "MTP_OPERATION_POWER_DOWN",                   0x1013 },
    { "MTP_OPERATION_GET_DEVICE_PROP_DESC",         0x1014 },
    { "MTP_OPERATION_GET_DEVICE_PROP_VALUE",        0x1015 },
    { "MTP_OPERATION_SET_DEVICE_PROP_VALUE",        0x1016 },
    { "MTP_OPERATION_RESET_DEVICE_PROP_VALUE",      0x1017 },
    { "MTP_OPERATION_TERMINATE_OPEN_CAPTURE",       0x1018 },
    { "MTP_OPERATION_MOVE_OBJECT",                  0x1019 },
    { "MTP_OPERATION_COPY_OBJECT",                  0x101A },
    { "MTP_OPERATION_GET_PARTIAL_OBJECT",           0x101B },
    { "MTP_OPERATION_INITIATE_OPEN_CAPTURE",        0x101C },
    { "MTP_OPERATION_GET_OBJECT_PROPS_SUPPORTED",   0x9801 },
    { "MTP_OPERATION_GET_OBJECT_PROP_DESC",         0x9802 },
    { "MTP_OPERATION_GET_OBJECT_PROP_VALUE",        0x9803 },
    { "MTP_OPERATION_SET_OBJECT_PROP_VALUE",        0x9804 },
    { "MTP_OPERATION_GET_OBJECT_PROP_LIST",         0x9805 },
    { "MTP_OPERATION_SET_OBJECT_PROP_LIST",         0x9806 },
    { "MTP_OPERATION_GET_INTERDEPENDENT_PROP_DESC", 0x9807 },
    { "MTP_OPERATION_SEND_OBJECT_PROP_LIST",        0x9808 },
    { "MTP_OPERATION_GET_OBJECT_REFERENCES",        0x9810 },
    { "MTP_OPERATION_SET_OBJECT_REFERENCES",        0x9811 },
    { "MTP_OPERATION_SKIP",                         0x9820 },
    // android extensions
    { "MTP_OPERATION_GET_PARTIAL_OBJECT_64",        0x95C1 },
    { "MTP_OPERATION_SEND_PARTIAL_OBJECT",          0x95C2 },
    { "MTP_OPERATION_TRUNCATE_OBJECT",              0x95C3 },
    { "MTP_OPERATION_BEGIN_EDIT_OBJECT",            0x95C4 },
    { "MTP_OPERATION_END_EDIT_OBJECT",              0x95C5 },
    { 0,                                            0      },
};

static const CodeEntry sFormatCodes[] =
{
    { "MTP_FORMAT_UNDEFINED",                       0x3000 },
    { "MTP_FORMAT_ASSOCIATION",                     0x3001 },
    { "MTP_FORMAT_SCRIPT",                          0x3002 },
    { "MTP_FORMAT_EXECUTABLE",                      0x3003 },
    { "MTP_FORMAT_TEXT",                            0x3004 },
    { "MTP_FORMAT_HTML",                            0x3005 },
    { "MTP_FORMAT_DPOF",                            0x3006 },
    { "MTP_FORMAT_AIFF",                            0x3007 },
    { "MTP_FORMAT_WAV",                             0x3008 },
    { "MTP_FORMAT_MP3",                             0x3009 },
    { "MTP_FORMAT_AVI",                             0x300A },
    { "MTP_FORMAT_MPEG",                            0x300B },
    { "MTP_FORMAT_ASF",                             0x300C },
    { "MTP_FORMAT_DEFINED",                         0x3800 },
    { "MTP_FORMAT_EXIF_JPEG",                       0x3801 },
    { "MTP_FORMAT_TIFF_EP",                         0x3802 },
    { "MTP_FORMAT_FLASHPIX",                        0x3803 },
    { "MTP_FORMAT_BMP",                             0x3804 },
    { "MTP_FORMAT_CIFF",                            0x3805 },
    { "MTP_FORMAT_GIF",                             0x3807 },
    { "MTP_FORMAT_JFIF",                            0x3808 },
    { "MTP_FORMAT_CD",                              0x3809 },
    { "MTP_FORMAT_PICT",                            0x380A },
    { "MTP_FORMAT_PNG",                             0x380B },
    { "MTP_FORMAT_TIFF",                            0x380D },
    { "MTP_FORMAT_TIFF_IT",                         0x380E },
    { "MTP_FORMAT_JP2",                             0x380F },
    { "MTP_FORMAT_JPX",                             0x3810 },
    { "MTP_FORMAT_UNDEFINED_FIRMWARE",              0xB802 },
    { "MTP_FORMAT_WINDOWS_IMAGE_FORMAT",            0xB881 },
    { "MTP_FORMAT_UNDEFINED_AUDIO",                 0xB900 },
    { "MTP_FORMAT_WMA",                             0xB901 },
    { "MTP_FORMAT_OGG",                             0xB902 },
    { "MTP_FORMAT_AAC",                             0xB903 },
    { "MTP_FORMAT_AUDIBLE",                         0xB904 },
    { "MTP_FORMAT_FLAC",                            0xB906 },
    { "MTP_FORMAT_UNDEFINED_VIDEO",                 0xB980 },
    { "MTP_FORMAT_WMV",                             0xB981 },
    { "MTP_FORMAT_MP4_CONTAINER",                   0xB982 },
    { "MTP_FORMAT_MP2",                             0xB983 },
    { "MTP_FORMAT_3GP_CONTAINER",                   0xB984 },
    { "MTP_FORMAT_UNDEFINED_COLLECTION",            0xBA00 },
    { "MTP_FORMAT_ABSTRACT_MULTIMEDIA_ALBUM",       0xBA01 },
    { "MTP_FORMAT_ABSTRACT_IMAGE_ALBUM",            0xBA02 },
    { "MTP_FORMAT_ABSTRACT_AUDIO_ALBUM",            0xBA03 },
    { "MTP_FORMAT_ABSTRACT_VIDEO_ALBUM",            0xBA04 },
    { "MTP_FORMAT_ABSTRACT_AV_PLAYLIST",            0xBA05 },
    { "MTP_FORMAT_ABSTRACT_CONTACT_GROUP",          0xBA06 },
    { "MTP_FORMAT_ABSTRACT_MESSAGE_FOLDER",         0xBA07 },
    { "MTP_FORMAT_ABSTRACT_CHAPTERED_PRODUCTION",   0xBA08 },
    { "MTP_FORMAT_ABSTRACT_AUDIO_PLAYLIST",         0xBA09 },
    { "MTP_FORMAT_ABSTRACT_VIDEO_PLAYLIST",         0xBA0A },
    { "MTP_FORMAT_ABSTRACT_MEDIACAST",              0xBA0B },
    { "MTP_FORMAT_WPL_PLAYLIST",                    0xBA10 },
    { "MTP_FORMAT_M3U_PLAYLIST",                    0xBA11 },
    { "MTP_FORMAT_MPL_PLAYLIST",                    0xBA12 },
    { "MTP_FORMAT_ASX_PLAYLIST",                    0xBA13 },
    { "MTP_FORMAT_PLS_PLAYLIST",                    0xBA14 },
    { "MTP_FORMAT_UNDEFINED_DOCUMENT",              0xBA80 },
    { "MTP_FORMAT_ABSTRACT_DOCUMENT",               0xBA81 },
    { "MTP_FORMAT_XML_DOCUMENT",                    0xBA82 },
    { "MTP_FORMAT_MS_WORD_DOCUMENT",                0xBA83 },
    { "MTP_FORMAT_MHT_COMPILED_HTML_DOCUMENT",      0xBA84 },
    { "MTP_FORMAT_MS_EXCEL_SPREADSHEET",            0xBA85 },
    { "MTP_FORMAT_MS_POWERPOINT_PRESENTATION",      0xBA86 },
    { "MTP_FORMAT_UNDEFINED_MESSAGE",               0xBB00 },
    { "MTP_FORMAT_ABSTRACT_MESSSAGE",               0xBB01 },
    { "MTP_FORMAT_UNDEFINED_CONTACT",               0xBB80 },
    { "MTP_FORMAT_ABSTRACT_CONTACT",                0xBB81 },
    { "MTP_FORMAT_VCARD_2",                         0xBB82 },
    { 0,                                            0      },
};

static const CodeEntry sObjectPropCodes[] =
{
    { "MTP_PROPERTY_STORAGE_ID",                             0xDC01 },
    { "MTP_PROPERTY_OBJECT_FORMAT",                          0xDC02 },
    { "MTP_PROPERTY_PROTECTION_STATUS",                      0xDC03 },
    { "MTP_PROPERTY_OBJECT_SIZE",                            0xDC04 },
    { "MTP_PROPERTY_ASSOCIATION_TYPE",                       0xDC05 },
    { "MTP_PROPERTY_ASSOCIATION_DESC",                       0xDC06 },
    { "MTP_PROPERTY_OBJECT_FILE_NAME",                       0xDC07 },
    { "MTP_PROPERTY_DATE_CREATED",                           0xDC08 },
    { "MTP_PROPERTY_DATE_MODIFIED",                          0xDC09 },
    { "MTP_PROPERTY_KEYWORDS",                               0xDC0A },
    { "MTP_PROPERTY_PARENT_OBJECT",                          0xDC0B },
    { "MTP_PROPERTY_ALLOWED_FOLDER_CONTENTS",                0xDC0C },
    { "MTP_PROPERTY_HIDDEN",                                 0xDC0D },
    { "MTP_PROPERTY_SYSTEM_OBJECT",                          0xDC0E },
    { "MTP_PROPERTY_PERSISTENT_UID",                         0xDC41 },
    { "MTP_PROPERTY_SYNC_ID",                                0xDC42 },
    { "MTP_PROPERTY_PROPERTY_BAG",                           0xDC43 },
    { "MTP_PROPERTY_NAME",                                   0xDC44 },
    { "MTP_PROPERTY_CREATED_BY",                             0xDC45 },
    { "MTP_PROPERTY_ARTIST",                                 0xDC46 },
    { "MTP_PROPERTY_DATE_AUTHORED",                          0xDC47 },
    { "MTP_PROPERTY_DESCRIPTION",                            0xDC48 },
    { "MTP_PROPERTY_URL_REFERENCE",                          0xDC49 },
    { "MTP_PROPERTY_LANGUAGE_LOCALE",                        0xDC4A },
    { "MTP_PROPERTY_COPYRIGHT_INFORMATION",                  0xDC4B },
    { "MTP_PROPERTY_SOURCE",                                 0xDC4C },
    { "MTP_PROPERTY_ORIGIN_LOCATION",                        0xDC4D },
    { "MTP_PROPERTY_DATE_ADDED",                             0xDC4E },
    { "MTP_PROPERTY_NON_CONSUMABLE",                         0xDC4F },
    { "MTP_PROPERTY_CORRUPT_UNPLAYABLE",                     0xDC50 },
    { "MTP_PROPERTY_PRODUCER_SERIAL_NUMBER",                 0xDC51 },
    { "MTP_PROPERTY_REPRESENTATIVE_SAMPLE_FORMAT",           0xDC81 },
    { "MTP_PROPERTY_REPRESENTATIVE_SAMPLE_SIZE",             0xDC82 },
    { "MTP_PROPERTY_REPRESENTATIVE_SAMPLE_HEIGHT",           0xDC83 },
    { "MTP_PROPERTY_REPRESENTATIVE_SAMPLE_WIDTH",            0xDC84 },
    { "MTP_PROPERTY_REPRESENTATIVE_SAMPLE_DURATION",         0xDC85 },
    { "MTP_PROPERTY_REPRESENTATIVE_SAMPLE_DATA",             0xDC86 },
    { "MTP_PROPERTY_WIDTH",                                  0xDC87 },
    { "MTP_PROPERTY_HEIGHT",                                 0xDC88 },
    { "MTP_PROPERTY_DURATION",                               0xDC89 },
    { "MTP_PROPERTY_RATING",                                 0xDC8A },
    { "MTP_PROPERTY_TRACK",                                  0xDC8B },
    { "MTP_PROPERTY_GENRE",                                  0xDC8C },
    { "MTP_PROPERTY_CREDITS",                                0xDC8D },
    { "MTP_PROPERTY_LYRICS",                                 0xDC8E },
    { "MTP_PROPERTY_SUBSCRIPTION_CONTENT_ID",                0xDC8F },
    { "MTP_PROPERTY_PRODUCED_BY",                            0xDC90 },
    { "MTP_PROPERTY_USE_COUNT",                              0xDC91 },
    { "MTP_PROPERTY_SKIP_COUNT",                             0xDC92 },
    { "MTP_PROPERTY_LAST_ACCESSED",                          0xDC93 },
    { "MTP_PROPERTY_PARENTAL_RATING",                        0xDC94 },
    { "MTP_PROPERTY_META_GENRE",                             0xDC95 },
    { "MTP_PROPERTY_COMPOSER",                               0xDC96 },
    { "MTP_PROPERTY_EFFECTIVE_RATING",                       0xDC97 },
    { "MTP_PROPERTY_SUBTITLE",                               0xDC98 },
    { "MTP_PROPERTY_ORIGINAL_RELEASE_DATE",                  0xDC99 },
    { "MTP_PROPERTY_ALBUM_NAME",                             0xDC9A },
    { "MTP_PROPERTY_ALBUM_ARTIST",                           0xDC9B },
    { "MTP_PROPERTY_MOOD",                                   0xDC9C },
    { "MTP_PROPERTY_DRM_STATUS",                             0xDC9D },
    { "MTP_PROPERTY_SUB_DESCRIPTION",                        0xDC9E },
    { "MTP_PROPERTY_IS_CROPPED",                             0xDCD1 },
    { "MTP_PROPERTY_IS_COLOUR_CORRECTED",                    0xDCD2 },
    { "MTP_PROPERTY_IMAGE_BIT_DEPTH",                        0xDCD3 },
    { "MTP_PROPERTY_F_NUMBER",                               0xDCD4 },
    { "MTP_PROPERTY_EXPOSURE_TIME",                          0xDCD5 },
    { "MTP_PROPERTY_EXPOSURE_INDEX",                         0xDCD6 },
    { "MTP_PROPERTY_TOTAL_BITRATE",                          0xDE91 },
    { "MTP_PROPERTY_BITRATE_TYPE",                           0xDE92 },
    { "MTP_PROPERTY_SAMPLE_RATE",                            0xDE93 },
    { "MTP_PROPERTY_NUMBER_OF_CHANNELS",                     0xDE94 },
    { "MTP_PROPERTY_AUDIO_BIT_DEPTH",                        0xDE95 },
    { "MTP_PROPERTY_SCAN_TYPE",                              0xDE97 },
    { "MTP_PROPERTY_AUDIO_WAVE_CODEC",                       0xDE99 },
    { "MTP_PROPERTY_AUDIO_BITRATE",                          0xDE9A },
    { "MTP_PROPERTY_VIDEO_FOURCC_CODEC",                     0xDE9B },
    { "MTP_PROPERTY_VIDEO_BITRATE",                          0xDE9C },
    { "MTP_PROPERTY_FRAMES_PER_THOUSAND_SECONDS",            0xDE9D },
    { "MTP_PROPERTY_KEYFRAME_DISTANCE",                      0xDE9E },
    { "MTP_PROPERTY_BUFFER_SIZE",                            0xDE9F },
    { "MTP_PROPERTY_ENCODING_QUALITY",                       0xDEA0 },
    { "MTP_PROPERTY_ENCODING_PROFILE",                       0xDEA1 },
    { "MTP_PROPERTY_DISPLAY_NAME",                           0xDCE0 },
    { "MTP_PROPERTY_BODY_TEXT",                              0xDCE1 },
    { "MTP_PROPERTY_SUBJECT",                                0xDCE2 },
    { "MTP_PROPERTY_PRIORITY",                               0xDCE3 },
    { "MTP_PROPERTY_GIVEN_NAME",                             0xDD00 },
    { "MTP_PROPERTY_MIDDLE_NAMES",                           0xDD01 },
    { "MTP_PROPERTY_FAMILY_NAME",                            0xDD02 },
    { "MTP_PROPERTY_PREFIX",                                 0xDD03 },
    { "MTP_PROPERTY_SUFFIX",                                 0xDD04 },
    { "MTP_PROPERTY_PHONETIC_GIVEN_NAME",                    0xDD05 },
    { "MTP_PROPERTY_PHONETIC_FAMILY_NAME",                   0xDD06 },
    { "MTP_PROPERTY_EMAIL_PRIMARY",                          0xDD07 },
    { "MTP_PROPERTY_EMAIL_PERSONAL_1",                       0xDD08 },
    { "MTP_PROPERTY_EMAIL_PERSONAL_2",                       0xDD09 },
    { "MTP_PROPERTY_EMAIL_BUSINESS_1",                       0xDD0A },
    { "MTP_PROPERTY_EMAIL_BUSINESS_2",                       0xDD0B },
    { "MTP_PROPERTY_EMAIL_OTHERS",                           0xDD0C },
    { "MTP_PROPERTY_PHONE_NUMBER_PRIMARY",                   0xDD0D },
    { "MTP_PROPERTY_PHONE_NUMBER_PERSONAL",                  0xDD0E },
    { "MTP_PROPERTY_PHONE_NUMBER_PERSONAL_2",                0xDD0F },
    { "MTP_PROPERTY_PHONE_NUMBER_BUSINESS",                  0xDD10 },
    { "MTP_PROPERTY_PHONE_NUMBER_BUSINESS_2",                0xDD11 },
    { "MTP_PROPERTY_PHONE_NUMBER_MOBILE",                    0xDD12 },
    { "MTP_PROPERTY_PHONE_NUMBER_MOBILE_2",                  0xDD13 },
    { "MTP_PROPERTY_FAX_NUMBER_PRIMARY",                     0xDD14 },
    { "MTP_PROPERTY_FAX_NUMBER_PERSONAL",                    0xDD15 },
    { "MTP_PROPERTY_FAX_NUMBER_BUSINESS",                    0xDD16 },
    { "MTP_PROPERTY_PAGER_NUMBER",                           0xDD17 },
    { "MTP_PROPERTY_PHONE_NUMBER_OTHERS",                    0xDD18 },
    { "MTP_PROPERTY_PRIMARY_WEB_ADDRESS",                    0xDD19 },
    { "MTP_PROPERTY_PERSONAL_WEB_ADDRESS",                   0xDD1A },
    { "MTP_PROPERTY_BUSINESS_WEB_ADDRESS",                   0xDD1B },
    { "MTP_PROPERTY_INSTANT_MESSANGER_ADDRESS",              0xDD1C },
    { "MTP_PROPERTY_INSTANT_MESSANGER_ADDRESS_2",            0xDD1D },
    { "MTP_PROPERTY_INSTANT_MESSANGER_ADDRESS_3",            0xDD1E },
    { "MTP_PROPERTY_POSTAL_ADDRESS_PERSONAL_FULL",           0xDD1F },
    { "MTP_PROPERTY_POSTAL_ADDRESS_PERSONAL_LINE_1",         0xDD20 },
    { "MTP_PROPERTY_POSTAL_ADDRESS_PERSONAL_LINE_2",         0xDD21 },
    { "MTP_PROPERTY_POSTAL_ADDRESS_PERSONAL_CITY",           0xDD22 },
    { "MTP_PROPERTY_POSTAL_ADDRESS_PERSONAL_REGION",         0xDD23 },
    { "MTP_PROPERTY_POSTAL_ADDRESS_PERSONAL_POSTAL_CODE",    0xDD24 },
    { "MTP_PROPERTY_POSTAL_ADDRESS_PERSONAL_COUNTRY",        0xDD25 },
    { "MTP_PROPERTY_POSTAL_ADDRESS_BUSINESS_FULL",           0xDD26 },
    { "MTP_PROPERTY_POSTAL_ADDRESS_BUSINESS_LINE_1",         0xDD27 },
    { "MTP_PROPERTY_POSTAL_ADDRESS_BUSINESS_LINE_2",         0xDD28 },
    { "MTP_PROPERTY_POSTAL_ADDRESS_BUSINESS_CITY",           0xDD29 },
    { "MTP_PROPERTY_POSTAL_ADDRESS_BUSINESS_REGION",         0xDD2A },
    { "MTP_PROPERTY_POSTAL_ADDRESS_BUSINESS_POSTAL_CODE",    0xDD2B },
    { "MTP_PROPERTY_POSTAL_ADDRESS_BUSINESS_COUNTRY",        0xDD2C },
    { "MTP_PROPERTY_POSTAL_ADDRESS_OTHER_FULL",              0xDD2D },
    { "MTP_PROPERTY_POSTAL_ADDRESS_OTHER_LINE_1",            0xDD2E },
    { "MTP_PROPERTY_POSTAL_ADDRESS_OTHER_LINE_2",            0xDD2F },
    { "MTP_PROPERTY_POSTAL_ADDRESS_OTHER_CITY",              0xDD30 },
    { "MTP_PROPERTY_POSTAL_ADDRESS_OTHER_REGION",            0xDD31 },
    { "MTP_PROPERTY_POSTAL_ADDRESS_OTHER_POSTAL_CODE",       0xDD32 },
    { "MTP_PROPERTY_POSTAL_ADDRESS_OTHER_COUNTRY",           0xDD33 },
    { "MTP_PROPERTY_ORGANIZATION_NAME",                      0xDD34 },
    { "MTP_PROPERTY_PHONETIC_ORGANIZATION_NAME",             0xDD35 },
    { "MTP_PROPERTY_ROLE",                                   0xDD36 },
    { "MTP_PROPERTY_BIRTHDATE",                              0xDD37 },
    { "MTP_PROPERTY_MESSAGE_TO",                             0xDD40 },
    { "MTP_PROPERTY_MESSAGE_CC",                             0xDD41 },
    { "MTP_PROPERTY_MESSAGE_BCC",                            0xDD42 },
    { "MTP_PROPERTY_MESSAGE_READ",                           0xDD43 },
    { "MTP_PROPERTY_MESSAGE_RECEIVED_TIME",                  0xDD44 },
    { "MTP_PROPERTY_MESSAGE_SENDER",                         0xDD45 },
    { "MTP_PROPERTY_ACTIVITY_BEGIN_TIME",                    0xDD50 },
    { "MTP_PROPERTY_ACTIVITY_END_TIME",                      0xDD51 },
    { "MTP_PROPERTY_ACTIVITY_LOCATION",                      0xDD52 },
    { "MTP_PROPERTY_ACTIVITY_REQUIRED_ATTENDEES",            0xDD54 },
    { "MTP_PROPERTY_ACTIVITY_OPTIONAL_ATTENDEES",            0xDD55 },
    { "MTP_PROPERTY_ACTIVITY_RESOURCES",                     0xDD56 },
    { "MTP_PROPERTY_ACTIVITY_ACCEPTED",                      0xDD57 },
    { "MTP_PROPERTY_ACTIVITY_TENTATIVE",                     0xDD58 },
    { "MTP_PROPERTY_ACTIVITY_DECLINED",                      0xDD59 },
    { "MTP_PROPERTY_ACTIVITY_REMAINDER_TIME",                0xDD5A },
    { "MTP_PROPERTY_ACTIVITY_OWNER",                         0xDD5B },
    { "MTP_PROPERTY_ACTIVITY_STATUS",                        0xDD5C },
    { "MTP_PROPERTY_OWNER",                                  0xDD5D },
    { "MTP_PROPERTY_EDITOR",                                 0xDD5E },
    { "MTP_PROPERTY_WEBMASTER",                              0xDD5F },
    { "MTP_PROPERTY_URL_SOURCE",                             0xDD60 },
    { "MTP_PROPERTY_URL_DESTINATION",                        0xDD61 },
    { "MTP_PROPERTY_TIME_BOOKMARK",                          0xDD62 },
    { "MTP_PROPERTY_OBJECT_BOOKMARK",                        0xDD63 },
    { "MTP_PROPERTY_BYTE_BOOKMARK",                          0xDD64 },
    { "MTP_PROPERTY_LAST_BUILD_DATE",                        0xDD70 },
    { "MTP_PROPERTY_TIME_TO_LIVE",                           0xDD71 },
    { "MTP_PROPERTY_MEDIA_GUID",                             0xDD72 },
    { 0,                                                     0      },
};

static const CodeEntry sDevicePropCodes[] =
{
    { "MTP_DEVICE_PROPERTY_UNDEFINED",                       0x5000 },
    { "MTP_DEVICE_PROPERTY_BATTERY_LEVEL",                   0x5001 },
    { "MTP_DEVICE_PROPERTY_FUNCTIONAL_MODE",                 0x5002 },
    { "MTP_DEVICE_PROPERTY_IMAGE_SIZE",                      0x5003 },
    { "MTP_DEVICE_PROPERTY_COMPRESSION_SETTING",             0x5004 },
    { "MTP_DEVICE_PROPERTY_WHITE_BALANCE",                   0x5005 },
    { "MTP_DEVICE_PROPERTY_RGB_GAIN",                        0x5006 },
    { "MTP_DEVICE_PROPERTY_F_NUMBER",                        0x5007 },
    { "MTP_DEVICE_PROPERTY_FOCAL_LENGTH",                    0x5008 },
    { "MTP_DEVICE_PROPERTY_FOCUS_DISTANCE",                  0x5009 },
    { "MTP_DEVICE_PROPERTY_FOCUS_MODE",                      0x500A },
    { "MTP_DEVICE_PROPERTY_EXPOSURE_METERING_MODE",          0x500B },
    { "MTP_DEVICE_PROPERTY_FLASH_MODE",                      0x500C },
    { "MTP_DEVICE_PROPERTY_EXPOSURE_TIME",                   0x500D },
    { "MTP_DEVICE_PROPERTY_EXPOSURE_PROGRAM_MODE",           0x500E },
    { "MTP_DEVICE_PROPERTY_EXPOSURE_INDEX",                  0x500F },
    { "MTP_DEVICE_PROPERTY_EXPOSURE_BIAS_COMPENSATION",      0x5010 },
    { "MTP_DEVICE_PROPERTY_DATETIME",                        0x5011 },
    { "MTP_DEVICE_PROPERTY_CAPTURE_DELAY",                   0x5012 },
    { "MTP_DEVICE_PROPERTY_STILL_CAPTURE_MODE",              0x5013 },
    { "MTP_DEVICE_PROPERTY_CONTRAST",                        0x5014 },
    { "MTP_DEVICE_PROPERTY_SHARPNESS",                       0x5015 },
    { "MTP_DEVICE_PROPERTY_DIGITAL_ZOOM",                    0x5016 },
    { "MTP_DEVICE_PROPERTY_EFFECT_MODE",                     0x5017 },
    { "MTP_DEVICE_PROPERTY_BURST_NUMBER",                    0x5018 },
    { "MTP_DEVICE_PROPERTY_BURST_INTERVAL",                  0x5019 },
    { "MTP_DEVICE_PROPERTY_TIMELAPSE_NUMBER",                0x501A },
    { "MTP_DEVICE_PROPERTY_TIMELAPSE_INTERVAL",              0x501B },
    { "MTP_DEVICE_PROPERTY_FOCUS_METERING_MODE",             0x501C },
    { "MTP_DEVICE_PROPERTY_UPLOAD_URL",                      0x501D },
    { "MTP_DEVICE_PROPERTY_ARTIST",                          0x501E },
    { "MTP_DEVICE_PROPERTY_COPYRIGHT_INFO",                  0x501F },
    { "MTP_DEVICE_PROPERTY_SYNCHRONIZATION_PARTNER",         0xD401 },
    { "MTP_DEVICE_PROPERTY_DEVICE_FRIENDLY_NAME",            0xD402 },
    { "MTP_DEVICE_PROPERTY_VOLUME",                          0xD403 },
    { "MTP_DEVICE_PROPERTY_SUPPORTED_FORMATS_ORDERED",       0xD404 },
    { "MTP_DEVICE_PROPERTY_DEVICE_ICON",                     0xD405 },
    { "MTP_DEVICE_PROPERTY_PLAYBACK_RATE",                   0xD410 },
    { "MTP_DEVICE_PROPERTY_PLAYBACK_OBJECT",                 0xD411 },
    { "MTP_DEVICE_PROPERTY_PLAYBACK_CONTAINER_INDEX",        0xD412 },
    { "MTP_DEVICE_PROPERTY_SESSION_INITIATOR_VERSION_INFO",  0xD406 },
    { "MTP_DEVICE_PROPERTY_PERCEIVED_DEVICE_TYPE",           0xD407 },
    { 0,                                                     0      },
};

static const char *getCodeName(uint16_t code, const CodeEntry *table)
{
    const CodeEntry *entry = table;

    while (entry->name)
    {
        if (entry->code == code)
        {
            return entry->name;
        }
        entry++;
    }
    return "UNKNOWN";
}


const char *getOperationCodeName(MtpOperationCode code)
{
    return getCodeName(code, sOperationCodes);
}

const char *getFormatCodeName(MtpObjectFormat code)
{
    return getCodeName(code, sFormatCodes);
}

const char *getObjectPropCodeName(MtpPropertyCode code)
{
    return getCodeName(code, sObjectPropCodes);
}

const char *getDevicePropCodeName(MtpPropertyCode code)
{
    return getCodeName(code, sDevicePropCodes);
}/*
#else
#define getOperationCodeName(code)  ""
#define getFormatCodeName(code)     ""
#define getObjectPropCodeName(code) ""
#define getDevicePropCodeName(code) ""
#endif
*/
#endif
