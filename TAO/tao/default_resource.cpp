// $Id$

#include "tao/default_resource.h"
#include "ace/Select_Reactor.h"
#include "ace/XtReactor.h"
#include "ace/FlReactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Msg_WFMO_Reactor.h"
#include "ace/Arg_Shifter.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/ORB_Core.h"

#if !defined (__ACE_INLINE__)
# include "tao/default_resource.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, default_resource, "$Id$")

TAO_Default_Resource_Factory::TAO_Default_Resource_Factory (void)
  : resource_source_ (TAO_GLOBAL),
    poa_source_ (TAO_GLOBAL),
    reactor_type_ (TAO_REACTOR_SELECT_MT),
    cdr_allocator_source_ (TAO_GLOBAL)
{
}

TAO_Default_Resource_Factory::~TAO_Default_Resource_Factory (void)
{
}

int
TAO_Default_Resource_Factory::init (int argc, char **argv)
{
  ACE_TRACE ("TAO_Default_Server_Strategy_Factory::parse_args");
  // This table shows the arguments that are parsed with their valid
  // combinations.
  //
  //   ORB      POA    comments
  // +-------+-------+-----------------+
  // | TSS   | TSS   | if ORB==TSS     |
  // |       |       | then POA=TSS    |
  // |       |       | as def.value.   |
  // +-------+-------+-----------------+
  // | TSS   | GLOBAL| ok.             |
  // +-------+-------+-----------------+
  // | GLOBAL| GLOBAL| if ORB==Global  |
  // |       |       | then POA=Global |
  // |       |       | as def.value.   |
  // +-------+-------+-----------------+
  // | GLOBAL| TSS   | *NOT VALID*     |
  // +-------+-------+-----------------+

  int local_poa_source      = -1;
  int local_resource_source = -1;

  for (int curarg = 0; curarg < argc; curarg++)
    if (ACE_OS::strcmp (argv[curarg], "-ORBresources") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "global") == 0)
              local_resource_source = TAO_GLOBAL;
            else if (ACE_OS::strcasecmp (name, "tss") == 0)
              local_resource_source = TAO_TSS;
          }
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBpoa") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "global") == 0)
              local_poa_source = TAO_GLOBAL;
            else if (ACE_OS::strcasecmp (name, "tss") == 0)
              local_poa_source = TAO_TSS;
          }
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBreactorlock") == 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Default_Resource obsolete -ORBreactorlock "
                    "option, please use -ORBreactortype\n"));
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "null") == 0)
              reactor_type_ = TAO_REACTOR_SELECT_MT;
            else if (ACE_OS::strcasecmp (name, "token") == 0)
              reactor_type_= TAO_REACTOR_SELECT_ST;
          }
      }

    else if (ACE_OS::strcmp (argv[curarg], "-ORBreactortype") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "select_mt") == 0)
              reactor_type_ = TAO_REACTOR_SELECT_MT;
            else if (ACE_OS::strcasecmp (name, "select_st") == 0)
              reactor_type_ = TAO_REACTOR_SELECT_ST;
            else if (ACE_OS::strcasecmp (name, "fl_reactor") == 0)
#if defined(ACE_HAS_FL)
              reactor_type_ = TAO_REACTOR_FL;
#else
              ACE_DEBUG ((LM_DEBUG,
                          "TAO_Default_Factory - FlReactor"
                          " not supported on this platform\n"));
#endif /* ACE_HAS_FL */
            else if (ACE_OS::strcasecmp (name, "xt_reactor") == 0)
#if defined(ACE_HAS_XT)
              reactor_type_ = TAO_REACTOR_XT;
#else
              ACE_DEBUG ((LM_DEBUG,
                          "TAO_Default_Factory - XtReactor"
                          " not supported on this platform\n"));
#endif /* ACE_HAS_XT */
            else if (ACE_OS::strcasecmp (name, "WFMO") == 0)
#if defined(ACE_WIN32)
              reactor_type_ = TAO_REACTOR_WFMO;
#else
              ACE_DEBUG ((LM_DEBUG,
                          "TAO_Default_Factory - WFMO Reactor"
                          " not supported on this platform\n"));
#endif /* ACE_WIN32 */
            else if (ACE_OS::strcasecmp (name, "MsgWFMO") == 0)
#if defined(ACE_WIN32)
              reactor_type_ = TAO_REACTOR_MSGWFMO;
#else
              ACE_DEBUG ((LM_DEBUG,
                          "TAO_Default_Factory - MsgWFMO Reactor"
                          " not supported on this platform\n"));
#endif /* ACE_WIN32 */
            else
              ACE_DEBUG ((LM_DEBUG,
                          "TAO_Default_Factory - unknown argument"
                          " <%s> for -ORBreactorytype\n", name));
          }
      }

    else if (ACE_OS::strcmp (argv[curarg], "-ORBinputcdrallocator") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "global") == 0)
              this->cdr_allocator_source_ = TAO_GLOBAL;
            else if (ACE_OS::strcasecmp (name, "tss") == 0)
              this->cdr_allocator_source_ = TAO_TSS;
          }
      }
    else if (ACE_OS::strcmp (argv[curarg], "-ORBprotocolfactory") == 0)
      {
        TAO_ProtocolFactorySet *pset = this->get_protocol_factories ();
        curarg++;
        if (curarg < argc)
          {
            TAO_Protocol_Item *item = new TAO_Protocol_Item (argv[curarg]);
            if (pset->insert (item) == -1)
              {
                ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Unable to add protocol factories for %s: %p\n", argv[curarg]));
              }
          }
      }

  // Don't allow a global ORB and a tss POA.
  if ( (local_resource_source == TAO_GLOBAL) &&
       (local_poa_source == TAO_TSS) )
    return -1;

  // make poa=tss the default, if ORB is tss and the user didn't
  // specify a value.
  if ( (local_resource_source == TAO_TSS) &&
       (local_poa_source      == -1) )
    local_poa_source = TAO_TSS;

  // update the object data members.
  if (local_resource_source != -1)
    this->resource_source_ = local_resource_source;
  if (local_poa_source != -1)
    this->poa_source_      = local_poa_source;

  // Don't allow a global ORB and a tss POA.
  if ( (this->resource_source_ == TAO_GLOBAL) &&
       (this->poa_source_      == TAO_TSS) )
    return -1;

  return 0;
}

int
TAO_Default_Resource_Factory::init_protocol_factories (void)
{
  TAO_ProtocolFactorySetItor end = protocol_factories_.end ();
  TAO_ProtocolFactorySetItor factory = protocol_factories_.begin ();

  if (factory == end)
    {
      TAO_Protocol_Item *item =
        new TAO_Protocol_Item ("IIOP_Factory");
      item->factory (
          ACE_Dynamic_Service<TAO_Protocol_Factory>::instance ("IIOP_Factory"));
      this->protocol_factories_.insert (item);
      return 0;
    }

  for ( ; factory != end ; factory++)
    {
      const ACE_CString name = (*factory)->protocol_name ();
      (*factory)->factory (
        ACE_Dynamic_Service<TAO_Protocol_Factory>::instance (name.c_str ()));
      if ((*factory)->factory () == 0)
        {
              ACE_ERROR_RETURN ((LM_ERROR,
                                "(%P|%t) Unable to load protocol %s\n",
                                name.c_str ()),
                                -1);

        }
    }
  return 0;
}

TAO_ProtocolFactorySet *
TAO_Default_Resource_Factory::get_protocol_factories (void)
{
  return &protocol_factories_;
}

#define IMPLEMENT_GET_METHOD(methodname,rtype,membername)\
rtype TAO_Default_Resource_Factory::methodname(void)\
{\
  switch (resource_source_)\
    {\
    case TAO_GLOBAL:\
      return &GLOBAL_ALLOCATED::instance ()->membername;\
    case TAO_TSS:\
      return &TSS_ALLOCATED::instance ()->membername;\
    }\
  return 0;\
}

IMPLEMENT_GET_METHOD(get_thr_mgr, ACE_Thread_Manager *, tm_)
IMPLEMENT_GET_METHOD(get_acceptor_registry, TAO_Acceptor_Registry *, ar_)
IMPLEMENT_GET_METHOD(get_connector_registry, TAO_Connector_Registry *, cr_)
IMPLEMENT_GET_METHOD(get_null_creation_strategy, TAO_NULL_CREATION_STRATEGY *, null_creation_strategy_)
IMPLEMENT_GET_METHOD(get_null_activation_strategy, TAO_NULL_ACTIVATION_STRATEGY *, null_activation_strategy_)

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
    }
  return impl;
}

ACE_Reactor *
TAO_Default_Resource_Factory::get_reactor (void)
{
  switch (this->resource_source_)
    {
    case TAO_GLOBAL:
      if (GLOBAL_ALLOCATED::instance ()->r_ == 0)
        {
          ACE_NEW_RETURN (GLOBAL_ALLOCATED::instance ()->r_,
                          ACE_Reactor (this->allocate_reactor_impl ()),
                          0);
        }
      return GLOBAL_ALLOCATED::instance ()->r_;
      ACE_NOTREACHED (break);
    case TAO_TSS:
      if (TSS_ALLOCATED::instance ()->r_ == 0)
        {
          ACE_NEW_RETURN (TSS_ALLOCATED::instance ()->r_,
                          ACE_Reactor (this->allocate_reactor_impl ()),
                          0);
        }
      return TSS_ALLOCATED::instance ()->r_;
      ACE_NOTREACHED (break);
    }
  return 0;
}

TAO_Object_Adapter *
TAO_Default_Resource_Factory::object_adapter (void)
{
  // @@ Remove this use of ORB_Core_instance() from here!!!!
  switch (this->resource_source_)
    {
    case TAO_GLOBAL:
      if (GLOBAL_ALLOCATED::instance ()->object_adapter_ == 0)
        {
          TAO_ORB_Core &orb_core = *TAO_ORB_Core_instance ();
          ACE_NEW_RETURN (GLOBAL_ALLOCATED::instance ()->object_adapter_,
                          TAO_Object_Adapter (orb_core.server_factory ()->active_object_map_creation_parameters (), orb_core),
                          0);
        }
      return GLOBAL_ALLOCATED::instance ()->object_adapter_;
      ACE_NOTREACHED (break);
    case TAO_TSS:
      if (TSS_ALLOCATED::instance ()->object_adapter_ == 0)
        {
          TAO_ORB_Core &orb_core = *TAO_ORB_Core_instance ();
          ACE_NEW_RETURN (TSS_ALLOCATED::instance ()->object_adapter_,
                          TAO_Object_Adapter (orb_core.server_factory ()->active_object_map_creation_parameters (), orb_core),
                          0);
        }
      return TSS_ALLOCATED::instance ()->object_adapter_;
      ACE_NOTREACHED (break);
    }
  return 0;
}

TAO_CACHED_CONNECT_STRATEGY *
TAO_Default_Resource_Factory::get_cached_connect_strategy (void)
{
  // @@ Remove this use of ORB_Core_instance() from here!!!!
  switch (this->resource_source_)
    {
    case TAO_GLOBAL:
      if (GLOBAL_ALLOCATED::instance ()->cached_connect_strategy_ == 0)
        {
          ACE_NEW_RETURN (GLOBAL_ALLOCATED::instance ()->cached_connect_strategy_,
                          TAO_CACHED_CONNECT_STRATEGY (TAO_ORB_Core_instance ()->client_factory ()->create_client_creation_strategy ()),
                          0);
        }
      return GLOBAL_ALLOCATED::instance ()->cached_connect_strategy_;
      ACE_NOTREACHED (break);
    case TAO_TSS:
      if (TSS_ALLOCATED::instance ()->cached_connect_strategy_ == 0)
        {
          ACE_NEW_RETURN (TSS_ALLOCATED::instance ()->cached_connect_strategy_,
                          TAO_CACHED_CONNECT_STRATEGY (TAO_ORB_Core_instance ()->client_factory ()->create_client_creation_strategy ()),
                          0);
        }
      return TSS_ALLOCATED::instance ()->cached_connect_strategy_;
      ACE_NOTREACHED (break);
    }
  return 0;
}

TAO_POA *
TAO_Default_Resource_Factory::get_root_poa (void)
{
  switch (poa_source_)
    {
    case TAO_GLOBAL:
      return GLOBAL_ALLOCATED::instance ()->poa_;
    case TAO_TSS:
      return TSS_ALLOCATED::instance ()->poa_;
    }
  return 0;
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
  if (TSS_ALLOCATED::instance ()->output_cdr_dblock_allocator_ == 0)
    {
      ACE_NEW_RETURN (TSS_ALLOCATED::instance ()->output_cdr_dblock_allocator_,
                      TSS_ALLOCATOR,
                      0);
    }
  return TSS_ALLOCATED::instance ()->output_cdr_dblock_allocator_;
}

ACE_Allocator *
TAO_Default_Resource_Factory::output_cdr_buffer_allocator (void)
{
  if (TSS_ALLOCATED::instance ()->output_cdr_buffer_allocator_ == 0)
    {
      ACE_NEW_RETURN (TSS_ALLOCATED::instance ()->output_cdr_buffer_allocator_,
                      TSS_ALLOCATOR,
                      0);
    }
  return TSS_ALLOCATED::instance ()->output_cdr_buffer_allocator_;
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
  : r_ (0),
    object_adapter_ (0),
    cached_connect_strategy_ (0),
    poa_(0),
    input_cdr_dblock_allocator_ (0),
    input_cdr_buffer_allocator_ (0),
    output_cdr_dblock_allocator_ (0),
    output_cdr_buffer_allocator_ (0)
{
  // Make sure that the thread manager does not wait for threads
  this->tm_.wait_on_exit (0);
}

TAO_Allocated_Resources::~TAO_Allocated_Resources (void)
{
  if (this->input_cdr_dblock_allocator_ != 0)
    this->input_cdr_dblock_allocator_->remove ();
  delete this->input_cdr_dblock_allocator_;

  if (this->input_cdr_buffer_allocator_ != 0)
    this->input_cdr_buffer_allocator_->remove ();
  delete this->input_cdr_buffer_allocator_;

  if (this->output_cdr_dblock_allocator_ != 0)
    this->output_cdr_dblock_allocator_->remove ();
  delete this->output_cdr_dblock_allocator_;

  if (this->output_cdr_buffer_allocator_ != 0)
    this->output_cdr_buffer_allocator_->remove ();
  delete this->output_cdr_buffer_allocator_;

  if (this->cached_connect_strategy_ != 0)
    {
      // Zap the creation strategy that we created earlier
      delete this->cached_connect_strategy_->creation_strategy ();
      delete this->cached_connect_strategy_;
    }

  delete this->object_adapter_;

  delete this->r_;
}

// ****************************************************************

TAO_Cached_Connector_Lock::TAO_Cached_Connector_Lock (void)
{
  this->lock_ = TAO_ORB_Core_instance ()->server_factory ()->create_cached_connector_lock ();
}

TAO_Cached_Connector_Lock::~TAO_Cached_Connector_Lock (void)
{
  delete this->lock_;
  this->lock_ = 0;
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

template class ACE_Strategy_Acceptor<TAO_Server_Connection_Handler, TAO_SOCK_ACCEPTOR>;
template class ACE_Creation_Strategy<TAO_Client_Connection_Handler>;
template class ACE_Connect_Strategy<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR>;
template class ACE_Strategy_Connector<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR>;
template class ACE_NOOP_Creation_Strategy<TAO_Client_Connection_Handler>;
template class ACE_Concurrency_Strategy<TAO_Client_Connection_Handler>;
template class ACE_NOOP_Concurrency_Strategy<TAO_Client_Connection_Handler>;
template class ACE_Recycling_Strategy<TAO_Client_Connection_Handler>;
template class ACE_Connector<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR>;

template class ACE_Node<TAO_Client_Connection_Handler *>;

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

#pragma instantiate ACE_Strategy_Acceptor<TAO_Server_Connection_Handler, TAO_SOCK_ACCEPTOR>
#pragma instantiate ACE_Creation_Strategy<TAO_Client_Connection_Handler>
#pragma instantiate ACE_Connect_Strategy<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR>
#pragma instantiate ACE_Strategy_Connector<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR>
#pragma instantiate ACE_NOOP_Creation_Strategy<TAO_Client_Connection_Handler>
#pragma instantiate ACE_Concurrency_Strategy<TAO_Client_Connection_Handler>
#pragma instantiate ACE_NOOP_Concurrency_Strategy<TAO_Client_Connection_Handler>
#pragma instantiate ACE_Recycling_Strategy<TAO_Client_Connection_Handler>
#pragma instantiate ACE_Connector<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR>

#pragma instantiate ACE_Node<TAO_Client_Connection_Handler *>

#pragma instantiate ACE_Singleton<TAO_Allocated_Resources, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_TSS_Singleton<TAO_Allocated_Resources, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_TSS<TAO_Allocated_Resources>

#pragma instantiate ACE_Select_Reactor_Token_T<ACE_Noop_Token>
#pragma instantiate ACE_Lock_Adapter< ACE_Select_Reactor_Token_T<ACE_Noop_Token> >
#pragma instantiate ACE_Select_Reactor_T< ACE_Select_Reactor_Token_T<ACE_Noop_Token> >

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
