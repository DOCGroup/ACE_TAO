// $Id$
#include "ace/Service_Config.h"
#include "tao/Strategies/advanced_resource.h"

#include "tao/Strategies/UIOP_Factory.h"
#include "tao/Strategies/SHMIOP_Factory.h"
#include "tao/Strategies/DIOP_Factory.h"
#include "tao/Strategies/SCIOP_Factory.h"
#include "tao/Strategies/COIOP_Factory.h"

#include "tao/Strategies/LFU_Connection_Purging_Strategy.h"
#include "tao/Strategies/FIFO_Connection_Purging_Strategy.h"
#include "tao/Strategies/NULL_Connection_Purging_Strategy.h"

#include "tao/Strategies/LF_Strategy_Null.h"

#include "tao/debug.h"
#include "tao/LRU_Connection_Purging_Strategy.h"
#include "tao/LF_Strategy_Complete.h"
#include "tao/LF_Follower.h"
#include "tao/Leader_Follower.h"
#include "tao/StringSeqC.h"
#include "tao/ORB_Core.h"
#include "tao/Load_Protocol_Factory_T.h"

#include "ace/Arg_Shifter.h"
#include "ace/Service_Config.h"
#include "ace/Select_Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Msg_WFMO_Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/Dev_Poll_Reactor.h"
#include "ace/Malloc_T.h"
#include "ace/Local_Memory_Pool.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_strings.h"
#include "ace/Auto_Ptr.h"

#if !defined (TAO_DEFAULT_REACTOR_TYPE)
#define TAO_DEFAULT_REACTOR_TYPE TAO_REACTOR_TP
#endif /* !TAO_DEFAULT_REACTOR_TYPE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Resource_Factory_Changer::TAO_Resource_Factory_Changer (void)
{

  TAO_ORB_Core::set_resource_factory ("Advanced_Resource_Factory");
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_Advanced_Resource_Factory);

#if TAO_HAS_UIOP == 1
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_UIOP_Protocol_Factory);
#endif /* TAO_HAS_UIOP == 1 */

#if TAO_HAS_SHMIOP == 1
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_SHMIOP_Protocol_Factory);
#endif /* TAO_HAS_SHMIOP == 1 */

#if TAO_HAS_DIOP == 1
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_DIOP_Protocol_Factory);
#endif /* TAO_HAS_DIOP == 1 */

#if TAO_HAS_SCIOP == 1
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_SCIOP_Protocol_Factory);
#endif /* TAO_HAS_SCIOP == 1 */

#if TAO_HAS_COIOP == 1
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_COIOP_Protocol_Factory);
#endif /* TAO_HAS_SCIOP == 1 */
}

TAO_Advanced_Resource_Factory::TAO_Advanced_Resource_Factory (void)
  : reactor_type_ (TAO_DEFAULT_REACTOR_TYPE),
    threadqueue_type_ (TAO_THREAD_QUEUE_NOT_SET),
    cdr_allocator_type_ (TAO_ALLOCATOR_THREAD_LOCK),
    amh_response_handler_allocator_lock_type_ (TAO_ALLOCATOR_THREAD_LOCK),
    ami_response_handler_allocator_lock_type_ (TAO_ALLOCATOR_THREAD_LOCK)
{
  // Constructor
}

TAO_Advanced_Resource_Factory::~TAO_Advanced_Resource_Factory (void)
{
  // Destructor
  TAO_ProtocolFactorySetItor end = this->protocol_factories_.end ();

  for (TAO_ProtocolFactorySetItor iterator =
         this->protocol_factories_.begin ();
       iterator != end;
       ++iterator)
    delete *iterator;

  this->protocol_factories_.reset ();
}

int
TAO_Advanced_Resource_Factory::init (int argc, ACE_TCHAR** argv)
{
  ACE_TRACE ("TAO_Advanced_Resource_Factory::init");

  // If this factory has already been disabled then
  // print a warning and exit because any options
  // are useless
  if (this->factory_disabled_)
  {
    ACE_DEBUG ((LM_WARNING,
                ACE_TEXT ("TAO (%P|%t) Warning: Resource_Factory options ignored\n")
                ACE_TEXT ("Advanced Resource Factory is disabled\n")));
    return 0;
  }
  this->options_processed_ = 1;


  // If the default resource factory exists, then disable it.
  // This causes any directives for the "Resource_Factory" to
  // report warnings.
  // Note: this is also being done in init_protocol_factories()
  // to cover the case where init() is not called.
  TAO_Resource_Factory *default_resource_factory =
    ACE_Dynamic_Service<TAO_Resource_Factory>::instance ("Resource_Factory");
  if (default_resource_factory != 0)
    {
      default_resource_factory->disable_factory ();
    }

  ACE_Arg_Shifter arg_shifter (argc, argv);

  //for (int curarg = 0; curarg < argc; ++curarg)
  const ACE_TCHAR *current_arg = 0;
  while (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-ORBReactorRegistry")) == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("TAO_Advanced_Resource_Factory::init - ")
                             ACE_TEXT("-ORBReactorRegistry no longer supported\n")),
                            -1);

        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBReactorLock"))))
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO_Advanced_Resource_Factory - obsolete -ORBReactorLock ")
                      ACE_TEXT("option, please use -ORBReactorType\n")));

          if (ACE_OS::strcasecmp (current_arg, ACE_TEXT("null")) == 0)
            this->reactor_type_ = TAO_REACTOR_SELECT_ST;
          else if (ACE_OS::strcasecmp (current_arg, ACE_TEXT("token")) == 0)
            this->reactor_type_= TAO_REACTOR_SELECT_MT;

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBReactorType"))))
        {
          if (ACE_OS::strcasecmp (current_arg,
                                  ACE_TEXT("select_mt")) == 0)
            this->reactor_type_ = TAO_REACTOR_SELECT_MT;
          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_TEXT("select_st")) == 0)
            this->reactor_type_ = TAO_REACTOR_SELECT_ST;
          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_TEXT("wfmo")) == 0)
#if defined(ACE_WIN32)
            this->reactor_type_ = TAO_REACTOR_WFMO;
#else
            this->report_unsupported_error (ACE_TEXT("WFMO Reactor"));
#endif /* ACE_WIN32 */
          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_TEXT("msg_wfmo")) == 0)
#if defined(ACE_WIN32) && !defined (ACE_LACKS_MSG_WFMO)
            this->reactor_type_ = TAO_REACTOR_MSGWFMO;
#else
            this->report_unsupported_error (ACE_TEXT("MsgWFMO Reactor"));
#endif /* ACE_WIN32 && !ACE_LACKS_MSG_WFMO */

          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_TEXT("tp")) == 0)
            this->reactor_type_ = TAO_REACTOR_TP;

          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_TEXT("dev_poll")) == 0)
            {
#if defined (ACE_HAS_EVENT_POLL) || defined (ACE_HAS_DEV_POLL)
              this->reactor_type_ = TAO_REACTOR_DEV_POLL;
#else
              this->report_unsupported_error (ACE_TEXT ("Dev_Poll Reactor"));
#endif  /* ACE_HAS_EVENT_POLL || ACE_HAS_DEV_POLL */
            }

          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_TEXT("fl")) == 0)
            this->report_option_value_error (
                ACE_TEXT("FlReactor not supported by Advanced_Resources_Factory. Please use TAO_FlResource_Loader instead."),
                         current_arg);
          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_TEXT("tk")) == 0)
            this->report_option_value_error (
                ACE_TEXT("TkReactor not supported by Advanced_Resources_Factory. Please use TAO_TkResource_Loader instead."),
                         current_arg);
          else
            this->report_option_value_error (ACE_TEXT("-ORBReactorType"), current_arg);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBInputCDRAllocator"))))
        {
          if (ACE_OS::strcasecmp (current_arg,
                                  ACE_TEXT("null")) == 0)
            {
              this->cdr_allocator_type_ = TAO_ALLOCATOR_NULL_LOCK;
              this->use_locked_data_blocks_ = 0;
            }
          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_TEXT("thread")) == 0)
            {
              this->cdr_allocator_type_ = TAO_ALLOCATOR_THREAD_LOCK;
              this->use_locked_data_blocks_ = 1;
            }
          else
            {
              this->report_option_value_error (ACE_TEXT("-ORBInputCDRAllocator"), current_arg);
            }

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBAMHResponseHandlerAllocator"))))
        {
          if (ACE_OS::strcasecmp (current_arg,
                                  ACE_TEXT("null")) == 0)
            {
              this->amh_response_handler_allocator_lock_type_ = TAO_ALLOCATOR_NULL_LOCK;
            }
          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_TEXT("thread")) == 0)
            {
              this->amh_response_handler_allocator_lock_type_ = TAO_ALLOCATOR_THREAD_LOCK;
            }
          else
            {
              this->report_option_value_error (ACE_TEXT("-ORBAMHResponseHandlerAllocator"), current_arg);
            }

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBAMIResponseHandlerAllocator"))))
        {
          if (ACE_OS::strcasecmp (current_arg,
                                  ACE_TEXT("null")) == 0)
            {
              this->ami_response_handler_allocator_lock_type_ = TAO_ALLOCATOR_NULL_LOCK;
            }
          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_TEXT("thread")) == 0)
            {
              this->ami_response_handler_allocator_lock_type_ = TAO_ALLOCATOR_THREAD_LOCK;
            }
          else
            {
              this->report_option_value_error (ACE_TEXT("-ORBAMIResponseHandlerAllocator"), current_arg);
            }

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBReactorThreadQueue"))))
        {
          if (ACE_OS::strcasecmp (current_arg,
                                  ACE_TEXT ("LIFO")) == 0)
            this->threadqueue_type_ = TAO_THREAD_QUEUE_LIFO;
          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_TEXT ("FIFO")) == 0)
            this->threadqueue_type_ = TAO_THREAD_QUEUE_FIFO;
          else
            this->report_option_value_error (ACE_TEXT ("-ORBReactorThreadQueue"),
                                             current_arg);

          arg_shifter.consume_arg ();
        }
      else
        // Any arguments that don't match are ignored so that they can
        // be passed to the TAO_Default_Resource_Factory.
        arg_shifter.ignore_arg ();
    }

  // If -ORBReactorThreadQueue was passed, make sure it matches the
  // correct reactor type.  Currently, only the tp reactor can take
  // advantage of the LIFO strategy, select_mt hangs.
  if (this->threadqueue_type_ != TAO_THREAD_QUEUE_NOT_SET &&
      this->reactor_type_ != TAO_REACTOR_TP)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO_Advanced_Resource_Factory: -ORBReactorThreadQueue ")
                ACE_TEXT ("option can only be used with -ORBReactorType ")
                ACE_TEXT ("tp.\n")));
  // Explicitely set the default only if not set.
  else if (this->threadqueue_type_ == TAO_THREAD_QUEUE_NOT_SET)
    this->threadqueue_type_ = TAO_THREAD_QUEUE_LIFO;

  return this->TAO_Default_Resource_Factory::init (argc, argv);
}

int
TAO_Advanced_Resource_Factory::load_default_protocols (void)
{
  int const r =
    this->TAO_Default_Resource_Factory::load_default_protocols ();

  this->protocol_factories_ =
    this->TAO_Default_Resource_Factory::protocol_factories_;

  this->TAO_Default_Resource_Factory::protocol_factories_.reset ();

  if (r == -1)
    return -1;
  // Load the UIOP and SHMIOP protocols...

  return 0;
}


int
TAO_Advanced_Resource_Factory::init_protocol_factories (void)
{
  // If the default resource factory exists, then disable it.
  // This causes any directives for the "Resource_Factory" to
  // report warnings.
  // This is needed to ensure warnings when no static directive
  // for this factory is used (and init() is not called).
  TAO_Resource_Factory *default_resource_factory =
    ACE_Dynamic_Service<TAO_Resource_Factory>::instance ("Resource_Factory");
  if (default_resource_factory != 0)
    {
      default_resource_factory->disable_factory();
    }

  TAO_ProtocolFactorySetItor end = protocol_factories_.end ();
  TAO_ProtocolFactorySetItor factory = protocol_factories_.begin ();

  if (factory == end)
    {
      int const r = this->load_default_protocols ();

      if (r == -1)
        return -1;

#if TAO_HAS_UIOP == 1
      if (TAO::details::load_protocol_factory <TAO_UIOP_Protocol_Factory> (
          this->protocol_factories_, "UIOP_Factory") == -1)
        return -1;
#endif /* TAO_HAS_UIOP == 1 */

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)
      if (TAO::details::load_protocol_factory <TAO_SHMIOP_Protocol_Factory> (
          this->protocol_factories_, "SHMIOP_Factory") == -1)
        return -1;
#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)
      if (TAO::details::load_protocol_factory <TAO_DIOP_Protocol_Factory> (
          this->protocol_factories_, "DIOP_Factory") == -1)
        return -1;
#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */

#if defined (TAO_HAS_SCIOP) && (TAO_HAS_SCIOP != 0)
      if (TAO::details::load_protocol_factory <TAO_SCIOP_Protocol_Factory> (
          this->protocol_factories_, "SCIOP_Factory") == -1)
        return -1;
#endif /* TAO_HAS_SCIOP && TAO_HAS_SCIOP != 0 */

#if defined (TAO_HAS_COIOP) && (TAO_HAS_COIOP != 0)
      if (TAO::details::load_protocol_factory <TAO_COIOP_Protocol_Factory> (
          this->protocol_factories_, "COIOP_Factory") == -1)
        return -1;
#endif /* TAO_HAS_COIOP && TAO_HAS_COIOP != 0 */

      return 0;

    }

  for (; factory != end; factory++)
    {
      const ACE_CString &name = (*factory)->protocol_name ();

      (*factory)->factory (
        ACE_Dynamic_Service<TAO_Protocol_Factory>::instance (name.c_str ()));
      if ((*factory)->factory () == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("TAO (%P|%t) Unable to load ")
                             ACE_TEXT("protocol <%C>, %m\n"),
                             name.c_str ()),
                            -1);
        }

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) Loaded protocol <%C>\n"),
                      name.c_str ()));
        }
    }
 return 0;
}



TAO_ProtocolFactorySet *
TAO_Advanced_Resource_Factory::get_protocol_factories (void)
{
  return &protocol_factories_;
}

ACE_Reactor_Impl *
TAO_Advanced_Resource_Factory::allocate_reactor_impl (void) const
{
  ACE_Reactor_Impl *impl = 0;

  /*
   * Hook for specializing the Reactor implementation in TAO.
   */
  // get a timer queue (or not) from a possibly configured
  // time policy
  TAO_RSF_Timer_Queue_Ptr tmq (*this, this->create_timer_queue ());
//@@ TAO_ADVANCED_RESOURCE_REACTOR_SPL_COMMENT_HOOK_START
  switch (this->reactor_type_)
    {
    case TAO_REACTOR_SELECT_MT:
      ACE_NEW_RETURN (impl,
                      TAO_REACTOR ((ACE_Sig_Handler*)0,
                                   tmq.get (),
                                   0,
                                   (ACE_Reactor_Notify*)0,
                                   this->reactor_mask_signals_),
                      0);
      break;

    case TAO_REACTOR_SELECT_ST:
      ACE_NEW_RETURN (impl,
                      TAO_NULL_LOCK_REACTOR ((ACE_Sig_Handler*)0,
                                             tmq.get (),
                                             0,
                                             (ACE_Reactor_Notify*)0,
                                             this->reactor_mask_signals_),
                      0);
      break;

    case TAO_REACTOR_WFMO:
#if defined(ACE_WIN32)
      ACE_NEW_RETURN (impl, ACE_WFMO_Reactor (0, tmq.get ()), 0);
#endif /* ACE_WIN32 */
      break;

#if defined(ACE_WIN32) \
  && !defined (ACE_LACKS_MSG_WFMO) \
  && !defined (ACE_HAS_WINCE)      \
  && !defined (ACE_HAS_PHARLAP)
    case TAO_REACTOR_MSGWFMO:
      ACE_NEW_RETURN (impl, ACE_Msg_WFMO_Reactor (0, tmq.get ()), 0);
      break;
#endif /* ACE_WIN32 && !ACE_LACKS_MSG_WFMO */

#if defined (ACE_HAS_EVENT_POLL) || defined (ACE_HAS_DEV_POLL)
    case TAO_REACTOR_DEV_POLL:
      ACE_NEW_RETURN (impl,
                      ACE_Dev_Poll_Reactor (ACE::max_handles (),
                                            1,  // restart
                                            (ACE_Sig_Handler*)0,
                                            tmq.get (),
                                            0, // Do not disable notify
                                            0, // Allocate notify handler
                                            this->reactor_mask_signals_,
                                            ACE_Select_Reactor_Token::LIFO),
                      0);
      break;
#endif  /* ACE_HAS_EVENT_POLL || ACE_HAS_DEV_POLL */

    default:
    case TAO_REACTOR_TP:
      ACE_NEW_RETURN (impl,
          ACE_TP_Reactor (ACE::max_handles (),
              1,
              (ACE_Sig_Handler*)0,
              tmq.get (),
              this->reactor_mask_signals_,
              this->threadqueue_type_ == TAO_THREAD_QUEUE_FIFO ?
              ACE_Select_Reactor_Token::FIFO :
              ACE_Select_Reactor_Token::LIFO),
          0);
      break;
    }

//@@ TAO_ADVANCED_RESOURCE_REACTOR_SPL_COMMENT_HOOK_END

  // safe to release timer queue
  tmq.release ();
  return impl;
}

typedef ACE_Malloc<ACE_LOCAL_MEMORY_POOL,ACE_Null_Mutex> NULL_LOCK_MALLOC;
typedef ACE_Allocator_Adapter<NULL_LOCK_MALLOC> NULL_LOCK_ALLOCATOR;

ACE_Allocator *
TAO_Advanced_Resource_Factory::input_cdr_dblock_allocator (void)
{
  ACE_Allocator *allocator = 0;
  switch (this->cdr_allocator_type_)
    {
    case TAO_ALLOCATOR_NULL_LOCK:
      ACE_NEW_RETURN (allocator,
                      NULL_LOCK_ALLOCATOR,
                      0);
      break;
    default:
      return
        this->TAO_Default_Resource_Factory::input_cdr_dblock_allocator();
    }

  return allocator;
}

ACE_Allocator *
TAO_Advanced_Resource_Factory::input_cdr_buffer_allocator (void)
{
  ACE_Allocator *allocator = 0;
  switch (this->cdr_allocator_type_)
    {
    case TAO_ALLOCATOR_NULL_LOCK:
      ACE_NEW_RETURN (allocator,
                      NULL_LOCK_ALLOCATOR,
                      0);
      break;
    default:
      return
        this->TAO_Default_Resource_Factory::input_cdr_buffer_allocator();
    }
  return allocator;
}

ACE_Allocator *
TAO_Advanced_Resource_Factory::input_cdr_msgblock_allocator (void)
{
  ACE_Allocator *allocator = 0;
  switch (this->cdr_allocator_type_)
    {
    case TAO_ALLOCATOR_NULL_LOCK:
      ACE_NEW_RETURN (allocator,
                      NULL_LOCK_ALLOCATOR,
                      0);
      break;
    default:
      return
        this->TAO_Default_Resource_Factory::input_cdr_msgblock_allocator();
    }

  return allocator;
}

ACE_Allocator *
TAO_Advanced_Resource_Factory::amh_response_handler_allocator (void)
{
  ACE_Allocator *allocator = 0;
  switch (this->amh_response_handler_allocator_lock_type_)
    {
    case TAO_ALLOCATOR_NULL_LOCK:
      ACE_NEW_RETURN (allocator,
                      NULL_LOCK_ALLOCATOR,
                      0);
      break;
    default:
      return
        this->TAO_Default_Resource_Factory::amh_response_handler_allocator();
    }

  return allocator;
}

ACE_Allocator *
TAO_Advanced_Resource_Factory::ami_response_handler_allocator (void)
{
  ACE_Allocator *allocator = 0;
  switch (this->ami_response_handler_allocator_lock_type_)
    {
    case TAO_ALLOCATOR_NULL_LOCK:
      ACE_NEW_RETURN (allocator,
                      NULL_LOCK_ALLOCATOR,
                      0);
      break;
    default:
      return
        this->TAO_Default_Resource_Factory::ami_response_handler_allocator();
    }

  return allocator;
}

int
TAO_Advanced_Resource_Factory::input_cdr_allocator_type_locked (void)
{
  return this->cdr_allocator_type_ == TAO_ALLOCATOR_NULL_LOCK ? 0 : 1;
}

TAO_Connection_Purging_Strategy *
TAO_Advanced_Resource_Factory::create_purging_strategy (void)
{
  TAO_Connection_Purging_Strategy *strategy = 0;

  switch(this->connection_purging_type_)
    {
    case TAO_Resource_Factory::LFU:
      ACE_NEW_RETURN (strategy,
                      TAO_LFU_Connection_Purging_Strategy (
                                               this->cache_maximum ()),
                      0);
      break;
    case TAO_Resource_Factory::FIFO:
      ACE_NEW_RETURN (strategy,
                      TAO_FIFO_Connection_Purging_Strategy (
                                               this->cache_maximum ()),
                      0);
      break;
    case TAO_Resource_Factory::NOOP:
      ACE_NEW_RETURN (strategy,
                      TAO_NULL_Connection_Purging_Strategy (
                                               this->cache_maximum ()),
                      0);
      break;
    case TAO_Resource_Factory::LRU:
      ACE_NEW_RETURN (strategy,
                      TAO_LRU_Connection_Purging_Strategy (
                                               this->cache_maximum ()),
                      0);
      break;
    default:
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("TAO (%P|%t) - ")
                  ACE_TEXT("Unknown connection purging strategy ")
                  ACE_TEXT("type was found.\n")));
    }

  return strategy;
}

TAO_LF_Strategy *
TAO_Advanced_Resource_Factory::create_lf_strategy (void)
{
  TAO_LF_Strategy *strategy = 0;

  if (this->reactor_type_ == TAO_REACTOR_SELECT_ST)
    {
      ACE_NEW_RETURN (strategy,
                      TAO_LF_Strategy_Null,
                      0);
    }
  else
    {
      ACE_NEW_RETURN (strategy,
                      TAO_LF_Strategy_Complete,
                      0);
    }
  return strategy;
}

void
TAO_Advanced_Resource_Factory::report_option_value_error (
                                 const ACE_TCHAR* option_name,
                                 const ACE_TCHAR* option_value)
{
  ACE_DEBUG((LM_DEBUG,
             ACE_TEXT("Advanced_Resource_Factory - unknown argument")
             ACE_TEXT(" <%s> for <%s>\n"),
             option_value,
             option_name));
}

void
TAO_Advanced_Resource_Factory::report_unsupported_error (
                                 const ACE_TCHAR* option_name)
{
  ACE_DEBUG((LM_DEBUG,
             ACE_TEXT("Advanced_Resource_Factory - <%s>")
             ACE_TEXT(" not supported on this platform\n"),
             option_name));
}


// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Advanced_Resource_Factory,
                       ACE_TEXT("Advanced_Resource_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Advanced_Resource_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_Strategies, TAO_Advanced_Resource_Factory)

// ****************************************************************
TAO_END_VERSIONED_NAMESPACE_DECL
