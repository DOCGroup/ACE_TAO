// $Id$

#include "Random.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"
#include "ace/Arg_Shifter.h"

ACE_RCSID(CEC_Tests, Random, "$Id$")

int
main (int argc, char* argv[])
{
  RND_Driver driver;
  return driver.run (argc, argv);
}

// ****************************************************************

void
deactivate_servant (PortableServer::Servant servant
                    ACE_ENV_ARG_DECL)
{
  PortableServer::POA_var poa =
    servant->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ObjectId_var oid =
    poa->servant_to_id (servant ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  poa->deactivate_object (oid.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


RND_Driver::RND_Driver (void)
  :  nsuppliers_ (4),
     nconsumers_ (4),
     max_recursion_ (1)
{
  TAO_CEC_Default_Factory::init_svcs ();
}

int
RND_Driver::run (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      ACE_Arg_Shifter arg_shifter (argc, argv);

      while (arg_shifter.is_anything_left ())
        {
          const char *arg = arg_shifter.get_current ();

          if (ACE_OS::strcasecmp (arg, "-suppliers") == 0)
            {
              arg_shifter.consume_arg ();

              if (arg_shifter.is_parameter_next ())
                {
                  const char* opt = arg_shifter.get_current ();
                  int n = ACE_OS::atoi (opt);
                  if (n >= 1)
                    this->nsuppliers_ = n;
                  arg_shifter.consume_arg ();
                }
            }
          else if (ACE_OS::strcasecmp (arg, "-consumers") == 0)
            {
              arg_shifter.consume_arg ();

              if (arg_shifter.is_parameter_next ())
                {
                  const char* opt = arg_shifter.get_current ();
                  int n = ACE_OS::atoi (opt);
                  if (n >= 1)
                    this->nconsumers_ = n;
                  arg_shifter.consume_arg ();
                }
            }
          else if (ACE_OS::strcasecmp (arg, "-max_recursion") == 0)
            {
              arg_shifter.consume_arg ();

              if (arg_shifter.is_parameter_next ())
                {
                  const char* opt = arg_shifter.get_current ();
                  int n = ACE_OS::atoi (opt);
                  if (n >= 0)
                    this->max_recursion_ = n;
                  arg_shifter.consume_arg ();
                }
            }
          else
            arg_shifter.ignore_arg ();
        }

      // ****************************************************************

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

      TAO_CEC_EventChannel_Attributes attributes (poa.in (),
                                                  poa.in ());
      attributes.consumer_reconnect = 1;
      attributes.supplier_reconnect = 1;

      TAO_CEC_EventChannel ec_impl (attributes);
      ec_impl.activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosEventChannelAdmin::EventChannel_var event_channel =
        ec_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      // Obtain the consumer admin..
      this->consumer_admin_ =
        event_channel->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain the supplier admin..
      this->supplier_admin_ =
        event_channel->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      this->supplier_.connect (this->supplier_admin_.in ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
            this->consumers_[i]->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // ****************************************************************

      ACE_NEW_RETURN (this->suppliers_,
                      RND_Supplier*[this->nsuppliers_],
                      1);
      for (int j = 0; j != this->nsuppliers_; ++j)
        {
          ACE_NEW_RETURN (this->suppliers_[j],
                          RND_Supplier,
                          1);
          this->suppliers_[j]->activate ();

          CORBA::Object_var obj =
            this->suppliers_[j]->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // ****************************************************************

      for (int event_count = 0; event_count != 500; ++event_count)
        {
          ACE_Time_Value tv (0, 50000);
          orb->run (tv);
          this->supplier_.push_new_event (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_Thread_Manager::instance ()->wait ();

      // ****************************************************************

      {
        for (int k = 0; k != this->nsuppliers_; ++k)
          {
            deactivate_servant (this->suppliers_[k]
                                ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
            this->suppliers_[k]->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
        delete[] this->suppliers_;
        this->suppliers_ = 0;
      }

      // ****************************************************************

      // We destroy now to verify that the callbacks work and do not
      // produce any problems.
      event_channel->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      {
        for (int k = 0; k != this->nconsumers_; ++k)
          {
            deactivate_servant (this->consumers_[k]
                                ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
            this->consumers_[k]->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
        delete[] this->consumers_;
        this->consumers_ = 0;
      }

      // ****************************************************************

      deactivate_servant (&ec_impl
                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Random");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}

void
RND_Driver::timer (const CORBA::Any &e
                   ACE_ENV_ARG_DECL)
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
        CORBA::Long recursion;
        e >>= recursion;
        // ACE_DEBUG ((LM_DEBUG, "Pushing an event\n"));
        if (recursion < this->max_recursion_)
          {
            CORBA::Any new_event;
            recursion++;
            new_event <<= recursion;
            this->supplier_.push (new_event ACE_ENV_ARG_PARAMETER);
            ACE_CHECK;
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

        this->suppliers_[n]->connect (this->supplier_admin_.in ()
                                      ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
      }
      break;

    case 7:
      {
        int n = ACE_OS::rand () % this->nconsumers_;

        // ACE_DEBUG ((LM_DEBUG, "Connecting consumer %d\n", n));

        this->consumers_[n]->connect (this->consumer_admin_.in ()
                                      ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
      }
      break;

    case 8:
      {
        int n = ACE_OS::rand () % this->nsuppliers_;

        // ACE_DEBUG ((LM_DEBUG, "Disconnecting supplier %d\n", n));

        this->suppliers_[n]->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;
      }
      break;

    case 9:
      {
        int n = ACE_OS::rand () % this->nconsumers_;

        // ACE_DEBUG ((LM_DEBUG, "Disconnecting consumer %d\n", n));

        this->consumers_[n]->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;
      }
      break;
    }
}

void
RND_Driver::event (const CORBA::Any &e
                   ACE_ENV_ARG_DECL)
{
  this->timer (e ACE_ENV_ARG_PARAMETER);
}

// ****************************************************************

void
RND_Consumer::connect (CosEventChannelAdmin::ConsumerAdmin_ptr admin
                       ACE_ENV_ARG_DECL)
{
  CosEventChannelAdmin::ProxyPushSupplier_var proxy;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

    if (CORBA::is_nil (this->proxy_.in ()))
      {
        this->proxy_ = admin->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;
      }
    proxy =
      CosEventChannelAdmin::ProxyPushSupplier::_duplicate(this->proxy_.in ());
  }
  CosEventComm::PushConsumer_var me =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  proxy->connect_push_consumer (me.in ()
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
RND_Consumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (CORBA::is_nil (this->proxy_.in ()))
    return;
  this->proxy_->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->proxy_ =
    CosEventChannelAdmin::ProxyPushSupplier::_nil ();
}

void
RND_Consumer::push (const CORBA::Any &event
                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->driver_->event (event ACE_ENV_ARG_PARAMETER);
}

void
RND_Consumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

// ****************************************************************

void
RND_Supplier::connect (CosEventChannelAdmin::SupplierAdmin_ptr admin
                       ACE_ENV_ARG_DECL)
{
  CosEventChannelAdmin::ProxyPushConsumer_var proxy;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

    if (CORBA::is_nil (this->proxy_.in ()))
      {
        this->proxy_ = admin->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;
      }

    proxy =
      CosEventChannelAdmin::ProxyPushConsumer::_duplicate(this->proxy_.in ());
  }
  CosEventComm::PushSupplier_var me;

  int r = ACE_OS::rand () % 2;
  if (r == 0)
    {
      me = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
  proxy->connect_push_supplier (me.in ()
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
RND_Supplier::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (CORBA::is_nil (this->proxy_.in ()))
    return;
  this->proxy_->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->proxy_ =
    CosEventChannelAdmin::ProxyPushConsumer::_nil ();
}

void
RND_Supplier::push_new_event (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Any event;
  CORBA::Long recursion = 0;
  event <<= recursion;

  this->push (event ACE_ENV_ARG_PARAMETER);
}

void
RND_Supplier::push (CORBA::Any &event
                    ACE_ENV_ARG_DECL)
{
  CosEventChannelAdmin::ProxyPushConsumer_var proxy;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

    if (CORBA::is_nil (this->proxy_.in ()))
      return;

    proxy =
      CosEventChannelAdmin::ProxyPushConsumer::_duplicate(this->proxy_.in ());
  }

  proxy->push (event ACE_ENV_ARG_PARAMETER);
}

void
RND_Supplier::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          ACE_Time_Value tv (0, 10000);
          ACE_OS::sleep (tv);

          this->push_new_event (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
        }
      ACE_ENDTRY;
      if (i * 100 / niterations >= percent)
        {
          ACE_DEBUG ((LM_DEBUG, "Thread %t %d%%\n", percent));
          percent += 10;
        }
    }
  ACE_DEBUG ((LM_DEBUG, "Thread %t completed\n"));
  return 0;
}
