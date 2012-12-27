// $Id$

#include "orbsvcs/Notify/Name_Value_Pair.h"
#include "orbsvcs/Notify/Property_T.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_ctype.h"

namespace
{
  ACE_UINT64 string_to_uint64 (const char * s)
  {
    size_t const len = ACE_OS::strlen (s);
    if (len == 0)
      return 0;
    if (! ACE_OS::ace_isdigit(s[0]))
      return 0;

    ACE_UINT64 t = 0;
    for (size_t i = 0; i < len; ++i)
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
  NVP::NVP()
  {
  }

  NVP::NVP(const TAO_Notify_Property_Short& p)
    : name(p.name())
  {
    char buf[64];
    ACE_OS::sprintf (buf, "%d", p.value ());
    value = buf;
  }

  NVP::NVP(const TAO_Notify_Property_Long& p)
    : name(p.name())
  {
    char buf[64];
    long temp = static_cast<long> (p.value ());
    ACE_OS::sprintf (buf, "%ld", temp);
    value = buf;
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

  NVP::NVP(const char * n, CORBA::Long v)
    : name(n)
  {
    char buf[64];
    long temp = v;
    ACE_OS::sprintf (buf, "%ld", temp);
    value = buf;
  }

  NVP::NVP(const char * n, const char * v)
    : name(n), value(v)
  {
  }

  NVP::NVP(const char * n, const ACE_CString & v)
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

  bool NVPList::find (const char * name, ACE_CString& val) const
  {
    for (size_t i = 0; i < list_.size(); ++i)
    {
      if (list_[i].name == name)
      {
        val = list_[i].value;
        return true;
      }
    }
    return false;
  }

  bool NVPList::find (const char * name, const char *& val) const
  {
    for (size_t i = 0; i < list_.size(); ++i)
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
    const char * v;
    if (find(p.name(), v))
    {
      p.assign (static_cast<CORBA::Short> (ACE_OS::atoi(v)));
    }
  }

  void NVPList::load(TAO_Notify_Property_Long& p) const
  {
    const char * v;
    if (find(p.name(), v))
    {
      p.assign (static_cast<CORBA::Long> (ACE_OS::atoi(v)));
    }
  }

  void NVPList::load(TAO_Notify_Property_Time& p) const
  {
    const char * v;
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
        p = 1;
      }
      else
      {
        p = 0;
      }
    }
  }

  bool NVPList::load(const char * n, CORBA::Long& v) const
  {
    const char * val;
    if (find(n, val))
    {
      v = static_cast<CORBA::Long> (ACE_OS::atoi(val));
      return true;
    }
    return false;
  }

  bool NVPList::load(const char * n, ACE_CString& v) const
  {
    return find(n, v);
  }

  void NVPList::push_back(const NVP& v)
  {
    for (size_t i = 0; i < list_.size(); ++i)
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
