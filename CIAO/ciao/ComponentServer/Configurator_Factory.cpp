#include "Configurator_Factory.h"
// $Id$
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
      bool const rt = this->parse_args (argc, argv);

      ComponentServer_Configurator *ptr = 0;
      ACE_NEW_THROW_EX (ptr,
                        ComponentServer_Configurator (rt),
                        CORBA::NO_MEMORY (TAO::VMCID,
                                          CORBA::COMPLETED_NO));

      return ptr;
    }

    bool
    Configurator_Factory::parse_args (int &argc, ACE_TCHAR **argv)
    {
      CIAO_TRACE ("Configurator_Factory::parse_args");
      ACE_Arg_Shifter shifter (argc, argv);
      bool retval = false;

      while (shifter.is_anything_left ())
        {
          if (shifter.cur_arg_strncasecmp (ACE_TEXT("-r")) == 0)
            {
              retval = true;
              shifter.consume_arg ();
            }
          else
            {
              shifter.ignore_arg ();
            }
        }
      return retval;
    }
  }
}
