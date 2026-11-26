#include "orbsvcs/Notify/Name_Value_Pair.h"
#include "orbsvcs/Notify/Property_T.h"
#include "ace/OS_NS_ctype.h"

#include <cstdlib>
#include <cstring>
#include <string>

namespace
{
  ACE_UINT64 string_to_uint64 (char const *s)
  {
    size_t const len = std::strlen (s);
    if (len == 0)
    {
      return 0;
    }
    if (! ACE_OS::ace_isdigit(s[0]))
    {
      return 0;
    }

    ACE_UINT64 t {};
    for (size_t i {}; i < len; ++i)
    {
      if (ACE_OS::ace_isdigit(s[i]) == 0)
      {
        break;
      }
      t *= 10;
      t += (s[i] - '0');
    }
    return t;
  }
}

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  NVP::NVP(const TAO_Notify_Property_Short& p)
    : name(p.name())
  {
    std::string const string_value = std::to_string (p.value ());
    value = string_value.c_str ();
  }

  NVP::NVP(const TAO_Notify_Property_Long& p)
    : name(p.name())
  {
    std::string const string_value = std::to_string (static_cast<long> (p.value ()));
    value = string_value.c_str ();
  }

  NVP::NVP(const TAO_Notify_Property_Time& p)
    : name(p.name())
  {
    ACE_TCHAR buf[128];
    ACE_UINT64 us = p.value();
    ACE_OS::sprintf(buf, ACE_UINT64_FORMAT_SPECIFIER, us);
    value = ACE_TEXT_ALWAYS_CHAR (buf);
  }

  NVP::NVP(const TAO_Notify_Property_Boolean& p)
    : name(p.name())
  {
    value = p.value() ? "true" : "false";
  }

  NVP::NVP(char const *n, CORBA::Long v)
    : name(n)
  {
    std::string const string_value = std::to_string (static_cast<long> (v));
    value = string_value.c_str ();
  }

  NVP::NVP(char const *n, char const *v)
    : name(n), value(v)
  {
  }

  NVP::NVP(char const *n, ACE_CString const & v)
    : name(n), value(v)
  {
  }

  bool NVP::operator==(const NVP& rhs) const
  {
    return (this->name == rhs.name) != 0;
  }

  bool NVP::operator!=(const NVP& rhs) const
  {
    return ! (rhs == *this);
  }

  bool NVPList::find (char const *name, ACE_CString& val) const
  {
    for (size_t i {}; i < list_.size(); ++i)
    {
      if (list_[i].name == name)
      {
        val = list_[i].value;
        return true;
      }
    }
    return false;
  }

  bool NVPList::find (char const *name, char const *& val) const
  {
    for (size_t i {}; i < list_.size(); ++i)
    {
      if (list_[i].name == name)
      {
        val = list_[i].value.c_str ();
        return true;
      }
    }
    return false;
  }

  void NVPList::load(TAO_Notify_Property_Short& p) const
  {
    char const *v;
    if (find(p.name(), v))
    {
      p.assign (static_cast<CORBA::Short> (std::atoi (v)));
    }
  }

  void NVPList::load(TAO_Notify_Property_Long& p) const
  {
    char const *v;
    if (find(p.name(), v))
    {
      p.assign (static_cast<CORBA::Long> (std::atoi (v)));
    }
  }

  void NVPList::load(TAO_Notify_Property_Time& p) const
  {
    char const *v;
    if (find(p.name (), v))
    {
      p.assign (static_cast<TimeBase::TimeT> (string_to_uint64(v)));
    }
  }

  void NVPList::load(TAO_Notify_Property_Boolean& p) const
  {
    ACE_CString v;
    if (find(p.name (), v))
    {
      if (v == "true")
      {
        p = true;
      }
      else
      {
        p = false;
      }
    }
  }

  bool NVPList::load(char const *n, CORBA::Long& v) const
  {
    char const *val;
    if (find(n, val))
    {
      v = static_cast<CORBA::Long> (std::atoi (val));
      return true;
    }
    return false;
  }

  bool NVPList::load(char const *n, ACE_CString& v) const
  {
    return find(n, v);
  }

  void NVPList::push_back(const NVP& v)
  {
    for (size_t i {}; i < list_.size(); ++i)
    {
      if (list_[i].name == v.name)
      {
        list_[i].value = v.value;
        return;
      }
    }
    list_.push_back(v);
  }

  size_t NVPList::size() const
  {
    return list_.size();
  }

  const NVP& NVPList::operator[](size_t ndx) const
  {
    ACE_ASSERT (ndx < list_.size());
    return list_[ndx];
  }

} // namespace TAO_Notify

TAO_END_VERSIONED_NAMESPACE_DECL
