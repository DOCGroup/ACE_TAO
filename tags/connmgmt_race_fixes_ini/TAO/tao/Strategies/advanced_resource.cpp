// $Id$

#include "advanced_resource.h"

#if !defined (__ACE_INLINE__)
# include "advanced_resource.i"
#endif /* ! __ACE_INLINE__ */

#include "UIOP_Factory.h"
#include "SHMIOP_Factory.h"

#include "Reactor_Per_Priority.h"
#include "Direct_Priority_Mapping.h"
#include "Linear_Priority_Mapping.h"

#include "tao/debug.h"
#include "tao/Single_Reactor.h"

#include "ace/Auto_Ptr.h"
#include "ace/Dynamic_Service.h"
#include "ace/Service_Config.h"
#include "ace/Select_Reactor.h"
#include "ace/FlReactor.h"
#include "ace/TkReactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Msg_WFMO_Reactor.h"
#include "ace/TP_Reactor.h"

ACE_RCSID(Strategies, advanced_resource, "$Id$")

TAO_Resource_Factory_Changer::TAO_Resource_Factory_Changer (void)
{
  TAO_ORB_Core::set_resource_factory ("Advanced_Resource_Factory");

#if TAO_HAS_UIOP == 1
  ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_UIOP_Protocol_Factory);
#endif /* TAO_HAS_UIOP == 1 */

#if TAO_HAS_SHMIOP == 1
  ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_SHMIOP_Protocol_Factory);
#endif /* TAO_HAS_UIOP == 1 */
}

TAO_Advanced_Resource_Factory::TAO_Advanced_Resource_Factory (void)
  :sched_policy_ (ACE_SCHED_OTHER),
   reactor_registry_type_ (TAO_SINGLE_REACTOR),
   reactor_type_ (TAO_REACTOR_TP),
   priority_mapping_type_ (TAO_PRIORITY_MAPPING_DIRECT),
   cdr_allocator_type_ (TAO_ALLOCATOR_THREAD_LOCK)
{
  // Constructor
}

TAO_Advanced_Resource_Factory::~TAO_Advanced_Resource_Factory (void)
{
  // Destructor
}

int
TAO_Advanced_Resource_Factory::init (int argc, char **argv)
{
  ACE_TRACE ("TAO_Advanced_Resource_Factory::init");

  int curarg = 0;

  for (curarg = 0; curarg < argc; curarg++)
    if (ACE_OS::strcasecmp (argv[curarg],
                            "-ORBReactorRegistry") == 0)
      {

        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    "single") == 0)
              this->reactor_registry_type_ = TAO_SINGLE_REACTOR;
            else if (ACE_OS::strcasecmp (name,
                                         "per-priority") == 0)
              this->reactor_registry_type_ = TAO_REACTOR_PER_PRIORITY;
            else
              ACE_DEBUG((LM_DEBUG,
                         ACE_TEXT ("TAO_Default_Factory - unknown argument")
                         ACE_TEXT (" <%s> for -ORBReactorRegistry\n"), name));
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBReactorLock") == 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO_Default_Resource obsolete -ORBReactorLock ")
                    ACE_TEXT ("option, please use -ORBReactorType\n")));
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "null") == 0)
              reactor_type_ = TAO_REACTOR_SELECT_ST;
            else if (ACE_OS::strcasecmp (name, "token") == 0)
              reactor_type_= TAO_REACTOR_SELECT_MT;
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBReactorType") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    "select_mt") == 0)
              reactor_type_ = TAO_REACTOR_SELECT_MT;
            else if (ACE_OS::strcasecmp (name,
                                         "select_st") == 0)
              reactor_type_ = TAO_REACTOR_SELECT_ST;
            else if (ACE_OS::strcasecmp (name,
                                         "fl") == 0)
#if defined(ACE_HAS_FL)
              reactor_type_ = TAO_REACTOR_FL;
#else
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO_Advanced_Factory - FlReactor")
                          ACE_TEXT (" not supported on this platform\n")));
#endif /* ACE_HAS_FL */
            else if (ACE_OS::strcasecmp (name, "tk_reactor") == 0)
#if defined(ACE_HAS_TK)
              reactor_type_ = TAO_REACTOR_TK;
#else
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO_Advanced_Factory - TkReactor")
                          ACE_TEXT (" not supported on this platform\n")));
#endif /* ACE_HAS_TK */
            else if (ACE_OS::strcasecmp (name,
                                         "wfmo") == 0)
#if defined(ACE_WIN32)
              reactor_type_ = TAO_REACTOR_WFMO;
#else
              ACE_DEBUG ((LM_DEBUG,
                          "TAO_Advanced_Factory - WFMO Reactor"
                          " not supported on this platform\n"));
#endif /* ACE_WIN32 */
            else if (ACE_OS::strcasecmp (name,
                                         "msg_wfmo") == 0)
#if defined(ACE_WIN32)
              reactor_type_ = TAO_REACTOR_MSGWFMO;
#else
              ACE_DEBUG ((LM_DEBUG,
                          "TAO_Advanced_Factory - MsgWFMO Reactor"
                          " not supported on this platform\n"));
#endif /* ACE_WIN32 */

            else if (ACE_OS::strcasecmp (name,
                                         "tp") == 0)
              reactor_type_ = TAO_REACTOR_TP;
            else
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO_Advanced_Factory - unknown argument")
                          ACE_TEXT (" <%s> for -ORBreactortype\n"), name));
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBInputCDRAllocator") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    "null") == 0)
              {
                this->cdr_allocator_type_ = TAO_ALLOCATOR_NULL_LOCK;
                this->use_locked_data_blocks_ = 0;
              }
            else if (ACE_OS::strcasecmp (name,
                                         "thread") == 0)
              {
                this->cdr_allocator_type_ = TAO_ALLOCATOR_THREAD_LOCK;
                this->use_locked_data_blocks_ = 1;
              }
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBSchedPolicy") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    "SCHED_OTHER") == 0)
              this->sched_policy_ = ACE_SCHED_OTHER;
            else if (ACE_OS::strcasecmp (name,
                                         "SCHED_FIFO") == 0)
              this->sched_policy_ = ACE_SCHED_FIFO;
            else if (ACE_OS::strcasecmp (name,
                                         "SCHED_RR") == 0)
              this->sched_policy_ = ACE_SCHED_RR;
            else
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO_Default_Factory - unknown argument")
                          ACE_TEXT (" <%s> for -ORBSchedPolicy\n"), name));
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBPriorityMapping") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    "linear") == 0)
              this->priority_mapping_type_ = TAO_PRIORITY_MAPPING_LINEAR;
            else if (ACE_OS::strcasecmp (name,
                                         "direct") == 0)
              this->priority_mapping_type_ = TAO_PRIORITY_MAPPING_DIRECT;
            else
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO_Default_Factory - unknown argument")
                          ACE_TEXT (" <%s> for -ORBPriorityMapping\n"), name));
          }
      }

  this->TAO_Default_Resource_Factory::init (argc, argv);
  return 0;
}

int
TAO_Advanced_Resource_Factory::load_default_protocols (void)
{
  int r = this->TAO_Default_Resource_Factory::load_default_protocols ();

  this->protocol_factories_ =
    this->TAO_Default_Resource_Factory::protocol_factories_;

  if (r == -1)
    return -1;
  // Load the UIOP and SHMIOP protocols...

  return 0;
}

int
TAO_Advanced_Resource_Factory::init_protocol_factories (void)
{
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
          if (TAO_orbdebug)
            ACE_ERROR ((LM_WARNING,
                        "(%P|%t) WARNING - No %s found in Service Repository."
                        "  Using default instance.\n",
                        "UIOP Protocol Factory"));

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
                      ACE_TEXT ("TAO (%P|%t) Unable to add ")
                      ACE_TEXT ("<%s> to protocol factory set.\n"),
                      item->protocol_name ().c_str ()));

          delete item;

          if (transfer_ownership == 0)
            delete protocol_factory;

          return -1;
        }

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) Loaded default ")
                      ACE_TEXT ("protocol <UIOP_Factory>\n")));
        }
#endif /* TAO_HAS_UIOP == 1 */

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)
      protocol_factory =
        ACE_Dynamic_Service<TAO_Protocol_Factory>::instance ("SHMIOP_Factory");

      if (protocol_factory == 0)
        {
          if (TAO_orbdebug)
            ACE_ERROR ((LM_WARNING,
                        "(%P|%t) WARNING - No %s found in Service Repository."
                        "  Using default instance.\n",
                        "SHMIOP Protocol Factory"));

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
                      ACE_TEXT ("TAO (%P|%t) Unable to add ")
                      ACE_TEXT ("<%s> to protocol factory set.\n"),
                      item->protocol_name ().c_str ()));

          delete item;

          if (transfer_ownership == 0)
            delete protocol_factory;

          return -1;
        }

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) Loaded default ")
                      ACE_TEXT ("protocol <SHMIOP_Factory>\n")));
        }
#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */

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
                             ACE_TEXT ("TAO (%P|%t) Unable to load ")
                             ACE_TEXT ("protocol <%s>, %p\n"),
                             name.c_str (), ""),
                            -1);
        }

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) Loaded protocol <%s>\n"),
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

TAO_Reactor_Registry *
TAO_Advanced_Resource_Factory::get_reactor_registry (void)
{
  TAO_Reactor_Registry *reactor_registry = 0;
  switch (this->reactor_registry_type_)
    {
    default:
    case TAO_SINGLE_REACTOR:
      ACE_NEW_RETURN (reactor_registry,
                      TAO_Single_Reactor,
                      0);
      break;

    case TAO_REACTOR_PER_PRIORITY:
      ACE_NEW_RETURN (reactor_registry,
                      TAO_Reactor_Per_Priority,
                      0);
      break;

    }

  return reactor_registry;
}

ACE_Reactor_Impl*
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
                                            this->reactor_mask_signals_),
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

TAO_Priority_Mapping *
TAO_Advanced_Resource_Factory::get_priority_mapping (void)
{
#if (TAO_HAS_RT_CORBA == 0)
  return 0;
#else
  TAO_Priority_Mapping *pm;
  switch (this->priority_mapping_type_)
    {
    case TAO_PRIORITY_MAPPING_LINEAR:
      ACE_NEW_RETURN (pm,
                      TAO_Linear_Priority_Mapping (this->sched_policy_),
                      0);
      break;
    case TAO_PRIORITY_MAPPING_DIRECT:
      ACE_NEW_RETURN (pm,
                      TAO_Direct_Priority_Mapping (this->sched_policy_),
                      0);
      break;
    default:
      ACE_NEW_RETURN (pm,
                      TAO_Priority_Mapping,
                      0);
      break;
    }
  return pm;
#endif /* TAO_HAS_RT_CORBA == 0 */
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Advanced_Resource_Factory,
                       ACE_TEXT ("Advanced_Resource_Factory"),
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
