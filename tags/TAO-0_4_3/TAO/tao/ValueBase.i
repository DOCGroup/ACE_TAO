// This may not look like C++, but it's really -*- C++ -*-
// $Id$

// static operations in namespace CORBA ========================

ACE_INLINE void
CORBA::add_ref (CORBA::ValueBase *val)
{
  if (val)
    val->_add_ref ();
}

ACE_INLINE void
CORBA::remove_ref (CORBA::ValueBase *val)
{
  if (val)
    val->_remove_ref ();
}

// ===========================================================

// constructor
ACE_INLINE
CORBA_ValueBase::CORBA_ValueBase (void)
{
}


// ===========================================================

// constructor
ACE_INLINE
CORBA_DefaultValueRefCountBase::CORBA_DefaultValueRefCountBase (void)
    : _tao_reference_count_ (1)
{
}


ACE_INLINE void
CORBA_DefaultValueRefCountBase::_tao_add_ref (void)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, guard, this->_tao_reference_count_lock_);
  ++_tao_reference_count_;
}

ACE_INLINE void
CORBA_DefaultValueRefCountBase::_tao_remove_ref (void)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, guard, this->_tao_reference_count_lock_);
    -- this->_tao_reference_count_;
    if (this->_tao_reference_count_ != 0)
      return;
  }
  // The guard has been already given up, but this is no problem
  // cause we have held the last reference, and this one we don't give
  // away anymore, we do:
  delete this;
}

ACE_INLINE CORBA::ULong
CORBA_DefaultValueRefCountBase::_tao_refcount_value (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->_tao_reference_count_lock_,0);
  return _tao_reference_count_;
}

// ===========================================================

// Detection of flags in the CDR Stream

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_null_ref (CORBA::ULong tag)
{
  return (tag == 0);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_value_tag (CORBA::ULong tag)
{
  return ((tag & Value_tag_sigbits) == 0x7FFFFF00L);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags:: has_codebase_url(CORBA::ULong tag)
{
  return (tag & Codebase_url);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::has_no_type_info (CORBA::ULong tag)
{
  return ((tag & Type_info_sigbits) == Type_info_none);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::has_single_type_info (CORBA::ULong tag)
{
  return ((tag & Type_info_sigbits) == Type_info_single);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::has_list_type_info (CORBA::ULong tag)
{
  return ((tag & Type_info_sigbits) == Type_info_list);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags:: is_chunked (CORBA::ULong tag)
{
  return (tag & 8);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_indirection_tag (CORBA::ULong tag)
{
  return (tag == 0xFFFFFFFFL);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_indirection (CORBA::ULong value)
{
  return (0x80000000L < value && value <= (0xFFFFFFFFL - 4));
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_block_size (CORBA::ULong value)
{
  return ( 0 < value && value < 0x7fffff00L);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_end_tag (CORBA::ULong tag)
{
  return (0x80000000L < tag  && tag <= 0xFFFFFFFFL);
}
