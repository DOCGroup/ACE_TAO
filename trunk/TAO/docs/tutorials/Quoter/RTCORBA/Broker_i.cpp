// $Id$

// TAO headers
#include "tao/RTPortableServer/RTPortableServer.h"

// local headers
#include "Broker_i.h"
#include "Stock_PriorityMapping.h"
#include "Stock_Database.h"

// Implementation skeleton constructor
Stock_StockBroker_i::Stock_StockBroker_i (CORBA::ORB_ptr orb,
                                          const char *stock_name,
                                          RTCORBA::Priority priority)
                                          : quoter_ (Stock::StockQuoter::_nil()),
                                            consumer_ (0)
{
  // Get a reference to the <RTORB>.
  CORBA::Object_var obj = orb->resolve_initial_references ("RTORB");
  RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (obj.in ());
  ACE_ASSERT (!CORBA::is_nil (rt_orb));

  // Create a <CORBA::PolicyList> for the child POA.
  CORBA::PolicyList consumer_policies (1);
  consumer_policies.length (1);

  // Create a <SERVER_DECLARED> priority model policy.
  consumer_policies[0] = rt_orb->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
    Stock_PriorityMapping::MEDIUM);

  // Create a child POA with SERVER_DECLARED policies. The name of the
  // POA will be <Stock_StockDistributorHome>. Any instances of the
  // Stock_StockDistributor_i created via the create() method will be
  // activated under this POA.
  PortableServer::POA_var poa = this->_default_POA()->create_POA (
    "StockNameConsumer_POA", PortableServer::POAManager::_nil (), consumer_policies);
  ACE_ASSERT (!CORBA::is_nil (poa));

  // Narrow the POA to a <RTPortableServer::POA>.
  RTPortableServer::POA_var rt_poa = RTPortableServer::POA::_narrow (poa);
  ACE_ASSERT (!CORBA::is_nil (rt_poa));

  // Activate the <consumer_> with the specified <priority>.
  this->consumer_ = new Stock_StockNameConsumer_i (this->_this (), stock_name);
  PortableServer::ServantBase_var nameconsumer_owner_transfer = this->consumer_;
  rt_poa->activate_object_with_priority (this->consumer_, priority);
}

// Implementation skeleton destructor
Stock_StockBroker_i::~Stock_StockBroker_i (void)
{
}

::Stock::StockNameConsumer_ptr Stock_StockBroker_i::get_consumer_notifier ()
  throw (::CORBA::SystemException)
{
  return Stock::StockNameConsumer::_duplicate (this->consumer_->_this ());
}

void Stock_StockBroker_i::connect_quoter_info (::Stock::StockQuoter_ptr c)
  throw (::CORBA::SystemException)
{
  this->quoter_ = Stock::StockQuoter::_duplicate (c);
}

::Stock::StockQuoter_ptr Stock_StockBroker_i::disconnect_quoter_info ()
  throw (::CORBA::SystemException)
{
  Stock::StockQuoter_var old_quoter = this->quoter_;
  this->quoter_ = Stock::StockQuoter::_nil();
  return old_quoter._retn ();
}

::Stock::StockQuoter_ptr Stock_StockBroker_i::get_connection_quoter_info ()
  throw (::CORBA::SystemException)
{
  return Stock::StockQuoter::_duplicate (this->quoter_);
}

// Implementation skeleton constructor
Stock_StockBrokerHome_i::Stock_StockBrokerHome_i (CORBA::ORB_ptr orb,
                                                  const char *stock_name,
                                                  RTCORBA::Priority priority)
                                                  : broker_ (0)
{
  // Register the necessary factories and mappings with the specified
  // <orb>. If we neglect to perform these registrations then the app
  // will not execute.
  Stock::StockName_init *stockname_factory = new Stock::StockName_init;
  orb->register_value_factory (stockname_factory->tao_repository_id (),
                               stockname_factory);

  Stock::Cookie_init *cookie_factory = new Stock::Cookie_init;
  orb->register_value_factory (cookie_factory->tao_repository_id (),
                               cookie_factory);

  Stock_PriorityMapping::register_mapping (orb);

  // Because the broker has nothing to do with any of the RTCORBA
  // mechanisms, we can register it under the <default_POA>.
  try {
    this->broker_ = new Stock_StockBroker_i (orb, stock_name, priority);
    PortableServer::ServantBase_var broker_owner_transfer = this->broker_;
    this->_default_POA ()->activate_object (this->broker_);
  }
  catch (PortableServer::POA::ServantAlreadyActive &) {
    // we only catch this exception and let any other exception
    // propogate to the upper level.
  }
}

// Implementation skeleton destructor
Stock_StockBrokerHome_i::~Stock_StockBrokerHome_i (void)
{
  // Leaking the broker_!
}

::Stock::StockBroker_ptr Stock_StockBrokerHome_i::create ()
  throw (::CORBA::SystemException)
{
  return Stock::StockBroker::_duplicate (this->broker_->_this ());
}
