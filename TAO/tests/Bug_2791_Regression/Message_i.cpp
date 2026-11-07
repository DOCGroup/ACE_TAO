#include "Message_i.h"

MessageImpl::MessageImpl (char const *user, char const *subject, char const *text)
{
  user_ (user);
  subject_ (subject);
  text_ (text);
}

::CORBA::ValueBase *
MessageImpl::_copy_value ()
{
  ::CORBA::ValueBase *ret_val {};
  ACE_NEW_THROW_EX (
    ret_val,
    MessageImpl (
      user_ (),
      subject_ (),
      text_ ()
    ),
    ::CORBA::NO_MEMORY ());
  // The valuetype API transfers ownership of the copied value to its caller.
  return ret_val;
}

char *
MessageImpl::user ()
{
  return CORBA::string_dup (user_ ());
}

void
MessageImpl::user (char const *s)
{
  user_ (s);
}

char *
MessageImpl::subject ()
{
  return CORBA::string_dup (subject_ ());
}

void
MessageImpl::subject (char const *s)
{
  subject_ (s);
}

char *
MessageImpl::text ()
{
  return CORBA::string_dup (text_ ());
}

void
MessageImpl::text (char const *s)
{
  text_ (s);
}

void
MessageImpl::print ()
{
  CORBA::String_var const user_value (user ());
  CORBA::String_var const subject_value (subject ());
  CORBA::String_var const text_value (text ());
  ACE_DEBUG ((LM_DEBUG, "User: %s\n", user_value.in ()));
  ACE_DEBUG ((LM_DEBUG, "Subject: %s\n", subject_value.in ()));
  ACE_DEBUG ((LM_DEBUG, "Text: %s\n", text_value.in ()));
}
