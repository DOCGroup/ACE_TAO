// -*- C++ -*-
//
// $Id$

ACE_INLINE CORBA::Boolean
CORBA::is_nil (CORBA::NamedValue_ptr nv)
{
  return nv == 0;
}

ACE_INLINE void
CORBA::release (CORBA::NamedValue_ptr nv)
{
  if (nv)
    nv->_decr_refcnt ();
}

// *************************************************************

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
  return ACE_const_cast (CORBA::Any_ptr, &this->any_);
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
      x->_incr_refcnt ();
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
CORBA::Boolean
CORBA::is_nil (CORBA::NVList_ptr nvl)
{
  return (CORBA::Boolean) (nvl == 0);
}

ACE_INLINE 
void
CORBA::release (CORBA::NVList_ptr nvl)
{
  if (nvl)
    {
      nvl->_decr_refcnt ();
    }
}

// ****************************************************************

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
CORBA::NVList::count (ACE_ENV_SINGLE_ARG_DECL) const
{
  (ACE_const_cast (CORBA::NVList *,
                   this))->evaluate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->max_;
}

ACE_INLINE 
CORBA::NVList *
CORBA::NVList::_duplicate (CORBA::NVList * x)
{
  if (x != 0)
    {
      x->_incr_refcnt ();
    }

  return x;
}

ACE_INLINE 
CORBA::NVList *
CORBA::NVList::_nil (void)
{
  return 0;
}
