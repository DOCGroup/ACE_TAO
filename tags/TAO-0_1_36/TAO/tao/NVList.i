// $Id$
// This may look like C, but it's really -*- C++ -*-

// constructor
ACE_INLINE
CORBA_NamedValue::CORBA_NamedValue (void)
  : refcount_ (1),
    flags_ (0),
    name_ (0)
{
}

// return the name
ACE_INLINE const char*
CORBA_NamedValue::name (void) const
{
  return this->name_;
}

// return the value
ACE_INLINE CORBA::Any_ptr
CORBA_NamedValue::value (void) const
{
  return ACE_const_cast (CORBA::Any_ptr, &this->any_);
}

// return the flags
ACE_INLINE CORBA::Flags
CORBA_NamedValue::flags (void) const
{
  return this->flags_;
}


// = methods for the NVList class

// constructor
ACE_INLINE
CORBA_NVList::CORBA_NVList (void)
  : max_ (0),
    refcount_ (1)
{
}

ACE_INLINE CORBA::ULong
CORBA_NVList::count (void) const
{
  return this->max_;
}
