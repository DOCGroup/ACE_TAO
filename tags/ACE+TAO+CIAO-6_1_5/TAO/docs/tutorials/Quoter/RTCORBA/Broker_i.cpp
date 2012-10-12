// $Id$

// ACE+TAO headers
#include "ace/Reactor.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/ORB_Core.h"
#include "tao/Utils/PolicyList_Destroyer.h"

// local headers
#include "Broker_i.h"
#include "Stock_PriorityMapping.h"
#include "Stock_Database.h"

// Implementation skeleton constructor
Stock_StockBroker_i::Stock_StockBroker_i (CORBA::ORB_ptr orb,
                                          Stock::StockDistributor_ptr dist,
                                          const char *stock_name)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    quoter_ (Stock::StockQuoter::_nil()),
    consumer_ (0),
    distributor_ (Stock::StockDistributor::_duplicate (dist))
{
  // Get a reference to the <RTORB>.
  CORBA::Object_var obj = orb->resolve_initial_references ("RTORB");
  RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (obj.in ());

  // Create a <CORBA::PolicyList> for the child POA.
  TAO::Utils::PolicyList_Destroyer  consumer_policies (1);
  consumer_policies.length (1);

  // Create a <CLIENT_PROPAGATED> priority model policy.
    consumer_policies[0] =
      rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                            Stock::Priority_Mapping::MEDIUM);

  PortableServer::POA_var poa = this->_default_POA ();
  PortableServer::POAManager_var poa_mgr = poa->the_POAManager ();

  // Create a child POA with CLIENT_PROPAGATED policies. The name of
  // the POA will be <StockNameConsumer_POA>.  Instances of the
  // Stock_StockNameConsumer_i will be activated under this POA.
  PortableServer::POA_var child_poa =
    poa->create_POA ("StockNameConsumer_POA",
                     poa_mgr. in(),
                     consumer_policies);

  // Narrow the POA to a <RTPortableServer::POA>.
  RTPortableServer::POA_var rt_poa =
    RTPortableServer::POA::_narrow (child_poa.in ());

  // Create and activate the <consumer_>.
  this->consumer_ =
    new Stock_StockNameConsumer_i (*this, stock_name);
  PortableServer::ServantBase_var nameconsumer_owner_transfer =
    this->consumer_;
  rt_poa->activate_object (this->consumer_);
}

// Implementation skeleton destructor
Stock_StockBroker_i::~Stock_StockBroker_i (void)
{
}

::Stock::StockNameConsumer_ptr Stock_StockBroker_i::get_consumer_notifier ()
{
  Stock::StockNameConsumer_var consumer = this->consumer_->_this ();
  return consumer._retn();
}

void Stock_StockBroker_i::connect_quoter_info (::Stock::StockQuoter_ptr c)
{
  this->quoter_ = Stock::StockQuoter::_duplicate (c);
}

::Stock::StockQuoter_ptr Stock_StockBroker_i::disconnect_quoter_info ()
{
  Stock::StockQuoter_var old_quoter = this->quoter_;
  this->quoter_ = Stock::StockQuoter::_nil();
  return old_quoter._retn ();
}

::Stock::StockQuoter_ptr Stock_StockBroker_i::get_connection_quoter_info ()
{
  return Stock::StockQuoter::_duplicate (this->quoter_.in ());
}

void
Stock_StockBroker_i::shutdown (void)
{
  // Unsubscribe
  ACE_DEBUG ((LM_DEBUG, "Shutdown unsubscribing notifiers\n"));
  this->distributor_->unsubscribe_notifier
    (this->consumer_->cookie_ ());

  ACE_DEBUG ((LM_DEBUG, "Shutdown deactivating object\n"));
  ::Stock::StockBroker_var broker = this->_this ();
  PortableServer::ObjectId_var oid =
    this->_default_POA ()->reference_to_id (broker.in ());

  this->_default_POA ()->deactivate_object (oid.in ());
}

// Implementation skeleton constructor
Stock_StockBrokerHome_i::Stock_StockBrokerHome_i (CORBA::ORB_ptr orb)
  : broker_ (0),
    orb_ (CORBA::ORB::_duplicate (orb))
{
  // Register the necessary factories and mappings with the specified
  // <orb>. If we neglect to perform these registrations then the app
  // will not execute.
  Stock::StockNames_init *stockname_factory =
    new Stock::StockNames_init;
  orb->register_value_factory (stockname_factory->tao_repository_id (),
                               stockname_factory);

  Stock::Cookie_init *cookie_factory =
    new Stock::Cookie_init;
  orb->register_value_factory (cookie_factory->tao_repository_id (),
                               cookie_factory);

  Stock::Priority_Mapping::register_mapping (orb);

  // Register this class as an event handler with the ORB to catch
  // ctrl-c from the command line.
  if (orb_->orb_core ()->reactor ()->register_handler (SIGINT, this) == -1)
    ACE_DEBUG ((LM_DEBUG, "ERROR: Failed to register as a signal handler: %p\n",
                "register_handler\n"));
}

// Implementation skeleton destructor
Stock_StockBrokerHome_i::~Stock_StockBrokerHome_i (void)
{
}

::Stock::StockBroker_ptr
Stock_StockBrokerHome_i::create (Stock::StockDistributor_ptr dist,
                                 const char *stock_name)
{
  if (CORBA::is_nil (this->broker_.in ()))
    {
      // Since the broker has nothing to do with any of the RTCORBA
      // mechanisms, we can activate it under the <default_POA>, which
      // is the <RootPOA>.
      Stock_StockBroker_i *broker =
        new Stock_StockBroker_i (orb_.in (), dist, stock_name);
      PortableServer::ServantBase_var owner_transfer
        = broker;
      this->broker_ = broker->_this ();
    }

  return Stock::StockBroker::_duplicate (this->broker_.in ());
}

int
Stock_StockBrokerHome_i::handle_signal (int,
                                        siginfo_t *,
                                        ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "Disconnecting all brokers..\n"));

  this->broker_->shutdown ();

  ACE_DEBUG ((LM_DEBUG, "Shutting down the ORB\n"));

  this->orb_->shutdown (0);

  return 0;
}
