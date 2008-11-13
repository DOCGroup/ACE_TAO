// $Id$
/**
 * @file Module_Main.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * 
 * Provides a generic main function used by individual modules.
 */

#ifndef DANCE_MODULE_MAIN_H_
#define DANCE_MODULE_MAIN_H_

#include "ace/Auto_Ptr.h"
#include "ace/Dynamic_Service.h"
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

using namespace DAnCE;

int
ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  DANCE_DISABLE_TRACE ();
  
  auto_ptr<Logger_Service> logger;
  
  try
    {
      Logger_Service
        * dlf = ACE_Dynamic_Service<Logger_Service>::instance ("DAnCE_Logger_Backend_Factory");  
      
      if (!dlf)
        {
          dlf = new Logger_Service;
          logger.reset (dlf);
        }
      
      dlf->init (argc, argv);

      DANCE_DEBUG ((LM_TRACE, DLINFO
                    "Module_main.h - initializing ORB\n"));
      
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
      
      ACE_Log_Msg_Backend * backend = dlf->get_logger_backend(orb);
      
      if (backend != 0)
        {
          backend->open(0);
          ACE_Log_Msg::msg_backend (backend);
          ACE_Log_Msg * ace = ACE_Log_Msg::instance();
          ace->clr_flags(ace->flags());
          ace->set_flags(ACE_Log_Msg::CUSTOM);
        }
      
      DANCE_DEBUG ((LM_TRACE, DLINFO 
                    "Module_Main.h - initializing module instance"));
      DANCE_MODULE_MAIN_CLASS_NAME module_instance;
      
      CORBA::Object_var obj = module_instance.create_object (orb.in (),
                                                             argc, 
                                                             argv);
#ifndef DANCE_MODULE_MAIN_SKIP_ORB_RUN      
      if (!CORBA::is_nil (obj.in ()))
        orb->run ();
      else
        DANCE_ERROR ((LM_ERROR, DLINFO "Module_Main.h - "
                      "Got nil object reference from Module\n"));
      
      DANCE_DEBUG ((LM_TRACE, DLINFO
                    "Module_Main.h - ORB event loop finished, exiting.\n"));
      
      orb->destroy ();
#endif
    }
  catch (...)
    {
      return -1;
    }
  
  return 0;
}

#endif
