// $Id$

#include "tao/default_client.h"
#include "tao/ORB_Core.h"
#include "tao/Wait_Strategy.h"
#include "tao/Request_Mux_Strategy.h"

#if !defined (__ACE_INLINE__)
# include "tao/default_client.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, default_client, "$Id$")

TAO_Default_Client_Strategy_Factory::TAO_Default_Client_Strategy_Factory (void)
  : iiop_profile_lock_type_ (TAO_THREAD_LOCK)
{
  // Use single thread client connection handler
#if defined (TAO_USE_ST_CLIENT_CONNECTION_HANDLER)
  this->client_connection_handler_ = ST_CLIENT_CONNECTION_HANDLER;
#else
  this->client_connection_handler_ = MT_CLIENT_CONNECTION_HANDLER;
#endif /* TAO_USE_ST_CLIENT_CONNECTION_HANDLER */

#if defined (TAO_USE_MUXED_REQUEST_MUX_STRATEGY)
  this->request_mux_strategy_ = TAO_MUXED_RMS;
#else 
  this->request_mux_strategy_ = TAO_EXCLUSIVE_RMS;
#endif /* TAO_USE_MUXED_REQUEST_MUX_STRATEGY */

#if defined (TAO_USE_WAIT_ON_LEADER_FOLLOWER)
  this->wait_strategy_ = TAO_WAIT_ON_LEADER_FOLLOWER;
#elif defined (TAO_USE_WAIT_ON_REACTOR)
  this->wait_strategy_ = TAO_WAIT_ON_REACTOR;
#else 
  this->wait_strategy_ = TAO_WAIT_ON_READ;
#endif /* TAO_USE_WAIT_ON_LEADER_FOLLOWER */
}

TAO_Default_Client_Strategy_Factory::~TAO_Default_Client_Strategy_Factory (void)
{
}

int
TAO_Default_Client_Strategy_Factory::init (int argc, char *argv[])
{
  return this->parse_args (argc, argv);
}

int
TAO_Default_Client_Strategy_Factory::parse_args (int argc, char ** argv)
{
  ACE_TRACE ("TAO_Default_Client_Strategy_Factory::parse_args");
  
  int curarg;

  for (curarg = 0; curarg < argc && argv[curarg]; curarg++)
    {
      if (ACE_OS::strcmp (argv[curarg], "-ORBiiopprofilelock") == 0)
        {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "thread") == 0)
              this->iiop_profile_lock_type_ = TAO_THREAD_LOCK;
            else if (ACE_OS::strcasecmp (name, "null") == 0)
              this->iiop_profile_lock_type_ = TAO_NULL_LOCK;
          }
        }

      else if (ACE_OS::strcmp (argv[curarg], "-ORBclientconnectionhandler") == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              char *name = argv[curarg];
              
              if (ACE_OS::strcasecmp (name, "MT") == 0)
                {
                  this->client_connection_handler_ = MT_CLIENT_CONNECTION_HANDLER;
                  this->wait_strategy_ = TAO_WAIT_ON_LEADER_FOLLOWER;
                }
              else if (ACE_OS::strcasecmp (name, "ST") == 0)
                {
                  this->client_connection_handler_ = ST_CLIENT_CONNECTION_HANDLER;
                  this->wait_strategy_ = TAO_WAIT_ON_REACTOR;
                }
              else if (ACE_OS::strcasecmp (name, "RW") == 0)
                {
                  this->client_connection_handler_ = RW_CLIENT_CONNECTION_HANDLER;
                  this->wait_strategy_ = TAO_WAIT_ON_READ;
                }
            }
        }
      else if (ACE_OS::strcmp (argv[curarg], "-ORBrequestmuxstrategy") == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              char *name = argv[curarg];
              
              if (ACE_OS::strcasecmp (name, "MUXED") == 0)
                this->request_mux_strategy_ = TAO_MUXED_RMS;
              else if (ACE_OS::strcasecmp (name, "EXCLUSIVE") == 0)
                this->request_mux_strategy_ = TAO_EXCLUSIVE_RMS;
            }
        }
    }
  return 0;
}

ACE_Lock *
TAO_Default_Client_Strategy_Factory::create_iiop_profile_lock (void)
{
  ACE_Lock* the_lock = 0;

  if (this->iiop_profile_lock_type_ == TAO_NULL_LOCK)
    ACE_NEW_RETURN (the_lock,
		    ACE_Lock_Adapter<ACE_SYNCH_NULL_MUTEX> (),
		    0);
  else
    ACE_NEW_RETURN (the_lock,
		    ACE_Lock_Adapter<ACE_SYNCH_MUTEX> (),
		    0);

  return the_lock;
}

ACE_Creation_Strategy<TAO_Client_Connection_Handler> *
TAO_Default_Client_Strategy_Factory::create_client_creation_strategy (void)
{
  // Create the correct client connection creation strategy
  ACE_Creation_Strategy<TAO_Client_Connection_Handler> *client_creation_strategy = 0;
  
  if (this->client_connection_handler_ == ST_CLIENT_CONNECTION_HANDLER)
    {
      ACE_NEW_RETURN (client_creation_strategy,
                      TAO_ST_Connect_Creation_Strategy,
                      0);
    }
  else if (this->client_connection_handler_ == MT_CLIENT_CONNECTION_HANDLER)
    {
      ACE_NEW_RETURN (client_creation_strategy,
                      TAO_MT_Connect_Creation_Strategy,
                      0);
    }
  else if (this->client_connection_handler_ == RW_CLIENT_CONNECTION_HANDLER)
    {
      ACE_NEW_RETURN (client_creation_strategy,
                      TAO_RW_Connect_Creation_Strategy,
                      0);
    }

  return client_creation_strategy;
}

// @@ Alex: implement the WS and RMS methods here, similar to the
//    create_iiop_profile_lock above...
// @@ Alex: remember your idea of using the
//    -ORBclientconnectionhandler option to implement the WS factory,
//    but you need new options for the RMS...

// Create the correct client request muxing strategy.
TAO_Request_Mux_Strategy *
TAO_Default_Client_Strategy_Factory::create_request_mux_strategy (void)
{
  TAO_Request_Mux_Strategy *rms = 0;

  if (this->request_mux_strategy_ == TAO_MUXED_RMS)
    {
      ACE_NEW_RETURN (rms,
                      TAO_Muxed_RMS,
                      0);
    }
  else
    {
      ACE_NEW_RETURN (rms,
                      TAO_Exclusive_RMS,
                      0);
    }
  
  return rms;
}  

TAO_Wait_Strategy *
TAO_Default_Client_Strategy_Factory::create_wait_strategy (TAO_Transport *transport)
{
  TAO_Wait_Strategy *ws = 0;

  if (this->wait_strategy_ == TAO_WAIT_ON_LEADER_FOLLOWER)
    {
      ACE_NEW_RETURN (ws,
                      TAO_Wait_On_Leader_Follower (transport),
                      0);
    }
  else if (this->wait_strategy_ == TAO_WAIT_ON_REACTOR)
    {
      ACE_NEW_RETURN (ws,
                      TAO_Wait_On_Reactor (transport),
                      0);
    }
  else
    {
      ACE_NEW_RETURN (ws,
                      TAO_Wait_On_Read (transport),
                      0);
    }
  
  return ws;
}

// ****************************************************************

TAO_ST_Connect_Creation_Strategy::TAO_ST_Connect_Creation_Strategy (ACE_Thread_Manager *t)
  : ACE_Creation_Strategy<TAO_Client_Connection_Handler> (t)
{
}

int
TAO_ST_Connect_Creation_Strategy::make_svc_handler (TAO_Client_Connection_Handler *&sh)
{
  if (sh == 0)
    ACE_NEW_RETURN (sh, TAO_Client_Connection_Handler (this->thr_mgr_), -1);

  return 0;
}

// ****************************************************************

TAO_MT_Connect_Creation_Strategy::TAO_MT_Connect_Creation_Strategy (ACE_Thread_Manager *t)
  : ACE_Creation_Strategy<TAO_Client_Connection_Handler> (t)
{
}

int
TAO_MT_Connect_Creation_Strategy::make_svc_handler (TAO_Client_Connection_Handler *&sh)
{
  if (sh == 0)
    ACE_NEW_RETURN (sh, TAO_MT_Client_Connection_Handler (this->thr_mgr_), -1);

  return 0;
}

// ****************************************************************

TAO_RW_Connect_Creation_Strategy::TAO_RW_Connect_Creation_Strategy (ACE_Thread_Manager *t)
  : ACE_Creation_Strategy<TAO_Client_Connection_Handler> (t)
{
}

int
TAO_RW_Connect_Creation_Strategy::make_svc_handler (TAO_Client_Connection_Handler *&sh)
{
  // @@ Creating the common connection handler. (Alex).
  if (sh == 0)
    ACE_NEW_RETURN (sh, TAO_Client_Connection_Handler (this->thr_mgr_), -1);

  return 0;
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Default_Client_Strategy_Factory,
		       ASYS_TEXT ("Client_Strategy_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Client_Strategy_Factory),
		       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Client_Strategy_Factory)
