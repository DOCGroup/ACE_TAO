// $Id$

#include "CPU_Affinity.h"
#include "dance/DAnCE_PropertiesC.h"
#include "dance/Deployment/Deployment_StartErrorC.h"
#include "dance/Logger/Log_Macros.h"
#include "ace/os_include/os_sched.h"
#include "ace/Auto_Ptr.h"
#include "ace/Tokenizer_T.h"
#include "ace/OS_NS_unistd.h"
#include <sstream>

namespace DAnCE
{
  CPU_Affinity::CPU_Affinity (void)
  {
  }

  // Implementation skeleton destructor
  CPU_Affinity::~CPU_Affinity (void)
  {
  }

  char * CPU_Affinity::type (void)
  {
    return CORBA::string_dup (DAnCE::DANCE_LM_CPUAFFINITY);
  }

  void CPU_Affinity::configure (const ::Deployment::Property & prop)
  {
#if defined (ACE_HAS_SCHED_SETAFFINITY)
    const char *extracted_affinity = 0;

    if (! (prop.value >>= CORBA::Any::to_string (extracted_affinity, 0)))
      {
        DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
                     (LM_ERROR, DLINFO
                      ACE_TEXT ("CPU_Affinity::configure - ")
                      ACE_TEXT ("Unable to extract CPU affinity string")));
        throw ::Deployment::StartError (prop.name.in (),
                                        "Unable to extract CPU affinity string");
      }

    char *affinity = ACE_OS::strdup (extracted_affinity);

    ACE_Tokenizer_T<char> tokenizer(affinity);
    tokenizer.delimiter (',');

    char *token = 0;
    cpu_set_t mask;

    CPU_ZERO (&mask);

    while ((token = tokenizer.next ()))
      {
        int i = ACE_OS::atoi (token);

        if (i >= 0)
          {
            DANCE_DEBUG (DANCE_LOG_MINOR_EVENT,
                         (LM_DEBUG, DLINFO
                          ACE_TEXT ("CPU_Affinity::configure - ")
                          ACE_TEXT ("Toggling affinity for CPU %i\n"),
                          i));
            CPU_SET (i, &mask);
          }
        else
          {
            DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
                         (LM_ERROR, DLINFO
                          ACE_TEXT ("CPU_Affinity::configure - ")
                          ACE_TEXT ("All affinity values should be greater than 0")));
            throw ::Deployment::StartError (prop.name.in (),
                                            "All affinity values should be greater than 0");
          }
      }

    ACE_OS::free (affinity);

    pid_t const pid = ACE_OS::getpid ();

    int retval = ::sched_setaffinity (pid,
                                      sizeof (cpu_set_t),
                                      &mask);

    if (retval != 0)
      {
        std::stringstream str;
        ACE_Auto_Basic_Array_Ptr<char> safe_error (ACE_OS::strerror (ACE_OS::last_error ()));

        str << "Unable to set CPU affinity to <" << affinity << ">: "
            << safe_error.get ();
        std::string message = str.str ();

        DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
                     (LM_ERROR, DLINFO
                      ACE_TEXT ("CPU_Affinity::configure - %C\n"),
                      message.c_str ()));

        throw ::Deployment::StartError (prop.name.in (),
                                        message.c_str ());
      }

    return;

#endif

    throw ::Deployment::StartError (prop.name.in (),
                                    "CPU Affinity not supported on this platform");
  }
}

extern "C"
{
  DAnCE::LocalityConfiguration_ptr create_CPU_Affinity (void)
  {
    DAnCE::LocalityConfiguration_ptr retval (0);

    ACE_NEW_RETURN (retval,
                    DAnCE::CPU_Affinity (),
                    0);

    return retval;
  }
}
