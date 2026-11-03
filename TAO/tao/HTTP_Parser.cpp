#include "tao/HTTP_Parser.h"

#if (TAO_HAS_HTTP_PARSER == 1)

#include "tao/HTTP_Client.h"
#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/SystemException.h"

#include "ace/Read_Buffer.h"
#include "ace/Malloc_Base.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/CORBA_macros.h"

#include <memory>
#include <string>

namespace
{
  struct Message_Block_Chain_Deleter
  {
    void operator() (ACE_Message_Block *message_block) const
    {
      while (message_block != nullptr)
        {
          ACE_Message_Block *next = message_block->cont ();
          message_block->cont (nullptr);
          message_block->release ();
          message_block = next;
        }
    }
  };
}

static const ACE_TCHAR file_prefix[] = ACE_TEXT ("http:");

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO_HTTP_Parser::match_prefix (const char *nior_string) const
{
  ACE_TString ior_string = ACE_TEXT_CHAR_TO_TCHAR (nior_string);
  return (ACE_OS::strncmp (ior_string.c_str (),
                           ::file_prefix,
                           sizeof (::file_prefix) - 1) == 0);
}

CORBA::Object_ptr
TAO_HTTP_Parser::parse_string (const char *nior,
                               CORBA::ORB_ptr orb)
{
  // Skip the prefix, we know it is there because this method in only
  // called if <match_prefix> returns 1.
  ACE_TString ior = ACE_TEXT_CHAR_TO_TCHAR (nior);
  const ACE_TCHAR *http_url =
    ior.c_str () + sizeof (::file_prefix) + 1;

  std::basic_string<ACE_TCHAR> hostname;
  std::basic_string<ACE_TCHAR> filename;
  const ACE_TCHAR *hostname_arg = nullptr;
  const ACE_TCHAR *ptr = nullptr;
  u_short port = 80;

  if (http_url[0] == '/')
    {
      filename = http_url;
    }
  else
    {
      ptr = ACE_OS::strstr (http_url, ACE_TEXT (":"));
      if (ptr)
        port = static_cast<u_short> (ACE_OS::atoi (ptr + 1));
      else
        ptr = ACE_OS::strstr (http_url, ACE_TEXT ("/"));

    if(!ptr)
      return nullptr;
    else
    {
      size_t const host_len = ptr - http_url;
      hostname.assign (http_url, host_len);
      hostname_arg = hostname.c_str ();
      ptr = ACE_OS::strstr (ptr, ACE_TEXT ("/"));
      if (ptr)
      {
        filename = ptr;
      }
      else
      {
        return nullptr;
      }
    }
  }

  ACE_Message_Block *mb = nullptr;
  ACE_NEW_THROW_EX (mb,
                    ACE_Message_Block (),
                    CORBA::INTERNAL ());
  std::unique_ptr<ACE_Message_Block, Message_Block_Chain_Deleter> mb_owner (mb);

  // Create a client
  TAO_HTTP_Client client;

  if (TAO_debug_level > 4)
    {
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - HTTP_Parser::parse_string, getting IOR from <%s> <%s> <%d>\n"),
                  hostname_arg, filename.c_str (), port));
    }

  // Open the client
  if (client.open (filename.c_str (),
                   hostname_arg,
                   port) == -1)
  {
      client.close ();
      return nullptr;
  }

  // Read from it
  if (client.read (mb_owner.get ()) <= 0)
  {
      client.close ();
      return nullptr;
  }

  // We get multiple message blocks back, concatenate them to
  // one large string
  ACE_CString string;
  for (ACE_Message_Block * curr = mb_owner.get ();
       curr != nullptr;
       curr = curr->cont ())
    string += curr->rd_ptr();

  return orb->string_to_object (string.c_str());
}

ACE_STATIC_SVC_DEFINE (TAO_HTTP_Parser,
                       ACE_TEXT ("HTTP_Parser"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_HTTP_Parser),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO, TAO_HTTP_Parser)

TAO_END_VERSIONED_NAMESPACE_DECL


#endif /* TAO_HAS_HTTP_PARSER == 1 */
