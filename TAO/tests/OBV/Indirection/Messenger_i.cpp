#include "Messenger_i.h"

#include "tao/AnyTypeCode/TypeCode.h"
#include "ace/Vector_T.h"

#include <algorithm>
#include <sstream>
#include <vector>

char * Messenger_i::receive_boxedvalue (::demo::value::idl::boxedValue * b)
{
  std::ostringstream os;
  if (b == nullptr)
    {
      os << "null boxed values";
    }
  else
    {
      ::demo::value::idl::boxedLong * const b1 = b->b1 ();
      ::demo::value::idl::boxedLong * const b2 = b->b2 ();
      if (b1 == nullptr || b2 == nullptr)
        {
          os << "null boxed member";
        }
      else
        {
          os << "boxed values: " << b1->_value () << ", " << b2->_value ();
        }
    }

  return CORBA::string_dup (os.str().c_str());
}

char * Messenger_i::receive_long (
                                  ::demo::value::idl::boxedLong * p1,
                                  ::demo::value::idl::boxedLong * p2)
{
  std::ostringstream os;
  if (p1 == nullptr || p2 == nullptr)
    {
      os << "one or two null values";
    }
  else if (p1 == p2)
    {
      os << "shared long: " << p1->_value ();
    }
  else
    {
      os << "two longs: " << p1->_value () << ", " << p2->_value ();
    }

  return CORBA::string_dup (os.str().c_str());
}

char * Messenger_i::receive_string (
                                    ::demo::value::idl::boxedString * s1,
                                    ::demo::value::idl::boxedString * s2)
{
  std::ostringstream os;
  if (s1 == nullptr || s2 == nullptr)
    {
      os << "one or two null values";
    }
  else if (s1 == s2)
    {
      os << "shared string: " << s1->_value ();
    }
  else
    {
      os << "two strings: " << s1->_value () << ", " << s2->_value ();
    }

  return CORBA::string_dup (os.str().c_str());
}

char * Messenger_i::receive_list (::demo::value::idl::Node * node)
{
  std::ostringstream os;
  // These pointers borrow nodes from the request's valuetype graph.
  std::vector<::demo::value::idl::Node *> nodes;
  ::demo::value::idl::Node *x = node;

  while (x != nullptr)
  {
    if (std::find (nodes.begin (), nodes.end (), x) != nodes.end ())
      break;

    nodes.push_back (x);
    x = x->next ();
  }

  size_t const len = nodes.size ();

  os << "list of length: " << len << " -- ";

  for (size_t i = 0; i < len; ++i)
  {
    os << nodes[i]->id ();
    if (nodes[i]->next () != nullptr)
      os << " ";
  }
  return CORBA::string_dup (os.str().c_str());
}

char *
Messenger_i::receive_truncatable (::demo::value::idl::TValue *& v)
{
  if (v == nullptr)
    {
      return CORBA::string_dup ("null truncatable value");
    }

  v->data (v->data () + 1);
  v->basic_data (v->basic_data () + 1);

  std::ostringstream os;
  os << "truncatable basic_data " << v->basic_data () << " data " << v->data ();
  return CORBA::string_dup (os.str().c_str());
}

char *
Messenger_i::receive_sequence (
    ::demo::value::idl::ConfigValues const &v)
{
  std::ostringstream os;
  os << "valuetype sequence: " << std::endl;
  CORBA::ULong const len = v.length ();
  for (CORBA::ULong i = 0; i < len; ++i)
  {
    ::demo::value::idl::ConfigValue * const value = v[i];
    if (value == nullptr)
      {
        os << "null config value" << std::endl;
        continue;
      }

    os << value->name ();
    char const *str {};
    if (value->value () >>= str)
      {
        os << " - " << (str == nullptr ? "" : str) << std::endl;
      }
    else
      {
        os << " - <not a string>" << std::endl;
      }
  }
  os << std::endl;
  return CORBA::string_dup (os.str().c_str());
}

