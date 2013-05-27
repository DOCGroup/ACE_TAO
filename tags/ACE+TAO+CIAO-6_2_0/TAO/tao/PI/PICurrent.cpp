// $Id$

#include "tao/PI/PICurrent.h"

#if TAO_HAS_INTERCEPTORS == 1

#if !defined (__ACE_INLINE__)
# include "tao/PI/PICurrent.inl"
#endif /* __ACE_INLINE__ */

#include "tao/PI/PICurrent_Impl.h"
#include "tao/ORB_Core.h"
#include "tao/TAO_Server_Request.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::PICurrent::PICurrent (TAO_ORB_Core &orb_core)
  : orb_core_ (orb_core),
    tss_slot_ (0), // Call initialize() before use.
    slot_count_ (0) // Call initialize() before use.
{
}

TAO::PICurrent::~PICurrent (void)
{
}

CORBA::Any *
TAO::PICurrent::get_slot (PortableInterceptor::SlotId identifier)
{
  this->check_validity (identifier);

  return this->tsc ()->get_slot (identifier);
}

void
TAO::PICurrent::set_slot (PortableInterceptor::SlotId identifier,
                         const CORBA::Any &data)
{
  this->check_validity (identifier);

  this->tsc ()->set_slot (identifier, data);
}

namespace
{
  extern "C" void CleanUpPICurrent(void *object, void *)
  {
    delete static_cast<TAO::PICurrent_Impl *> (object);
  }
}

TAO::PICurrent_Impl *
TAO::PICurrent::tsc (void)
{
  TAO::PICurrent_Impl *impl =
    static_cast<TAO::PICurrent_Impl *> (
      this->orb_core_.get_tss_resource (this->tss_slot_));

  // If this TSS has not yet been set-up, give it it's own PICurrent_Impl.
  if (0 == impl)
  {
    ACE_NEW_THROW_EX (impl,
                      TAO::PICurrent_Impl (&this->orb_core_, this->tss_slot_),
                      CORBA::NO_MEMORY (
                        CORBA::SystemException::_tao_minor_code (
                          TAO::VMCID,
                          ENOMEM),
                        CORBA::COMPLETED_NO));

    this->orb_core_.set_tss_resource (this->tss_slot_, impl);
  }

  return impl;
}

void
TAO::PICurrent::check_validity (const PortableInterceptor::SlotId &identifier)
{
  // If the slot_count is zero, no initialization has been done (if there are
  // no slots, then the PICurrent_impl object is not created as there is no
  // data to copy).
  if (0 == this->slot_count_)
    throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);

  // No need to acquire a lock for this check.  At this point, these
  // attributes are read only.
  if (identifier >= this->slot_count_)
    throw PortableInterceptor::InvalidSlot ();
}

CORBA::ORB_ptr
TAO::PICurrent::_get_orb (void)
{
  return CORBA::ORB::_duplicate (this->orb_core_.orb ());
}

void
TAO::PICurrent::initialize (PortableInterceptor::SlotId sc)
{
  // Only allow a single initialization; sc is the number of
  // allocated PICurrent data slots the end user wants. If 0
  // PICurrent is not used, as no data is to be stored.
  if ((0 == this->slot_count_) && (0 != sc))
  {
    // NOTE: This function not only adds the cleanup function
    // but ALSO allocates the TSS slot PICurrent is to use.
    // It MUST be called BEFORE we attempt to get/set any
    // PICurrent slot data.
    if (0 != orb_core_.add_tss_cleanup_func (CleanUpPICurrent, this->tss_slot_))
      throw ::CORBA::NO_MEMORY (
                   CORBA::SystemException::_tao_minor_code (
                     TAO::VMCID,
                     ENOMEM),
                   CORBA::COMPLETED_NO);

    this->slot_count_ = sc;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
