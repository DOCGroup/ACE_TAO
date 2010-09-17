// $Id$

#include "Messenger_i.h"

#include "tao/AnyTypeCode/TypeCode.h"

#include <sstream>

Messenger_i::Messenger_i()
{
}

Messenger_i::~Messenger_i()
{
}

char * Messenger_i::receive_boxedvalue (::demo::value::idl::boxedValue * b)
{
  std::ostringstream os;
  if (b == 0)
    os << "null boxed values";
  else
    os << "boxed values: " << b->b1 ()->_value() << ", " << b->b2 ()->_value();

  return CORBA::string_dup (os.str().c_str());
}

char * Messenger_i::receive_long (
                                  ::demo::value::idl::boxedLong * p1,
                                  ::demo::value::idl::boxedLong * p2)
{
  std::ostringstream os;
  if (p1 == 0 || p2 == 0)
    os << "one or two null values";
  else if (p1 == p2)
    os << "shared long: " << p1->_value ();
  else
    os << "two longs: " << p1->_value () << ", " << p2->_value ();

  return CORBA::string_dup (os.str().c_str());
}

char * Messenger_i::receive_string (
                                    ::demo::value::idl::boxedString * s1,
                                    ::demo::value::idl::boxedString * s2)
{
  std::ostringstream os;
  if (s1 == 0 || s2 == 0)
    os << "one or two null values";
  else if (s1 == s2)
    os << "shared string: " << s1->_value ();
  else
    os << "two strings: " << s1->_value () << ", " << s2->_value ();

  return CORBA::string_dup (os.str().c_str());
}

char * Messenger_i::receive_list (::demo::value::idl::Node * node)
{
  std::ostringstream os;
  typedef ACE_Vector< ::demo::value::idl::Node *> NodeVector;
  NodeVector l;
  ::demo::value::idl::Node* x = node;

  while (x != 0)
  {
    size_t const len = l.size ();
    size_t i = 0;
    for ( i = 0; i < len; ++i)
    {
      if (l[i] == x)
        break;
    }

    if (l[i] == x)
      break;

    l.push_back (x);
    x = x->next ();
  }

  size_t const len = l.size ();

  os << "list of length: " << len << " -- ";

  for (size_t i = 0; i < len; ++i)
  {
    os << l[i]->id ();
    if (l[i]->next () != 0)
      os << " ";
  }
  return CORBA::string_dup (os.str().c_str());
}

char *
Messenger_i::receive_truncatable (::demo::value::idl::TValue *& v)
{
  v->data (v->data () + 1);
  v->basic_data (v->basic_data () + 1);

  std::ostringstream os;
  os << "truncatable basic_data " << v->basic_data () << " data " << v->data ();
  return CORBA::string_dup (os.str().c_str());
}

char *
Messenger_i::receive_sequence (const ::demo::value::idl::ConfigValues & v)
{
  std::ostringstream os;
  os << "valuetype sequence: " << std::endl;
  CORBA::ULong const len = v.length ();
  for (CORBA::ULong i = 0; i < len; ++i)
  {
    os << v[i]->name();
    const char* str;
    v[i]->value () >>= str;
    os << " - " << str << std::endl;
  }
  os << std::endl;
  return CORBA::string_dup (os.str().c_str());
}

