// $Id$

#include "Constants.h"

#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Lifetime_Utils_T.h"
#include "orbsvcs/Event/ECG_UDP_Sender.h"
#include "orbsvcs/Event/ECG_UDP_Receiver.h"

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommS.h"

#include "tao/ORB_Core.h"

#include "ace/Array_Base.h"
#include "ace/Get_Opt.h"
#include "ace/Reactor.h"
#include "ace/OS_NS_unistd.h"
#include "ace/os_include/os_netdb.h"

// Indicates whether this application is responsible for destroying
// the Event Channel it's using upon exit.
int destroy_ec_flag = 0;

/**
 * @class Heartbeat_Application
 *
 * @brief A simple application for testing federation of Event
 *        Channels via multicast.
 *
 *        NOTE: Contains platform-specific code (event data), i.e.,
 *              might not work cross-platform.
 *
 * This class acts both as a receiver and a supplier of HEARTBEAT events
 * to a multicast-federated Event Channel.  After sending a prespecified
 * number of heartbeat events, it prints out a summary about received
 * heartbeats and shuts down.
 */
class Heartbeat_Application :
  public POA_RtecEventComm::PushConsumer,
  public TAO_EC_Deactivated_Object
{
public:

  /// Constructor.
  Heartbeat_Application (void);

  /// Destructor.
  ~Heartbeat_Application (void);

  // Initializes the object: connects with EC as a supplier and a
  // consumer and registers with reactor for timeouts.  If init ()
  // completes successfully, shutdown () must be called when this
  // object is no longer needed, for proper resource cleanup.  (This
  // is normally done by handle_timeout() method, but if handle_timeout()
  // will not have a chance to execute, it is the responsibility of
  // the user.)
  void init (CORBA::ORB_var orb,
             RtecEventChannelAdmin::EventChannel_var ec);

  //FUZZ: disable check_for_lack_ACE_OS
  // No-op if the object hasn't been fully initialized.  Otherwise,
  // deregister from reactor and poa, destroy ec or just disconnect from it
  // (based on <destroy_ec> flag), and shut down the orb.
  void shutdown (void);
  //FUZZ: enable check_for_lack_ACE_OS

  /// Send another heartbeat or, if we already sent/attempted the required
  /// number of heartbeats, perform shutdown().
  int handle_timeout (const ACE_Time_Value& tv,
                      const void* act);

  /// PushConsumer methods.
  //@{
  /// Update our <heartbeats_> database to reflect newly received heartbeats.
  virtual void push (const RtecEventComm::EventSet &events);

  /// Initiate shutdown().
  virtual void disconnect_push_consumer (void);
  //@}

private:

  /**
   * @class Timeout_Handler
   *
   * @brief Helper class for receiving timeouts from Reactor.
   */
  class Timeout_Handler : public ACE_Event_Handler
  {
  public:
    /// Constructor.
    Timeout_Handler (Heartbeat_Application *recv);
    /// Reactor callback.
    virtual int handle_timeout (const ACE_Time_Value& tv,
                                const void* act);
  private:
    /// We callback to this object when a message arrives.
    Heartbeat_Application* receiver_;
  };

  /// Helpers.
  //@{
  /// Verify that arguments are not nil and store their values.
  int check_args (CORBA::ORB_var orb,
                  RtecEventChannelAdmin::EventChannel_var ec);
  /// Connects to EC as a supplier.
  void connect_as_supplier (void);
  /// Connects to EC as a consumer.  Activate with default POA.
  void connect_as_consumer (void);
  /// Call destroy() on the EC.  Does not propagate exceptions.
  void destroy_ec (void);
  /// Registers with orb's reactor for timeouts ocurring every 0.5
  /// seconds. Returns 0 on success, -1 on error.
  int register_for_timeouts (void);
  /// Deregister from reactor.
  void stop_timeouts (void);
  //@}

  /// Flag indicating whether this object has been fully initialized.
  int initialized_;

  /// Helper object for receiving timeouts from Reactor.
  Timeout_Handler timeout_handler_;

  /// Number of heartbeats we sent so far.
  size_t n_timeouts_;

  /// Info we keep on each HEARTBEAT source.
  typedef struct {
    pid_t pid;
    char hostname [MAXHOSTNAMELEN];
    int total;
  } HEARTBEAT_SOURCE_ENTRY;

  /// Stores info on all heartbeats we received so far.
  ACE_Array_Base<HEARTBEAT_SOURCE_ENTRY> heartbeats_;

  /// Our identity: pid followed by hostname.  We include this info into each
  /// heartbeat we send.
  char hostname_and_pid_ [MAXHOSTNAMELEN+11];

  /// ORB and EC pointers - to allow cleanup down the road.
  CORBA::ORB_var orb_;
  RtecEventChannelAdmin::EventChannel_var ec_;

  /// Consumer proxy which represents us in EC as a supplier.
  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_;

  typedef TAO_EC_Auto_Command<TAO_ECG_UDP_Sender_Disconnect_Command>
  Supplier_Proxy_Disconnect;
  typedef TAO_EC_Auto_Command<TAO_ECG_UDP_Receiver_Disconnect_Command>
  Consumer_Proxy_Disconnect;

  /// Manages our connection to Supplier Proxy.
  Supplier_Proxy_Disconnect supplier_proxy_disconnect_;
  /// Manages our connection to Consumer Proxy.
  Consumer_Proxy_Disconnect consumer_proxy_disconnect_;
};
// **************************************************************************

Heartbeat_Application::Timeout_Handler::
Timeout_Handler (Heartbeat_Application* r)
  :  receiver_ (r)
{
}

int
Heartbeat_Application::Timeout_Handler::
handle_timeout (const ACE_Time_Value& tv,
                const void* act)
{
  return this->receiver_->handle_timeout (tv, act);
}

// **************************************************************************

Heartbeat_Application::Heartbeat_Application (void)
  : initialized_ (0)
  , timeout_handler_ (this)
  , n_timeouts_ (0)
  , orb_ ()
  , ec_ ()
  , consumer_ ()
  , supplier_proxy_disconnect_ ()
  , consumer_proxy_disconnect_ ()
{
}

Heartbeat_Application::~Heartbeat_Application (void)
{
}

int
Heartbeat_Application::check_args (CORBA::ORB_var orb,
                                   RtecEventChannelAdmin::EventChannel_var ec)
{
  if (CORBA::is_nil (ec.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%N (%l): Nil ec argument to "
                         "Heartbeat_Application::init\n"),
                        -1);
    }

  if (CORBA::is_nil (orb.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%N (%l): Nil orb argument to "
                         "Heartbeat_Application::init\n"),
                        -1);
    }

  this->ec_ = ec;
  this->orb_ = orb;

  return 0;
}

void
Heartbeat_Application::init (CORBA::ORB_var orb,
                             RtecEventChannelAdmin::EventChannel_var ec)
{
  // Verify arguments.
  if (this->check_args (orb, ec) == -1)
    {
      throw CORBA::INTERNAL ();
    }

  // Get hostname & process id, i.e., identity of this application.
  pid_t pid = ACE_OS::getpid ();
  ACE_OS::memcpy (this->hostname_and_pid_,
                  &pid,
                  sizeof (pid));

  if (gethostname (this->hostname_and_pid_ + sizeof (pid),
                   MAXHOSTNAMELEN)
      != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Heartbeat_Application::init - "
                  "cannot get hostname\n"));
      throw CORBA::INTERNAL ();
    }

  // Connect to EC as a supplier.
  this->connect_as_supplier ();

  // Connect to EC as a consumer.
  try
    {
      this->connect_as_consumer ();
    }
  catch (const CORBA::Exception&)
    {
      this->consumer_proxy_disconnect_.execute ();
      throw;
    }

  // Register for reactor timeouts.
  if (this->register_for_timeouts () == -1)
    {
      this->consumer_proxy_disconnect_.execute ();
      this->supplier_proxy_disconnect_.execute ();
      this->deactivator_.deactivate ();
      throw CORBA::INTERNAL ();
    }

  this->initialized_ = 1;
}

int
Heartbeat_Application::register_for_timeouts (void)
{
  // Schedule timeout every 0.5 seconds, for sending heartbeat events.
  ACE_Time_Value timeout_interval (0, 500000);
  ACE_Reactor *reactor = this->orb_->orb_core ()->reactor ();
  if (!reactor
      || reactor->schedule_timer (&this->timeout_handler_, 0,
                                  timeout_interval,
                                  timeout_interval) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Heartbeat_Application::register_for_timeouts - "
                         "cannot schedule timer\n"),
                        -1);
    }

  return 0;
}

void
Heartbeat_Application::stop_timeouts (void)
{
  ACE_Reactor *reactor = this->orb_->orb_core ()->reactor ();
  if (!reactor
      || reactor->cancel_timer (&this->timeout_handler_) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "Heartbeat_Application::stop_timeouts - "
                  "cannot deregister from reactor.\n"));
    }
}

void
Heartbeat_Application::connect_as_supplier (void)
{
  // Obtain reference to SupplierAdmin.
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    this->ec_->for_suppliers ();

  // Obtain ProxyPushConsumer and connect this supplier.
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy =
    supplier_admin->obtain_push_consumer ();
  Consumer_Proxy_Disconnect new_proxy_disconnect (proxy.in ());

  ACE_SupplierQOS_Factory qos;
  qos.insert (SOURCE_ID, HEARTBEAT, 0, 1);

  proxy->connect_push_supplier (RtecEventComm::PushSupplier::_nil (),
                                qos.get_SupplierQOS ());

  // Update resource managers.
  this->consumer_ = proxy._retn ();
  this->consumer_proxy_disconnect_.set_command (new_proxy_disconnect);
}

void
Heartbeat_Application::connect_as_consumer (void)
{
  // Activate with poa.
  RtecEventComm::PushConsumer_var consumer_ref;
  PortableServer::POA_var poa = this->_default_POA ();

  TAO_EC_Object_Deactivator deactivator;
  activate (consumer_ref,
            poa.in (),
            this,
            deactivator);

  // Obtain reference to ConsumerAdmin.
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->ec_->for_consumers ();

  // Obtain ProxyPushSupplier..
  RtecEventChannelAdmin::ProxyPushSupplier_var proxy =
    consumer_admin->obtain_push_supplier ();
  Supplier_Proxy_Disconnect new_proxy_disconnect (proxy.in ());

  // Connect this consumer.
  ACE_ConsumerQOS_Factory qos;
  qos.start_disjunction_group (1);
  qos.insert_type (ACE_ES_EVENT_ANY, 0);
  proxy->connect_push_consumer (consumer_ref.in (),
                                qos.get_ConsumerQOS ());

  // Update resource managers.
  this->supplier_proxy_disconnect_.set_command (new_proxy_disconnect);
  this->set_deactivator (deactivator);
}

int
Heartbeat_Application::handle_timeout (const ACE_Time_Value&,
                                       const void*)
{
  try
    {
      if (this->n_timeouts_++ < HEARTBEATS_TO_SEND)
        {
          RtecEventComm::EventSet events (1);
          events.length (1);
          // Events travelling through gateways must have a ttl count of at
          // least 1!
          events[0].header.ttl = 1;
          events[0].header.type = HEARTBEAT;
          events[0].header.source = SOURCE_ID;

          // Store our hostname and process id in the data portion of
          // the event.
          events[0].data.payload.replace (MAXHOSTNAMELEN+11,
                                          MAXHOSTNAMELEN+11,
                                          (u_char *)this->hostname_and_pid_,
                                          0);

          this->consumer_->push (events);
        }
      else
        // We already sent the required number of heartbeats.  Time to
        // shutdown this app.
        {
          this->shutdown ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Suppressed the following exception in ""Heartbeat_Application::handle_timeout:\n");
    }
  return 0;
}

void
Heartbeat_Application::push (const RtecEventComm::EventSet &events)
{
  for (CORBA::ULong i = 0; i < events.length (); ++i)
    {
      // Figure out heartbeat source.
      const u_char * buffer = events[i].data.payload.get_buffer ();
      pid_t pid = *((pid_t*) buffer);
      char * host = (char*) buffer + sizeof (pid);

      // Update heartbeat database.
      int found = 0;
      size_t size = this->heartbeats_.size ();
      for (size_t j = 0; j < size; ++j)
        {
          if (this->heartbeats_[j].pid == pid
              && ACE_OS::strcmp (this->heartbeats_[j].hostname, host)
              == 0)
            {
              this->heartbeats_[j].total++;
              found = 1;
              break;
            }
        }
      // Make new entry in the database.
      if (!found)
        {
          if (this->heartbeats_.size (size + 1)
              == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          "Unable to add new entry "
                          "to heartbeat database\n"));
              break;
            }

          this->heartbeats_[size].pid = pid;
          this->heartbeats_[size].total = 1;
          ACE_OS::memcpy (this->heartbeats_[size].hostname,
                          host,
                          ACE_OS::strlen (host) + 1);
        }
    }
}

void
Heartbeat_Application::disconnect_push_consumer (void)
{
  this->shutdown ();
}

void
Heartbeat_Application::destroy_ec (void)
{
  if (!CORBA::is_nil (this->ec_.in ()))
    {
      try
        {
          this->ec_->destroy ();
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception (
            "Suppressed the following exception in ""Application_Heartbeat::destroy_ec\n");
        }

      this->ec_ = RtecEventChannelAdmin::EventChannel::_nil ();
    }
}
void
Heartbeat_Application::shutdown (void)
{
  if (!this->initialized_)
    return;

  this->initialized_ = 0;

  // Deregister from Reactor.
  this->stop_timeouts ();

  // Disconnect from ECs as a consumer.
  this->supplier_proxy_disconnect_.execute ();
  // Disconnect from EC as a supplier.
  this->consumer_proxy_disconnect_.execute ();

  if (destroy_ec_flag)
    {
      this->destroy_ec ();
    }

  // Deregister from POA.
  this->deactivator_.deactivate ();

  // Print out heartbeats report.
  pid_t pid = ACE_OS::getpid ();
  char hostname[MAXHOSTNAMELEN + 1];
  if (gethostname (hostname, MAXHOSTNAMELEN) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Heartbeat_Application::shutdown - "
                  "cannot get hostname\n"));
      hostname[0] = '\0';
    }
  ACE_DEBUG ((LM_DEBUG,
              "%d@%s Received following heartbeats:\n",
              pid, hostname));
  for (size_t i = 0; i < this->heartbeats_.size (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Host %s, pid %d - total of %u\n",
                  this->heartbeats_[i].hostname,
                  this->heartbeats_[i].pid,
                  this->heartbeats_[i].total));
    }

  // Shutdown the ORB.
  try
    {
      this->orb_->shutdown (0);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "The following exception occured in ""Heartbeat_Application::shutdown:\n");
    }
}

////////////////////////////////////////////////////////////
int
check_for_nil (CORBA::Object_ptr obj, const char *message)
{
  if (CORBA::is_nil (obj))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: Object reference <%s> is nil\n",
                       message),
                      -1);
  else
    return 0;
}

int
parse_args (int argc, ACE_TCHAR ** argv)
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("d"));
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'd':
          destroy_ec_flag = 1;
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "-d"
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  // We may want this to be alive beyond the next block.
  PortableServer::Servant_var<Heartbeat_Application> app;

  try
    {
      // Initialize ORB and POA, POA Manager, parse args.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) == -1)
        return 1;

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (obj.in ());
      if (check_for_nil (poa.in (), "POA") == -1)
        return 1;

      PortableServer::POAManager_var manager =
        poa->the_POAManager ();

      // Obtain reference to EC.
      obj = orb->resolve_initial_references ("Event_Service");
      RtecEventChannelAdmin::EventChannel_var ec =
        RtecEventChannelAdmin::EventChannel::_narrow (obj.in ());
      if (check_for_nil (ec.in (), "EC") == -1)
        return 1;

      // Init our application.
      app = new Heartbeat_Application;
      if (!app.in ())
        return 1;

      app->init (orb, ec);

      // Allow processing of CORBA requests.
      manager->activate ();

      // Receive events from EC.
      orb->run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception in Heartbeat Application:");
      // Since there was an exception, application might not have had
      // a chance to shutdown.
      app->shutdown ();
      return 1;
    }

  return 0;
}
