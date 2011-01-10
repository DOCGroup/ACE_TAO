// $Id$

#include "dds4ccm/impl/ndds/Utils.h"
#include "ace/Log_Msg.h"
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
      char * ret = ACE_OS::strdup (tok.next ());
      ACE_OS::free (buf);
      return ret;
    }

    char * get_profile_name (const char * qos_profile)
    {
      char* buf = ACE_OS::strdup (qos_profile);
      ACE_Tokenizer_T<char> tok (buf);
      tok.delimiter_replace ('#', 0);
      tok.next ();
      char * ret = ACE_OS::strdup (tok.next ());
      ACE_OS::free (buf);
      return ret;
    }
  }
}
