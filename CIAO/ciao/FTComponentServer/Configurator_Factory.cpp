#include "Configurator_Factory.h"

#include "ace/Arg_Shifter.h"
#include "ciao/CIAO_common.h"
#include "Configurators/Server_Configurator.h"

namespace CIAO
{
  namespace Deployment
  {
    ComponentServer_Configurator *
    Configurator_Factory::operator() (int &argc, ACE_TCHAR **argv)
    {
      CIAO_TRACE ("Configurator_Factory::operator()");
      this->parse_args (argc, argv);

      ComponentServer_Configurator *ptr = 0;
      ACE_NEW_THROW_EX (ptr,
                        ComponentServer_Configurator (this->rt_),
                        CORBA::NO_MEMORY (TAO::VMCID,
                                          CORBA::COMPLETED_NO));

      return ptr;
    }

    void
    Configurator_Factory::parse_args (int &argc, ACE_TCHAR **argv)
    {
      CIAO_TRACE ("Configurator_Factory::parse_args");
      ACE_Arg_Shifter shifter (argc, argv);

      this->rt_ = false;

      while (shifter.is_anything_left ())
        {
          if (shifter.cur_arg_strncasecmp (ACE_TEXT("-r")) == 0)
            {
              this->rt_ = true;
              shifter.consume_arg ();
            }
          else
            {
              shifter.ignore_arg ();
            }
        }
    }
  }
}
