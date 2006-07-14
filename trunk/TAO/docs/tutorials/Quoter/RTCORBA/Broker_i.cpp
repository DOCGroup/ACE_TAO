// $Id$

// local headers
#include "Broker_i.h"
#include "Stock_PriorityMapping.h"
#include "Stock_Database.h"

// ACE headers
#include <ace/streams.h>
#include <ace/OS_NS_unistd.h>

// TAO headers
#include <tao/CORBA.h>
#include <tao/AnyTypeCode/Typecode.h>
#include <tao/RTCORBA/RTCORBA.h>
#include <tao/RTCORBA/Thread_Pool.h>
#include <tao/RTPortableServer/RTPortableServer.h>

// STL headers
#include <strstream>
#include <ctime>
#include <cstring>

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

  // Create a <CLIENT_PROPOGATED> priority model policy.
  consumer_policies[0] = rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
    Stock_PriorityMapping::MEDIUM);

  // Create a child POA with CLIENT_PROPAGATED policies. The name of the
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
  rt_poa->activate_object_with_priority (this->consumer_, priority);
}

// Implementation skeleton destructor
Stock_StockBroker_i::~Stock_StockBroker_i (void)
{
  // @@ Shanshan - Yikes!  Leaking memory - use an auto_ptr tomanage consumer_!
}

::Stock::StockNameConsumer_ptr Stock_StockBroker_i::get_consumer_notifier (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
  // @@ Shanshan - Is there any partifular reason you are using a C++
  // pointer to store the consumer? Why not store it as an objec
  // reference (_var type)?
  return Stock::StockNameConsumer::_duplicate (this->consumer_->_this ());
}

void Stock_StockBroker_i::connect_quoter_info (
    ::Stock::StockQuoter_ptr c
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
  this->quoter_ = Stock::StockQuoter::_duplicate (c);
}

::Stock::StockQuoter_ptr Stock_StockBroker_i::disconnect_quoter_info (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
  // @@ Shanshan - I think you are leaking memory here.  Could you
  // please store this in a _var type instead of a _ptr type, and do
  // return old_quoter._retn ();
  Stock::StockQuoter_ptr old_quoter = this->quoter_;
  this->quoter_ = Stock::StockQuoter::_nil();
  return Stock::StockQuoter::_duplicate (old_quoter);
}

::Stock::StockQuoter_ptr Stock_StockBroker_i::get_connection_quoter_info (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
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
  Stock::StockName_init *factory1 = new Stock::StockName_init;
  orb->register_value_factory (factory1->tao_repository_id (),
    factory1
    ACE_ENV_ARG_PARAMETER);

  Stock::Cookie_init *factory2 = new Stock::Cookie_init;
  orb->register_value_factory (factory2->tao_repository_id (),
    factory2
    ACE_ENV_ARG_PARAMETER);

  //Stock_CookieFactory_i::register_factory (orb);
  //Stock_StockNameFactory_i::register_factory (orb);
  Stock_PriorityMapping::register_mapping (orb);

  // Because the broker has nothing to do with any of the RTCORBA
  // mechanisms, we can register it under the <default_POA>.
  try {
    // @@ Shanshan - A couple comments here:
    // First, I think it would be better to store the object reference
    // in the class instead of a c++ pointer.
    // Second, please see me about memory management during activation.
    this->broker_ = new Stock_StockBroker_i (orb, stock_name, priority);
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

::Stock::StockBroker_ptr Stock_StockBrokerHome_i::create (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
  return Stock::StockBroker::_duplicate (this->broker_->_this ());
}
