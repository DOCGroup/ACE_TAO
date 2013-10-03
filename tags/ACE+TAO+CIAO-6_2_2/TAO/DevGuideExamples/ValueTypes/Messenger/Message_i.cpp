// $Id$

#include "_pch.h"
#include "Message_i.h"
#include "MessengerC.h"

#include "tao/AnyTypeCode/TypeCode.h"

#include <iostream>

MessageImpl::MessageImpl()
{
}

MessageImpl::~MessageImpl()
{
}

MessageImpl::MessageImpl
(Message::AddrList &address,
 const char* user,
 const char* subject,
 const char* txt
 ) : OBV_Message(address, user, subject, txt)
{
}

MessageImpl::MessageImpl
(const char* address,
 const char* user,
 const char* subject,
 const char* txt
 ) : OBV_Message(Message::AddrList(), user, subject, txt)
{
  addAddress(address);
}

::CORBA::ValueBase *
MessageImpl::_copy_value (void)
{
  ::CORBA::ValueBase *ret_val= 0;
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

Message::AddrList* MessageImpl::getAddresses() {
  return new AddrList(addrs_());
}

void MessageImpl::addAddress(const char* s) {
  AddrList& al = addrs_();
  CORBA::ULong idx = al.length();
  al.length(idx + 1);
  al[idx] = s;
}

char* MessageImpl::user() {
  return CORBA::string_dup(user_());
}
void MessageImpl::user(const char* s) {
  user_(s);
}

char* MessageImpl::subject() {
  return CORBA::string_dup(subject_());
}
void MessageImpl::subject(const char* s) {
  subject_(s);
}

char* MessageImpl::text() {
  return CORBA::string_dup(text_());
}
void MessageImpl::text(const char* s) {
  text_(s);
}

void MessageImpl::print() {
  std::cout << "Message from : " << user_() << std::endl;

  AddrList& addrs = addrs_();
  if (addrs.length() > 0) {
    std::cout << "\tTo : ";
    for (CORBA::ULong i = 0; i < addrs.length(); ++i) {
      CORBA::String_var s = CORBA::string_dup(addrs[i]);
      std::cout << s.in() << ";";
    }
    std::cout << std::endl;
  }

  std::cout << "\tSubject : " << subject_() << std::endl;
  std::cout << "\tText : " << text_() << std::endl;
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void
MessageFactory::register_new_factory(CORBA::ORB& orb) {
  CORBA::ValueFactoryBase_var mf = new MessageFactory;
  CORBA::String_var id = _tc_Message->id();
  orb.register_value_factory(id.in(), mf.in());
}

CORBA::ValueBase*
MessageFactory::create_for_unmarshal()
{
  return new MessageImpl;
}
