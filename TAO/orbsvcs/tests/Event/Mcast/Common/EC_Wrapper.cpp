// $Id$

#include "EC_Wrapper.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "ace/Auto_Ptr.h"

EC_Wrapper::EC_Wrapper (void)
  : ec_impl_ (0),
    orb_ ()
{
}

PortableServer::Servant_var<EC_Wrapper>
EC_Wrapper::create (void)
{
  PortableServer::Servant_var<EC_Wrapper> w;
  ACE_NEW_RETURN (w,
                  EC_Wrapper,
                  w);
  return w;
}

EC_Wrapper::~EC_Wrapper (void)
{
  try
    {
      this->destroy_ec ();
    }
  catch (const CORBA::Exception&)
    {
      // ignore
    }
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

  try
    {
      impl->activate ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Suppressed the following exception ""in EC_Wrapper::init:\n");
      return -1;
    }

  this->ec_impl_ = impl_release.release ();
  return 0;
}

RtecEventChannelAdmin::ConsumerAdmin_ptr
EC_Wrapper::for_consumers (void)
{
  if (this->ec_impl_)
    return this->ec_impl_->for_consumers ();
  else
    throw CORBA::OBJECT_NOT_EXIST ();
}

RtecEventChannelAdmin::SupplierAdmin_ptr
EC_Wrapper::for_suppliers (void)
{
  if (this->ec_impl_)
    return this->ec_impl_->for_suppliers ();
  else
    throw CORBA::OBJECT_NOT_EXIST ();
}

void
EC_Wrapper::destroy_ec (void)
{
  auto_ptr<TAO_EC_Event_Channel> ec_impl_aptr (this->ec_impl_);
  this->ec_impl_ = 0;

  if (ec_impl_aptr.get ())
    {
      ec_impl_aptr->destroy ();
    }
}

void
EC_Wrapper::destroy (void)
{
  // Deregister from POA.
  this->deactivator_.deactivate ();

  try
    {
      this->destroy_ec ();
    }
  catch (const CORBA::Exception&)
    {
      this->orb_->shutdown ();
      throw;
    }

  this->orb_->shutdown ();
}

RtecEventChannelAdmin::Observer_Handle
EC_Wrapper::append_observer (RtecEventChannelAdmin::Observer_ptr observer)
{
  if (this->ec_impl_)
    return this->ec_impl_->append_observer (observer);
  else
    throw CORBA::OBJECT_NOT_EXIST ();
}

void
EC_Wrapper::remove_observer (RtecEventChannelAdmin::Observer_Handle handle)
{
  if (this->ec_impl_)
    this->ec_impl_->remove_observer (handle);
  else
    throw CORBA::OBJECT_NOT_EXIST ();
}
