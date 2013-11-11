// -*- C++ -*-
// $Id$

#include "tao/default_client.h"
#include "tao/Wait_On_Read.h"
#include "tao/Wait_On_Reactor.h"
#include "tao/Wait_On_Leader_Follower.h"
#include "tao/Wait_On_LF_No_Upcall.h"
#include "tao/Exclusive_TMS.h"
#include "tao/Muxed_TMS.h"
#include "tao/Blocked_Connect_Strategy.h"
#include "tao/Reactive_Connect_Strategy.h"
#include "tao/LF_Connect_Strategy.h"
#include "tao/orbconf.h"
#include "tao/Invocation_Utils.h"

#include "ace/Lock_Adapter_T.h"
#include "ace/Recursive_Thread_Mutex.h"

#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Default_Client_Strategy_Factory::TAO_Default_Client_Strategy_Factory (void)
  : transport_mux_strategy_ (TAO_MUXED_TMS)
  , wait_strategy_ (TAO_WAIT_ON_LEADER_FOLLOWER)
  , connect_strategy_ (TAO_LEADER_FOLLOWER_CONNECT)
  , rd_table_size_ (TAO_RD_TABLE_SIZE)
  , muxed_strategy_lock_type_ (TAO_THREAD_LOCK)
  , use_cleanup_options_ (false)
{
  // Use single thread client connection handler
#if defined (TAO_USE_ST_CLIENT_CONNECTION_HANDLER)
  this->wait_strategy_ = TAO_WAIT_ON_REACTOR;
#elif defined (TAO_USE_WAIT_ON_LF_NO_UPCALL)
  this->wait_strategy_ = TAO_WAIT_ON_LF_NO_UPCALL;
#elif defined (TAO_USE_WAIT_RW_STRATEGY)
  this->wait_strategy_ = TAO_WAIT_ON_READ;
#else
  this->wait_strategy_ = TAO_WAIT_ON_LEADER_FOLLOWER;
#endif /* TAO_USE_ST_CLIENT_CONNECTION_HANDLER */

#if TAO_USE_MUXED_TRANSPORT_MUX_STRATEGY == 1
  this->transport_mux_strategy_ = TAO_MUXED_TMS;
#else
  this->transport_mux_strategy_ = TAO_EXCLUSIVE_TMS;
#endif /* TAO_USE_MUXED_TRANSPORT_MUX_STRATEGY */

#if defined (TAO_USE_BLOCKING_CONNECT_STRATEGY)
  this->connect_strategy_ = TAO_BLOCKED_CONNECT;
#elif defined (TAO_USE_REACTIVE_CONNECT_STRATEGY)
  this->connect_strategy_ = TAO_REACTIVE_CONNECT;
#else
  // @todo: will be changed when other strategies are implemented.
  this->connect_strategy_ = TAO_LEADER_FOLLOWER_CONNECT;
#endif
}

TAO_Default_Client_Strategy_Factory::~TAO_Default_Client_Strategy_Factory (void)
{
}

int
TAO_Default_Client_Strategy_Factory::init (int argc, ACE_TCHAR* argv[])
{
  return this->parse_args (argc, argv);
}

int
TAO_Default_Client_Strategy_Factory::parse_args (int argc, ACE_TCHAR* argv[])
{
  ACE_TRACE ("TAO_Default_Client_Strategy_Factory::parse_args");

  int curarg;

  for (curarg = 0; curarg < argc && argv[curarg]; ++curarg)
    {
      if (ACE_OS::strcasecmp (argv[curarg],
                              ACE_TEXT("-ORBClientConnectionHandler")) == 0
          ||
          ACE_OS::strcasecmp (argv[curarg],
                              ACE_TEXT("-ORBWaitStrategy")) == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              ACE_TCHAR* name = argv[curarg];

              if (ACE_OS::strcasecmp (name,
                                      ACE_TEXT("MT")) == 0)
                this->wait_strategy_ = TAO_WAIT_ON_LEADER_FOLLOWER;
              else if (ACE_OS::strcasecmp (name,
                                           ACE_TEXT("ST")) == 0)
                this->wait_strategy_ = TAO_WAIT_ON_REACTOR;
              else if (ACE_OS::strcasecmp (name,
                                           ACE_TEXT("RW")) == 0)
                this->wait_strategy_ = TAO_WAIT_ON_READ;
              else if (ACE_OS::strcasecmp (name,
                                           ACE_TEXT("MT_NOUPCALL")) == 0)
                this->wait_strategy_ = TAO_WAIT_ON_LF_NO_UPCALL;
              else
                this->report_option_value_error (
                  ACE_TEXT("-ORBClientConnectionHandler"), name);
            }
        }
      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-ORBTransportMuxStrategy"))
               == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              ACE_TCHAR* name = argv[curarg];

              if (ACE_OS::strcasecmp (name,
                                      ACE_TEXT("MUXED")) == 0)
                this->transport_mux_strategy_ = TAO_MUXED_TMS;
              else if (ACE_OS::strcasecmp (name,
                                           ACE_TEXT("EXCLUSIVE")) == 0)
                this->transport_mux_strategy_ = TAO_EXCLUSIVE_TMS;
              else
                this->report_option_value_error (
                  ACE_TEXT("-ORBTransportMuxStrategy"), name);
            }
        }
      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-ORBTransportMuxStrategyLock"))
               == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              ACE_TCHAR* name = argv[curarg];

              if (ACE_OS::strcasecmp (name,
                                      ACE_TEXT("null")) == 0)
                this->muxed_strategy_lock_type_ = TAO_NULL_LOCK;
              else if (ACE_OS::strcasecmp (name,
                                           ACE_TEXT("thread")) == 0)
                this->muxed_strategy_lock_type_ = TAO_THREAD_LOCK;
              else
                this->report_option_value_error (
                  ACE_TEXT("-ORBTransportMuxStrategyLock"), name);
            }
        }
      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-ORBConnectStrategy")) == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              ACE_TCHAR* name = argv[curarg];

              if (ACE_OS::strcasecmp (name,
                                      ACE_TEXT("Blocked")) == 0)
                this->connect_strategy_ = TAO_BLOCKED_CONNECT;
              else if (ACE_OS::strcasecmp (name,
                                           ACE_TEXT("Reactive")) == 0)
                this->connect_strategy_ = TAO_REACTIVE_CONNECT;
              else if (ACE_OS::strcasecmp (name,
                                           ACE_TEXT("LF")) == 0)
                this->connect_strategy_ = TAO_LEADER_FOLLOWER_CONNECT;
              else
                this->report_option_value_error (
                  ACE_TEXT("-ORBConnectStrategy"),
                  name);
            }
        }
      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-ORBReplyDispatcherTableSize"))
               == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              this->rd_table_size_ = ACE_OS::atoi (argv[curarg]);
            }
        }
      else if (ACE_OS::strcmp (argv[curarg],
                               ACE_TEXT("-ORBConnectionHandlerCleanup")) == 0)
         {
           curarg++;
           if (curarg < argc)
             {
               ACE_TCHAR* name = argv[curarg];

               if (ACE_OS::strcmp (name, ACE_TEXT("0")) == 0 ||
                   ACE_OS::strcasecmp (name, ACE_TEXT("false")) == 0)
                 this->use_cleanup_options_ = false;
               else if (ACE_OS::strcmp (name, ACE_TEXT("1")) == 0 ||
                        ACE_OS::strcasecmp (name, ACE_TEXT("true")) == 0)
                 this->use_cleanup_options_ = true;
               else
                 this->report_option_value_error (
                   ACE_TEXT("-ORBConnectionHandlerCleanup"), name);
             }
         }
      else if (ACE_OS::strcmp (argv[curarg],
                               ACE_TEXT("-ORBForwardOnCommFailureLimit"))
               == 0)
         {
           curarg++;
           if (curarg < argc)
             {
               ACE_TCHAR* name = argv[curarg];

               ACE_TCHAR *err = 0;
               long limit = ACE_OS::strtol (name, &err, 10);
               if (err && *err != 0)
                 {
                   this->report_option_value_error (
                     ACE_TEXT("-ORBForwardOnCommFailureLimit"),
                     name);
                 }
               else
                 this->invocation_retry_params_
                   .forward_on_exception_limit_[TAO::FOE_COMM_FAILURE] =
                   limit;
             }
         }
      else if (ACE_OS::strcmp (argv[curarg],
                               ACE_TEXT("-ORBForwardOnTransientLimit")) == 0)
         {
           curarg++;
           if (curarg < argc)
             {
               ACE_TCHAR* name = argv[curarg];

               ACE_TCHAR *err = 0;
               long limit = ACE_OS::strtol (name, &err, 10);
               if (err && *err != 0)
                 {
                   this->report_option_value_error (
                     ACE_TEXT("-ORBForwardOnTransientLimit"),
                     name);
                 }
               else
                 this->invocation_retry_params_
                   .forward_on_exception_limit_[TAO::FOE_TRANSIENT] =
                   limit;
             }
         }
      else if (ACE_OS::strcmp (argv[curarg],
                               ACE_TEXT("-ORBForwardOnObjectNotExistLimit"))
               == 0)
         {
           curarg++;
           if (curarg < argc)
             {
               ACE_TCHAR* name = argv[curarg];

               ACE_TCHAR *err = 0;
               long limit = ACE_OS::strtol (name, &err, 10);
               if (err && *err != 0)
                 {
                   this->report_option_value_error (
                     ACE_TEXT("-ORBForwardOnObjectNotExistLimit"),
                     name);
                 }
               else
                 this->invocation_retry_params_
                   .forward_on_exception_limit_[TAO::FOE_OBJECT_NOT_EXIST] =
                   limit;
             }
         }
      else if (ACE_OS::strcmp (argv[curarg],
                               ACE_TEXT("-ORBForwardOnInvObjrefLimit")) == 0)
         {
           curarg++;
           if (curarg < argc)
             {
               ACE_TCHAR* name = argv[curarg];

               ACE_TCHAR *err = 0;
               long limit = ACE_OS::strtol (name, &err, 10);
               if (err && *err != 0)
                 {
                   this->report_option_value_error (
                     ACE_TEXT("-ORBForwardOnInvObjrefLimit"), name);
                 }
               else
                 this->invocation_retry_params_
                   .forward_on_exception_limit_[TAO::FOE_INV_OBJREF] =
                   limit;
             }
         }
      else if (ACE_OS::strcmp (argv[curarg],
                               ACE_TEXT("-ORBForwardOnReplyClosedLimit"))
               == 0)
         {
           curarg++;
           if (curarg < argc)
             {
               ACE_TCHAR* name = argv[curarg];

               ACE_TCHAR *err = 0;
               long limit = ACE_OS::strtol (name, &err, 10);
               if (err && *err != 0)
                 {
                   this->report_option_value_error (
                     ACE_TEXT("-ORBForwardOnReplyClosedLimit"), name);
                 }
               else
                 this->invocation_retry_params_
                   .forward_on_reply_closed_limit_ = limit;
             }
         }
      else if (ACE_OS::strncmp (argv[curarg], ACE_TEXT("-ORB"), 4) == 0)
        {
          // Can we assume there is an argument after the option?
          // curarg++;
          TAOLIB_ERROR ((LM_ERROR,
                      "Client_Strategy_Factory - "
                      "unknown option <%s>\n",
                      argv[curarg]));
        }
      else
        {
          TAOLIB_DEBUG ((LM_DEBUG,
                      "Client_Strategy_Factory - "
                      "ignoring option <%s>\n",
                      argv[curarg]));
        }
    }
  return 0;
}

/// Create the correct client transport muxing strategy.
TAO_Transport_Mux_Strategy *
TAO_Default_Client_Strategy_Factory::create_transport_mux_strategy (
   TAO_Transport *transport)
{
  TAO_Transport_Mux_Strategy *tms = 0;

  switch (this->transport_mux_strategy_)
    {
      case TAO_MUXED_TMS:
      {
        ACE_NEW_RETURN (tms,
                        TAO_Muxed_TMS (transport),
                        0);
        break;
      }
      case TAO_EXCLUSIVE_TMS:
      {
        ACE_NEW_RETURN (tms,
                        TAO_Exclusive_TMS (transport),
                        0);
        break;
      }
    }

  return tms;
}

ACE_Lock *
TAO_Default_Client_Strategy_Factory::create_transport_mux_strategy_lock (void)
{
  ACE_Lock *the_lock = 0;

  switch (this->muxed_strategy_lock_type_)
    {
      case TAO_NULL_LOCK:
        {
          ACE_NEW_RETURN (the_lock,
                          ACE_Lock_Adapter<ACE_SYNCH_NULL_MUTEX> (),
                          0);
          break;
        }
      case TAO_THREAD_LOCK:
        {
          ACE_NEW_RETURN (the_lock,
                          ACE_Lock_Adapter<TAO_SYNCH_RECURSIVE_MUTEX> (),
                          0);
          break;
        }
    }

  return the_lock;
}

int
TAO_Default_Client_Strategy_Factory::reply_dispatcher_table_size (void) const
{
  return this->rd_table_size_;
}

TAO_Wait_Strategy *
TAO_Default_Client_Strategy_Factory::create_wait_strategy (
  TAO_Transport *transport)
{
  TAO_Wait_Strategy *ws = 0;

/*
 * Hook to customize the wait strategy object when the concrete
 * wait strategy object is known a priori.
 */
//@@ WAIT_STRATEGY_SPL_COMMENT_HOOK_START
  switch (this->wait_strategy_)
    {
      case TAO_WAIT_ON_LEADER_FOLLOWER :
        {
          ACE_NEW_RETURN (ws,
                          TAO_Wait_On_Leader_Follower (transport),
                          0);
          break;
        }
      case TAO_WAIT_ON_REACTOR:
        {
          ACE_NEW_RETURN (ws,
                          TAO_Wait_On_Reactor (transport),
                          0);
          break;
        }
      case TAO_WAIT_ON_READ:
        {
          ACE_NEW_RETURN (ws,
                          TAO_Wait_On_Read (transport),
                          0);
          break;
        }
      case TAO_WAIT_ON_LF_NO_UPCALL:
        {
          ACE_NEW_RETURN (ws,
                          TAO::Wait_On_LF_No_Upcall (transport),
                          0);
          break;
        }
    }
//@@ WAIT_STRATEGY_SPL_COMMENT_HOOK_END

  return ws;
}

TAO_Client_Strategy_Factory::Connect_Strategy
TAO_Default_Client_Strategy_Factory::connect_strategy (void) const
{
  return this->connect_strategy_;
}

TAO_Connect_Strategy *
TAO_Default_Client_Strategy_Factory::create_connect_strategy (
  TAO_ORB_Core *orb_core)
{
  TAO_Connect_Strategy *cs = 0;

  switch (this->connect_strategy_)
    {
      case TAO_BLOCKED_CONNECT:
        {
          ACE_NEW_RETURN (cs,
                          TAO_Blocked_Connect_Strategy (orb_core),
                          0);
          break;
        }
      case TAO_REACTIVE_CONNECT:
        {
          ACE_NEW_RETURN (cs,
                          TAO_Reactive_Connect_Strategy (orb_core),
                          0);
          break;
        }
      case TAO_LEADER_FOLLOWER_CONNECT :
        {
          ACE_NEW_RETURN (cs,
                          TAO_LF_Connect_Strategy (orb_core),
                          0);
          break;
        }
    }

  return cs;
}


int
TAO_Default_Client_Strategy_Factory::allow_callback (void)
{
  return (this->wait_strategy_ != TAO_WAIT_ON_READ);
}

void
TAO_Default_Client_Strategy_Factory::report_option_value_error (
                                 const ACE_TCHAR* option_name,
                                 const ACE_TCHAR* option_value)
{
  TAOLIB_DEBUG((LM_DEBUG,
             ACE_TEXT ("Client_Strategy_Factory - unknown argument")
             ACE_TEXT (" <%s> for <%s>\n"),
             option_value, option_name));
}

bool
TAO_Default_Client_Strategy_Factory::use_cleanup_options (void) const
{
  return this->use_cleanup_options_;
}

const TAO::Invocation_Retry_Params &
TAO_Default_Client_Strategy_Factory::invocation_retry_params (void) const
{
  return this->invocation_retry_params_;
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Default_Client_Strategy_Factory,
                       ACE_TEXT ("Client_Strategy_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Client_Strategy_Factory),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Client_Strategy_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

