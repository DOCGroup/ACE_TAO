// $Id$

#include "dds4ccm/tools/QOS_XML_Handler/Utils.h"
#include "dds4ccm/impl/logger/Log_Macros.h"
#include "ace/Tokenizer_T.h"
#include "ace/SString.h"

namespace DDS4CCM
{
  ACE_CString get_xml_file_name (const char * qos_profile)
  {
    ACE_CString name;

    if (qos_profile)
      {
        char* buf = ACE_OS::strdup (qos_profile);
        ACE_Tokenizer_T<char> tok (buf);
        tok.delimiter_replace ('#', 0);
        const char * file_name = tok.next ();

        if (file_name == 0)
          {
            ACE_OS::free (buf);
            DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                          "get_xml_file_name <%C> - "
                          "Error: malformed qos_profile. Expected format: "
                          "<xml_file_base_name>#<profile_name>\n",
                          qos_profile));
            return "";
          }

        char * ret = ACE_OS::strdup (file_name);
        ACE_OS::free (buf);

        ACE_CString xml(".xml");
        name = ret;
        name = name + xml;
      }

    return name;
  }

  ACE_CString get_profile_name (const char * qos_profile)
  {
    ACE_CString profile_string;

    if (qos_profile)
      {
        char* buf = ACE_OS::strdup (qos_profile);
        ACE_Tokenizer_T<char> tok (buf);
        tok.delimiter_replace ('#', 0);
        const char * lib_name = tok.next ();
        const char * prof_name = tok.next ();

        if (lib_name == 0 || prof_name == 0 || tok.next () != 0)
          {
            ACE_OS::free (buf);
            DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                          "get_profile_name <%C> - "
                          "Error: malformed qos_profile. Expected format: "
                          "<xml_file_base_name>#<profile_name>\n",
                          qos_profile));
            return "";
          }

        char * ret = ACE_OS::strdup (prof_name);
        ACE_OS::free (buf);
        profile_string = ret;
      }

    return profile_string;
  }
}
