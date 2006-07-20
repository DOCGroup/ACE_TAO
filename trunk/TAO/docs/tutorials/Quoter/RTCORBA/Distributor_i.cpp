// $Id$

// ACE headers
#include "ace/OS_NS_unistd.h"

// TAO headers
#include "tao/RTPortableServer/RTPortableServer.h"

// local headers
#include "Distributor_i.h"
#include "Stock_PriorityMapping.h"
#include "Stock_Database.h"

// STL headers
#include <strstream>

// Implementation skeleton constructor
Stock_StockDistributor_i::Stock_StockDistributor_i (CORBA::ORB_ptr orb,
                                                    CORBA::PolicyList &policy_list)
                                                    : rate_(3000),
                                                      active_ (false),
                                                      quoter_ (0)
{
  // Create a child POA with CLIENT_PROPAGATED policies. The name of the
  // POA will be <StockQuoter_POA>. Any instances of the
  // Stock_StockQuoter_i created by this Stock_StockDistributor_i object
  // will be activated under this POA.
  PortableServer::POA_var poa = this->_default_POA()->create_POA (
    "StockQuoter_POA", PortableServer::POAManager::_nil (), policy_list);
  // @@ Shanshan - Please get rid of all ACE_ACCERTs in the code.  If
  // you would like to do the check, please do so using an if statment
  // - most of them may not be strictly necessary, as exceptions would
  // be thrown if creation failed.
  ACE_ASSERT (!CORBA::is_nil (poa));

  // _narrow () the POA to a RTPortableServer::POA.
  RTPortableServer::POA_var rt_poa = RTPortableServer::POA::_narrow (poa);
  ACE_ASSERT (!CORBA::is_nil (rt_poa));

  // Create a new instance of the <quoter_> under it's own personal
  // POA. But the POA shares the <threadpool> with other POA's as well.
  this->quoter_ = new Stock_StockQuoter_i;
  PortableServer::ServantBase_var quoter_owner_transfer = this->quoter_;
  rt_poa->activate_object (this->quoter_);

  // Seed the random number generator
  ACE_OS::srand (time (0));
}

// Implementation skeleton destructor
Stock_StockDistributor_i::~Stock_StockDistributor_i (void)
{
  this->active_ = false;
}

::Stock::Cookie * Stock_StockDistributor_i::subscribe_notifier (::Stock::StockNameConsumer_ptr c,
                                                                ::RTCORBA::Priority priority)
  throw (::CORBA::SystemException)
{
  // Get mutual exclusion of the <subscribers_list_>.
  ACE_GUARD_RETURN (ACE_RW_Thread_Mutex, g, lock_, 0);

  // Generate an unique id for the cookie.
  std::ostrstream cookie_id;
  cookie_id << "COOKIE:" << time (0) << "_" << ACE_OS::rand () << std::ends;

  // @@ Shanshan - Please use the OBV_Stock::Cookie_var here to manage your memory,
  // there is a possibility that it could leak. Also, you may want to store a _var in the map, 
  // or instead simply the value of the cookie in the CookieMak.
  // Create a new cookie object; initialize its value.
  OBV_Stock::Cookie *cookie = new OBV_Stock::Cookie (cookie_id.str ());

  // Insert the cookie into the <subscribers_list_>.
  std::pair <CookieMap::iterator, bool> result =
    this->subscribers_list_.insert (std::make_pair (cookie,
    std::make_pair (Stock::StockNameConsumer::_duplicate (c), priority)));

  if (result.second == true)
  {
    return cookie;
  }

  return 0;
}

::Stock::StockNameConsumer_ptr Stock_StockDistributor_i::unsubscribe_notifier (::Stock::Cookie *ck)
  throw (::CORBA::SystemException)
{
  // Get mutual exclusion of the <subscribers_list_>.
  ACE_GUARD_RETURN (ACE_RW_Thread_Mutex, g, lock_, 0);
  
  // @@ Shanshan - Yow! I am not certain this find operation will succeed, 
  // and if it does, you are very lucky!  You are comparing the _address_
  // of the stored cookie, not its value!  Please see my note above about storing
  // the *contents* of the cookie, not the cookie itself!
  // Search for the <cookie> in the <subscribers_list_>.
  CookieMap::iterator iter = this->subscribers_list_.find (ck);

  // Verify we have located the <cookie>.
  if (iter == this->subscribers_list_.end ())
    return Stock::StockNameConsumer::_nil ();

  // Erase the mapping from the <subscribers_list_>.
  Stock::StockNameConsumer_var consumer = iter->second.first;
  this->subscribers_list_.erase (iter);

  // Return the StockNameConsumer to the client.
  return Stock::StockNameConsumer::_duplicate (consumer);
}

::Stock::StockQuoter_ptr Stock_StockDistributor_i::provide_quoter_info ()
  throw (::CORBA::SystemException)
{
  // @@ Shanshan - Again, I am not sure why you are storing a raw
  // pointer to the StockQuoter_i in the class, it seems like a very
  // bad idea since you don't have ownership over the memory.  It
  // would be better to store a _var, and duplicate that.
  return Stock::StockQuoter::_duplicate (this->quoter_->_this ());
}

::CORBA::Long Stock_StockDistributor_i::notification_rate ()
  throw (::CORBA::SystemException)
{
  return this->rate_;
}

void Stock_StockDistributor_i::notification_rate (::CORBA::Long notification_rate)
  throw (::CORBA::SystemException)
{
  this->rate_ = notification_rate;
}

void Stock_StockDistributor_i::start ()
  throw (::CORBA::SystemException)
{
  if (!this->active_)
  {
    this->active_ = true;
    
    this->activate (THR_NEW_LWP | THR_JOINABLE, 1);
  }
}

void Stock_StockDistributor_i::stop ()
  throw (::CORBA::SystemException)
{
  this->active_ = false;
}

// Implementation skeleton constructor
Stock_StockDistributorHome_i::Stock_StockDistributorHome_i (CORBA::ORB_ptr orb)
  : distributor_ (0)
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
  
  // Initialize the database
  STOCK_DATABASE->activate (THR_NEW_LWP | THR_JOINABLE, 1);

  // Get a reference to the <RTORB>.
  CORBA::Object_var obj = orb->resolve_initial_references ("RTORB");
  RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (obj.in ());
  // @@ Shanshan - Please go through your code and remove the ACE_ASSERTs.
  ACE_ASSERT (!CORBA::is_nil (rt_orb));

  // Create a <CORBA::PolicyList> for the child POA.
  CORBA::PolicyList stock_distributor_policies (2);
  stock_distributor_policies.length (2);
  
  // @@ Shanshan - You need to make sure to explicitly call destroy on
  // each element of any policy list you create, or use the
  // PolicyListDestroyer in the TAO::Utils namespace

  // Create a <CLIENT_PROPOGATED> priority model policy.
  stock_distributor_policies[0] =
    rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
    Stock_PriorityMapping::MEDIUM);

  // Create a threadpool with lanes for the distributor. Since the brokers
  // will have various priorities, create a lane for each priority.
  RTCORBA::ThreadpoolLanes lanes (5); lanes.length (5);
  for (CORBA::ULong i = 0; i < lanes.length (); ++i) {
    lanes[i].lane_priority = i;
    lanes[i].static_threads = 5;
    lanes[i].dynamic_threads = 2;
  }

  RTCORBA::ThreadpoolId threadpool_id =	
    rt_orb->create_threadpool_with_lanes (1024 * 10, lanes, false, false, 0, 0);
  stock_distributor_policies[1] = rt_orb->create_threadpool_policy (threadpool_id);

  // Create a child POA with CLIENT_PROPAGATED policies. The name of the
  // POA will be <StockDistributor_POA>. Any instances of the
  // Stock_StockDistributor_i created via the create() method will be
  // activated under this POA.
  PortableServer::POA_var poa = this->_default_POA()->create_POA (
    "StockDistributor_POA", PortableServer::POAManager::_nil (), stock_distributor_policies);
  ACE_ASSERT (!CORBA::is_nil (poa));

  // Create a new instance of the <Stock_StockDistributor_i>. Then activate
  // the <distributor> under the located POA. This will cause the objec
  // to have the CLIENT_PROPAGATED policies.
  RTPortableServer::POA_var rt_poa = RTPortableServer::POA::_narrow (poa);
  ACE_ASSERT (!CORBA::is_nil (rt_poa));

  try
  {
    this->distributor_ = new Stock_StockDistributor_i (orb, stock_distributor_policies);
    PortableServer::ServantBase_var distributor_owner_transfer = this->distributor_;
    rt_poa->activate_object (this->distributor_);
  }
  catch (RTPortableServer::POA::ServantAlreadyActive &)	
  {
  }
}

// Implementation skeleton destructor
Stock_StockDistributorHome_i::~Stock_StockDistributorHome_i (void)
{
}

::Stock::StockDistributor_ptr Stock_StockDistributorHome_i::create ()
  throw (::CORBA::SystemException)
{
  // @@ Shanshan - Again, I am not sure why you are storing a raw C++
  // pointer here.  Please consider using a _var.
  return Stock::StockDistributor::_duplicate (this->distributor_->_this ());
}

int Stock_StockDistributor_i::svc (void)
{
  OBV_Stock::StockName *stock_name = new OBV_Stock::StockName;

  // Continue looping while the stock distributor is active.
  while (this->active_) 
  {
    ACE_DEBUG ((LM_DEBUG, "*** message: transmitting data to the StockNameConsumer...\n"));

    ACE_GUARD_RETURN (ACE_RW_Thread_Mutex, g, lock_, 0);
    
    // Push the information to all the consumers.
    Stock_StockDistributor_i::CookieMap::iterator iter;

    for (iter = this->subscribers_list_.begin ();
         iter != this->subscribers_list_.end ();
         ++iter)
    {
      try 
      {
        // Tell the database to push its information to the <consumer>.
        STOCK_DATABASE->publish_stock_info (iter->second.first);
      }
      catch (CORBA::Exception &ex)
      {
        ACE_PRINT_EXCEPTION (ex, "Stock_StockDistributor_i::svc: ");
      }
    }

    // Sleep for the specified amount of seconds. The notification
    // rate is in milliseconds.
    ACE_OS::sleep(this->notification_rate ());
  }
  return 0;
}
