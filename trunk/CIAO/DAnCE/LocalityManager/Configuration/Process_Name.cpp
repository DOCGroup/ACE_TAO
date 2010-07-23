// $Id$

#include "Process_Name.h"
#include "DAnCE/DAnCE_PropertiesC.h"
#include "Deployment/Deployment_StartErrorC.h"

#include "DAnCE/Logger/Log_Macros.h"

#if defined (LINUX_VERSION_CODE) && defined (KERNEL_VERSION)
# if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,9))
#include <sys/prctl.h>
#include <sstream>

#include "ace/Auto_Ptr.h"

#endif
#endif

namespace DAnCE
{
  Process_Name::Process_Name (void)
  {
  }

  // Implementation skeleton destructor
  Process_Name::~Process_Name (void)
  {
  }

  char * Process_Name::type (void)
  {
    return CORBA::string_dup (DAnCE::DANCE_LM_PROCESSNAME);
  }

  void Process_Name::configure (const ::Deployment::Property & prop)
  {
#if defined (LINUX_VERSION_CODE) && defined (KERNEL_VERSION)
# if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,9))
    CORBA::String_var pname;
    
    prop.value >>= CORBA::Any::to_string (pname, 0);
    unsigned long arg (0);

    ACE_Auto_Basic_Array_Ptr<char> safe_array;

    if (!(ACE_OS::strlen (pname.in ()) <= 16))
      {
        char *tmp (0);
        ACE_NEW_THROW_EX (tmp,
			  char[16],
			  CORBA::NO_MEMORY ());
        
        safe_array.reset (tmp);
        
        ACE_OS::strncpy (tmp, pname.in (), 15);
        tmp[15] = '\0';
        
        arg = reinterpret_cast<unsigned long> (tmp);
      }
    else
      arg = reinterpret_cast<unsigned long> (pname.in ());

    int retval = ::prctl (PR_SET_NAME,
                          arg);
    
    if (retval != 0)
      {
        std::stringstream str;
        ACE_Auto_Basic_Array_Ptr<char> safe_error (ACE_OS::strerror (ACE_OS::last_error ()));
        
        str << "Unable to set process name to <" << pname.in () << ">: " 
            << safe_error.get ();
        std::string message = str.str ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT ("Process_Name::configure - %C\n"),
                         message.c_str ()));

        throw ::Deployment::StartError (prop.name.in (),
                                        message.c_str ());
      }
    
    std::cerr << "***********setting process name to " << pname.in () << std::endl;

    return;
#endif
#endif
    
    throw ::Deployment::StartError (prop.name.in (),
                                    "Setting process name not supported on this platform");
  }
}

extern "C"
{
  DAnCE::LocalityConfiguration_ptr create_Process_Name (void)
  {
    DAnCE::LocalityConfiguration_ptr retval (0);
    
    ACE_NEW_THROW_EX (retval,
                      DAnCE::Process_Name (),
                      CORBA::NO_MEMORY ());

    return retval;
  }
}
