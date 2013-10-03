// $Id$

#include "Consumer.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/Event_Service_Constants.h"

#include "TestC.h"

Consumer::Consumer (bool valuetype)
  : event_count_ (0),
    valuetype_ (valuetype)
{
}

CORBA::ULong
Consumer::event_count (void) const
{
  return this->event_count_;
}

void
Consumer::connect (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin)
{
  this->proxy_ =
    consumer_admin->obtain_push_supplier ();

  RtecEventComm::PushConsumer_var me =
    this->_this ();

  // Simple subscription, but usually the helper classes in
  // $TAO_ROOT/orbsvcs/Event_Utils.h are a better way to do this.
  RtecEventChannelAdmin::ConsumerQOS qos;
  qos.is_gateway = 0;

  qos.dependencies.length (2);
  RtecEventComm::EventHeader& h0 =
    qos.dependencies[0].event.header;
  h0.type   = ACE_ES_DISJUNCTION_DESIGNATOR;
  h0.source = 1; // The disjunction has one element

  RtecEventComm::EventHeader& h1 =
    qos.dependencies[1].event.header;
  h1.type   = ACE_ES_EVENT_UNDEFINED;  // first free event type
  h1.source = ACE_ES_EVENT_SOURCE_ANY; // Any source is OK

  this->proxy_->connect_push_consumer (me.in (), qos);
}

void
Consumer::disconnect (void)
{
  try
    {
      // Disconnect from the proxy
      this->proxy_->disconnect_push_supplier ();
    }
  catch (const CORBA::Exception&)
    {
      // Ignore exceptions
    }
  this->proxy_ = RtecEventChannelAdmin::ProxyPushSupplier::_nil ();

  // Deactivate this object
  PortableServer::POA_var poa =
    this->_default_POA ();
  // Get the Object Id used for the servant..
  PortableServer::ObjectId_var oid =
    poa->servant_to_id (this);
  // Deactivate the object
  poa->deactivate_object (oid.in ());
}

void
Consumer::push (const RtecEventComm::EventSet& events)
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Consumer (%P|%t) no events\n"));
      return;
    }

  for (size_t i = 0; i < events.length (); ++i)
    {
      ++this->event_count_;

      if (this->valuetype_)
        {
          Hello::ValueTypeData * test_data = 0;
          if (events[i].data.any_value >>= test_data)
            {
              ACE_DEBUG ((LM_DEBUG, "Consumer (%P|%t): Received message <%d>: %C\n",
                          events[i].header.source, test_data->data ()));
              if (ACE_OS::strcmp (test_data->data (), "ACE/TAO/CIAO") != 0)
                {
                  ACE_ERROR ((LM_ERROR, "Consumer (%P|%t): ERROR received not expected message\n"));
                }
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "Consumer (%P|%t): ERROR failed to extract valuetype data\n"));
            }
        }
      else
        {
          const char* mystring = 0;
          if (events[i].data.any_value >>= mystring)
            {
              ACE_DEBUG ((LM_DEBUG, "Consumer (%P|%t): Received message <%d>: %C\n",
                                    events[i].header.source, mystring));
              if (ACE_OS::strcmp (mystring, "ACE/TAO/CIAO") != 0)
                {
                  ACE_ERROR ((LM_ERROR, "Consumer (%P|%t): ERROR received not expected message\n"));
                }
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "Consumer (%P|%t): ERROR failed to extract string data\n"));
            }
        }
    }

  ACE_DEBUG ((LM_DEBUG,
              "Consumer (%P|%t): %d events received\n",
              this->event_count_));
}

void
Consumer::disconnect_push_consumer (void)
{
}

