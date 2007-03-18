/* -*- C++ -*- */
// $Id$

#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/CosEventCommS.h"
#include "ProxyPushConsumer_i.h"
#include "ace/Auto_Ptr.h"

#if defined(_MSC_VER)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_CosEC_PushSupplierWrapper :
  public virtual POA_RtecEventComm::PushSupplier
{
  // = TITLE
  //   A Wrapper for the Rtec PushSupplier.
  //
  // = DESCRIPTION
  //   The Rtec ProxyPushConsumer uses a Rtec PushSupplier.  This
  //   class wraps the Cos PushSupplier to make it look like a Rtec
  //   PushSupplier.
public:
  // = Initialization and termination methods.
  TAO_CosEC_PushSupplierWrapper (CosEventComm::PushSupplier_ptr supplier);
  // Constructor.

  ~TAO_CosEC_PushSupplierWrapper (void);
  // Destructor.

  virtual void disconnect_push_supplier (void);
  // Disconnects the push supplier.

private:
  // @@ Pradeep: are you sure you want to go through the CORBA
  // interface? Maybe the implementation (ProxyPushConsumer_i) is good
  // enough at this point?  The tradeoff is flexibility (your scheme
  // can use remote CosPushSuppliers), but suffers some performance
  // penalty: do you need the extra flexibility? Can you use it? [I
  // suspect the answers are "not" for both]
  CosEventComm::PushSupplier_var supplier_;
  // The Cos PushSupplier that we're proxying for.
};

#if defined(_MSC_VER)
#pragma warning(default:4250)
#endif /* _MSC_VER */

TAO_CosEC_PushSupplierWrapper::TAO_CosEC_PushSupplierWrapper
(CosEventComm::PushSupplier_ptr supplier)
  : supplier_ (CosEventComm::PushSupplier::_duplicate (supplier))
{
  // No-Op.
}

TAO_CosEC_PushSupplierWrapper::~TAO_CosEC_PushSupplierWrapper (void)
{
  // No-Op.
}

void
TAO_CosEC_PushSupplierWrapper::disconnect_push_supplier (void)
{
  this->supplier_->disconnect_push_supplier ();

  // Deactivate the supplier proxy
  PortableServer::POA_var poa =
    this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());

  // @@ If we keep a list remember to remove this object from the
  // list.
}

TAO_CosEC_ProxyPushConsumer_i::TAO_CosEC_ProxyPushConsumer_i (const RtecEventChannelAdmin::SupplierQOS &qos,
                                                              RtecEventChannelAdmin::ProxyPushConsumer_ptr proxypushconsumer)
  : qos_ (qos),
    proxypushconsumer_ (RtecEventChannelAdmin::ProxyPushConsumer::_duplicate (proxypushconsumer)),
    wrapper_ (0)
{
  // No-Op.
}

TAO_CosEC_ProxyPushConsumer_i::~TAO_CosEC_ProxyPushConsumer_i (void)
{
  // No-Op.
}

void
TAO_CosEC_ProxyPushConsumer_i::push (const CORBA::Any &data)
{
  RtecEventComm::Event buffer[1];
  // Create an event set that does not own the buffer....
  RtecEventComm::EventSet events (1, 1, buffer, 0);
  events.length (1);

  RtecEventComm::Event &e = events[0];
  RtecEventComm::Event eqos =
    qos_.publications[0].event;

  // @@ what if i initialize the entire <EventSet> with corresponding
  // publications entries.

  // NOTE: we initialize the <EventHeader> field using the 1st
  // <publications> from the <SupplierQOS>.so we assume that
  // publications[0] is initialized.
  e.header.source = eqos.header.source;
  e.header.ttl = eqos.header.ttl;
  e.header.type = eqos.header.type;

  ACE_hrtime_t t = ACE_OS::gethrtime ();

  ORBSVCS_Time::hrtime_to_TimeT (e.header.creation_time,
                                 t);
#if !defined(TAO_LACKS_EVENT_CHANNEL_TIMESTAMPS)
  e.header.ec_recv_time = ORBSVCS_Time::zero ();
  e.header.ec_send_time = ORBSVCS_Time::zero ();
#endif /* TAO_LACKS_EVENT_CHANNEL_TIMESTAMPS */

  e.data.any_value = data;

  this->proxypushconsumer_->push (events);
}

void
TAO_CosEC_ProxyPushConsumer_i::disconnect_push_consumer (void)
{
  this->proxypushconsumer_->disconnect_push_consumer ();

  // Deactivate the ProxyPushConsumer
  PortableServer::POA_var poa =
    this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());
}

void
TAO_CosEC_ProxyPushConsumer_i::connect_push_supplier (CosEventComm::PushSupplier_ptr push_supplier)
{
  if (this->connected ())
    throw CosEventChannelAdmin::AlreadyConnected ();

  TAO_CosEC_PushSupplierWrapper *wrapper;

  ACE_NEW_THROW_EX (wrapper,
                    TAO_CosEC_PushSupplierWrapper (push_supplier),
                    CORBA::NO_MEMORY ());

  auto_ptr <TAO_CosEC_PushSupplierWrapper>
    auto_wrapper (wrapper);

  RtecEventComm::PushSupplier_ptr rtecpushsupplier
    = auto_wrapper.get ()->_this ();

  // give the ownership to the POA.
  auto_wrapper.get ()->_remove_ref ();

  this->proxypushconsumer_->connect_push_supplier
    (rtecpushsupplier,
     this->qos_);

  this->wrapper_ = auto_wrapper.release ();
}

int
TAO_CosEC_ProxyPushConsumer_i::connected (void)
{
  return this->wrapper_ == 0 ? 0 : 1;
}

