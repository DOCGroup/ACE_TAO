// Name_Space.cpp
// $Id$

#include "ace/Name_Space.h"

ACE_RCSID(ace, Name_Space, "$Id$")

ACE_Name_Binding::ACE_Name_Binding (void)
  : type_ (ACE_OS_String::strdup (""))
{
  ACE_TRACE ("ACE_Name_Binding::ACE_Name_Binding");
}


ACE_Name_Binding::~ACE_Name_Binding (void)
{
  ACE_TRACE ("ACE_Name_Binding::~ACE_Name_Binding");
  ACE_OS_Memory::free ((void *) this->type_);
}

ACE_Name_Binding::ACE_Name_Binding (const ACE_NS_WString &name,
                                    const ACE_NS_WString &value,
                                    const char *type)
  : name_ (name),
    value_ (value),
    type_ (type == 0 ? ACE_OS_String::strdup ("") : ACE_OS_String::strdup (type))
{
  ACE_TRACE ("ACE_Name_Binding::ACE_Name_Binding");
}

ACE_Name_Binding::ACE_Name_Binding (const ACE_Name_Binding &s)
  : name_ (s.name_),
    value_ (s.value_),
    type_ (ACE_OS_String::strdup (s.type_))
{
  ACE_TRACE ("ACE_Name_Binding::ACE_Name_Binding");
}

void
ACE_Name_Binding::operator = (const ACE_Name_Binding &s)
{
  ACE_TRACE ("ACE_Name_Binding::operator =");

  if (this != &s)
    {
      ACE_OS_Memory::free ((void *) this->type_);
      this->name_ = s.name_;
      this->value_ = s.value_;
      this->type_ = ACE_OS_String::strdup (s.type_);
    }
}

int
ACE_Name_Binding::operator == (const ACE_Name_Binding &s) const
{
  ACE_TRACE ("ACE_Name_Binding::operator ==");
  return this->name_ == s.name_
    && this->value_ == s.value_
    && ACE_OS_String::strcmp (this->type_, s.type_) == 0;
}

ACE_Name_Space::~ACE_Name_Space (void)
{
  ACE_TRACE ("ACE_Name_Space::~ACE_Name_Space");
}
