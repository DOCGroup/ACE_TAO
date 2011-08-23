// $Id$

#include "Message_i.h"


MessageImpl::MessageImpl ()
{
}


MessageImpl::MessageImpl (const char *user, const char *subject, const char *text)
{
  user_ (user);
  subject_ (subject);
  text_ (text);
}


char *
MessageImpl::user ()
{
  return CORBA::string_dup (user_ ());
}


void
MessageImpl::user (const char *s)
{
  user_ (s);
}


char *
MessageImpl::subject ()
{
  return CORBA::string_dup (subject_ ());
}


void
MessageImpl::subject (const char *s)
{
  subject_ (s);
}


char *
MessageImpl::text ()
{
  return CORBA::string_dup (text_ ());
}


void
MessageImpl::text (const char *s)
{
  text_ (s);
}


void
MessageImpl::print ()
{
  ACE_DEBUG ((LM_DEBUG, "User: %s\n", user ()));
  ACE_DEBUG ((LM_DEBUG, "Subject: %s\n", subject ()));
  ACE_DEBUG ((LM_DEBUG, "Text: %s\n", text ()));
}
