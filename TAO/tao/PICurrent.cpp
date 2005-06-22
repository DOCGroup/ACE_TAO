#include "PICurrent.h"

#if TAO_HAS_INTERCEPTORS == 1

ACE_RCSID (tao,
           PICurrent,
           "$Id$")


#if !defined (__ACE_INLINE__)
# include "PICurrent.inl"
#endif /* __ACE_INLINE__ */

#include "ORB_Core.h"
#include "ORB_Core_TSS_Resources.h"
#include "TAO_Server_Request.h"
#include "PICurrent_Copy_Callback.h"
#include "SystemException.h"


TAO::PICurrent::PICurrent (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core),
    slot_count_ (0)
{
  // ACE_ASSERT (orb_core != 0);
}

TAO::PICurrent::~PICurrent (void)
{
}

CORBA::Any *
TAO::PICurrent::get_slot (PortableInterceptor::SlotId identifier
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  this->check_validity (identifier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  PICurrent_Impl *impl = this->tsc ();

  if (impl == 0)
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14,
                                            CORBA::COMPLETED_NO),
                      0);

  return impl->get_slot (identifier ACE_ENV_ARG_PARAMETER);
}

void
TAO::PICurrent::set_slot (PortableInterceptor::SlotId identifier,
                         const CORBA::Any &data
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  this->check_validity (identifier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PICurrent_Impl *impl = this->tsc ();

  if (impl == 0)
    ACE_THROW (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14,
                                     CORBA::COMPLETED_NO));

  impl->set_slot (identifier, data ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

TAO::PICurrent_Impl *
TAO::PICurrent::tsc (void)
{
  TAO_ORB_Core_TSS_Resources *tss =
    this->orb_core_->get_tss_resources ();

  return &tss->pi_current_;
}

void
TAO::PICurrent::check_validity (const PortableInterceptor::SlotId &identifier
                                ACE_ENV_ARG_DECL)
{
  // No need to acquire a lock for this check.  At this point, these
  // attributes are read only.
  if (identifier >= this->slot_count_)
    ACE_THROW (PortableInterceptor::InvalidSlot ());
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
