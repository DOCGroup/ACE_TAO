/**
 * @file Bug_4008_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 4008
 *   http://bugzilla.dre.vanderbilt.edu/show_bug.cgi?id=4008
 */

#define ACE_SVC_HAS_DLL 0  /* Needed to ensure Global_Macros Work */

#include "test_config.h"

#include "ace/svc_export.h"
#include "ace/Service_Config.h"
#include "ace/Service_Object.h"

struct TestService : ACE_Service_Object
{
  /// Initializes object when dynamic linking occurs.
  virtual int init(int, ACE_TCHAR *[]) {
      ACE_ERROR_RETURN((LM_INFO, ACE_TEXT("TestService::init().\n")), 0);
  };

  /// Terminates object when dynamic unlinking occurs.
  virtual int fini(void) {
      ACE_ERROR_RETURN((LM_INFO, ACE_TEXT("TestService::fini().\n")), 0);
  }

  /// Returns information on a service object.
  virtual int info(ACE_TCHAR **, size_t = 0) const {
      ACE_ERROR_RETURN((LM_INFO, ACE_TEXT("TestService::info().\n")), 0);
  }
};

ACE_SVC_FACTORY_DEFINE(TestService);
ACE_STATIC_SVC_DEFINE (TestService,
                       ACE_TEXT ("TestService"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME(TestService),
                       (ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ),
                       false);
ACE_STATIC_SVC_REQUIRE(TestService);

int
run_main (int, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_4008_Regression_Test"));

  int result = 0;

  do { // Without fix 4008 this ACE_Service_Config::open will fail in ACE_Service_Gestalt::open_i().

      if ((result = ACE_Service_Config::open(argv[0], ACE_DEFAULT_LOGGER_KEY, false, false, false)) == -1) {
          ACE_DEBUG((LM_ERROR, ACE_TEXT("ERROR: Failed to open Service Configurator.\n"))); break;
      } else if (result > 0) {
          ACE_DEBUG((LM_INFO,  ACE_TEXT("WARNING: Gestalt failed to load (%d) entries - ignored.\n"), result));
      } else if ((result = ACE_Service_Config::initialize(ACE_TEXT("TestService"), 0)) == -1) {
          ACE_DEBUG((LM_INFO,  ACE_TEXT("ERROR: Failed to initialize TestService static service.\n")));
      }

      result = ACE_Service_Config::close();

  } while (0);

  ACE_END_TEST;
  return result;
}
