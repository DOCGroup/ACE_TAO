#include "PICurrent.h"

#if TAO_HAS_INTERCEPTORS == 1

ACE_RCSID (tao,
           PICurrent,
           "$Id$")


#if !defined (__ACE_INLINE__)
# include "PICurrent.inl"
#endif /* __ACE_INLINE__ */

#include "ORB_Core.h"
#include "TAO_Server_Request.h"
#include "PICurrent_Copy_Callback.h"
#include "SystemException.h"


TAO::PICurrent::PICurrent (TAO_ORB_Core * orb_core)
  : orb_core_ (orb_core),
    slot_count_ (0)
{
  // ACE_ASSERT (orb_core != 0);
}

TAO::PICurrent::~PICurrent (void)
{
}

CORBA::Any *
TAO::PICurrent::get_slot (PortableInterceptor::SlotId id
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  this->check_validity (id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  PICurrent_Impl * impl = this->tsc ();

  if (impl == 0)
    {
      ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14,
                                              CORBA::COMPLETED_NO),
                        0);
    }

  return impl->get_slot (id ACE_ENV_ARG_PARAMETER);
}

void
TAO::PICurrent::set_slot (PortableInterceptor::SlotId id,
                         const CORBA::Any & data
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  this->check_validity (id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PICurrent_Impl * impl = this->tsc ();

  if (impl == 0)
    {
      ACE_THROW (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14,
                                       CORBA::COMPLETED_NO));
    }

  impl->set_slot (id, data ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

TAO::PICurrent_Impl *
TAO::PICurrent::tsc (void)
{
  TAO_ORB_Core_TSS_Resources * tss =
    this->orb_core_->get_tss_resources ();

  return &tss->pi_current_;
}

// ------------------------------------------------------------------

TAO::PICurrent_Impl::PICurrent_Impl (void)
  : slot_table_ (),
    lc_slot_table_ (0),
    copy_callback_ (0),
    destruction_callback_ (0)
{
}

TAO::PICurrent_Impl::~PICurrent_Impl (void)
{
  // Break any existing ties with PICurrent to which our table was
  // logically copied since our table no longer exists once this
  // destructor completes.
  if (this->destruction_callback_ != 0)
    this->destruction_callback_->execute_destruction_callback (0);
}


CORBA::Any *
TAO::PICurrent_Impl::get_slot (PortableInterceptor::SlotId id
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  // No need to check validity of SlotId.  It is validated before this
  // method is invoked.

  PICurrent_Impl::Table & table = this->current_slot_table ();

  ACE_ASSERT (this->lc_slot_table_ != &this->slot_table_);

  CORBA::Any * any = 0;

  if (id < table.size ())
    {
      ACE_NEW_THROW_EX (any,
                        CORBA::Any (table[id]), // Make a copy.
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            0,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (any);
    }
  else
    {
      // In accordance with the Portable Interceptor specification,
      // return an Any with a TCKind of tk_null.  A default
      // constructed Any has that TCKind.
      ACE_NEW_THROW_EX (any,
                        CORBA::Any,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            0,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (any);
    }

  return any;
}

void
TAO::PICurrent_Impl::set_slot (PortableInterceptor::SlotId id,
                               const CORBA::Any & data
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  // No need to check validity of SlotId.  It is validated before this
  // method is invoked.

  // Perform deep copy of the logically copied slot table, if
  // necessary, before modifying our own slot table.
  if (this->copy_callback_ != 0
      && this->copy_callback_->execute () != 0)
    {
      ACE_THROW (CORBA::INTERNAL ());
    }

  // If the slot table array isn't large enough, then increase its
  // size.  We're guaranteed not to exceed the number of allocated
  // slots for the reason stated above.
  if (id >= this->slot_table_.size ()
      && this->slot_table_.size (id + 1) != 0)
    {
      ACE_THROW (CORBA::INTERNAL ());
    }

  this->slot_table_[id] = CORBA::Any (data);
}

void
TAO::PICurrent_Impl::execute_destruction_callback (
  TAO::PICurrent_Impl::Table * old_lc_slot_table)
{
  this->lc_slot_table_ = old_lc_slot_table;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Array_Base<CORBA::Any>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Array_Base<CORBA::Any>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


#endif  /* TAO_HAS_INTERCEPTORS == 1 */
