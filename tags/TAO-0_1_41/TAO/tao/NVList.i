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

ACE_INLINE void
CORBA::release (CORBA::NamedValue_ptr nv)
{
  if (nv)
    nv->_decr_refcnt ();
}

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::NamedValue_ptr nv)
{
  return nv == 0;
}

ACE_INLINE CORBA_NamedValue*
CORBA_NamedValue::_duplicate (CORBA_NamedValue *x)
{
  if (x != 0)
    x->_incr_refcnt ();
  return x;
}

ACE_INLINE CORBA_NamedValue*
CORBA_NamedValue::_nil (void)
{
  return 0;
}

// ****************************************************************

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

ACE_INLINE void
CORBA::release (CORBA::NVList_ptr nvl)
{
  if (nvl)
    nvl->_decr_refcnt ();
}

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::NVList_ptr nvl)
{
  return (CORBA::Boolean) (nvl == 0);
}

ACE_INLINE CORBA_NVList*
CORBA_NVList::_duplicate (CORBA_NVList *x)
{
  if (x != 0)
    x->_incr_refcnt ();
  return x;
}

ACE_INLINE CORBA_NVList*
CORBA_NVList::_nil (void)
{
  return 0;
}

