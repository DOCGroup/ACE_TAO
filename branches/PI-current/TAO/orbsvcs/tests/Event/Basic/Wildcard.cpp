// $Id$

#include "Wildcard.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

ACE_RCSID(EC_Tests, Wildcard, "$Id$")

int
main (int argc, char* argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      TAO_EC_Event_Channel_Attributes attributes (poa.in (),
                                                  poa.in ());
      
      TAO_EC_Event_Channel ec_impl (attributes);
      ec_impl.activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var event_channel =
        ec_impl._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;


      // ****************************************************************
      
      // Obtain the consumer admin..
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      const int event_type = 20;
      const int event_source = 10;
      const int milliseconds = 50;

      Supplier supplier;

      supplier.connect (event_channel.in (),
                        milliseconds,
                        event_source,
                        event_type,
                        event_source,
                        event_type,
                        ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      Supplier other_supplier;

      other_supplier.connect (event_channel.in (),
                              milliseconds,
                              event_source + 1,
                              event_type + 1,
                              event_source + 1,
                              event_type + 1,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      Supplier any_source_supplier;

      any_source_supplier.connect (event_channel.in (),
                                   milliseconds,
                                   0,
                                   event_type,
                                   event_source,
                                   event_type,
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      Supplier any_type_supplier;

      any_type_supplier.connect (event_channel.in (),
                                 50,
                                 event_source,
                                 0,
                                 event_source,
                                 event_type,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      Supplier wildcard_supplier;

      wildcard_supplier.connect (event_channel.in (),
                                 50,
                                 0,
                                 0,
                                 event_source,
                                 event_type,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      Consumer regular_consumer ("Consumer/regular");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....


      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group ();
        consumer_qos.insert (event_source, event_type, 0);

        regular_consumer.connect (consumer_admin.in (),
                                  consumer_qos.get_ConsumerQOS (),
                                  ACE_TRY_ENV);
        ACE_TRY_CHECK;
      }

      // ****************************************************************

      Consumer any_type_consumer ("Consumer/any_type");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....


      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group ();
        consumer_qos.insert (event_source, 0, 0);

        any_type_consumer.connect (consumer_admin.in (),
                                   consumer_qos.get_ConsumerQOS (),
                                   ACE_TRY_ENV);
        ACE_TRY_CHECK;
      }

      // ****************************************************************

      Consumer any_source_consumer ("Consumer/any_source");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....


      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group ();
        consumer_qos.insert (0, event_type, 0);

        any_source_consumer.connect (consumer_admin.in (),
                                     consumer_qos.get_ConsumerQOS (),
                                     ACE_TRY_ENV);
        ACE_TRY_CHECK;
      }

      // ****************************************************************

      Consumer wildcard_consumer ("Consumer/wildcard");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....


      {
        ACE_ConsumerQOS_Factory consumer_qos;
        consumer_qos.start_disjunction_group ();
        consumer_qos.insert (0, 0, 0);

        wildcard_consumer.connect (consumer_admin.in (),
                                   consumer_qos.get_ConsumerQOS (),
                                   ACE_TRY_ENV);
        ACE_TRY_CHECK;
      }

      // ****************************************************************

      ACE_Time_Value tv (5, 0);
      // Wait for events, using work_pending()/perform_work() may help 
      // or using another thread, this example is too simple for that.
      orb->run (tv);

      // ****************************************************************

      wildcard_consumer.disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      any_source_consumer.disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      any_type_consumer.disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      regular_consumer.disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      wildcard_supplier.disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      any_type_supplier.disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      any_source_supplier.disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      other_supplier.disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      supplier.disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      event_channel->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      wildcard_consumer.dump_results (500, 5);
      any_source_consumer.dump_results (400, 5);
      any_type_consumer.dump_results (400, 5);
      regular_consumer.dump_results (400, 5);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Service");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}

// ****************************************************************

Consumer::Consumer (const char* name)
  : event_count (0),
    name_ (name)
{
}

void
Consumer::connect (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                   const RtecEventChannelAdmin::ConsumerQOS &qos,
                   CORBA::Environment &ACE_TRY_ENV)
{
  // The canonical protocol to connect to the EC

  RtecEventComm::PushConsumer_var consumer =
    this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier (ACE_TRY_ENV);
  ACE_CHECK;

  this->supplier_proxy_->connect_push_consumer (consumer.in (),
                                                qos,
                                                ACE_TRY_ENV);
  ACE_CHECK;
}

void
Consumer::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  this->supplier_proxy_->disconnect_push_supplier (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::POA_var consumer_poa =
    this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;
  PortableServer::ObjectId_var consumer_id =
    consumer_poa->servant_to_id (this, ACE_TRY_ENV);
  ACE_CHECK;
  consumer_poa->deactivate_object (consumer_id.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

void
Consumer::dump_results (int expected_count, int tolerance)
{
  int diff = this->event_count - expected_count;
  if (diff > tolerance || diff < -tolerance)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "ERROR - %s unexpected number of events  <%d>\n",
                  this->name_,
                  this->event_count));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s - number of events <%d> within margins\n",
                  this->name_,
                  this->event_count));
    }
}

void
Consumer::push (const RtecEventComm::EventSet& events,
                CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s (%P|%t) no events\n", this->name_));
      return;
    }

  this->event_count ++;
#if 0
  if (this->event_count % 10 == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s (%P|%t): %d events received\n",
                  this->name_,
                  this->event_count));
    }
#endif /* 0 */
}

void
Consumer::disconnect_push_consumer (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

// ****************************************************************

Supplier::Supplier (void)
  :  consumer_adapter_ (this),
     event_source_ (-1),
     event_type_ (ACE_ES_EVENT_UNDEFINED)
{
}

void
Supplier::connect (RtecEventChannelAdmin::EventChannel_ptr event_channel,
                   int milliseconds,
                   int published_source,
                   int published_type,
                   int event_source,
                   int event_type,
                   CORBA::Environment &ACE_TRY_ENV)
{
  this->event_source_ = event_source;
  this->event_type_ = event_type;

  // The canonical protocol to connect to the EC
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    event_channel->for_consumers (ACE_TRY_ENV);
  ACE_CHECK;

  RtecEventComm::PushConsumer_var consumer =
    this->consumer_adapter_._this (ACE_TRY_ENV);
  ACE_CHECK;

  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier (ACE_TRY_ENV);
  ACE_CHECK;

  // Let's say that the execution time for event 2 is 1
  // milliseconds...
  ACE_Time_Value tv (0, milliseconds * 1000);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);

  ACE_ConsumerQOS_Factory consumer_qos;
  consumer_qos.start_disjunction_group ();
  consumer_qos.insert_time (ACE_ES_EVENT_INTERVAL_TIMEOUT,
                            time,
                            0);

  this->supplier_proxy_->connect_push_consumer (consumer.in (),
                                                consumer_qos.get_ConsumerQOS (),
                                                ACE_TRY_ENV);
  ACE_CHECK;

  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    event_channel->for_suppliers (ACE_TRY_ENV);
  ACE_CHECK;

  RtecEventComm::PushSupplier_var supplier =
    this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  this->consumer_proxy_ =
    supplier_admin->obtain_push_consumer (ACE_TRY_ENV);
  ACE_CHECK;

  ACE_SupplierQOS_Factory supplier_qos;
  supplier_qos.insert (published_source,
                       published_type,
                       0, 1);
  
  this->consumer_proxy_->connect_push_supplier (supplier.in (),
                                                supplier_qos.get_SupplierQOS (),
                                                ACE_TRY_ENV);
  ACE_CHECK;
}

void
Supplier::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  this->supplier_proxy_->disconnect_push_supplier (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::POA_var consumer_poa =
    this->consumer_adapter_._default_POA (ACE_TRY_ENV);
  ACE_CHECK;
  PortableServer::ObjectId_var consumer_id =
    consumer_poa->servant_to_id (&this->consumer_adapter_, ACE_TRY_ENV);
  ACE_CHECK;
  consumer_poa->deactivate_object (consumer_id.in (), ACE_TRY_ENV);
  ACE_CHECK;

  this->consumer_proxy_->disconnect_push_consumer (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::POA_var supplier_poa =
    this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;
  PortableServer::ObjectId_var supplier_id =
    supplier_poa->servant_to_id (this, ACE_TRY_ENV);
  ACE_CHECK;
  supplier_poa->deactivate_object (supplier_id.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

void
Supplier::push (const RtecEventComm::EventSet&,
                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  RtecEventComm::EventSet event (1);
  event.length (1);
  event[0].header.source = this->event_source_;
  event[0].header.type = this->event_type_;

  this->consumer_proxy_->push (event, ACE_TRY_ENV);

#if 0
  static int event_count = 0;
  event_count++;
  if (event_count % 10 == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Supplier (%P|%t): %d events sent\n",
                  event_count));
    }
#endif /* 0 */
}

void
Supplier::disconnect_push_consumer (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Supplier::disconnect_push_supplier (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_PushConsumer_Adapter<Supplier>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_PushConsumer_Adapter<Supplier>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
