// $Id$

#include "Process_Priority.h"
#include "DAnCE/DAnCE_PropertiesC.h"
#include "Deployment/Deployment_StartErrorC.h"

#include "DAnCE/Logger/Log_Macros.h"

#include <sstream>
#include "ace/Auto_Ptr.h"

namespace DAnCE
{
  Process_Priority::Process_Priority (void)
  {
  }

  // Implementation skeleton destructor
  Process_Priority::~Process_Priority (void)
  {
  }

  char * Process_Priority::type (void)
  {
    return CORBA::string_dup (DAnCE::DANCE_LM_PROCESSPRIORITY);
  }

  void Process_Priority::configure (const ::Deployment::Property & prop)
  {
    CORBA::Long prio;
    
    if (!(prop.value >>= prio))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Process_Priority::configure - ")
                         ACE_TEXT ("Unable to extract priority value from config property\n")));
        throw ::Deployment::PlanError (prop.name.in (),
                                       "Unable to extract priority value from config property.");
      }
    
    int retval = ACE_OS::thr_setprio (ACE_OS::thr_self (),
                                      static_cast<int> (prio));
    
    if (retval != 0)
      {
        std::stringstream str;
        ACE_Auto_Basic_Array_Ptr<char> safe_error (ACE_OS::strerror (ACE_OS::last_error ()));
        
        str << "Unable to set process priority to <" << prio << ">: " 
            << safe_error.get ();
        std::string message = str.str ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Process_Priority::configure - %C\n"),
                         message.c_str ()));

        throw ::Deployment::StartError (prop.name.in (),
                                        message.c_str ());

      }
    
  }
}

extern "C"
{
  DAnCE::LocalityConfiguration_ptr create_Process_Priority (void)
  {
    DAnCE::LocalityConfiguration_ptr retval (0);
    
    ACE_NEW_RETURN (retval,
                    DAnCE::Process_Priority (),
                    0);

    return retval;
  }
}
