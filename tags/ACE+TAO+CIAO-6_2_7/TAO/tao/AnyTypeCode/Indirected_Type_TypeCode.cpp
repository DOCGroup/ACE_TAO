// $Id$

#include "tao/CDR.h"

#include "tao/AnyTypeCode/Indirected_Type_TypeCode.h"

#ifndef __ACE_INLINE__
# include "tao/AnyTypeCode/Indirected_Type_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#include "ace/Guard_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO::TypeCode::Indirected_Type::tao_marshal_kind (TAO_OutputCDR & cdr) const
{
  return this->recursive_tc_ ? this->recursive_tc_->tao_marshal_kind (cdr) : false;
}

bool
TAO::TypeCode::Indirected_Type::tao_marshal (TAO_OutputCDR & cdr,
                                            CORBA::ULong offset) const
{
  return this->recursive_tc_ ? this->recursive_tc_->tao_marshal (cdr, offset) : false;
}

void
TAO::TypeCode::Indirected_Type::tao_duplicate (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);

  // for every reference beside the master reference (1)
  // increase the master tc's reference count to keep it
  // safe while we're referenced outside the master's context
  if (this->refcount_ >= 1 && this->recursive_tc_)
  {
    this->recursive_tc_->tao_duplicate ();
  }
  this->refcount_ += 1;
}

void
TAO::TypeCode::Indirected_Type::tao_release (void)
{
  unsigned long newcount;
  CORBA::TypeCode_ptr curmaster;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);

    newcount = --this->refcount_;
    curmaster = this->recursive_tc_;
  }
  if (newcount >= 1 && curmaster)
  {
    curmaster->tao_release ();
  }
  else if (newcount == 0)
  {
    delete this;
  }
}

void
TAO::TypeCode::Indirected_Type::set_recursive_tc (CORBA::TypeCode_ptr tc)
{
  // link only once (should never happen that this is called twice but test anyway)
  if (this->recursive_tc_ == 0)
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);

    if (tc == 0)  // should never happen
      return;

    // make sure we are the right kind
    CORBA::TCKind & mutable_kind = const_cast<CORBA::TCKind &> (this->kind_);

    mutable_kind = tc->kind ();

    // for every reference beside the master reference (1)
    // increase the master tc's reference count to keep it
    // safe while we're referenced outside the master's context
    for (unsigned long cnt = this->refcount_; cnt > 1 ;--cnt)
    {
      tc->tao_duplicate ();
    }
    this->recursive_tc_ = tc;
  }
}

// No need to use a guard for the methods below since these will ever only be called *after*
// set_recursive_tc() is called.

CORBA::Boolean
TAO::TypeCode::Indirected_Type::equal_i (CORBA::TypeCode_ptr tc) const
{
  return this->recursive_tc_ ? this->recursive_tc_->equal (tc) : false;
}

CORBA::Boolean
TAO::TypeCode::Indirected_Type::equivalent_i (
  CORBA::TypeCode_ptr tc) const
{
  return this->recursive_tc_ ? this->recursive_tc_->equivalent (tc) : false;
}

CORBA::TypeCode_ptr
TAO::TypeCode::Indirected_Type::get_compact_typecode_i (void) const
{
  return this->recursive_tc_ ? this->recursive_tc_->get_compact_typecode () : 0;
}

char const *
TAO::TypeCode::Indirected_Type::id_i (void) const
{
  return this->id_.in ();
}

char const *
TAO::TypeCode::Indirected_Type::name_i (void) const
{
  return this->recursive_tc_ ? this->recursive_tc_->name () : 0;
}

CORBA::ULong
TAO::TypeCode::Indirected_Type::member_count_i (void) const
{
  return this->recursive_tc_ ? this->recursive_tc_->member_count () : 0;
}

char const *
TAO::TypeCode::Indirected_Type::member_name_i (
  CORBA::ULong index) const
{
  return this->recursive_tc_ ? this->recursive_tc_->member_name (index) : 0;
}

CORBA::TypeCode_ptr
TAO::TypeCode::Indirected_Type::member_type_i (
  CORBA::ULong index) const
{
  return this->recursive_tc_ ? this->recursive_tc_->member_type (index) : 0;
}

CORBA::Visibility
TAO::TypeCode::Indirected_Type::member_visibility_i (CORBA::ULong index) const
{
  return this->recursive_tc_ ? this->recursive_tc_->member_visibility (index) : 0;
}

CORBA::ValueModifier
TAO::TypeCode::Indirected_Type::type_modifier_i (void) const
{
  return this->recursive_tc_ ? this->recursive_tc_->type_modifier () : 0;
}

CORBA::TypeCode_ptr
TAO::TypeCode::Indirected_Type::concrete_base_type_i (void) const
{
  return this->recursive_tc_ ? this->recursive_tc_->concrete_base_type () : 0;
}

CORBA::Any *
TAO::TypeCode::Indirected_Type::member_label_i (CORBA::ULong index) const
{
  return this->recursive_tc_ ? this->recursive_tc_->member_label (index) : 0;
}

CORBA::TypeCode_ptr
TAO::TypeCode::Indirected_Type::discriminator_type_i (void) const
{
  return this->recursive_tc_ ? this->recursive_tc_->discriminator_type () : 0;
}

CORBA::Long
TAO::TypeCode::Indirected_Type::default_index_i (void) const
{
  return this->recursive_tc_ ? this->recursive_tc_->default_index () : 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
