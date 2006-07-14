// $Id$

// local headers
#include "Distributor_i.h"
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


// @@ Shanshan - Please remove the ACE exception macros from this
// code.

// Implementation skeleton constructor
Stock_StockDistributor_i::Stock_StockDistributor_i (CORBA::ORB_ptr orb,
                                                    CORBA::PolicyList &policy_list)
                                                    : rate_(3000),
                                                      active_ (false)
{
  // Get at reference to the <RTCurrent> object. This will be used
  // in the _thread_func for changing the priority of the client thread
  // to the priority of the connected <StockNameConsumer>.
  CORBA::Object_var obj = orb->resolve_initial_references ("RTCurrent");
  this->current_ = RTCORBA::Current::_narrow (obj);

  // @@ Shanshan - Yikes, please don't use ACE_ASSERT here, it would be better to
  // throw an exception.
  ACE_ASSERT (!CORBA::is_nil (this->current_));

  // Create a child POA with CLIENT_PROPAGATED policies. The name of the
  // POA will be <Stock_StockDistributorHome>. Any instances of the
  // Stock_StockDistributor_i created via the create() method will be
  // activated under this POA.
  PortableServer::POA_var poa = this->_default_POA()->create_POA (
    "StockQuoter_POA", PortableServer::POAManager::_nil (), policy_list);
  ACE_ASSERT (!CORBA::is_nil (poa));

  // _narrow () the POA to a RTPortableServer::POA.
  RTPortableServer::POA_var rt_poa = RTPortableServer::POA::_narrow (poa);
  ACE_ASSERT (!CORBA::is_nil (rt_poa));

  // Create a new instance of the <quoter_> under it's own personal
  // POA. But the POA shares the <threadpool> with other POA's as well.
  this->quoter_ = new Stock_StockQuoter_i;
  rt_poa->activate_object (this->quoter_);

  // Get a reference to the <RTORB>.
  obj = orb->resolve_initial_references ("RTORB");
  this->rt_orb_ = RTCORBA::RTORB::_narrow (obj.in ());
  ACE_ASSERT (!CORBA::is_nil (this->rt_orb_));

  // Create a RTCOBRA::Mutex.
  this->list_mutex_ = this->rt_orb_->create_mutex ();

  // Seed the random number generator
  ACE_OS::srand (time (0));
}

// Implementation skeleton destructor
Stock_StockDistributor_i::~Stock_StockDistributor_i (void)
{
  this->active_ = false;
  this->rt_orb_->destroy_mutex (this->list_mutex_);
}

::Stock::Cookie * Stock_StockDistributor_i::subscribe_notifier (
    ::Stock::StockNameConsumer_ptr c,
    ::RTCORBA::Priority priority
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
  // Get mutual exclusion of the <subscribers_list_>.
  // @@ Shanshan - I would rather we use an ACE mutex type and the
  // ACE_GUARD macro.
  this->list_mutex_->lock ();

  for (; ;)
  {
    // Generate an unique id for the cookie.
    std::ostrstream cookie_id;
    cookie_id << "COOKIE:" << time (0) << "_" << ACE_OS::rand () << std::ends;

    // Create a new cookie object; initialize its value.
    //Stock::Cookie * cookie = new Stock_Cookie_i (cookie_id.str ());
    OBV_Stock::Cookie * cookie = new OBV_Stock::Cookie (cookie_id.str ());

    // Insert the cookie into the <subscribers_list_>.
    std::pair <CookieMap::iterator, bool> result =
      this->subscribers_list_.insert (std::make_pair (cookie,
      std::make_pair (Stock::StockNameConsumer::_duplicate (c), priority)));

    // The <Cookie> was successfully created. Now that it has been
    // inserted into the map, we need to activate the StockNameConsumer
    // using the specified priority
    if (result.second == true)	{
      // Release the lock. This will allow the <_thread_func> to continue
      // processing the <subscribers_list_>.
      this->list_mutex_->unlock ();
      return cookie;
    }
  }

  // Release the mutex. Although this part of the code will never
  // be reached, it just makes me feel more comfortable with i
  // there.
  this->list_mutex_->unlock ();
  return 0;
}

::Stock::StockNameConsumer_ptr Stock_StockDistributor_i::unsubscribe_notifier (
    ::Stock::Cookie * ck
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
  // Search for the <cookie> in the <subscribers_list_>.
  this->list_mutex_->lock();
  CookieMap::iterator iter = this->subscribers_list_.find (ck);

  // Verify we have located the <cookie>.
  if (iter == this->subscribers_list_.end ())
    return Stock::StockNameConsumer::_nil ();

  // Erase the mapping from the <subscribers_list_>.
  Stock::Cookie_var cookie = iter->first;
  Stock::StockNameConsumer_var consumer = iter->second.first;

  this->subscribers_list_.erase (iter);
  this->list_mutex_->unlock ();

  // Return the StockNameConsumer to the client.
  return Stock::StockNameConsumer::_duplicate (consumer);
}

::Stock::StockQuoter_ptr Stock_StockDistributor_i::provide_quoter_info (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
  return Stock::StockQuoter::_duplicate (this->quoter_->_this ());
}

::CORBA::Long Stock_StockDistributor_i::notification_rate (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
  return this->rate_;
}

void Stock_StockDistributor_i::notification_rate (
    ::CORBA::Long notification_rate
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
  this->rate_ = notification_rate;
}

void Stock_StockDistributor_i::start (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
  if (!this->active_) {
    this->active_ = true;
    // Create the thread in suspended mode.
    ACE_Thread::spawn (Stock_StockDistributor_i::_thread_func, this, THR_NEW_LWP |
      THR_JOINABLE, &this->_thread_id, &this->_thread);
  }
}

void Stock_StockDistributor_i::stop (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
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
  Stock::StockName_init *factory1 = new Stock::StockName_init;
  orb->register_value_factory (factory1->tao_repository_id (),
    factory1
    ACE_ENV_ARG_PARAMETER);

  Stock::Cookie_init *factory2 = new Stock::Cookie_init;
  orb->register_value_factory (factory2->tao_repository_id (),
    factory2
    ACE_ENV_ARG_PARAMETER);

  // @@ Shanshan - be sure to call _remove_ref () on the factory1 and 2 to
  // release ownership.

  //Stock_CookieFactory_i::register_factory (orb);
  //Stock_StockNameFactory_i::register_factory (orb);
  Stock_PriorityMapping::register_mapping (orb);

  // Get a reference to the <RTORB>.
  CORBA::Object_var obj = orb->resolve_initial_references ("RTORB");
  RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (obj.in ());
  ACE_ASSERT (!CORBA::is_nil (rt_orb));

  // Initialize the database
  Stock_Database::instance ()->init (rt_orb);

  // Create a <CORBA::PolicyList> for the child POA.
  CORBA::PolicyList stock_distributor_policies (2);
  stock_distributor_policies.length (2);

  // Create a <CLIENT_PROPOGATED> priority model policy.
  stock_distributor_policies[0] =
    rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
    Stock_PriorityMapping::MEDIUM);

  // Create a threadpool with lanes for the distributor. Since the brokers
  // will have various priorities, create a lane for each priority.
  RTCORBA::ThreadpoolLanes lanes (5); lanes.length (5);
  for (CORBA::ULong i = 0; i < lanes.length (); i ++) {
    lanes[i].lane_priority = i;
    lanes[i].static_threads = 5;
    lanes[i].dynamic_threads = 2;
  }

  RTCORBA::ThreadpoolId threadpool_id =	
    rt_orb->create_threadpool_with_lanes (1024 * 10, lanes, false, false, 0, 0);
  stock_distributor_policies[1] = rt_orb->create_threadpool_policy (threadpool_id);

  // Create a child POA with CLIENT_PROPAGATED policies. The name of the
  // POA will be <Stock_StockDistributorHome>. Any instances of the
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
      // @@ Shanshan - Please use a ServantBase_var to manage the memory of the servant.
      this->distributor_ = new Stock_StockDistributor_i (orb, stock_distributor_policies);
      rt_poa->activate_object (this->distributor_);
    }
  catch (RTPortableServer::POA::ServantAlreadyActive &)	
    {
    }
}

// Implementation skeleton destructor
Stock_StockDistributorHome_i::~Stock_StockDistributorHome_i (void)
{
  // @@ Shanshan - Leaking the distributor_!
}

::Stock::StockDistributor_ptr Stock_StockDistributorHome_i::create (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
  return Stock::StockDistributor::_duplicate (this->distributor_->_this ());
}

//
// _thread_func
//
// @@ Shanshan - Please use ACE_Task instead of threading by hand.
ACE_THR_FUNC_RETURN Stock_StockDistributor_i::_thread_func(void *param)
{
  // Extract the <Stock_StockDistributor_i> object pointer.
  Stock_StockDistributor_i *stock_distributor = (Stock_StockDistributor_i *)param;
  //Stock::StockName_var stock_name = new Stock_StockName_i;
  OBV_Stock::StockName * stock_name = new OBV_Stock::StockName;

  // @@ Shanshan - Please review the ACE style guidelines, and forma
  // your code appropriately.

  // Continue looping while the stock distributor is active.
  while (stock_distributor->active_) {
    ACE_DEBUG ((LM_DEBUG, "*** message: transmitting data to the StockNameConsumer...\n"));
    stock_distributor->list_mutex_->lock ();

    // Push the information to all the consumers.
    Stock_StockDistributor_i::CookieMap::iterator iter;

    // @@ Shanshan - please use ++iter instead of iter++
    for (iter = stock_distributor->subscribers_list_.begin ();
         iter != stock_distributor->subscribers_list_.end ();
         iter ++)
    {
      try {
        // Change the current priority and thread priority to that of the
        // connected <StockNameConsumer>. Then tell the database to push
        // its information to the <consumer>.
        stock_distributor->current_->the_priority (iter->second.second);
        Stock_Database::instance ()->publish_stock_info (iter->second.first);
      }
      // @@ Shanshan - Is swallowing exceptions really a good idea here?
      catch (CORBA::Exception &ex) {
        // @@ Shanshan - Please use ACE_DEBUG instead of iostreams.
        // Also, please look into the ACE_PRINT_EXCEPTION macro, which
        // is used to print CORBA exceptions.
        cerr << "*** exception: " << ex << endl;
      }
      catch (...) {
        cerr << "*** exception: an unknown exception has occured." << endl;
      }
    }

    // Sleep for the specified amount of seconds. The notification
    // rate is in milliseconds.
    stock_distributor->list_mutex_->unlock ();
    ACE_OS::sleep(stock_distributor->notification_rate ());
  }
  return 0;
}


