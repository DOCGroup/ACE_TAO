// -*- C++ -*-
//
// $Id$

template <typename StringType, class CaseArrayType, class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Union<StringType, CaseArrayType, RefCountPolicy>::Union (
  char const * id,
  char const * name,
  CORBA::TypeCode_ptr * discriminant_type,
  Case const * cases,
  CORBA::ULong ncases,
  CORBA::Long default_index,
  char const * default_member_name,
  CORBA::TypeCode_ptr * default_member_type)
  : base_attributes_ (id, name)
  , default_index_ (default_index),
  , ncases_ (ncases)
  , cases_ (cases)
  , default_case_ (default_member_name,
                   default_member_type)
{
}

template <typename StringType, typename CaseArrayType, class RefCountPolicy>
ACE_INLINE CORBA::ULong
TAO::TypeCode::Union<StringType,
                     CaseArrayType>::case_count (void) const
{
  return (this->default_index_ < 0 ? this->ncases_ : this->ncases_ + 1);
}

template <typename StringType, typename CaseArrayType, class RefCountPolicy>
ACE_INLINE TAO::TypeCode::Union::case_type const &
TAO::TypeCode::Union<StringType,
                     CaseArrayType>::case (CORBA::ULong index) const
{
  if (default_index_ >= 0)
    {
      if (index == static_cast<CORBA::ULong> (this->default_index_))
        return this->default_case_;

      // Shift by one if default case was declared prior to
      // non-default cases.
      else if (index > static_cast<CORBA::ULong> (this->default_index_))
        return this->cases_[index - 1];

      // Remaining (index < this->default_index_) case is handled
      // below.
    }

  return this->cases_[index];
}


// -------------------------------------------------------------
// Member specializations
// -------------------------------------------------------------

ACE_INLINE TAO::TypeCode::Union::Case const *
TAO::TypeCode::Union<CORBA::String_var,
                     ACE_Auto_Ptr_Array<Case const> >::cases (void) const
{
  return this->cases_.get ();
}
