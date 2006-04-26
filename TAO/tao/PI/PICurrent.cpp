#include "tao/PI/PICurrent.h"

#if TAO_HAS_INTERCEPTORS == 1

ACE_RCSID (tao,
           PICurrent,
           "$Id$")


#if !defined (__ACE_INLINE__)
# include "tao/PI/PICurrent.inl"
#endif /* __ACE_INLINE__ */

#include "tao/PI/PICurrent_Impl.h"

#include "tao/ORB_Core.h"
#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/TAO_Server_Request.h"
#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::PICurrent::PICurrent (TAO_ORB_Core &orb_core)
  : orb_core_ (orb_core),
    tss_slot_ (0),
    slot_count_ (0)
{
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
  TAO::PICurrent_Impl *impl =
    static_cast<TAO::PICurrent_Impl *> (
      this->orb_core_.get_tss_resource (this->tss_slot_));

  return impl;
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

CORBA::ORB_ptr
TAO::PICurrent::_get_orb (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return CORBA::ORB::_duplicate (this->orb_core_.orb ());
}

int
TAO::PICurrent::initialize (PortableInterceptor::SlotId sc
                            ACE_ENV_ARG_DECL_NOT_USED)
{
  this->slot_count_ = sc;

  if (this->tsc () == 0 && tss_slot_ == 0)
    {
      TAO::PICurrent_Impl *impl = 0;
      ACE_NEW_RETURN (impl,
                      TAO::PICurrent_Impl,
                      0);

      const int result = this->orb_core_.add_tss_cleanup_func (0,
                                                               tss_slot_);

      if (result != 0)
        return result;

      this->orb_core_.set_tss_resource (tss_slot_, impl);
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
