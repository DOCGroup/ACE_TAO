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
  return (tag == Null_ref);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_value_tag (CORBA::ULong tag)
{
  return ((tag & Value_tag_sigbits) == Value_tag_base);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags:: has_codebase_url(CORBA::ULong tag)
{
  return (tag & Codebase_url);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::has_no_type_info (CORBA::ULong tag)
{
  return ((tag & Type_info_sigbits) == Type_info_no);
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
  return (tag & Chunked_encoding);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_indirection_tag (CORBA::ULong tag)
{
  return (tag == Indirection_tag);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags:: block_size (CORBA::ULong tag, CORBA::ULong &size)
{
  if ((tag >= Block_size_tag_min) &&
      (tag <= Block_size_tag_max))
    {
      size = tag;
      return 1;
    }
  return 0;
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags:: end_tag_depth (CORBA::ULong tag,
                                    CORBA::ULong &depth)
{
  // @@ Torsten: could you please check that I did not break anything
  //    here?  Your code was giving us all sort of headaches on NT
  //    because the compiler insists in making 0x8000000L an unsigned
  //    long (and IMHO it is right).
  if (End_tag_min <= tag
      && tag <= End_tag_max)
    {
      // @@ Torsten: do you think this is right? After all you are
      //    storing things back into a ULong....
      depth = - ACE_static_cast (CORBA::Long, tag);
      return 1;
    }
  return 0;
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags:: indirection (CORBA::ULong tag,
                                  CORBA::Long &jump)
{
  if (Indirection_min <= tag
      && tag <= Indirection_max)
    {
      jump = ACE_static_cast (CORBA::Long, tag);
      return 1;
    }
  return 0;
}

/*
ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags:: (CORBA::ULong tag)
{
  return ( );
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags:: (CORBA::ULong tag)
{
  return ( );
}

*/
