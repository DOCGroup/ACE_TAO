// $Id$

// local headers
#include "Distributor_i.h"
#include "Stock_PriorityMapping.h"
#include "Stock_Database.h"

// ACE headers
#include "ace/OS_NS_unistd.h"
#include "ace/UUID.h"
#include "ace/Reactor.h"

// TAO headers
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/ORB_Core.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "tao/StringSeqC.h"

// STL headers
#include <sstream>

// Implementation skeleton constructor
StockDistributor_i::StockDistributor_i (RTPortableServer::POA_ptr poa)
  : rate_ (3), // Default is 3 seconds (3000 milliseconds).
    active_ (false),
    rt_poa_ (RTPortableServer::POA::_duplicate (poa)),
    orb_ (CORBA::ORB::_duplicate (poa->_get_orb ()))
{
}

// Implementation skeleton destructor
StockDistributor_i::~StockDistributor_i (void)
{
  this->active_ = false;
}

::Stock::Cookie *
StockDistributor_i::subscribe_notifier (::Stock::StockNameConsumer_ptr c,
                                              ::RTCORBA::Priority priority)
{
  // Get mutual exclusion of the <subscribers_list_>.
  ACE_WRITE_GUARD_RETURN (ACE_RW_Thread_Mutex, g, lock_, 0);

  // Generate a unique id for the cookie.
  ACE_Utils::UUID uuid;
  ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID (uuid);

  // Create a new cookie object; initialize its value.
  Stock::Cookie_var cookie = new OBV_Stock::Cookie ();
  cookie->cookie_id (uuid.to_string ()->c_str ());

  // Insert the cookie into the <subscribers_list_>.
  this->subscribers_list_[cookie->cookie_id ()] =
    std::make_pair (Stock::StockNameConsumer::_duplicate (c),
                    priority);

  ACE_DEBUG ((LM_DEBUG, "Subscribing  %s\n", cookie->cookie_id ()));

  return cookie._retn();
}

::Stock::StockNameConsumer_ptr
StockDistributor_i::unsubscribe_notifier (::Stock::Cookie *ck)
{
  if (ck == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "ERROR: Cookie given to unsubscribe_notifier was null\n"));
      return 0;
    }

  // Get mutual exclusion of the <subscribers_list_>.
  ACE_WRITE_GUARD_RETURN (ACE_RW_Thread_Mutex, guard, lock_, 0);

  ACE_DEBUG ((LM_DEBUG, "Unsubscribing  %s\n", ck->cookie_id ()));

  // Search for the <cookie> in the <subscribers_list_>.
  CookieMap::iterator iter = this->subscribers_list_.find (ck->cookie_id ());

  // Verify we have located the <cookie>.
  if (iter == this->subscribers_list_.end ())
    throw ::Stock::Invalid_Subscription ();

  // Erase the mapping from the <subscribers_list_>.
  Stock::StockNameConsumer_var consumer (iter->second.first._retn ());
  this->subscribers_list_.erase (iter);

  // Return the StockNameConsumer to the client.
  return consumer._retn ();
}

::Stock::StockQuoter_ptr
StockDistributor_i::provide_quoter_info (::Stock::Cookie *ck)
{
  ACE_READ_GUARD_RETURN (ACE_RW_Thread_Mutex, guard, lock_, 0);

  try
    {
      CookieMap::const_iterator iter = this->subscribers_list_.find (ck->cookie_id ());

      ACE_DEBUG ((LM_DEBUG, "Looking up cookie %s\n", ck->cookie_id ()));

      if (iter == this->subscribers_list_.end ())
        throw ::Stock::Invalid_Subscription ();


      StockQuoter_i *quoter = new StockQuoter_i;
      PortableServer::ServantBase_var owner_transfer (quoter);
      PortableServer::ObjectId *oid =
        this->rt_poa_->activate_object_with_priority (quoter,
                                                      iter->second.second);

      CORBA::Object_var obj = rt_poa_->id_to_reference (*oid);
      ::Stock::StockQuoter_var quoter_var = ::Stock::StockQuoter::_narrow (obj.in ());

      if (CORBA::is_nil (quoter_var.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: No reference to quoter returned from narrow!\n"),
                          0);

      return quoter_var._retn ();
    }
  catch (CORBA::Exception &ex)
    {
      ex._tao_print_exception ("StockDistributor_i::provide_quoter_info");
      return ::Stock::StockQuoter::_nil ();
    }
}

::CORBA::Long
StockDistributor_i::notification_rate ()
{
  return this->rate_;
}

void
StockDistributor_i::notification_rate (::CORBA::Long notification_rate)
{
  this->rate_ = notification_rate;
  STOCK_DATABASE->update_rate (this->rate_);
}

void
StockDistributor_i::start ()
{
  STOCK_DATABASE->start ();
}

void
StockDistributor_i::stop ()
{
  STOCK_DATABASE->stop ();
}

void
StockDistributor_i::shutdown ()
{
  ACE_DEBUG ((LM_DEBUG, "Stopping publisher\n"));
  // Stop publishing events
  this->stop ();

  ACE_DEBUG ((LM_DEBUG, "Deactivating the Distributor object\n"));
  // Deactivate this obj
  ::Stock::StockDistributor_var dist = this->_this ();
  PortableServer::ObjectId_var oid = this->rt_poa_->reference_to_id (dist.in ());

  this->rt_poa_->deactivate_object (oid.in ());
}

// @@Todo: It would be cool to be able to use boost::Lambda, so we wouldn't
// have to create stupid functors like this!
struct StrSeq_Converter
{
  StrSeq_Converter (CORBA::StringSeq &seq)
    : seq_ (seq),
      pos_ (0)
  {
  }

  void operator () (const std::string &str)
  {
    this->seq_[pos_++] = str.c_str ();
  }

  CORBA::StringSeq &seq_;
  CORBA::ULong pos_;
};

struct Stock_Publisher
{
  Stock_Publisher (CORBA::ORB_ptr orb,
                   CORBA::StringSeq &stocks)
  {
    CORBA::Object_var obj = orb->resolve_initial_references ("RTCurrent");
    rt_current_ = RTCORBA::Current::_narrow (obj.in ());

    // Create the message
    sn_ = new OBV_Stock::StockNames ();
    sn_->names (stocks);
  }

  void operator () (const StockDistributor_i::CookieMap::value_type &item)
  {
    try
      {
        // Set priority of the request
        this->rt_current_->the_priority (item.second.second);

        // publish!
        item.second.first->push_StockName (this->sn_.in ());
      }
    catch (CORBA::Exception &ex)
      {
        ex._tao_print_exception ("Distributor: Stock_Publisher");
      }
  }

  RTCORBA::Current_var rt_current_;
  Stock::StockNames_var sn_;
};


void
StockDistributor_i::operator () (std::vector <std::string> &stocks)
{
  ACE_DEBUG ((LM_DEBUG,
              "*** message: transmitting data to the StockNameConsumer, got %i stocks\n",
              stocks.size ()));

  // Convert the stocks into a stringseq
  CORBA::StringSeq_var corba_stocks = new CORBA::StringSeq ();
  corba_stocks->length (stocks.size ());

  std::for_each (stocks.begin (),
                 stocks.end (),
                 StrSeq_Converter (corba_stocks));


  ACE_READ_GUARD (ACE_RW_Thread_Mutex, g, lock_);
  std::for_each (this->subscribers_list_.begin (),
                 this->subscribers_list_.end (),
                 Stock_Publisher (this->orb_.in (),
                                  corba_stocks));
}

// Implementation skeleton constructor
StockQuoter_i::StockQuoter_i (void)
{
}

// Implementation skeleton destructor
StockQuoter_i::~StockQuoter_i (void)
{
}

::Stock::StockInfo *
StockQuoter_i::get_stock_info (const char * stock_name)
{
  // Obtain the stock information from the database.
  ACE_DEBUG ((LM_DEBUG, "*** message: requesting stock_info from the database\n"));

  try
    {
      Stock_Database<StockDistributor_i>::StockInfo si
        (STOCK_DATABASE->get_stock_info (stock_name));

      Stock::StockInfo_var stock_info = new Stock::StockInfo;
      stock_info->name = si.name_.c_str ();
      stock_info->high = si.high_;
      stock_info->low = si.low_;
      stock_info->last = si.last_;

      return stock_info._retn ();

    }
  catch (Stock_Database<StockDistributor_i>::Invalid_Stock &ex)
    {
      throw Stock::Invalid_Stock ();
    }

}

// Implementation skeleton constructor
StockDistributorHome_i::StockDistributorHome_i (CORBA::ORB_ptr orb)
  :     orb_ (CORBA::ORB::_duplicate (orb)),
        rt_poa_ (RTPortableServer::POA::_nil ()),
        dist_id_ (0)
{
  // Register this class as a signal handler to catch keyboard interrupts.
  if (orb_->orb_core ()->reactor ()->register_handler (SIGINT, this) == -1)
    ACE_DEBUG ((LM_DEBUG, "ERROR: Failed to register as a signal handler: %p\n",
                "register_handler\n"));

  // Register the necessary factories and mappings with the specified
  // <orb>. If we neglect to perform these registrations then the app
  // will not execute.
  Stock::StockNames_init *stockname_factory = new Stock::StockNames_init;
  orb_->register_value_factory (stockname_factory->tao_repository_id (),
                               stockname_factory);

  Stock::Cookie_init *cookie_factory = new Stock::Cookie_init;
  this->orb_->register_value_factory (cookie_factory->tao_repository_id (),
                               cookie_factory);

  Stock::Priority_Mapping::register_mapping (orb_.in ());

  // Get a reference to the <RTORB>.
  CORBA::Object_var obj = orb_->resolve_initial_references ("RTORB");
  RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (obj.in ());

  TAO::Utils::PolicyList_Destroyer policies (2);
  policies.length (2);

  // Create a <SERVER_DECLARED> priority model policy.
  policies[0] =
    rt_orb->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
                                          Stock::Priority_Mapping::VERY_LOW);

  // Create a threadpool with lanes for the distributor. Since the brokers
  // will have various priorities, create a lane for each priority.
  RTCORBA::ThreadpoolLanes lanes (5); lanes.length (5);

  for (CORBA::ULong i = 0; i < lanes.length (); ++i)
    {
      lanes[i].lane_priority = static_cast<RTCORBA::Priority> (i);
      lanes[i].static_threads = 5;
      lanes[i].dynamic_threads = 0;
    }

  RTCORBA::ThreadpoolId threadpool_id =
    rt_orb->create_threadpool_with_lanes (1024*1024,
                                          lanes,
                                          false, false, 0, 0);

  policies[1] = rt_orb->create_threadpool_policy (threadpool_id);

  PortableServer::POA_var poa = this->_default_POA ();
  PortableServer::POAManager_var poa_mgr = poa->the_POAManager ();

  // Create a child POA with <SERVER_DECLARED> policies. The name of
  // the POA will be <StockDistributor_POA>. Any instances of the
  // StockDistributor_i created via the create() method will be
  // activated under this POA.
  PortableServer::POA_var child_poa =
    poa->create_POA ("StockDistributor_POA",
                     poa_mgr.in (),
                     policies);

  // Narrow the POA to a RT POA, and cache the reference
  this->rt_poa_ = RTPortableServer::POA::_narrow (child_poa.in ());

  // Create the initial distributor reference
  this->create_distributor ();
}

// Implementation skeleton destructor
StockDistributorHome_i::~StockDistributorHome_i (void)
{
}

::Stock::StockDistributor_ptr
StockDistributorHome_i::create ()
{
  CORBA::Object_var obj;

  try
    {
      obj = this->rt_poa_->id_to_reference (this->dist_id_.in ());
    }
  catch (/* PortableServer::ObjectNotActive &ex */ ...)
    {
      ACE_DEBUG ((LM_DEBUG, "Caught an exception creating a reference to "
                  "distributor, creating a new one\n"));

      this->create_distributor ();
      obj = this->rt_poa_->id_to_reference (this->dist_id_.in ());
    }

  return Stock::StockDistributor::_narrow (obj.in ());
}


int
StockDistributorHome_i::handle_signal (int,
                                       siginfo_t *,
                                       ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "SIGNAL HANDLER CALLED!!!\n"));

  try
    {
      ACE_DEBUG ((LM_DEBUG, "Commanding the distributor to shut down\n"));
      CORBA::Object_var obj
        (this->rt_poa_->id_to_reference (this->dist_id_.in ()));

      Stock::StockDistributor_var dist
        (Stock::StockDistributor::_narrow (obj.in ()));

      // kill the active distributor
      dist->shutdown ();
    }
  catch (...)
    {
      // Swallow any exceptions, distributor may already be dead
    }

  ACE_DEBUG ((LM_DEBUG, "Shutting down the ORB\n"));
  this->orb_->shutdown (true);

  return 0;
}

void
StockDistributorHome_i::create_distributor (void)
{
  // Create a new instance of the <StockDistributor_i>. Then
  // activate the <distributor> under the located POA. This will cause
  // the object to have the <CLIENT_PROPAGATED> policies.
  StockDistributor_i *servant = new StockDistributor_i (this->rt_poa_.in ());
  PortableServer::ServantBase_var distributor_owner_transfer = servant;
  this->dist_id_ = this->rt_poa_->activate_object (servant);

  // Register the distributor with the database as a callback.
  STOCK_DATABASE->register_callback (*servant);
}
