// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    corbacom.i
//
// = DESCRIPTION
//    CORBA C/C++/COM mapping for Win32
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
//     and
//     Aniruddha Gokhale
//
// ============================================================================


// =methods of class CORBA::NamedValue

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
  return ACE_const_cast(CORBA::Any_ptr,&this->any_);
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
  : values_ (0),
    max_ (0),
    len_ (0),
    refcount_ (1)
{
}

ACE_INLINE CORBA::ULong
CORBA_NVList::count (void) const
{
  return this->max_;
}

ACE_INLINE CORBA::NamedValue_ptr
CORBA_NVList::item (CORBA::ULong n, CORBA::Environment &env)
{
  env.clear ();
  if (n >= this->max_) // 0 based indexing
    {
      env.exception (new CORBA::TypeCode::Bounds ());
      return 0;
    }
  else
    return &this->values_ [n];
}
