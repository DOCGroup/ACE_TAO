// $Id$

#include "tao/default_resource.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/IIOP_Factory.h"
#include "tao/UIOP_Factory.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Connector_Registry.h"

#include "ace/Select_Reactor.h"
#include "ace/XtReactor.h"
#include "ace/FlReactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Msg_WFMO_Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/Dynamic_Service.h"
#include "ace/Arg_Shifter.h"

#if !defined (__ACE_INLINE__)
# include "tao/default_resource.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, default_resource, "$Id$")

TAO_Default_Resource_Factory::TAO_Default_Resource_Factory (void)
  : use_tss_resources_ (0),
    reactor_type_ (TAO_REACTOR_SELECT_MT),
    cdr_allocator_source_ (TAO_GLOBAL)
{
}

TAO_Default_Resource_Factory::~TAO_Default_Resource_Factory (void)
{
  TAO_ProtocolFactorySetItor end = this->protocol_factories_.end ();

  for (TAO_ProtocolFactorySetItor iterator =
         this->protocol_factories_.begin ();
       iterator != end;
       ++iterator)
    delete *iterator;

  this->protocol_factories_.reset ();
}

int
TAO_Default_Resource_Factory::init (int argc, char **argv)
{
  ACE_TRACE ("TAO_Default_Server_Strategy_Factory::parse_args");

  for (int curarg = 0; curarg < argc; curarg++)
    if (ACE_OS::strcasecmp (argv[curarg],
                            "-ORBResources") == 0)
      {
        ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) The -ORBResources option "
                    "has been moved to the ORB parameters\n"));
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    "global") == 0)
              this->use_tss_resources_ = 0;
            else if (ACE_OS::strcasecmp (name,
                                         "tss") == 0)
              this->use_tss_resources_ = 1;
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBReactorLock") == 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Default_Resource obsolete -ORBReactorLock "
                    "option, please use -ORBReactorType\n"));
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
                                         "fl_reactor") == 0)
#if defined(ACE_HAS_FL)
              reactor_type_ = TAO_REACTOR_FL;
#else
              ACE_DEBUG ((LM_DEBUG,
                          "TAO_Default_Factory - FlReactor"
                          " not supported on this platform\n"));
#endif /* ACE_HAS_FL */
            else if (ACE_OS::strcasecmp (name,
                                         "xt_reactor") == 0)
#if defined(ACE_HAS_XT)
              reactor_type_ = TAO_REACTOR_XT;
#else
              ACE_DEBUG ((LM_DEBUG,
                          "TAO_Default_Factory - XtReactor"
                          " not supported on this platform\n"));
#endif /* ACE_HAS_XT */
            else if (ACE_OS::strcasecmp (name,
                                         "WFMO") == 0)
#if defined(ACE_WIN32)
              reactor_type_ = TAO_REACTOR_WFMO;
#else
              ACE_DEBUG ((LM_DEBUG,
                          "TAO_Default_Factory - WFMO Reactor"
                          " not supported on this platform\n"));
#endif /* ACE_WIN32 */
            else if (ACE_OS::strcasecmp (name,
                                         "MsgWFMO") == 0)
#if defined(ACE_WIN32)
              reactor_type_ = TAO_REACTOR_MSGWFMO;
#else
              ACE_DEBUG ((LM_DEBUG,
                          "TAO_Default_Factory - MsgWFMO Reactor"
                          " not supported on this platform\n"));
#endif /* ACE_WIN32 */

            else if (ACE_OS::strcasecmp (name,
                                         "ThreadPool") == 0)
              reactor_type_ = TAO_REACTOR_TP;
            else
              ACE_DEBUG ((LM_DEBUG,
                          "TAO_Default_Factory - unknown argument"
                          " <%s> for -ORBreactortype\n", name));
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
                                    "global") == 0)
              this->cdr_allocator_source_ = TAO_GLOBAL;
            else if (ACE_OS::strcasecmp (name,
                                         "tss") == 0)
              this->cdr_allocator_source_ = TAO_TSS;
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBProtocolFactory") == 0)
      {
        TAO_ProtocolFactorySet *pset = this->get_protocol_factories ();
        curarg++;
        if (curarg < argc)
          {
            TAO_Protocol_Item *item;
            ACE_NEW_RETURN (item,
                            TAO_Protocol_Item (argv[curarg]),
                            -1);
            if (pset->insert (item) == -1)
              ACE_ERROR ((LM_ERROR,
                          "(%P|%t) Unable to add protocol factories for %s: %p\n",
                          argv[curarg]));
          }
      }

  return 0;
}

int
TAO_Default_Resource_Factory::init_protocol_factories (void)
{
  TAO_ProtocolFactorySetItor end = protocol_factories_.end ();
  TAO_ProtocolFactorySetItor factory = protocol_factories_.begin ();

  // @@ Ossama, if you want to be very paranoid, you could get memory
  // leak if insert operations failed.

  if (factory == end)
    {
      TAO_Protocol_Factory *protocol_factory = 0;
      TAO_Protocol_Item *item = 0;

      protocol_factory =
        ACE_Dynamic_Service<TAO_Protocol_Factory>::instance ("IIOP_Factory");

      if (protocol_factory == 0)
        {
          if (TAO_orbdebug)
            ACE_ERROR ((LM_WARNING,
                        "(%P|%t) WARNING - No %s found in Service Repository."
                        "  Using default instance.\n",
                        "IIOP Protocol Factory"));

          ACE_NEW_RETURN (protocol_factory,
                          TAO_IIOP_Protocol_Factory,
                          -1);
        }

      ACE_NEW_RETURN (item, TAO_Protocol_Item ("IIOP_Factory"), -1);
      item->factory (protocol_factory);

      this->protocol_factories_.insert (item);
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) Loaded default protocol <IIOP_Factory>\n"));
        }

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)
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
        }

      ACE_NEW_RETURN (item, TAO_Protocol_Item ("UIOP_Factory"), -1);
      item->factory (protocol_factory);

      this->protocol_factories_.insert (item);
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) Loaded default protocol <UIOP_Factory>\n"));
        }
#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
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
                             "TAO (%P|%t) Unable to load protocol <%s>, %p\n",
                             name.c_str (), ""),
                            -1);
        }

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) Loaded protocol <%s>\n",
                      name.c_str ()));
        }
    }
  return 0;
}

int
TAO_Default_Resource_Factory::use_tss_resources (void) const
{
  return this->use_tss_resources_;
}

TAO_ProtocolFactorySet *
TAO_Default_Resource_Factory::get_protocol_factories (void)
{
  return &protocol_factories_;
}

TAO_Acceptor_Registry*
TAO_Default_Resource_Factory::get_acceptor_registry (void)
{
  return new TAO_Acceptor_Registry;
}

TAO_Connector_Registry*
TAO_Default_Resource_Factory::get_connector_registry (void)
{
  return new TAO_Connector_Registry;
}

// @@ TODO We may be changing the state of the global
//    Allocated_Resources structure, but without any locks?
//    It seems to be done all over the place.

ACE_Reactor_Impl*
TAO_Default_Resource_Factory::allocate_reactor_impl (void) const
{
  ACE_Reactor_Impl *impl = 0;
  switch (this->reactor_type_)
    {
    default:
    case TAO_REACTOR_SELECT_MT:
      ACE_NEW_RETURN (impl, TAO_REACTOR, 0);
      break;

    case TAO_REACTOR_SELECT_ST:
      ACE_NEW_RETURN (impl, TAO_NULL_LOCK_REACTOR, 0);
      break;

    case TAO_REACTOR_FL:
#if defined(ACE_HAS_FL)
      ACE_NEW_RETURN (impl, ACE_FlReactor, 0);
#endif /* ACE_HAS_FL */
      break;

    case TAO_REACTOR_XT:
#if defined(ACE_HAS_XT)
      ACE_NEW_RETURN (impl, ACE_XtReactor, 0);
#endif /* ACE_HAS_FL */
      break;

    case TAO_REACTOR_WFMO:
#if defined(ACE_WIN32) && !defined (ACE_HAS_WINCE)
      ACE_NEW_RETURN (impl, ACE_WFMO_Reactor, 0);
#endif /* ACE_WIN32 && !ACE_HAS_WINCE */
      break;

    case TAO_REACTOR_MSGWFMO:
#if defined(ACE_WIN32)
      ACE_NEW_RETURN (impl, ACE_Msg_WFMO_Reactor, 0);
#endif /* ACE_WIN32 && !ACE_HAS_WINCE */
      break;

    case TAO_REACTOR_TP:
      ACE_NEW_RETURN (impl, ACE_TP_Reactor, 0);
      break;
    }
  return impl;
}

ACE_Reactor *
TAO_Default_Resource_Factory::get_reactor (void)
{
  ACE_Reactor *reactor;
  ACE_NEW_RETURN (reactor,
                  ACE_Reactor (this->allocate_reactor_impl (), 1),
                  0);
  return reactor;
}

typedef ACE_Malloc<ACE_LOCAL_MEMORY_POOL,ACE_Null_Mutex> TSS_MALLOC;
typedef ACE_Allocator_Adapter<TSS_MALLOC> TSS_ALLOCATOR;

typedef ACE_Malloc<ACE_LOCAL_MEMORY_POOL,ACE_SYNCH_MUTEX> GBL_MALLOC;
typedef ACE_Allocator_Adapter<GBL_MALLOC> GBL_ALLOCATOR;

ACE_Allocator*
TAO_Default_Resource_Factory::input_cdr_dblock_allocator (void)
{
  switch (this->cdr_allocator_source_)
    {
    case TAO_GLOBAL:
      if (GLOBAL_ALLOCATED::instance ()->input_cdr_dblock_allocator_ == 0)
        {
          ACE_NEW_RETURN (GLOBAL_ALLOCATED::instance ()->input_cdr_dblock_allocator_,
                          GBL_ALLOCATOR,
                          0);
        }
      return GLOBAL_ALLOCATED::instance ()->input_cdr_dblock_allocator_;
      ACE_NOTREACHED (break);
    case TAO_TSS:
      if (TSS_ALLOCATED::instance ()->input_cdr_dblock_allocator_ == 0)
        {
          ACE_NEW_RETURN (TSS_ALLOCATED::instance ()->input_cdr_dblock_allocator_,
                          TSS_ALLOCATOR,
                          0);
        }
      return TSS_ALLOCATED::instance ()->input_cdr_dblock_allocator_;
      ACE_NOTREACHED (break);
    }
  return 0;
}

ACE_Allocator *
TAO_Default_Resource_Factory::input_cdr_buffer_allocator (void)
{
  switch (this->cdr_allocator_source_)
    {
    case TAO_GLOBAL:
      if (GLOBAL_ALLOCATED::instance ()->input_cdr_buffer_allocator_ == 0)
        {
          ACE_NEW_RETURN (GLOBAL_ALLOCATED::instance ()->input_cdr_buffer_allocator_,
                          GBL_ALLOCATOR,
                          0);
        }
      return GLOBAL_ALLOCATED::instance ()->input_cdr_buffer_allocator_;
    case TAO_TSS:
      if (TSS_ALLOCATED::instance ()->input_cdr_buffer_allocator_ == 0)
        {
          ACE_NEW_RETURN (TSS_ALLOCATED::instance ()->input_cdr_buffer_allocator_,
                          TSS_ALLOCATOR,
                          0);
        }
      return TSS_ALLOCATED::instance ()->input_cdr_buffer_allocator_;
    }
  return 0;
}

ACE_Allocator*
TAO_Default_Resource_Factory::output_cdr_dblock_allocator (void)
{
  ACE_Allocator *allocator;
  ACE_NEW_RETURN (allocator, TSS_ALLOCATOR, 0);
  return allocator;
}

ACE_Allocator *
TAO_Default_Resource_Factory::output_cdr_buffer_allocator (void)
{
  ACE_Allocator *allocator;
  ACE_NEW_RETURN (allocator, TSS_ALLOCATOR, 0);
  return allocator;
}

ACE_Data_Block*
TAO_Default_Resource_Factory::create_input_cdr_data_block (size_t size)
{
  switch (this->cdr_allocator_source_)
    {
    case TAO_GLOBAL:
      {
        ACE_Allocator* buffer_alloc =
          this->input_cdr_buffer_allocator ();
        ACE_Allocator* dblock_alloc =
          this->input_cdr_dblock_allocator ();

        typedef
          ACE_Locked_Data_Block<ACE_Lock_Adapter<ACE_SYNCH_MUTEX> >
          Global_Data_Block;
        Global_Data_Block *nb;

        ACE_NEW_MALLOC_RETURN (
            nb,
            ACE_static_cast(Global_Data_Block*,
                            dblock_alloc->malloc (sizeof (Global_Data_Block))),
            Global_Data_Block (size,
                               ACE_Message_Block::MB_DATA,
                               0,
                               buffer_alloc,
                               0,
                               dblock_alloc),
            0);
        return nb;
      }

    case TAO_TSS:
      {
        ACE_Allocator* buffer_alloc =
          this->input_cdr_buffer_allocator ();
        ACE_Allocator* dblock_alloc =
          this->input_cdr_dblock_allocator ();

        ACE_Data_Block *nb;

        ACE_NEW_MALLOC_RETURN (
            nb,
            ACE_static_cast(ACE_Data_Block*,
                            dblock_alloc->malloc (sizeof (ACE_Data_Block))),
            ACE_Data_Block (size,
                            ACE_Message_Block::MB_DATA,
                            0,
                            buffer_alloc,
                            0,
                            0,
                            dblock_alloc),
            0);
        return nb;
      }
    }
  return 0;
}

// ****************************************************************

TAO_Allocated_Resources::TAO_Allocated_Resources (void)
  : input_cdr_dblock_allocator_ (0),
    input_cdr_buffer_allocator_ (0)
{
}

TAO_Allocated_Resources::~TAO_Allocated_Resources (void)
{
  if (this->input_cdr_dblock_allocator_ != 0)
    this->input_cdr_dblock_allocator_->remove ();
  delete this->input_cdr_dblock_allocator_;

  if (this->input_cdr_buffer_allocator_ != 0)
    this->input_cdr_buffer_allocator_->remove ();
  delete this->input_cdr_buffer_allocator_;
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Default_Resource_Factory,
                       ASYS_TEXT ("Resource_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Resource_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Resource_Factory)

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Malloc<ACE_LOCAL_MEMORY_POOL,ACE_SYNCH_MUTEX>;
template class ACE_Allocator_Adapter<ACE_Malloc<ACE_LOCAL_MEMORY_POOL,ACE_SYNCH_MUTEX> >;
template class ACE_Locked_Data_Block<ACE_Lock_Adapter<ACE_SYNCH_MUTEX> >;

template class ACE_Singleton<TAO_Allocated_Resources, ACE_SYNCH_MUTEX>;
template class ACE_TSS_Singleton<TAO_Allocated_Resources, ACE_SYNCH_MUTEX>;
template class ACE_TSS<TAO_Allocated_Resources>;

template class ACE_Select_Reactor_Token_T<ACE_Noop_Token>;
template class ACE_Lock_Adapter<ACE_Select_Reactor_Token_T<ACE_Noop_Token> >;
template class ACE_Select_Reactor_T< ACE_Select_Reactor_Token_T<ACE_Noop_Token> >;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Malloc<ACE_LOCAL_MEMORY_POOL,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Allocator_Adapter<ACE_Malloc<ACE_LOCAL_MEMORY_POOL,ACE_SYNCH_MUTEX> >
#pragma instantiate ACE_Locked_Data_Block<ACE_Lock_Adapter<ACE_SYNCH_MUTEX> >

#pragma instantiate ACE_Singleton<TAO_Allocated_Resources, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_TSS_Singleton<TAO_Allocated_Resources, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_TSS<TAO_Allocated_Resources>

#pragma instantiate ACE_Select_Reactor_Token_T<ACE_Noop_Token>
#pragma instantiate ACE_Lock_Adapter< ACE_Select_Reactor_Token_T<ACE_Noop_Token> >
#pragma instantiate ACE_Select_Reactor_T< ACE_Select_Reactor_Token_T<ACE_Noop_Token> >

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
