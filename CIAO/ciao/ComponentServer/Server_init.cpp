// $Id$

#include "Server_init.h"

#include <ccm/CCM_StandardConfiguratorC.h>
#include "ciao/Valuetype_Factories/Cookies.h"
#include "ciao/Logger/Log_Macros.h"
#include "Client_init.h"
#include "ace/Tokenizer_T.h"

namespace CIAO
{
  int
  Server_init (CORBA::ORB_ptr o)
  {
    Client_init (o);
    CIAO_REGISTER_VALUE_FACTORY (o,
                                 CIAO::Cookie_Impl_init,
                                 Components::Cookie);
    return 0;
  }

  namespace Utility
  {
    int write_IOR (const ACE_TCHAR *pathname, const char *ior)
    {
      FILE* ior_output_file_ = ACE_OS::fopen (pathname, ACE_TEXT("w"));

      if (ior_output_file_)
        {
          ACE_OS::fprintf (ior_output_file_,
                          "%s",
                          ior);
          ACE_OS::fclose (ior_output_file_);
          return 0;
        }

      return -1;
    }
  } /* namespace Utility */
} /* namespace CIAO */
