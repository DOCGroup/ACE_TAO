// $Id$

#include "EC_Wrapper.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "ace/Auto_Ptr.h"

EC_Wrapper::EC_Wrapper (void)
  : ec_impl_ (0),
    orb_ ()
{
}

TAO_EC_Servant_Var<EC_Wrapper>
EC_Wrapper::create (void)
{
  TAO_EC_Servant_Var<EC_Wrapper> w;
  ACE_NEW_RETURN (w,
                  EC_Wrapper,
                  w);
  return w;
}

EC_Wrapper::~EC_Wrapper (void)
{
  ACE_TRY_NEW_ENV
    {
      this->destroy_ec (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // ignore
    }
  ACE_ENDTRY;
}

int
EC_Wrapper::init (CORBA::ORB_ptr orb,
                  PortableServer::POA_ptr poa)
{
  if (CORBA::is_nil (orb)
      || this->ec_impl_)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "EC_Wrapper::init invoked improperly\n"),
                        -1);
    }

  this->orb_ = CORBA::ORB::_duplicate (orb);

  TAO_EC_Event_Channel_Attributes attr (poa, poa);

  // Allow reconnections - used by some tests.
  attr.consumer_reconnect = 1;

  TAO_EC_Event_Channel * impl = 0;
  ACE_NEW_RETURN (impl,
                  TAO_EC_Event_Channel (attr),
                  -1);
  auto_ptr<TAO_EC_Event_Channel> impl_release (impl);

  ACE_TRY_NEW_ENV
    {
      impl->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Suppressed the following exception "
                           "in EC_Wrapper::init:\n");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  this->ec_impl_ = impl_release.release ();
  return 0;
}

RtecEventChannelAdmin::ConsumerAdmin_ptr
EC_Wrapper::for_consumers (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->ec_impl_)
    return this->ec_impl_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  else
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
}

RtecEventChannelAdmin::SupplierAdmin_ptr
EC_Wrapper::for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->ec_impl_)
    return this->ec_impl_->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
  else
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
}

void
EC_Wrapper::destroy_ec (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  auto_ptr<TAO_EC_Event_Channel> ec_impl_aptr (this->ec_impl_);
  this->ec_impl_ = 0;

  if (ec_impl_aptr.get ())
    {
      ec_impl_aptr->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
EC_Wrapper::destroy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Deregister from POA.
  this->deactivator_.deactivate ();

  ACE_TRY
    {
      this->destroy_ec (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      this->orb_->shutdown ();
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;

  this->orb_->shutdown ();
}

RtecEventChannelAdmin::Observer_Handle
EC_Wrapper::append_observer (RtecEventChannelAdmin::Observer_ptr observer
                             ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
          CORBA::SystemException,
          RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
          RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER))
{
  if (this->ec_impl_)
    return this->ec_impl_->append_observer (observer ACE_ENV_ARG_PARAMETER);
  else
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
}

void
EC_Wrapper::remove_observer (RtecEventChannelAdmin::Observer_Handle handle
                             ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
          CORBA::SystemException,
          RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
          RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER))
{
  if (this->ec_impl_)
    this->ec_impl_->remove_observer (handle ACE_ENV_ARG_PARAMETER);
  else
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
