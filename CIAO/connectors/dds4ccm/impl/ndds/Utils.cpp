// $Id$

#include "dds4ccm/impl/ndds/Utils.h"
#include "dds4ccm/impl/logger/Log_Macros.h"
#include "ace/Tokenizer_T.h"

namespace CIAO
{
  namespace NDDS
  {
    char * get_library_name (const char * qos_profile)
    {
      char* buf = ACE_OS::strdup (qos_profile);
      ACE_Tokenizer_T<char> tok (buf);
      tok.delimiter_replace ('#', 0);
      const char * lib_name = tok.next ();

      if (lib_name == 0)
        {
          ACE_OS::free (buf);
          return 0;
        }

      char * ret = ACE_OS::strdup (lib_name);
      ACE_OS::free (buf);
      return ret;
    }

    char * get_profile_name (const char * qos_profile)
    {
      char* buf = ACE_OS::strdup (qos_profile);
      ACE_Tokenizer_T<char> tok (buf);
      tok.delimiter_replace ('#', 0);
      const char * lib_name = tok.next ();
      const char * prof_name = tok.next ();

      if (lib_name == 0 || prof_name == 0)
        {
          ACE_OS::free (buf);
          return 0;
        }

      char * ret = ACE_OS::strdup (prof_name);
      ACE_OS::free (buf);
      return ret;
    }
  }
}
