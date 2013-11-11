// $Id$

#include "Supplier.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/Event_Service_Constants.h"

Supplier::Supplier (bool valuetype) : valuetype_ (valuetype), event_count_ (0)
{
}

void
Supplier::connect (RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin)
{
  this->proxy_ =
    supplier_admin->obtain_push_consumer ();

  RtecEventComm::PushSupplier_var me =
    this->_this ();

  // Simple publication, but usually the helper classes in
  // $TAO_ROOT/orbsvcs/Event_Utils.h are a better way to do this.
  RtecEventChannelAdmin::SupplierQOS qos;
  qos.is_gateway = 0;

  qos.publications.length (1);
  RtecEventComm::EventHeader& h0 =
    qos.publications[0].event.header;
  h0.type   = ACE_ES_EVENT_UNDEFINED; // first free event type
  h0.source = 1;                      // first free event source

  this->proxy_->connect_push_supplier (me.in (), qos);
}

void
Supplier::disconnect (void)
{
  // Disconnect from the EC
  try
    {
      this->proxy_->disconnect_push_consumer ();
    }
  catch (const CORBA::Exception&)
    {
    }

  PortableServer::POA_var poa =
    this->_default_POA ();
  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);
  poa->deactivate_object (id.in ());
}

void
Supplier::insert_into_any (CORBA::Any& any, Components::EventBase* vb)
{
  any <<= vb;
}

void
Supplier::perform_push (void)
{
  try
    {
      // The event type and source must match our publications
      ++event_count_;
      ACE_DEBUG ((LM_DEBUG, "Sending event %d\n", event_count_));
      RtecEventComm::EventSet event (1);
      event.length (1);
      event[0].header.type   = ACE_ES_EVENT_UNDEFINED;
      event[0].header.source = event_count_;
      // Avoid loops throught the event channel federations
      event[0].header.ttl    = 1;

      if (this->valuetype_)
        {
          OBV_Hello::ValueTypeData * test_data = 0;
          ACE_NEW (test_data, OBV_Hello::ValueTypeData ());
          test_data->data ("ACE/TAO/CIAO");
          insert_into_any (event[0].data.any_value, test_data);
        }
      else
        {
          event[0].data.any_value <<= CORBA::string_dup( "ACE/TAO/CIAO");
        }

      this->proxy_->push (event);
    }
  catch (const CORBA::Exception&)
    {
    }
}

void
Supplier::disconnect_push_supplier (void)
{
}

