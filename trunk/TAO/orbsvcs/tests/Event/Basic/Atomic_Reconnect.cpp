// $Id$

#include "Atomic_Reconnect.h"
#include "Counting_Supplier.h"

#include "ace/OS_NS_unistd.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

const int event_type = 20;
const int event_source = 10;

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  try
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in ());
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

      // ****************************************************************

      TAO_EC_Event_Channel_Attributes attributes (poa.in (),
                                                  poa.in ());
      attributes.consumer_reconnect = 1;
      attributes.supplier_reconnect = 1;

      TAO_EC_Event_Channel ec_impl (attributes);
      ec_impl.activate ();

      RtecEventChannelAdmin::EventChannel_var event_channel =
        ec_impl._this ();


      // ****************************************************************

      // Obtain the consumer admin..
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers ();

      // Obtain the supplier admin..
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers ();

      // ****************************************************************

      EC_Counting_Supplier supplier0;
      EC_Counting_Supplier supplier1;

      supplier0.connect (supplier_admin.in (),
                         event_source,
                         event_type,
                         event_source,
                         event_type);

      supplier1.connect (supplier_admin.in (),
                         event_source,
                         event_type + 1,
                         event_source,
                         event_type + 1);

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
                          consumer_qos01.get_ConsumerQOS ());

      Consumer consumer0 ("Consumer/0", event_type);
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      consumer0.connect (consumer_admin.in (),
                         consumer_qos0.get_ConsumerQOS ());

      Consumer consumer1 ("Consumer/1", event_type);
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      consumer1.connect (consumer_admin.in (),
                         consumer_qos1.get_ConsumerQOS ());

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
                             consumer_qos0.get_ConsumerQOS ());
          consumer1.connect (consumer_admin.in (),
                             consumer_qos1.get_ConsumerQOS ());
          if (i % 2 == 0)
            {
              consumer01.connect (consumer_admin.in (),
                                  consumer_qos0.get_ConsumerQOS ());
            }
          else
            {
              consumer01.connect (consumer_admin.in (),
                                  consumer_qos01.get_ConsumerQOS ());
            }

          ACE_OS::sleep (tv);
        }

      task0.stop ();
      task1.stop ();

      ACE_Thread_Manager::instance ()->wait ();


      // ****************************************************************

      // Cleanup..

      consumer01.disconnect ();
      consumer1.disconnect ();
      consumer0.disconnect ();

      // ****************************************************************

      supplier1.disconnect ();
      supplier0.disconnect ();

      // ****************************************************************

      event_channel->destroy ();

      // ****************************************************************

      poa->destroy (1, 1);

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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Service");
      return 1;
    }
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
                  "ERROR - %C unexpected number of base events  <%d>\n",
                  this->name_,
                  this->event_base_count));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%C - number of base events <%d> within margins\n",
                  this->name_,
                  this->event_base_count));
    }

  if (this->event_count < CORBA::ULong(base_count)
      || this->event_count >= CORBA::ULong(base_count + extra_count))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "ERROR - %C unexpected number of events  <%d,%d,%d>\n",
                  this->name_,
                  base_count,
                  this->event_count,
                  base_count + extra_count));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%C number of events "
                  "<%d,%d,%d> within margins\n",
                  this->name_,
                  base_count,
                  this->event_count,
                  base_count + extra_count));
    }
}

void
Consumer::push (const RtecEventComm::EventSet& events)
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%C (%P|%t) no events\n", this->name_));
      return;
    }

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->event_count++;

  // ACE_DEBUG ((LM_DEBUG,
  //               "Consumer %C has received %d events\n",
  //               this->name_, this->event_count));

  if (events[0].header.type == this->event_base_type_)
    this->event_base_count++;
}
