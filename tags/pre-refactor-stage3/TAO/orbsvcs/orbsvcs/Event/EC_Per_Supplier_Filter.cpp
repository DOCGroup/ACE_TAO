// $Id$

#include "EC_Per_Supplier_Filter.h"
#include "EC_Event_Channel_Base.h"
#include "EC_ProxySupplier.h"
#include "EC_ProxyConsumer.h"
#include "EC_Scheduling_Strategy.h"
#include "EC_QOS_Info.h"

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"

#include "orbsvcs/Event_Service_Constants.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Per_Supplier_Filter.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Per_Supplier_Filter, "$Id$")

TAO_EC_Per_Supplier_Filter::
    TAO_EC_Per_Supplier_Filter (TAO_EC_Event_Channel_Base* ec)
  :  event_channel_ (ec),
     consumer_ (0),
     refcnt_ (1)
{
  this->event_channel_->create_proxy_collection (this->collection_);
  // @@
  // @@ this->collection_->busy_hwm (this->event_channel_->busy_hwm ());
  // @@ this->collection_->max_write_delay (
  // @@           this->event_channel_->max_write_delay ()
  // @@ );
}

TAO_EC_Per_Supplier_Filter::~TAO_EC_Per_Supplier_Filter (void)
{
  this->event_channel_->destroy_proxy_collection (this->collection_);
  this->collection_ = 0;
}

void
TAO_EC_Per_Supplier_Filter::bind (TAO_EC_ProxyPushConsumer* consumer)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->consumer_ != 0)
    return;

  this->consumer_ = consumer;
}

void
TAO_EC_Per_Supplier_Filter::unbind (TAO_EC_ProxyPushConsumer* consumer)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->consumer_ == 0 || this->consumer_ != consumer)
    return;

  this->consumer_ = 0;

  ACE_TRY_NEW_ENV
    {
      this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // @@ Ignore exceptions
    }
  ACE_ENDTRY;
}

void
TAO_EC_Per_Supplier_Filter::connected (TAO_EC_ProxyPushSupplier* supplier
                                       ACE_ENV_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->consumer_ == 0)
    return;

  const RtecEventChannelAdmin::SupplierQOS& pub =
    this->consumer_->publications_i ();

  for (CORBA::ULong j = 0; j < pub.publications.length (); ++j)
    {
      const RtecEventComm::Event& event =
        pub.publications[j].event;

#if TAO_EC_ENABLE_DEBUG_MESSAGES
      ACE_DEBUG ((LM_DEBUG, "Connecting consumer <%x> to <%x>, "
                  "trying event <%d:%d>  ",
                  supplier, this,
                  event.header.source, event.header.type));
#endif /* TAO_EC_ENABLED_DEBUG_MESSAGES */
      if (supplier->can_match (event.header))
        {
#if TAO_EC_ENABLE_DEBUG_MESSAGES
          ACE_DEBUG ((LM_DEBUG, "  matched\n"));
#endif /* TAO_EC_ENABLED_DEBUG_MESSAGES */
          this->collection_->connected (supplier ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
          return;
        }
#if TAO_EC_ENABLE_DEBUG_MESSAGES
      ACE_DEBUG ((LM_DEBUG, "  not matched\n"));
#endif /* TAO_EC_ENABLED_DEBUG_MESSAGES */
    }
}

void
TAO_EC_Per_Supplier_Filter::reconnected (TAO_EC_ProxyPushSupplier* supplier
                                         ACE_ENV_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->consumer_ == 0)
    return;

  const RtecEventChannelAdmin::SupplierQOS& pub =
    this->consumer_->publications_i ();

  for (CORBA::ULong j = 0; j < pub.publications.length (); ++j)
    {
      const RtecEventComm::Event& event =
        pub.publications[j].event;

      //      ACE_DEBUG ((LM_DEBUG, "Trying %d:%d in %x\n",
      //                  event.header.source, event.header.type,
      //                  this));
      if (supplier->can_match (event.header))
        {
          //          ACE_DEBUG ((LM_DEBUG, "  matched %x\n", supplier));
          this->collection_->connected (supplier ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
          return;
        }
    }
  this->collection_->disconnected (supplier ACE_ENV_ARG_PARAMETER);
}

void
TAO_EC_Per_Supplier_Filter::disconnected (TAO_EC_ProxyPushSupplier* supplier
                                          ACE_ENV_ARG_DECL)
{
  this->collection_->disconnected (supplier ACE_ENV_ARG_PARAMETER);
}

void
TAO_EC_Per_Supplier_Filter::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  this->collection_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_EC_Per_Supplier_Filter::push (const RtecEventComm::EventSet& event,
                                  TAO_EC_ProxyPushConsumer *consumer
                                  ACE_ENV_ARG_DECL)
{
  TAO_EC_Scheduling_Strategy* scheduling_strategy =
    this->event_channel_->scheduling_strategy ();
  scheduling_strategy->schedule_event (event,
                                       consumer,
                                       this
                                       ACE_ENV_ARG_PARAMETER);
}

void
TAO_EC_Per_Supplier_Filter::push_scheduled_event (RtecEventComm::EventSet &event,
                                                  const TAO_EC_QOS_Info &event_info
                                                  ACE_ENV_ARG_DECL)
{
  TAO_EC_Filter_Worker worker (event, event_info);
  this->collection_->for_each (&worker ACE_ENV_ARG_PARAMETER);
}

CORBA::ULong
TAO_EC_Per_Supplier_Filter::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

  this->refcnt_++;
  return this->refcnt_;
}

CORBA::ULong
TAO_EC_Per_Supplier_Filter::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

    this->refcnt_--;
    if (this->refcnt_ != 0)
      return this->refcnt_;
  }
  this->event_channel_->supplier_filter_builder ()->destroy (this);
  return 0;
}

// ****************************************************************

TAO_EC_Supplier_Filter*
TAO_EC_Per_Supplier_Filter_Builder::create (
    RtecEventChannelAdmin::SupplierQOS&)
{
  return new TAO_EC_Per_Supplier_Filter (this->event_channel_);
}

void
TAO_EC_Per_Supplier_Filter_Builder::destroy (
    TAO_EC_Supplier_Filter* x)
{
  delete x;
}
