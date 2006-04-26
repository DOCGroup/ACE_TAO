// $Id$

#include "Service_Configuration_Per_ORB.h"


// Return the name of the service configuration file, based on the current ACE
// support for wide characters and unicode
/// Borrowing this from the $ACE_ROOT/test/Service_Config_Test
/// The various config files have the same effect of loading 5
/// new service objects.

const ACE_TCHAR*
file_Service_Config_Test ()
{
#if defined (ACE_USES_WCHAR)
  // When using full Unicode support, use the version of the Service
  // Configurator file appropriate to the platform.
  // For example, Windows Unicode uses UTF-16.
  //
  //          iconv(1) found on Linux and Solaris, for example, can
  //          be used to convert between encodings.
  //
  //          Byte ordering is also an issue, so we should be
  //          generating this file on-the-fly from the UTF-8 encoded
  //          file by using functions like iconv(1) or iconv(3).
#  if defined (ACE_WIN32)
  static const ACE_TCHAR svc_conf[] =
    ACE_TEXT ("Service_Config_Test.UTF-16")
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);
#  else
  static const ACE_TCHAR svc_conf[] =
    ACE_TEXT ("Service_Config_Test.WCHAR_T")
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);
#  endif /* ACE_WIN32 */
#else
    // ASCII (UTF-8) encoded Service Configurator file.
  static const ACE_TCHAR svc_conf[] =
    ACE_TEXT ("Service_Config_Test")
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);
#endif  /* ACE_USES_WCHAR */

  return svc_conf;
}

// Return the name of the service configuration file, for the ORB-based
// service object test

const ACE_TCHAR*
file_Service_Config_ORB_Test ()
{
  // ASCII (UTF-8) encoded Service Configurator file.
  static const ACE_TCHAR svc_conf[] =
    ACE_TEXT ("Service_Config_ORB_Test")
    ACE_TEXT (ACE_DEFAULT_SVC_CONF_EXT);

  return svc_conf;
}

