// $Id$
/**
 * @file Module_Main.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * Provides a generic main function used by individual modules.
 */

#ifndef DANCE_MODULE_MAIN_H_
#define DANCE_MODULE_MAIN_H_

#include "ace/Dynamic_Service.h"
#include "tao/ORB.h"
#include "tao/Object.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/Logger/Logger_Service.h"

#ifndef DANCE_MODULE_MAIN_CLASS_NAME
#error Module_Main.h used without DANCE_MODULE_MAIN_CLASS_NAME defined.
#endif

//#ifndef DANCE_MODULE_MAIN_INCLUDE_NAME
//#error Module_Main.h used without DANCE_MODULE_MAIN_INCLUDE_NAME defined.
//#endif

//#include DANCE_MODULE_MAIN_INCLUDE_NAME

int
ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  DANCE_DISABLE_TRACE ();

  int retval = 0;

  try
    {
      DAnCE::Logger_Service
        * dlf = ACE_Dynamic_Service<DAnCE::Logger_Service>::instance ("DAnCE_Logger");

      if (dlf)
        {
          dlf->init (argc, argv);
        }

      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                    ACE_TEXT("Module_main.h - initializing ORB\n")));

      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                   ACE_TEXT("Module_Main.h - initializing module instance\n")));

      DANCE_MODULE_MAIN_CLASS_NAME module_instance;

      CORBA::Object_var obj = module_instance.create_object (orb.in (),
                                                             argc,
                                                             argv);

#ifndef DANCE_MODULE_MAIN_SKIP_ORB_RUN
      if (!CORBA::is_nil (obj.in ()))
        {
          orb->run ();
        }
      else
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO
                       ACE_TEXT("Module_Main.h - Error: No object created.\n")));
          retval = -1;
        }
      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                   ACE_TEXT("Module_Main.h - ORB event loop finished, exiting.\n")));
#endif

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Module_Main.h");
      retval = -1;
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, "Module_Main.h - Error: Unknown exception.\n"));
      retval = -1;
    }

  return retval;
}

#endif
