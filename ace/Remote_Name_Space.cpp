// Remote_Name_Space.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Remote_Name_Space.h"

ACE_RCSID(ace, Remote_Name_Space, "$Id$")

int
ACE_Remote_Name_Space::open (const char *servername, u_short port)
{
  ACE_TRACE ("ACE_Remote_Name_Space::open");
  ACE_INET_Addr servaddr;
  
  // Initialize Addr
  if (servaddr.set (port, servername) == -1)
    return -1;

  // Connect to Name Server process.
  if (this->ns_proxy_.open (servaddr) == -1)
    return -1;
      
  return 0;
}

ACE_Remote_Name_Space::ACE_Remote_Name_Space (void)
{
  ACE_TRACE ("ACE_Remote_Name_Space::ACE_Remote_Name_Space");
}

ACE_Remote_Name_Space::ACE_Remote_Name_Space (const char *hostname, 
					      u_short port)
{
  ACE_TRACE ("ACE_Remote_Name_Space::ACE_Remote_Name_Space");
  if (this->open (hostname, port) == -1)
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("ACE_Remote_Name_Space::ACE_Remote_Name_Space")));
}

int 
ACE_Remote_Name_Space::bind (const ACE_WString &name, 
			     const ACE_WString &value, 
			     const char *type)
{
  ACE_TRACE ("ACE_Remote_Name_Space::bind");
  ACE_Name_Request request (ACE_Name_Request::BIND,
                            name.fast_rep (),
			    name.length () * sizeof (ACE_USHORT16),
			    value.fast_rep (),
			    value.length () * sizeof (ACE_USHORT16),
			    type, 
                            ACE_OS::strlen (type));
  return this->ns_proxy_.request_reply (request);
}

int 
ACE_Remote_Name_Space::rebind (const ACE_WString &name, 
			       const ACE_WString &value, 
			       const char *type)
{
  ACE_TRACE ("ACE_Remote_Name_Space::rebind");
  ACE_Name_Request request (ACE_Name_Request::REBIND,
                            name.fast_rep (),
			    name.length () * sizeof (ACE_USHORT16),
			    value.fast_rep (),
			    value.length () * sizeof (ACE_USHORT16),
			    type, 
                            ACE_OS::strlen (type));
  return this->ns_proxy_.request_reply (request);
}

int 
ACE_Remote_Name_Space::resolve (const ACE_WString &name, 
				ACE_WString &value, 
				char *&type)
{
  ACE_TRACE ("ACE_Remote_Name_Space::resolve");
  ACE_Name_Request request (ACE_Name_Request::RESOLVE,
                            name.fast_rep (),
			    name.length () * sizeof (ACE_USHORT16),
			    0, 0, 0, 0);

  if (this->ns_proxy_.send_request (request) == -1)
    return -1;

  ACE_Name_Request reply;

  if (this->ns_proxy_.recv_reply (reply) == -1)
    return -1;

  ACE_WString temp (reply.value (), reply.value_len () / sizeof (ACE_USHORT16));
  value = temp;
  ACE_NEW_RETURN (type, 
                  char[reply.type_len () + 1],
                  -1);
  ACE_OS::strcpy (type, reply.type ());
  return 0;
}

int  
ACE_Remote_Name_Space::unbind (const ACE_WString &name)
{
  ACE_TRACE ("ACE_Remote_Name_Space::unbind");
  ACE_Name_Request request (ACE_Name_Request::UNBIND,
                            name.fast_rep (),
			    name.length () * sizeof (ACE_USHORT16),
			    0, 0, 0, 0);
  return this->ns_proxy_.request_reply (request);
}

int 
ACE_Remote_Name_Space::list_names (ACE_WSTRING_SET &set, 
				   const ACE_WString &pattern)
{
  ACE_TRACE ("ACE_Remote_Name_Space::list_names");
  ACE_Name_Request request (ACE_Name_Request::LIST_NAMES,
                            pattern.fast_rep (),
			    pattern.length () * sizeof (ACE_USHORT16),
			    0, 0, 0, 0);
  if (this->ns_proxy_.send_request (request) == -1)
    return -1;
  
  ACE_Name_Request reply (0, 0, 0, 0, 0, 0, 0, 0);

  while (reply.msg_type () != ACE_Name_Request::MAX_ENUM)
    {
      if (this->ns_proxy_.recv_reply (reply) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("%p\n"),
                           ASYS_TEXT ("ACE_Remote_Name_Space::list_names")),
                           -1);
      if (reply.msg_type () != ACE_Name_Request::MAX_ENUM)
	{
	  ACE_WString name (reply.name (),
                            reply.name_len () / sizeof (ACE_USHORT16));
	  set.insert (name);
	}
    }
  return 0;
}

int 
ACE_Remote_Name_Space::list_values (ACE_WSTRING_SET &set, 
				    const ACE_WString &pattern)
{
  ACE_TRACE ("ACE_Remote_Name_Space::list_values");
  ACE_Name_Request request (ACE_Name_Request::LIST_VALUES,
                            pattern.fast_rep (),
			    pattern.length () * sizeof (ACE_USHORT16),
			    0, 0, 0, 0);
  if (this->ns_proxy_.send_request (request) == -1)
    return -1;
  
  ACE_Name_Request reply (0, 0, 0, 0, 0, 0, 0, 0);

  while (reply.msg_type () != ACE_Name_Request::MAX_ENUM)
    {
      if (this->ns_proxy_.recv_reply (reply) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("%p\n"),
                           ASYS_TEXT ("ACE_Remote_Name_Space::list_values")),
                           -1);
      if (reply.msg_type () != ACE_Name_Request::MAX_ENUM)
	{
	  ACE_WString value (reply.value (),
                             reply.value_len () / sizeof (ACE_USHORT16));
	  set.insert (value);
	}
    }

  return 0;
}

int 
ACE_Remote_Name_Space::list_types (ACE_WSTRING_SET &set, 
				   const ACE_WString &pattern)
{
  ACE_TRACE ("ACE_Remote_Name_Space::list_types");
  ACE_Name_Request request (ACE_Name_Request::LIST_TYPES,
                            pattern.fast_rep (),
			    pattern.length () * sizeof (ACE_USHORT16),
			    0, 0, 0, 0);

  if (this->ns_proxy_.send_request (request) == -1)
    return -1;
  
  ACE_Name_Request reply (0, 0, 0, 0, 0, 0, 0, 0);

  while (reply.msg_type () != ACE_Name_Request::MAX_ENUM)
    {
      if (this->ns_proxy_.recv_reply (reply) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("%p\n"),
                           ASYS_TEXT ("ACE_Remote_Name_Space::list_values")),
                           -1);
      if (reply.msg_type () != ACE_Name_Request::MAX_ENUM)
	{
	  ACE_WString type (reply.type ());
	  set.insert (type);
	}
    }

  return 0;
}

int 
ACE_Remote_Name_Space::list_name_entries (ACE_BINDING_SET &set, 
					  const ACE_WString &pattern)
{
  ACE_TRACE ("ACE_Remote_Name_Space::list_names");
  ACE_Name_Request request (ACE_Name_Request::LIST_NAME_ENTRIES,
                            pattern.fast_rep (),
			    pattern.length () * sizeof (ACE_USHORT16),
			    0, 0, 0, 0);

  if (this->ns_proxy_.send_request (request) == -1)
    return -1;
  
  ACE_Name_Request reply (0, 0, 0, 0, 0, 0, 0, 0);

  while (reply.msg_type () != ACE_Name_Request::MAX_ENUM)
    {
      if (this->ns_proxy_.recv_reply (reply) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("%p\n"),
                           ASYS_TEXT ("ACE_Remote_Name_Space::list_names")),
                           -1);
      if (reply.msg_type () != ACE_Name_Request::MAX_ENUM)
	{
	  ACE_WString name (reply.name (),
                            reply.name_len () / sizeof (ACE_USHORT16));
	  ACE_WString value (reply.value (),
                             reply.value_len () / sizeof (ACE_USHORT16));
	  ACE_Name_Binding entry (name,
                                  value,
                                  reply.type ());
	  if (set.insert (entry) == -1)
	    return -1;
	}
    }
  return 0;
}

int 
ACE_Remote_Name_Space::list_value_entries (ACE_BINDING_SET &set, 
					   const ACE_WString &pattern)
{
  ACE_TRACE ("ACE_Remote_Name_Space::list_values");
  ACE_Name_Request request (ACE_Name_Request::LIST_VALUE_ENTRIES,
                            pattern.fast_rep (),
			    pattern.length () * sizeof (ACE_USHORT16),
			    0, 0, 0, 0);

  if (this->ns_proxy_.send_request (request) == -1)
    return -1;
  
  ACE_Name_Request reply (0, 0, 0, 0, 0, 0, 0, 0);

  while (reply.msg_type () != ACE_Name_Request::MAX_ENUM)
    {
      if (this->ns_proxy_.recv_reply (reply) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("%p\n"),
                           ASYS_TEXT ("ACE_Remote_Name_Space::list_values")),
                           -1);
      if (reply.msg_type () != ACE_Name_Request::MAX_ENUM)
	{
	  ACE_WString name (reply.name (),
                            reply.name_len () / sizeof (ACE_USHORT16));
	  ACE_WString value (reply.value (),
                             reply.value_len () / sizeof (ACE_USHORT16));
	  ACE_Name_Binding entry (name,
                                  value,
                                  reply.type());
	  if (set.insert (entry) == -1)
	    return -1;
	}
    }
  return 0;
}

int 
ACE_Remote_Name_Space::list_type_entries (ACE_BINDING_SET &set, 
					  const ACE_WString &pattern)
{
  ACE_TRACE ("ACE_Remote_Name_Space::list_types");
  ACE_Name_Request request (ACE_Name_Request::LIST_TYPE_ENTRIES,
                            pattern.fast_rep (),
			    pattern.length () * sizeof (ACE_USHORT16),
			    0, 0, 0, 0);

  if (this->ns_proxy_.send_request (request) == -1)
    return -1;
  
  ACE_Name_Request reply (0, 0, 0, 0, 0, 0, 0, 0);

  while (reply.msg_type () != ACE_Name_Request::MAX_ENUM)
    {
      if (this->ns_proxy_.recv_reply (reply) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("%p\n"),
                           ASYS_TEXT ("ACE_Remote_Name_Space::list_values")),
                          -1);
      if (reply.msg_type () != ACE_Name_Request::MAX_ENUM)
	{
	  ACE_WString name (reply.name (),
                            reply.name_len () / sizeof (ACE_USHORT16));
	  ACE_WString value (reply.value (),
                             reply.value_len () / sizeof (ACE_USHORT16));
	  ACE_Name_Binding entry (name,
                                  value,
                                  reply.type ());
	  if (set.insert (entry) == -1)
	      return -1;
	}
    }
  return 0;
}

ACE_Remote_Name_Space::~ACE_Remote_Name_Space (void)
{
  ACE_TRACE ("ACE_Remote_Name_Space::~ACE_Remote_Name_Space");
}

void
ACE_Remote_Name_Space::dump (void) const
{
  ACE_TRACE ("ACE_Remote_Name_Space::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->ns_proxy_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

