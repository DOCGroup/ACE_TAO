// $Id$

#include "Random.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Time_Utilities.h"
#include "ace/Arg_Shifter.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_unistd.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  RND_Driver driver;
  return driver.run (argc, argv);
}

// ****************************************************************

const int base_type = 20;

void
deactivate_servant (PortableServer::Servant servant)
{
  PortableServer::POA_var poa =
    servant->_default_POA ();
  PortableServer::ObjectId_var oid =
    poa->servant_to_id (servant);
  poa->deactivate_object (oid.in ());
}


RND_Driver::RND_Driver (void)
  :  timer_ (this),
     supplier_ (0),
     nsuppliers_ (4),
     nconsumers_ (4),
     max_recursion_ (1),
     verbose_ (0)
{
  TAO_EC_Default_Factory::init_svcs ();
}

int
RND_Driver::run (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // ****************************************************************

      ACE_Arg_Shifter arg_shifter (argc, argv);

      while (arg_shifter.is_anything_left ())
        {
          const ACE_TCHAR *arg = arg_shifter.get_current ();

          if (ACE_OS::strcasecmp (arg, ACE_TEXT("-suppliers")) == 0)
            {
              arg_shifter.consume_arg ();

              if (arg_shifter.is_parameter_next ())
                {
                  const ACE_TCHAR* opt = arg_shifter.get_current ();
                  int n = ACE_OS::atoi (opt);
                  if (n >= 1)
                    this->nsuppliers_ = n;
                  arg_shifter.consume_arg ();
                }
            }
          else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-consumers")) == 0)
            {
              arg_shifter.consume_arg ();

              if (arg_shifter.is_parameter_next ())
                {
                  const ACE_TCHAR* opt = arg_shifter.get_current ();
                  int n = ACE_OS::atoi (opt);
                  if (n >= 1)
                    this->nconsumers_ = n;
                  arg_shifter.consume_arg ();
                }
            }
          else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-max_recursion")) == 0)
            {
              arg_shifter.consume_arg ();

              if (arg_shifter.is_parameter_next ())
                {
                  const ACE_TCHAR* opt = arg_shifter.get_current ();
                  int n = ACE_OS::atoi (opt);
                  if (n >= 0)
                    this->max_recursion_ = n;
                  arg_shifter.consume_arg ();
                }
            }
          else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-verbose")) == 0)
            {
              arg_shifter.consume_arg ();

              this->verbose_ = 1;
            }
          else
            arg_shifter.ignore_arg ();
        }

      // ****************************************************************

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
      this->consumer_admin_ =
        event_channel->for_consumers ();

      // Obtain the supplier admin..
      this->supplier_admin_ =
        event_channel->for_suppliers ();

      // ****************************************************************

      {
        // Let's say that the execution time for event 2 is 1
        // milliseconds...
        ACE_Time_Value tv (0, 50000);
        TimeBase::TimeT time;
        ORBSVCS_Time::Time_Value_to_TimeT (time, tv);

        ACE_ConsumerQOS_Factory qos;
        qos.start_disjunction_group ();
        // The types int the range [0,ACE_ES_EVENT_UNDEFINED) are
        // reserved for the EC...
        qos.insert_time (ACE_ES_EVENT_INTERVAL_TIMEOUT,
                         time,
                         0);

        this->timer_.connect (this->consumer_admin_.in (),
                              qos.get_ConsumerQOS ());
      }

      // ****************************************************************

      {
        ACE_SupplierQOS_Factory qos;
        qos.insert (0, base_type, 0, 1);

        this->supplier_.connect (this->supplier_admin_.in (),
                                 qos.get_SupplierQOS ());
      }

      // ****************************************************************

      ACE_NEW_RETURN (this->consumers_,
                      RND_Consumer*[this->nconsumers_],
                      1);
      for (int i = 0; i != this->nconsumers_; ++i)
        {
          ACE_NEW_RETURN (this->consumers_[i],
                          RND_Consumer (this),
                          1);

          CORBA::Object_var obj =
            this->consumers_[i]->_this ();
        }

      // ****************************************************************

      ACE_NEW_RETURN (this->suppliers_,
                      RND_Supplier*[this->nsuppliers_],
                      1);
      for (int j = 0; j != this->nsuppliers_; ++j)
        {
          ACE_NEW_RETURN (this->suppliers_[j],
                          RND_Supplier (this->verbose_),
                          1);
          this->suppliers_[j]->activate ();

          CORBA::Object_var obj =
            this->suppliers_[j]->_this ();
        }

      // ****************************************************************

      ACE_Time_Value tv (30, 0);
      orb->run (tv);

      ACE_Thread_Manager::instance ()->wait ();

      // ****************************************************************

      {
        for (int k = 0; k != this->nsuppliers_; ++k)
          {
            deactivate_servant (this->suppliers_[k]);
            this->suppliers_[k]->_remove_ref ();
          }
        delete[] this->suppliers_;
        this->suppliers_ = 0;
      }

      // ****************************************************************

      // We destroy now to verify that the callbacks work and do not
      // produce any problems.
      event_channel->destroy ();

      // ****************************************************************

      {
        for (int k = 0; k != this->nconsumers_; ++k)
          {
            deactivate_servant (this->consumers_[k]);
            this->consumers_[k]->_remove_ref ();
          }
        delete[] this->consumers_;
        this->consumers_ = 0;
      }

      // ****************************************************************

      deactivate_servant (&ec_impl);

      // ****************************************************************

      poa->destroy (1, 1);

      // ****************************************************************

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Random");
      return 1;
    }
  return 0;
}

void
RND_Driver::timer (const RtecEventComm::Event &e)
{
  int r = ACE_OS::rand ();
  if (r < 0)
    r = -r;

  int n = r% 20;

  switch (n)
    {
    case 0:
    case 1:
      {
        // ACE_DEBUG ((LM_DEBUG, "Pushing an event\n"));
        if (e.header.source < this->max_recursion_)
          {
            RtecEventComm::EventSet event (1);
            event.length (1);
            event[0] = e;
            event[0].header.source ++;
            this->supplier_.push (event);
          }
      }
      break;

    default:
    case 2:
    case 3:
    case 4:
    case 5:
      // ACE_DEBUG ((LM_DEBUG, "Received event\n"));
      break;

    case 6:
      {
        int n = ACE_OS::rand () % this->nsuppliers_;

        // ACE_DEBUG ((LM_DEBUG, "Connecting supplier %d\n", n));

        ACE_SupplierQOS_Factory qos;
        qos.insert (0, base_type, 0, 1);

        this->suppliers_[n]->connect (this->supplier_admin_.in (),
                                      qos.get_SupplierQOS ());
      }
      break;

    case 7:
      {
        int n = ACE_OS::rand () % this->nconsumers_;

        // ACE_DEBUG ((LM_DEBUG, "Connecting consumer %d\n", n));

        ACE_ConsumerQOS_Factory qos;
        qos.start_disjunction_group ();
        qos.insert_type (base_type, 0);

        this->consumers_[n]->connect (this->consumer_admin_.in (),
                                      qos.get_ConsumerQOS ());
      }
      break;

    case 8:
      {
        int n = ACE_OS::rand () % this->nsuppliers_;

        // ACE_DEBUG ((LM_DEBUG, "Disconnecting supplier %d\n", n));

        this->suppliers_[n]->disconnect ();
      }
      break;

    case 9:
      {
        int n = ACE_OS::rand () % this->nconsumers_;

        // ACE_DEBUG ((LM_DEBUG, "Disconnecting consumer %d\n", n));

        this->consumers_[n]->disconnect ();
      }
      break;
    }
}

void
RND_Driver::event (const RtecEventComm::Event &e)
{
  this->timer (e);
}

// ****************************************************************

void
RND_Timer::push (const RtecEventComm::EventSet &event)
{
  try
    {
      this->driver_->timer (event[0]);
    }
  catch (const CORBA::Exception&)
    {
    }
}

// ****************************************************************

void
RND_Consumer::connect (RtecEventChannelAdmin::ConsumerAdmin_ptr admin,
                       const RtecEventChannelAdmin::ConsumerQOS &qos)
{
  RtecEventChannelAdmin::ProxyPushSupplier_var proxy;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

    if (CORBA::is_nil (this->proxy_.in ()))
      {
        this->proxy_ = admin->obtain_push_supplier ();
      }
    proxy =
      RtecEventChannelAdmin::ProxyPushSupplier::_duplicate(this->proxy_.in ());
  }
  RtecEventComm::PushConsumer_var me =
    this->_this ();
  proxy->connect_push_consumer (me.in (),
                                qos);
}

void
RND_Consumer::disconnect (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (CORBA::is_nil (this->proxy_.in ()))
    return;
  this->proxy_->disconnect_push_supplier ();
  this->proxy_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
}

void
RND_Consumer::push (const RtecEventComm::EventSet &event)
{
  this->driver_->event (event[0]);
}

void
RND_Consumer::disconnect_push_consumer (void)
{
}

// ****************************************************************

void
RND_Supplier::connect (RtecEventChannelAdmin::SupplierAdmin_ptr admin,
                       const RtecEventChannelAdmin::SupplierQOS &qos)
{
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

    if (CORBA::is_nil (this->proxy_.in ()))
      {
        this->proxy_ = admin->obtain_push_consumer ();
      }

    proxy =
      RtecEventChannelAdmin::ProxyPushConsumer::_duplicate(this->proxy_.in ());
  }
  RtecEventComm::PushSupplier_var me =
    this->_this ();
  proxy->connect_push_supplier (me.in (),
                                qos);
}

void
RND_Supplier::disconnect (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (CORBA::is_nil (this->proxy_.in ()))
    return;
  this->proxy_->disconnect_push_consumer ();
  this->proxy_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();
}

void
RND_Supplier::push_new_event (void)
{
  RtecEventComm::EventSet event (1);
  event.length (1);
  event[0].header.type   = base_type;
  event[0].header.source = 0;

  this->push (event);
}

void
RND_Supplier::push (RtecEventComm::EventSet &event)
{
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

    if (CORBA::is_nil (this->proxy_.in ()))
      return;

    proxy =
      RtecEventChannelAdmin::ProxyPushConsumer::_duplicate(this->proxy_.in ());
  }

  proxy->push (event);
}

void
RND_Supplier::disconnect_push_supplier (void)
{
}

int
RND_Supplier::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "Thread %t started\n"));
  int percent = 10;
  int niterations = 5000;
  for (int i = 0; i != niterations; ++i)
    {
      try
        {
          ACE_Time_Value tv (0, 10000);
          ACE_OS::sleep (tv);

          this->push_new_event ();
        }
      catch (const CORBA::Exception&)
        {
        }
      if (this->verbose_
          && i * 100 / niterations >= percent)
        {
          ACE_DEBUG ((LM_DEBUG, "Thread %t %d%%\n", percent));
          percent += 10;
        }
    }
  ACE_DEBUG ((LM_DEBUG, "Thread %t completed\n"));
  return 0;
}
