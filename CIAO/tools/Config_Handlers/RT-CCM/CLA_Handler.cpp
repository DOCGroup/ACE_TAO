// $Id$Exp

#include "CLA_Handler.h"
#include "CIAOServerResources.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    bool CLA_Handler::commandline_arguments (const ServerResourcesDef &src,
                                             ::CIAO::DAnCE::CommandlineArgs &dest)
    {
      ServerCmdlineOptions sco = src.cmdline();

      for(ServerCmdlineOptions::arg_iterator loc = sco.begin_arg();
          loc != sco.end_arg();
          loc++)
        {
          CORBA::ULong len = dest.length();

          dest.length(len+1);

          dest[len] = CORBA::string_dup (loc->c_str ());
        }

      return true;
    }

    ServerCmdlineOptions
    CLA_Handler::commandline_argument (const CIAO::DAnCE::CommandlineArgs &src)
    {
      ServerCmdlineOptions co;

      for  (CORBA::ULong i = 0;
            i < src.length ();
            ++i)
        {
          co.add_arg (src[i].in ());
        }

      return co;
    }
  }
}
