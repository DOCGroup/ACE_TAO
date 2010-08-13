// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
CORBA::NamedValue::NamedValue (void)
  : refcount_ (1),
    flags_ (0),
    name_ (0)
{
}

ACE_INLINE
const char *
CORBA::NamedValue::name (void) const
{
  return this->name_;
}

ACE_INLINE
CORBA::Any_ptr
CORBA::NamedValue::value (void) const
{
  return const_cast<CORBA::Any_ptr> (&this->any_);
}

ACE_INLINE
CORBA::Flags
CORBA::NamedValue::flags (void) const
{
  return this->flags_;
}

ACE_INLINE
CORBA::NamedValue *
CORBA::NamedValue::_duplicate (CORBA::NamedValue * x)
{
  if (x != 0)
    {
      x->_incr_refcount ();
    }

  return x;
}

ACE_INLINE
CORBA::NamedValue*
CORBA::NamedValue::_nil (void)
{
  return 0;
}

// *************************************************************

ACE_INLINE
CORBA::NVList::NVList (void)
  : max_ (0),
    refcount_ (1),
    incoming_ (0),
    incoming_flag_ (0)
{
}

ACE_INLINE
CORBA::ULong
CORBA::NVList::count (void) const
{
  (const_cast<CORBA::NVList *> (this))->evaluate ();

  return this->max_;
}

ACE_INLINE
CORBA::NVList *
CORBA::NVList::_duplicate (CORBA::NVList * x)
{
  if (x != 0)
    {
      x->_incr_refcount ();
    }

  return x;
}

ACE_INLINE
CORBA::NVList *
CORBA::NVList::_nil (void)
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
