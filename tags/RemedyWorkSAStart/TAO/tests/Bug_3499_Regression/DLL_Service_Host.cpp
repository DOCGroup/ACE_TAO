// $Id$

#include "DLL_Service.h"
#include "ace/DLL.h"
#include "ace/Service_Gestalt.h"
#include "ace/Service_Config.h"
#include "ace/CORBA_macros.h"
#include "ace/OS_NS_unistd.h"
//
// main
//
int ACE_TMAIN (int argc, ACE_TCHAR * argv [])
{
  try
  {
    ACE_DLL module;

    ACE_Service_Gestalt * gestalt = 0;
    ACE_NEW_NORETURN (gestalt, ACE_Service_Gestalt ());
    if (gestalt == 0)
      {
        ACE_throw_bad_alloc;
      }

    ACE_Intrusive_Auto_Ptr <ACE_Service_Gestalt> auto_clean (gestalt);

    // Without the following line, the application will crash while ACE
    // is *cleaning* the ACE_Service_Config::global () object. We want
    // to force all services to be loaded under this configuration.
    ACE_Service_Config_Guard guard (gestalt);

    if (module.open (ACE_TEXT ("Bug_3499_Regression_ACE_DLL_TAO_Service")) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("failed to load ACE_DLL_TAO_Service\n")),
                         -1);
    }

    void * symbol = module.symbol (ACE_TEXT ("_make_ACE_DLL_TAO_Service"));

    if (symbol == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("failed to load symbol _make_ACE_DLL_TAO_Service\n")),
                         -1);
    }

    typedef ACE_DLL_Service * (*factory_type) (void);
    ptrdiff_t tmp_ptr = reinterpret_cast <ptrdiff_t> (symbol);
    factory_type f = reinterpret_cast <factory_type> (tmp_ptr);

    ACE_DLL_Service * svc = (*f) ();

    if (svc != 0)
    {
      // Initialize the service.
      //
      // If '-ORBGestalt CURRENT' does not appear in the command-line options,
      // which is a fix for the bug in question, then the service will fail
      // when trying to resolve to RootPOA.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) %T - %M - initializing the loaded service\n")));
      svc->init (argc, argv);

      // Sleep for a few seconds
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) %T - %M - sleeping for 5 seconds\n")));
      ACE_OS::sleep (5);

      // Finalize the service.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) %T - %M - finalizing the service\n")));
      svc->fini ();

      // Destroy the service.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) %T - %M - destroying the service\n")));
      svc->destroy ();
    }
  }
  catch (...)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%t) %T - %M - caught unknown exception\n")));
  }

  return 0;
}
