// $Id$

#include "advanced_resource.h"

#if !defined (__ACE_INLINE__)
# include "advanced_resource.i"
#endif /* ! __ACE_INLINE__ */

#include "UIOP_Factory.h"
#include "SHMIOP_Factory.h"
#include "DIOP_Factory.h"
#include "SCIOP_Factory.h"

#include "LFU_Connection_Purging_Strategy.h"
#include "FIFO_Connection_Purging_Strategy.h"
#include "NULL_Connection_Purging_Strategy.h"

#include "LF_Strategy_Null.h"

#include "tao/debug.h"
#include "tao/LRU_Connection_Purging_Strategy.h"
#include "tao/LF_Strategy_Complete.h"
#include "tao/Leader_Follower.h"
#include "tao/StringSeqC.h"

#include "ace/Arg_Shifter.h"
#include "ace/Auto_Ptr.h"
#include "ace/Dynamic_Service.h"
#include "ace/Service_Config.h"
#include "ace/Select_Reactor.h"
#include "ace/FlReactor.h"
#include "ace/TkReactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Msg_WFMO_Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/Malloc.h"

ACE_RCSID(Strategies, advanced_resource, "$Id$")

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

}

TAO_Advanced_Resource_Factory::TAO_Advanced_Resource_Factory (void)
  : reactor_type_ (TAO_REACTOR_TP),
    threadqueue_type_ (TAO_THREAD_QUEUE_NOT_SET),
    cdr_allocator_type_ (TAO_ALLOCATOR_THREAD_LOCK)
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
  if (this->factory_disabled_) {
    ACE_DEBUG ((LM_DEBUG,
                ACE_LIB_TEXT("TAO (%P|%t) Warning: Resource_Factory options ignored\n")
                ACE_LIB_TEXT("Default Resource Factory is disabled\n")));
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
      default_resource_factory->disable_factory();
    }

  ACE_Arg_Shifter arg_shifter (argc, argv);

  //for (int curarg = 0; curarg < argc; ++curarg)
  const ACE_TCHAR *current_arg = 0;
  while (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp (ACE_LIB_TEXT("-ORBReactorRegistry")) == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_LIB_TEXT("TAO_Advanced_Resource_Factory::init - ")
                             ACE_LIB_TEXT("-ORBReactorRegistry no longer supported\n")),
                            -1);

        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_LIB_TEXT("-ORBReactorLock"))))
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_LIB_TEXT("TAO_Advanced_Resource_Factory - obsolete -ORBReactorLock ")
                      ACE_LIB_TEXT("option, please use -ORBReactorType\n")));

          if (ACE_OS::strcasecmp (current_arg, ACE_LIB_TEXT("null")) == 0)
            this->reactor_type_ = TAO_REACTOR_SELECT_ST;
          else if (ACE_OS::strcasecmp (current_arg, ACE_LIB_TEXT("token")) == 0)
            this->reactor_type_= TAO_REACTOR_SELECT_MT;

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_LIB_TEXT("-ORBReactorType"))))
        {
          if (ACE_OS::strcasecmp (current_arg,
                                  ACE_LIB_TEXT("select_mt")) == 0)
            this->reactor_type_ = TAO_REACTOR_SELECT_MT;
          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_LIB_TEXT("select_st")) == 0)
            this->reactor_type_ = TAO_REACTOR_SELECT_ST;
          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_LIB_TEXT("fl")) == 0)
#if defined(ACE_HAS_FL)
            this->reactor_type_ = TAO_REACTOR_FL;
#else
            this->report_unsupported_error (ACE_LIB_TEXT("FlReactor"));
#endif /* ACE_HAS_FL */
          else if (ACE_OS::strcasecmp (current_arg, ACE_LIB_TEXT("tk_reactor")) == 0)
#if defined(ACE_HAS_TK)
            this->reactor_type_ = TAO_REACTOR_TK;
#else
            this->report_unsupported_error (ACE_LIB_TEXT("TkReactor"));
#endif /* ACE_HAS_TK */
          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_LIB_TEXT("wfmo")) == 0)
#if defined(ACE_WIN32)
            this->reactor_type_ = TAO_REACTOR_WFMO;
#else
            this->report_unsupported_error (ACE_LIB_TEXT("WFMO Reactor"));
#endif /* ACE_WIN32 */
          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_LIB_TEXT("msg_wfmo")) == 0)
#if defined(ACE_WIN32)
            this->reactor_type_ = TAO_REACTOR_MSGWFMO;
#else
            this->report_unsupported_error (ACE_LIB_TEXT("MsgWFMO Reactor"));
#endif /* ACE_WIN32 */

          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_LIB_TEXT("tp")) == 0)
            this->reactor_type_ = TAO_REACTOR_TP;
          else
            this->report_option_value_error (ACE_LIB_TEXT("-ORBReactorType"), current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_LIB_TEXT("-ORBInputCDRAllocator"))))
        {
          if (ACE_OS::strcasecmp (current_arg,
                                  ACE_LIB_TEXT("null")) == 0)
            {
              this->cdr_allocator_type_ = TAO_ALLOCATOR_NULL_LOCK;
              this->use_locked_data_blocks_ = 0;
            }
          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_LIB_TEXT("thread")) == 0)
            {
              this->cdr_allocator_type_ = TAO_ALLOCATOR_THREAD_LOCK;
              this->use_locked_data_blocks_ = 1;
            }
          else
            {
              this->report_option_value_error (ACE_LIB_TEXT("-ORBInputCDRAllocator"), current_arg);
            }

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_LIB_TEXT("-ORBReactorThreadQueue"))))
        {
          if (ACE_OS::strcasecmp (current_arg,
                                  ACE_LIB_TEXT ("LIFO")) == 0)
            this->threadqueue_type_ = TAO_THREAD_QUEUE_LIFO;
          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_LIB_TEXT ("FIFO")) == 0)
            this->threadqueue_type_ = TAO_THREAD_QUEUE_FIFO;
          else
            this->report_option_value_error (ACE_LIB_TEXT ("-ORBReactorThreadQueue"),
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
                ACE_LIB_TEXT ("TAO_Advanced_Resource_Factory: -ORBReactorThreadQueue ")
                ACE_LIB_TEXT ("option can only be used with -ORBReactorType ")
                ACE_LIB_TEXT ("tp (default).\n")));
  // Explicitely set the default only if not set.
  else if (this->threadqueue_type_ == TAO_THREAD_QUEUE_NOT_SET)
    this->threadqueue_type_ = TAO_THREAD_QUEUE_LIFO;

  return this->TAO_Default_Resource_Factory::init (argc, argv);
}

int
TAO_Advanced_Resource_Factory::load_default_protocols (void)
{
  int r = this->TAO_Default_Resource_Factory::load_default_protocols ();

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
      TAO_Protocol_Factory *protocol_factory = 0;
      auto_ptr<TAO_Protocol_Factory> safe_protocol_factory;

      TAO_Protocol_Item *item = 0;

      // If a protocol factory is obtained from the Service
      // Configurator then do not transfer ownership to the
      // TAO_Protocol_Item.
      int transfer_ownership = 0;

      int r = this->load_default_protocols ();

      if (r == -1)
        return -1;

#if TAO_HAS_UIOP == 1
      protocol_factory =
        ACE_Dynamic_Service<TAO_Protocol_Factory>::instance ("UIOP_Factory");

      if (protocol_factory == 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_WARNING,
                        ACE_LIB_TEXT("(%P|%t) WARNING - No %s found in Service Repository.")
                        ACE_LIB_TEXT("  Using default instance.\n"),
                        ACE_LIB_TEXT("UIOP Protocol Factory")));

          ACE_NEW_RETURN (protocol_factory,
                          TAO_UIOP_Protocol_Factory,
                          -1);

          ACE_AUTO_PTR_RESET (safe_protocol_factory,
                              protocol_factory,
                              TAO_Protocol_Factory);

          transfer_ownership = 1;
        }
      else
        {
          transfer_ownership = 0;
        }

      ACE_NEW_RETURN (item, TAO_Protocol_Item ("UIOP_Factory"), -1);
      // If the TAO_Protocol_Item retains ownership of the
      // TAO_Protocol_Factory then we used an auto_ptr<> above, so
      // release the TAO_Protocol_Factory from it.  Otherwise, we
      // obtained the TAO_Protocol_Factory from the Service
      // Configurator so an auto_ptr<> wasn't used since the Service
      // Configurator retains ownership, hence there was no need to
      // use an auto_ptr<> in this method.
      item->factory ((transfer_ownership ?
                      safe_protocol_factory.release () :
                      protocol_factory),
                     transfer_ownership);

      if (this->protocol_factories_.insert (item) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT("TAO (%P|%t) Unable to add ")
                      ACE_LIB_TEXT("<%s> to protocol factory set.\n"),
                      ACE_TEXT_CHAR_TO_TCHAR(item->protocol_name ().c_str ())));

          delete item;

          if (transfer_ownership == 0)
            delete protocol_factory;

          return -1;
        }

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_LIB_TEXT("TAO (%P|%t) Loaded default ")
                      ACE_LIB_TEXT("protocol <UIOP_Factory>\n")));
        }
#endif /* TAO_HAS_UIOP == 1 */

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)
      protocol_factory =
        ACE_Dynamic_Service<TAO_Protocol_Factory>::instance ("SHMIOP_Factory");

      if (protocol_factory == 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_WARNING,
                        ACE_LIB_TEXT("(%P|%t) WARNING - No %s found in Service Repository.")
                        ACE_LIB_TEXT("  Using default instance.\n"),
                        ACE_LIB_TEXT("SHMIOP Protocol Factory")));

          ACE_NEW_RETURN (protocol_factory,
                          TAO_SHMIOP_Protocol_Factory,
                          -1);

          ACE_AUTO_PTR_RESET (safe_protocol_factory,
                              protocol_factory,
                              TAO_Protocol_Factory);

          transfer_ownership = 1;
        }
      else
        {
          transfer_ownership = 0;
        }

      ACE_NEW_RETURN (item, TAO_Protocol_Item ("SHMIOP_Factory"), -1);
      // If the TAO_Protocol_Item retains ownership of the
      // TAO_Protocol_Factory then we used an auto_ptr<> above, so
      // release the TAO_Protocol_Factory from it.  Otherwise, we
      // obtained the TAO_Protocol_Factory from the Service
      // Configurator so an auto_ptr<> wasn't used since the Service
      // Configurator retains ownership, hence there was no need to
      // use an auto_ptr<> in this method.
      item->factory ((transfer_ownership ?
                      safe_protocol_factory.release () :
                      protocol_factory),
                     transfer_ownership);

      if (this->protocol_factories_.insert (item) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT("TAO (%P|%t) Unable to add ")
                      ACE_LIB_TEXT("<%s> to protocol factory set.\n"),
                      ACE_TEXT_CHAR_TO_TCHAR(item->protocol_name ().c_str ())));

          delete item;

          if (transfer_ownership == 0)
            delete protocol_factory;

          return -1;
        }

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_LIB_TEXT("TAO (%P|%t) Loaded default ")
                      ACE_LIB_TEXT("protocol <SHMIOP_Factory>\n")));
        }
#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)
      protocol_factory =
        ACE_Dynamic_Service<TAO_Protocol_Factory>::instance ("DIOP_Factory");

      if (protocol_factory == 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_WARNING,
                        ACE_LIB_TEXT("(%P|%t) WARNING - No %s found in Service Repository.")
                        ACE_LIB_TEXT("  Using default instance.\n"),
                        ACE_LIB_TEXT("DIOP Protocol Factory")));

          ACE_NEW_RETURN (protocol_factory,
                          TAO_DIOP_Protocol_Factory,
                          -1);

          ACE_AUTO_PTR_RESET (safe_protocol_factory,
                              protocol_factory,
                              TAO_Protocol_Factory);

          transfer_ownership = 1;
        }
      else
        {
          transfer_ownership = 0;
        }

      ACE_NEW_RETURN (item, TAO_Protocol_Item ("DIOP_Factory"), -1);
      // If the TAO_Protocol_Item retains ownership of the
      // TAO_Protocol_Factory then we used an auto_ptr<> above, so
      // release the TAO_Protocol_Factory from it.  Otherwise, we
      // obtained the TAO_Protocol_Factory from the Service
      // Configurator so an auto_ptr<> wasn't used since the Service
      // Configurator retains ownership, hence there was no need to
      // use an auto_ptr<> in this method.
      item->factory ((transfer_ownership ?
                      safe_protocol_factory.release () :
                      protocol_factory),
                     transfer_ownership);

      if (this->protocol_factories_.insert (item) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT("TAO (%P|%t) Unable to add ")
                      ACE_LIB_TEXT("<%s> to protocol factory set.\n"),
                      ACE_TEXT_CHAR_TO_TCHAR(item->protocol_name ().c_str ())));

          delete item;

          if (transfer_ownership == 0)
            delete protocol_factory;

          return -1;
        }

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_LIB_TEXT("TAO (%P|%t) Loaded default ")
                      ACE_LIB_TEXT("protocol <DIOP_Factory>\n")));
        }
#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */

#if defined (TAO_HAS_SCIOP) && (TAO_HAS_SCIOP != 0)
      protocol_factory =
        ACE_Dynamic_Service<TAO_Protocol_Factory>::instance ("SCIOP_Factory");

      if (protocol_factory == 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_WARNING,
                        ACE_LIB_TEXT("(%P|%t) WARNING - No %s found in Service Repository.")
                        ACE_LIB_TEXT("  Using default instance.\n"),
                        ACE_LIB_TEXT("SCIOP Protocol Factory")));

          ACE_NEW_RETURN (protocol_factory,
                          TAO_SCIOP_Protocol_Factory,
                          -1);

          ACE_AUTO_PTR_RESET (safe_protocol_factory,
                              protocol_factory,
                              TAO_Protocol_Factory);

          transfer_ownership = 1;
        }
      else
        {
          transfer_ownership = 0;
        }

      ACE_NEW_RETURN (item, TAO_Protocol_Item ("SCIOP_Factory"), -1);
      // If the TAO_Protocol_Item retains ownership of the
      // TAO_Protocol_Factory then we used an auto_ptr<> above, so
      // release the TAO_Protocol_Factory from it.  Otherwise, we
      // obtained the TAO_Protocol_Factory from the Service
      // Configurator so an auto_ptr<> wasn't used since the Service
      // Configurator retains ownership, hence there was no need to
      // use an auto_ptr<> in this method.
      item->factory ((transfer_ownership ?
                      safe_protocol_factory.release () :
                      protocol_factory),
                     transfer_ownership);

      if (this->protocol_factories_.insert (item) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT("TAO (%P|%t) Unable to add ")
                      ACE_LIB_TEXT("<%s> to protocol factory set.\n"),
                      ACE_TEXT_CHAR_TO_TCHAR(item->protocol_name ().c_str ())));

          delete item;

          if (transfer_ownership == 0)
            delete protocol_factory;

          return -1;
        }

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_LIB_TEXT("TAO (%P|%t) Loaded default ")
                      ACE_LIB_TEXT("protocol <SCIOP_Factory>\n")));
        }
#endif /* TAO_HAS_SCIOP && TAO_HAS_SCIOP != 0 */

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
                             ACE_LIB_TEXT("TAO (%P|%t) Unable to load ")
                             ACE_LIB_TEXT("protocol <%s>, %p\n"),
                             ACE_TEXT_CHAR_TO_TCHAR(name.c_str ()), ""),
                            -1);
        }

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_LIB_TEXT("TAO (%P|%t) Loaded protocol <%s>\n"),
                      ACE_TEXT_CHAR_TO_TCHAR(name.c_str ())));
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
  switch (this->reactor_type_)
    {
    case TAO_REACTOR_SELECT_MT:
      ACE_NEW_RETURN (impl,
                      TAO_REACTOR ((ACE_Sig_Handler*)0,
                                   (ACE_Timer_Queue*)0,
                                   0,
                                   (ACE_Reactor_Notify*)0,
                                   this->reactor_mask_signals_),
                      0);
      break;

    case TAO_REACTOR_SELECT_ST:
      ACE_NEW_RETURN (impl,
                      TAO_NULL_LOCK_REACTOR ((ACE_Sig_Handler*)0,
                                             (ACE_Timer_Queue*)0,
                                             0,
                                             (ACE_Reactor_Notify*)0,
                                             this->reactor_mask_signals_),
                      0);
      break;

    case TAO_REACTOR_FL:
#if defined(ACE_HAS_FL)
      ACE_NEW_RETURN (impl, ACE_FlReactor, 0);
#endif /* ACE_HAS_FL */
      break;

    case TAO_REACTOR_TK:
#if defined(ACE_HAS_TK)
      ACE_NEW_RETURN (impl, ACE_TkReactor, 0);
#endif /* ACE_HAS_TK */
      break;

    case TAO_REACTOR_WFMO:
#if defined(ACE_WIN32) && !defined (ACE_LACKS_MSG_WFMO)
      ACE_NEW_RETURN (impl, ACE_WFMO_Reactor, 0);
#endif /* ACE_WIN32 && !ACE_LACKS_MSG_WFMO */
      break;

    case TAO_REACTOR_MSGWFMO:
#if defined(ACE_WIN32) && !defined (ACE_HAS_WINCE)
      ACE_NEW_RETURN (impl, ACE_Msg_WFMO_Reactor, 0);
#endif /* ACE_WIN32 && !ACE_HAS_WINCE */
      break;

    default:
    case TAO_REACTOR_TP:
      ACE_NEW_RETURN (impl, ACE_TP_Reactor ((ACE_Sig_Handler*)0,
                                            (ACE_Timer_Queue*)0,
                                            this->reactor_mask_signals_,
                                            this->threadqueue_type_ == TAO_THREAD_QUEUE_FIFO ?
                                              ACE_Select_Reactor_Token::FIFO :
                                              ACE_Select_Reactor_Token::LIFO),
                      0);
      break;
    }

  return impl;
}

typedef ACE_Malloc<ACE_LOCAL_MEMORY_POOL,ACE_Null_Mutex> NULL_LOCK_MALLOC;
typedef ACE_Allocator_Adapter<NULL_LOCK_MALLOC> NULL_LOCK_ALLOCATOR;

typedef ACE_Malloc<ACE_LOCAL_MEMORY_POOL,TAO_SYNCH_MUTEX> LOCKED_MALLOC;
typedef ACE_Allocator_Adapter<LOCKED_MALLOC> LOCKED_ALLOCATOR;

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
    case TAO_ALLOCATOR_THREAD_LOCK:
    default:
      ACE_NEW_RETURN (allocator,
                      LOCKED_ALLOCATOR,
                      0);
      break;
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
                  ACE_LIB_TEXT("TAO (%P|%t) - ")
                  ACE_LIB_TEXT("Unknown connection purging strategy ")
                  ACE_LIB_TEXT("type was found.\n")));
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
             ACE_LIB_TEXT("Advanced_Resource_Factory - unknown argument")
             ACE_LIB_TEXT(" <%s> for <%s>\n"),
             option_value,
             option_name));
}

void
TAO_Advanced_Resource_Factory::report_unsupported_error (
                                 const ACE_TCHAR* option_name)
{
  ACE_DEBUG((LM_DEBUG,
             ACE_LIB_TEXT("Advanced_Resource_Factory - <%s>")
             ACE_LIB_TEXT(" not supported on this platform\n"),
             option_name));
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Advanced_Resource_Factory,
                       ACE_LIB_TEXT("Advanced_Resource_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Advanced_Resource_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_Strategies, TAO_Advanced_Resource_Factory)

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Advanced_Resource_Factory>;

template class ACE_Select_Reactor_Token_T<ACE_Noop_Token>;
template class ACE_Lock_Adapter<ACE_Select_Reactor_Token_T<ACE_Noop_Token> >;
template class ACE_Select_Reactor_T< ACE_Select_Reactor_Token_T<ACE_Noop_Token> >;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Advanced_Resource_Factory>

#pragma instantiate ACE_Select_Reactor_Token_T<ACE_Noop_Token>
#pragma instantiate ACE_Lock_Adapter< ACE_Select_Reactor_Token_T<ACE_Noop_Token> >
#pragma instantiate ACE_Select_Reactor_T< ACE_Select_Reactor_Token_T<ACE_Noop_Token> >

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
