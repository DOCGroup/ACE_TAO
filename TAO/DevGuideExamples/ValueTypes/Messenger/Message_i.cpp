#include "_pch.h"
#include "Message_i.h"
#include "MessengerC.h"

#include "tao/AnyTypeCode/TypeCode.h"

#include <iostream>

MessageImpl::MessageImpl
(Message::AddrList &address,
 char const *user,
 char const *subject,
 char const *txt)
  : OBV_Message (address, user, subject, txt)
{
}

MessageImpl::MessageImpl
(char const *address,
 char const *user,
 char const *subject,
 char const *txt)
  : OBV_Message (Message::AddrList (), user, subject, txt)
{
  addAddress (address);
}

::CORBA::ValueBase *
MessageImpl::_copy_value ()
{
  ::CORBA::ValueBase *ret_val = nullptr;
  ACE_NEW_THROW_EX (
    ret_val,
    MessageImpl (
      addrs_ (),
      user (),
      subject (),
      text ()
    ),
    ::CORBA::NO_MEMORY ()
  );
  return ret_val;
}

Message::AddrList *MessageImpl::getAddresses ()
{
  return new AddrList (addrs_ ());
}

void MessageImpl::addAddress (char const *address)
{
  AddrList &al = addrs_ ();
  CORBA::ULong const idx = al.length ();
  al.length (idx + 1);
  al[idx] = address;
}

char *MessageImpl::user ()
{
  return CORBA::string_dup (user_ ());
}
void MessageImpl::user (char const *value)
{
  user_ (value);
}

char *MessageImpl::subject ()
{
  return CORBA::string_dup (subject_ ());
}
void MessageImpl::subject (char const *value)
{
  subject_ (value);
}

char *MessageImpl::text ()
{
  return CORBA::string_dup (text_ ());
}
void MessageImpl::text (char const *value)
{
  text_ (value);
}

void MessageImpl::print ()
{
  std::cout << "Message from : " << user_ () << std::endl;

  AddrList &addrs = addrs_ ();
  if (addrs.length () > 0)
  {
    std::cout << "\tTo : ";
    for (CORBA::ULong i = 0; i < addrs.length (); ++i)
    {
      CORBA::String_var s = CORBA::string_dup (addrs[i]);
      std::cout << s.in() << ";";
    }
    std::cout << std::endl;
  }

  std::cout << "\tSubject : " << subject_ () << std::endl;
  std::cout << "\tText : " << text_ () << std::endl;
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void
MessageFactory::register_new_factory (CORBA::ORB &orb)
{
  CORBA::ValueFactoryBase_var mf = new MessageFactory;
  CORBA::String_var id = _tc_Message->id();
  orb.register_value_factory(id.in(), mf.in());
}

CORBA::ValueBase *
MessageFactory::create_for_unmarshal ()
{
  return new MessageImpl;
}
