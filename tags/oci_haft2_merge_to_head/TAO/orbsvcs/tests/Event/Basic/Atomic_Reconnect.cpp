// $Id$

#include "Atomic_Reconnect.h"
#include "Counting_Supplier.h"

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
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      TAO_EC_Event_Channel_Attributes attributes (poa.in (),
                                                  poa.in ());
      attributes.consumer_reconnect = 1;
      attributes.supplier_reconnect = 1;

      TAO_EC_Event_Channel ec_impl (attributes);
      ec_impl.activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var event_channel =
        ec_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;


      // ****************************************************************

      // Obtain the consumer admin..
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain the supplier admin..
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      EC_Counting_Supplier supplier0;
      EC_Counting_Supplier supplier1;

      supplier0.connect (supplier_admin.in (),
                         event_source,
                         event_type,
                         event_source,
                         event_type
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      supplier1.connect (supplier_admin.in (),
                         event_source,
                         event_type + 1,
                         event_source,
                         event_type + 1
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      Consumer consumer01 ("Consumer/01", event_type);
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
                          consumer_qos01.get_ConsumerQOS ()
                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Consumer consumer0 ("Consumer/0", event_type);
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      consumer0.connect (consumer_admin.in (),
                         consumer_qos0.get_ConsumerQOS ()
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Consumer consumer1 ("Consumer/1", event_type);
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      consumer1.connect (consumer_admin.in (),
                         consumer_qos1.get_ConsumerQOS ()
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      EC_Counting_Supplier_Task task0 (&supplier0);
      EC_Counting_Supplier_Task task1 (&supplier1);

      task0.activate ();
      task1.activate ();

      // ****************************************************************

      const int iterations = 1000;

      for (int i = 0; i != iterations; ++i)
        {
          ACE_Time_Value tv (0, 10000);
          consumer0.connect (consumer_admin.in (),
                             consumer_qos0.get_ConsumerQOS ()
                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          consumer1.connect (consumer_admin.in (),
                             consumer_qos1.get_ConsumerQOS ()
                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          if (i % 2 == 0)
            {
              consumer01.connect (consumer_admin.in (),
                                  consumer_qos0.get_ConsumerQOS ()
                                  ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          else
            {
              consumer01.connect (consumer_admin.in (),
                                  consumer_qos01.get_ConsumerQOS ()
                                  ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          ACE_OS::sleep (tv);
        }

      task0.stop ();
      task1.stop ();

      ACE_Thread_Manager::instance ()->wait ();


      // ****************************************************************

      // Cleanup..

      consumer01.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      consumer1.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      consumer0.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      supplier1.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier0.disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      event_channel->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      ACE_DEBUG ((LM_DEBUG,
                  "Task 0 pushed %d events\n", task0.push_count ()));
      ACE_DEBUG ((LM_DEBUG,
                  "Task 1 pushed %d events\n", task1.push_count ()));
      ACE_DEBUG ((LM_DEBUG,
                  "Supplier 0 pushed %d events\n", supplier0.event_count));
      ACE_DEBUG ((LM_DEBUG,
                  "Supplier 1 pushed %d events\n", supplier1.event_count));
      consumer0.dump_results (task0.push_count (), 5);
      consumer1.dump_results (task1.push_count (), 5);
      consumer01.dump_results (task0.push_count (),
                               task1.push_count (),
                               1);
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

Consumer::Consumer (const char* name,
                    int base_type)
  : EC_Counting_Consumer (name),
    event_base_count (0),
    event_base_type_ (base_type)
{
}

void
Consumer::dump_results (int expected_count,
                        int tolerance)
{
  this->EC_Counting_Consumer::dump_results (expected_count, tolerance);
}

void
Consumer::dump_results (int base_count,
                        int extra_count,
                        int tolerance)
{
  int diff = this->event_base_count - base_count;
  if (diff > tolerance || diff < -tolerance)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "ERROR - %s unexpected number of base events  <%d>\n",
                  this->name_,
                  this->event_base_count));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s - number of base events <%d> within margins\n",
                  this->name_,
                  this->event_base_count));
    }

  if (this->event_count < CORBA::ULong(base_count)
      || this->event_count >= CORBA::ULong(base_count + extra_count))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "ERROR - %s unexpected number of events  <%d,%d,%d>\n",
                  this->name_,
                  base_count,
                  this->event_count,
                  base_count + extra_count));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s number of events "
                  "<%d,%d,%d> within margins\n",
                  this->name_,
                  base_count,
                  this->event_count,
                  base_count + extra_count));
    }
}

void
Consumer::push (const RtecEventComm::EventSet& events
                ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s (%P|%t) no events\n", this->name_));
      return;
    }

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->event_count++;

  // ACE_DEBUG ((LM_DEBUG,
  //               "Consumer %s has received %d events\n",
  //               this->name_, this->event_count));

  if (events[0].header.type == this->event_base_type_)
    this->event_base_count++;
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
