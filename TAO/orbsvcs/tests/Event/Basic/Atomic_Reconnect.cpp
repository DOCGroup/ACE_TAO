// $Id$

#include "Atomic_Reconnect.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

ACE_RCSID(EC_Tests, Atomic_Reconnect, "$Id$")

const int event_type = 20;
const int event_source = 10;

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
      attributes.consumer_reconnect = 1;
      attributes.supplier_reconnect = 1;

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

      const int milliseconds = 50;

      Supplier supplier0;
      Supplier supplier1;

      supplier0.connect (event_channel.in (),
                         milliseconds,
                         event_source,
                         event_type,
                         event_source,
                         event_type,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Supplier second_supplier;

      supplier1.connect (event_channel.in (),
                         milliseconds,
                         event_source,
                         event_type + 1,
                         event_source,
                         event_type + 1,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      Consumer consumer01 ("Consumer/010");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      ACE_ConsumerQOS_Factory consumer_qos0;
      consumer_qos0.start_disjunction_group ();
      consumer_qos0.insert (event_source, event_type, 0);

      ACE_ConsumerQOS_Factory consumer_qos1;
      consumer_qos1.start_disjunction_group ();
      consumer_qos1.insert (event_source, event_type + 1, 0);

      ACE_ConsumerQOS_Factory consumer_qos01;
      consumer_qos01.start_disjunction_group ();
      consumer_qos01.insert (event_source, event_type, 0);
      consumer_qos01.insert (event_source, event_type + 1, 0);

      consumer01.connect (consumer_admin.in (),
                          consumer_qos01.get_ConsumerQOS (),
                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Consumer consumer0 ("Consumer/0");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      consumer0.connect (consumer_admin.in (),
                         consumer_qos0.get_ConsumerQOS (),
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Consumer consumer1 ("Consumer/1");
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      consumer1.connect (consumer_admin.in (),
                         consumer_qos1.get_ConsumerQOS (),
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      RtecEventComm::EventSet event0 (1);
      event0.length (1);
      event0[0].header.type = event_type;
      event0[0].header.source = event_source;

      RtecEventComm::EventSet event1 (1);
      event1.length (1);
      event1[0].header.type = event_type + 1;
      event1[0].header.source = event_source;

      // ****************************************************************

      Task task0 (&supplier0, event0);
      Task task1 (&supplier1, event1);

      task0.activate ();
      task1.activate ();

      // ****************************************************************

      const int iterations = 1000;

      for (int i = 0; i != iterations; ++i)
        {
          ACE_Time_Value tv (0, 10000);
          consumer0.connect (consumer_admin.in (),
                             consumer_qos0.get_ConsumerQOS (),
                             ACE_TRY_ENV);
          ACE_TRY_CHECK;
          consumer1.connect (consumer_admin.in (),
                             consumer_qos1.get_ConsumerQOS (),
                             ACE_TRY_ENV);
          ACE_TRY_CHECK;
          if (i % 2 == 0)
            {
              consumer01.connect (consumer_admin.in (),
                                  consumer_qos0.get_ConsumerQOS (),
                                  ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          else
            {
              consumer01.connect (consumer_admin.in (),
                                  consumer_qos01.get_ConsumerQOS (),
                                  ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }

          ACE_OS::sleep (tv);
        }

      task0.stop ();
      task1.stop ();

      ACE_Thread_Manager::instance ()->wait ();


      // ****************************************************************

      // Cleanup..

      consumer01.disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      consumer1.disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      consumer0.disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      supplier1.disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      supplier0.disconnect (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      event_channel->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // ****************************************************************

      ACE_DEBUG ((LM_DEBUG,
                  "Supplier 0 pushed %d events\n", task0.push_count ()));
      ACE_DEBUG ((LM_DEBUG,
                  "Supplier 1 pushed %d events\n", task1.push_count ()));
      consumer0.dump_results (task0.push_count (), 5);
      consumer1.dump_results (task1.push_count (), 5);
      CORBA::ULong max = task0.push_count () + task1.push_count ();
      if (consumer01.event0_count != task0.push_count ()
          || consumer01.event_count > max)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "ERROR - %s unexpected number of events  <%d,%d>\n",
                      "Consumer/01",
                      consumer01.event0_count,
                      consumer01.event_count));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Consumer/01 number of events "
                      "<%d,%d> within margins\n",
                      consumer01.event0_count,
                      consumer01.event_count));
        }
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

Task::Task (Supplier *s,
            const RtecEventComm::EventSet &e)
  :  supplier_ (s),
     event_ (e),
     stop_flag_ (0),
     push_count_ (0)
{
}

int
Task::svc ()
{
  ACE_TRY_NEW_ENV
    {
      this->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

void
Task::stop (void)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);
  this->stop_flag_ = 1;
}

CORBA::ULong
Task::push_count (void)
{
  return this->push_count_;
}

void
Task::run (CORBA::Environment &ACE_TRY_ENV)
{
  int stop = 0;
  do {
    this->supplier_->push (this->event_, ACE_TRY_ENV);
    ACE_CHECK;

    // Sleep for a short time to avoid spinning...
    ACE_OS::sleep (0);

    ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);

    this->push_count_++;

    stop = this->stop_flag_;
  } while (stop == 0);
}

// ****************************************************************

Consumer::Consumer (const char* name)
  : event_count (0),
    event0_count (0),
    name_ (name)
{
}

void
Consumer::connect (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                   const RtecEventChannelAdmin::ConsumerQOS &qos,
                   CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    {
      this->supplier_proxy_ =
        consumer_admin->obtain_push_supplier (ACE_TRY_ENV);
      ACE_CHECK;
    }

  // The canonical protocol to connect to the EC

  RtecEventComm::PushConsumer_var consumer =
    this->_this (ACE_TRY_ENV);
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
Consumer::dump_results (int expected_count,
                        int tolerance)
{
  int diff = expected_count - event_count;
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

  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);
  this->event_count++;
  if (events[0].header.type == event_type)
    this->event0_count++;
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
  :  event_source_ (-1),
     event_type_ (ACE_ES_EVENT_UNDEFINED)
{
}

void
Supplier::connect (RtecEventChannelAdmin::EventChannel_ptr event_channel,
                   int /* milliseconds */,
                   int published_source,
                   int published_type,
                   int event_source,
                   int event_type,
                   CORBA::Environment &ACE_TRY_ENV)
{
  this->event_source_ = event_source;
  this->event_type_ = event_type;

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
Supplier::push (const RtecEventComm::EventSet &event,
                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->consumer_proxy_->push (event, ACE_TRY_ENV);
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

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
