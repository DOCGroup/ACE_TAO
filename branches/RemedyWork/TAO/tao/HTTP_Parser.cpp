// $Id$

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

static const ACE_TCHAR file_prefix[] = ACE_TEXT ("http:");

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_HTTP_Parser::~TAO_HTTP_Parser (void)
{
}


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

  ACE_TCHAR *hostname = 0;
  ACE_TCHAR *filename = 0;
  const ACE_TCHAR *ptr = 0;
  u_short port = 80;

  if (http_url[0] == '/')
    {
      filename = ACE_OS::strdup (http_url);
    }
  else
    {
      ptr = ACE_OS::strstr (http_url, ACE_TEXT (":"));
      if (ptr)
        port = ACE_OS::atoi (ptr + 1);
      else
        ptr = ACE_OS::strstr (http_url, ACE_TEXT ("/"));

    if(!ptr)
      return 0;
    else
    {
      size_t const host_len = ptr - http_url;
      ACE_NEW_RETURN (hostname, ACE_TCHAR [host_len + 1], 0 );
      ACE_OS::strncpy (hostname, http_url, host_len);
      hostname [host_len] = '\0';
      ptr = ACE_OS::strstr (ptr, ACE_TEXT ("/"));
      if (ptr)
      {
        filename = ACE_OS::strdup(ptr);
      }
      else
        return 0;
    }
  }

  ACE_Message_Block* mb = 0;
  ACE_NEW_THROW_EX (mb,
                    ACE_Message_Block (),
                    CORBA::INTERNAL ());

  // Create a client
  TAO_HTTP_Client client;

  if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - HTTP_Parser::parse_string, getting IOR from <%s> <%s> <%d>\n"),
                  hostname, filename, port));
    }

  // Open the client
  if (client.open (filename,
                   hostname,
                   port) == -1)
  {
      client.close ();
      return 0;
  }

  delete [] hostname;
  ACE_OS::free (filename);

  // Read from it
  if (client.read (mb) <= 0)
  {
      client.close ();
      return 0;
  }

  // We get multiple message blocks back, concatenate them to
  // one large string
  ACE_CString string;
  for (ACE_Message_Block * curr = mb; curr != 0; curr = curr->cont ())
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
