// $Id$

#include "tao/default_resource.h"

#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/IIOP_Factory.h"

#include "tao/Acceptor_Registry.h"
#include "tao/Connector_Registry.h"
#include "tao/Single_Reactor.h"

#include "tao/Reactive_Flushing_Strategy.h"
#include "tao/Block_Flushing_Strategy.h"
#include "tao/Leader_Follower_Flushing_Strategy.h"

#include "tao/Leader_Follower.h"
#include "tao/LRU_Connection_Purging_Strategy.h"

#include "tao/LF_Strategy_Complete.h"

#include "ace/TP_Reactor.h"
#include "ace/Dynamic_Service.h"
#include "ace/Arg_Shifter.h"
#include "ace/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
# include "tao/default_resource.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           default_resource,
           "$Id$")

TAO_Default_Resource_Factory::TAO_Default_Resource_Factory (void)
  : use_tss_resources_ (0),
    use_locked_data_blocks_ (1),
    parser_names_count_ (0),
    parser_names_ (0),
    protocol_factories_ (),
    connection_caching_type_ (TAO_CONNECTION_CACHING_STRATEGY),
    cache_maximum_ (TAO_CONNECTION_CACHE_MAXIMUM),
    purge_percentage_ (TAO_PURGE_PERCENT),
    reactor_mask_signals_ (1),
    dynamically_allocated_reactor_ (0),
    options_processed_ (0),
    factory_disabled_ (0),
    cached_connection_lock_type_ (TAO_THREAD_LOCK),
    flushing_strategy_type_ (TAO_LEADER_FOLLOWER_FLUSHING)
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

  for (int i = 0;
       i < this->parser_names_count_;
       ++i)
    CORBA::string_free (this->parser_names_[i]);

  delete [] this->parser_names_;
}

int
TAO_Default_Resource_Factory::init (int argc, char *argv[])
{
  ACE_TRACE ("TAO_Default_Resource_Factory::init");

  // If this factory has already been disabled then
  // print a warning and exit because any options
  // are useless
  if (this->factory_disabled_) {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) Warning: Resource_Factory options ignored\n")
                ACE_TEXT ("Default Resource Factory is disabled\n")));
    return 0;
  }
  this->options_processed_ = 1;

  this->parser_names_count_ = 0;

  int curarg = 0;

  for (curarg = 0; curarg < argc; ++curarg)
  {
    // Parse thro' and find the number of Parsers to be loaded.
    if (ACE_OS::strcasecmp (argv[curarg],
                            "-ORBIORParser") == 0)
      ++this->parser_names_count_;

    ++curarg;

    if (curarg == (argc-1) && this->parser_names_count_ != 0)
      {
        // This is the last loop..
        ACE_NEW_RETURN (this->parser_names_,
                        char *[this->parser_names_count_],
                        -1);

        for (int i = 0;
             i < this->parser_names_count_;
             ++i)
          this->parser_names_[i] = 0;

        this->index_ = 0;
      }
  }


  for (curarg = 0; curarg < argc; curarg++)
    if (ACE_OS::strcasecmp (argv[curarg],
                            "-ORBResources") == 0)
      {
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
            else
              this->report_option_value_error ("-ORBResources", name);
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBReactorMaskSignals") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "0") == 0)
              this->reactor_mask_signals_ = 0;
            else if (ACE_OS::strcasecmp (name, "1") == 0)
              this->reactor_mask_signals_= 1;
            else
              this->report_option_value_error ("-ORBReactorMaskSignals", name);
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBProtocolFactory") == 0)
      {
        TAO_ProtocolFactorySet *pset = this->get_protocol_factories ();
        curarg++;
        if (curarg < argc)
          {
            TAO_Protocol_Item *item = 0;
            ACE_NEW_RETURN (item,
                            TAO_Protocol_Item (argv[curarg]),
                            -1);
            if (pset->insert (item) == -1)
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) Unable to add protocol factories for %s: %p\n"),
                          argv[curarg]));
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBConnectionCachingStrategy") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    "lru") == 0)
              this->connection_caching_type_ =
                TAO_Resource_Factory::LRU;
            else if (ACE_OS::strcasecmp (name,
                                         "lfu") == 0)
              this->connection_caching_type_ =
                TAO_Resource_Factory::LFU;
            else if (ACE_OS::strcasecmp (name,
                                         "fifo") == 0)
              this->connection_caching_type_ =
                TAO_Resource_Factory::FIFO;
            else if (ACE_OS::strcasecmp (name,
                                         "null") == 0)
              this->connection_caching_type_ =
                  TAO_Resource_Factory::NOOP;
            else
              this->report_option_value_error ("-ORBConnectionCachingStrategy", name);
          }
      }

   else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBConnectionCacheMax") == 0)
      {
        curarg++;
        if (curarg < argc)
            this->cache_maximum_ = ACE_OS::atoi (argv[curarg]);
        else
          this->report_option_value_error ("-ORBConnectionCacheMax", argv[curarg]);
      }

   else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBConnectionCachePurgePercentage") == 0)
      {
        curarg++;
        if (curarg < argc)
            this->purge_percentage_ = ACE_OS::atoi (argv[curarg]);
        else
          this->report_option_value_error ("-ORBConnectionCachePurgePercentage",
                                           argv[curarg]);
      }

   else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBPurgePercentage") == 0)
      {
         ACE_DEBUG ((LM_DEBUG,
                     ACE_TEXT ("TAO (%P|%t) This option has been ")
                     ACE_TEXT ("deprecated\n")
                     ACE_TEXT ("using -ORBConnectionCachePurgePercentage ")
                     ACE_TEXT ("instead ")));
        curarg++;
        if (curarg < argc)
            this->purge_percentage_ = ACE_OS::atoi (argv[curarg]);
        else
          this->report_option_value_error ("-ORBPurgePercentage",
                                           argv[curarg]);
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBIORParser") == 0)
      {
        curarg++;

        if (curarg < argc)
          {
            this->add_to_ior_parser_names (argv[curarg]);
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBConnectionCacheLock") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    "thread") == 0)
              this->cached_connection_lock_type_ = TAO_THREAD_LOCK;
            else if (ACE_OS::strcasecmp (name,
                                         "null") == 0)
              {
                // @@ Bug 940 :This is a sort of hack now. We need to put
                // this in a common place once we get the common
                // switch that is documented in bug 940...
                this->use_locked_data_blocks_  = 0;
                this->cached_connection_lock_type_ = TAO_NULL_LOCK;
              }
            else
              this->report_option_value_error ("-ORBConnectionCacheLock", name);
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBConnectionLock") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) This option has been deprecated \n")
                        ACE_TEXT ("using -ORBConnectionCacheLock instead ")));

            if (ACE_OS::strcasecmp (name,
                                    "thread") == 0)
              this->cached_connection_lock_type_ = TAO_THREAD_LOCK;
            else if (ACE_OS::strcasecmp (name,
                                         "null") == 0)
              {
                // @@ Bug 940 :This is a sort of hack now. We need to put
                // this in a common place once we get teh common
                // switch that is documented in bug 940...
                this->use_locked_data_blocks_  = 0;
                this->cached_connection_lock_type_ = TAO_NULL_LOCK;
              }
            else
              this->report_option_value_error ("-ORBConnectionLock", name);
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBConnectorLock") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) This option has been deprecated \n")
                        ACE_TEXT ("using -ORBConnectionCacheLock instead \n")));

            if (ACE_OS::strcasecmp (name,
                                    "thread") == 0)
              this->cached_connection_lock_type_ = TAO_THREAD_LOCK;
            else if (ACE_OS::strcasecmp (name,
                                         "null") == 0)
              {
                // @@ Bug 940 :This is a sort of hack now. We need to put
                // this in a common place once we get teh common
                // switch that is documented in bug 940...
                this->use_locked_data_blocks_  = 0;
                this->cached_connection_lock_type_ = TAO_NULL_LOCK;
              }
            else
              this->report_option_value_error ("-ORBConnectorLock", name);
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 "-ORBFlushingStrategy") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    "leader_follower") == 0)
              this->flushing_strategy_type_ = TAO_LEADER_FOLLOWER_FLUSHING;
            else if (ACE_OS::strcasecmp (name,
                                    "reactive") == 0)
              this->flushing_strategy_type_ = TAO_REACTIVE_FLUSHING;
            else if (ACE_OS::strcasecmp (name,
                                         "blocking") == 0)
              this->flushing_strategy_type_ = TAO_BLOCKING_FLUSHING;
            else
              this->report_option_value_error ("-ORBFlushingStrategy", name);
          }
      }
    else if (ACE_OS::strncmp (argv[curarg], "-ORB", 4) == 0)
      {
        // Can we assume there is an argument after the option?
        // curarg++;
        ACE_ERROR ((LM_ERROR,
                    "Default_Resource_Factory - "
                    "unknown option <%s>\n",
                    argv[curarg]));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG,
                    "Default_Resource_Factory - "
                    "ignoring option <%s>\n",
                    argv[curarg]));
      }

  return 0;
}

int
TAO_Default_Resource_Factory::get_parser_names (char **&names,
                                                int &number_of_names)
{
  if (this->parser_names_count_ != 0)
    {
      // The user used some -ORBIORParser options, just return those.
      names = this->parser_names_;
      number_of_names = this->parser_names_count_;

      return 0;
    }

  // OK fallback on the hardcoded ones....
  this->parser_names_count_ = 4; // HOW MANY DO WE HAVE?

  this->parser_names_ = new char *[this->parser_names_count_];

  for (int i = 0;
       i < this->parser_names_count_;
       ++i)
    this->parser_names_[i] = 0;

  // Ensure that there is enough space in the parser_names_ array

  size_t index = 0;

  // DLL_Parser
  TAO_IOR_Parser *tmp =
    ACE_Dynamic_Service<TAO_IOR_Parser>::instance ("DLL_Parser");

  if (tmp == 0)
    {
      int r = ACE_Service_Config::process_directive
        (
         "dynamic DLL_Parser Service_Object * TAO:_make_TAO_DLL_Parser()"
         );
      if (r != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error configuring DLL parser\n"), -1);
        }
    }

  this->parser_names_[index] = CORBA::string_dup ("DLL_Parser");
  index++;

  // FILE_Parser
  tmp =
    ACE_Dynamic_Service<TAO_IOR_Parser>::instance ("FILE_Parser");

  if (tmp == 0)
    {
      int r = ACE_Service_Config::process_directive
        (
         "dynamic FILE_Parser Service_Object * TAO:_make_TAO_FILE_Parser()"
         );

      if (r != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error Configuring FILE Parser\n"), -1);
        }
    }

  this->parser_names_[index] = CORBA::string_dup ("FILE_Parser");
  index++;

  // CORBALOC_Parser
  tmp =
    ACE_Dynamic_Service<TAO_IOR_Parser>::instance ("CORBALOC_Parser");

  if (tmp == 0)
    {
      int r = ACE_Service_Config::process_directive
        (
         "dynamic CORBALOC_Parser Service_Object * TAO:_make_TAO_CORBALOC_Parser()"
         );

      if (r != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error Configuring CORBALOC Parser\n"), -1);
        }
    }

  this->parser_names_[index] = CORBA::string_dup ("CORBALOC_Parser");
  index++;

  // CORBANAME_Parser
  tmp =
    ACE_Dynamic_Service<TAO_IOR_Parser>::instance ("CORBANAME_Parser");

  if (tmp == 0)
    {
      int r = ACE_Service_Config::process_directive
        (
         "dynamic CORBANAME_Parser Service_Object * TAO :_make_TAO_CORBANAME_Parser()"
         );

      if (r != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error Configuring CORBANAME Parser\n"), -1);
        }
    }

  this->parser_names_[index] = CORBA::string_dup ("CORBANAME_Parser");
  index++;

  names = this->parser_names_;

  //  number_of_names = this->parser_names_count_;
  number_of_names = index;

  return 0;
}

int
TAO_Default_Resource_Factory::add_to_ior_parser_names (const char *curarg)
{
  this->parser_names_[this->index_] = CORBA::string_dup (curarg);

  ++this->index_;

  return 0;
}

// This is virtual and protected...
int
TAO_Default_Resource_Factory::load_default_protocols (void)
{
      // If the user did not list any protocols in her svc.conf file
      // then default to TAO's basic protocols.
      // You do *NOT* need modify this code to add your own protocol,
      // instead simply add the following to your svc.conf file:
      //
      // dynamic PP_Factory Service_Object * LIB:_make_PP_Protocol_Factory() ""
      // static Resource_Factory "-ORBProtocolFactory PP_Factory"
      //
      // where "PP_Factory" is the name of your protocol, i.e. the
      // second argument passed to the ACE_STATIC_SVC_DEFINE macro:
      //
      // ACE_STATIC_SVC_DEFINE (PP_Protocol_Factory,
      //                        ACE_TEXT ("PP_Factory"), ...)
      //
      // "PP_Protocol_Factory" is the name of your protocol factory
      // class.  A "_make_" is prepended to your protocol factory
      // class name by the ACE_FACTORY_DECLARE macro.  The resulting
      // factory function "_make_PP_Protocol_Factory()" is what should
      // be used in the "dynamic" line in your svc.conf file.
      //
      // LIB is the base name of the shared library that implements
      // the protocol.  The directory containing your library must be
      // in your library search path, typically defined by the
      // LD_LIBRARY_PATH environment variable on UNIX systems, and/or
      // the `/etc/ld.so.conf' file on some UNIX systems.  Remember to
      // run "ldconfig" if you modify `/etc/ld.so.conf'.

      TAO_Protocol_Factory *protocol_factory = 0;
      auto_ptr<TAO_Protocol_Factory> safe_protocol_factory;

      TAO_Protocol_Item *item = 0;

      // If a protocol factory is obtained from the Service
      // Configurator then do not transfer ownership to the
      // TAO_Protocol_Item.
      int transfer_ownership = 0;

      protocol_factory =
        ACE_Dynamic_Service<TAO_Protocol_Factory>::instance ("IIOP_Factory");

      if (protocol_factory == 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_WARNING,
                        ACE_TEXT ("TAO (%P|%t) No %s found in ")
                        ACE_TEXT ("Service Repository. ")
                        ACE_TEXT ("Using default instance IIOP ")
                        ACE_TEXT ("Protocol Factory.\n"),
                        ACE_TEXT ("IIOP Protocol Factory")));

          ACE_NEW_RETURN (protocol_factory,
                          TAO_IIOP_Protocol_Factory,
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

      ACE_NEW_RETURN (item,
                      TAO_Protocol_Item ("IIOP_Factory"),
                      -1);
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
                      ACE_TEXT ("protocol <IIOP_Factory>\n")));
        }

  return 0;
}

int
TAO_Default_Resource_Factory::init_protocol_factories (void)
{
  TAO_ProtocolFactorySetItor end = protocol_factories_.end ();
  TAO_ProtocolFactorySetItor factory = protocol_factories_.begin ();

  if (factory == end)
    {
      return this->load_default_protocols ();
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

int
TAO_Default_Resource_Factory::use_tss_resources (void) const
{
  return this->use_tss_resources_;
}

int
TAO_Default_Resource_Factory::use_locked_data_blocks (void) const
{
  return this->use_locked_data_blocks_;
}

TAO_ProtocolFactorySet *
TAO_Default_Resource_Factory::get_protocol_factories (void)
{
  return &protocol_factories_;
}

TAO_Acceptor_Registry*
TAO_Default_Resource_Factory::get_acceptor_registry (void)
{
  TAO_Acceptor_Registry *ar = 0;

  ACE_NEW_RETURN(ar,
                 TAO_Acceptor_Registry,
                 0);

  return ar;
}

TAO_Connector_Registry*
TAO_Default_Resource_Factory::get_connector_registry (void)
{
  TAO_Connector_Registry *cr = 0;

  ACE_NEW_RETURN(cr,
                 TAO_Connector_Registry,
                 0);

  return cr;
}

TAO_Reactor_Registry *
TAO_Default_Resource_Factory::get_reactor_registry (void)
{
  TAO_Reactor_Registry *reactor_registry = 0;

  ACE_NEW_RETURN (reactor_registry,
                  TAO_Single_Reactor,
                  0);
  return reactor_registry;
}

ACE_Reactor_Impl*
TAO_Default_Resource_Factory::allocate_reactor_impl (void) const
{
  ACE_Reactor_Impl *impl = 0;

  ACE_NEW_RETURN (impl, ACE_TP_Reactor ((ACE_Sig_Handler*)0,
                                        (ACE_Timer_Queue*)0,
                                        this->reactor_mask_signals_),
                  0);
  return impl;
}

ACE_Reactor *
TAO_Default_Resource_Factory::get_reactor (void)
{
  ACE_LOG_MSG->errnum (0);

  ACE_Reactor *reactor = 0;
  ACE_NEW_RETURN (reactor,
                  ACE_Reactor (this->allocate_reactor_impl (), 1),
                  0);

  if (ACE_LOG_MSG->errnum () != 0)
    {
      delete reactor;
      reactor = 0;
    }
  else
    this->dynamically_allocated_reactor_ = 1;

  return reactor;
}

void
TAO_Default_Resource_Factory::reclaim_reactor (ACE_Reactor *reactor)
{
  if (this->dynamically_allocated_reactor_ == 1)
    delete reactor;
}


typedef ACE_Malloc<ACE_LOCAL_MEMORY_POOL,ACE_Null_Mutex> NULL_LOCK_MALLOC;
typedef ACE_Allocator_Adapter<NULL_LOCK_MALLOC> NULL_LOCK_ALLOCATOR;

typedef ACE_Malloc<ACE_LOCAL_MEMORY_POOL,TAO_SYNCH_MUTEX> LOCKED_MALLOC;
typedef ACE_Allocator_Adapter<LOCKED_MALLOC> LOCKED_ALLOCATOR;

ACE_Allocator *
TAO_Default_Resource_Factory::input_cdr_dblock_allocator (void)
{
  ACE_Allocator *allocator = 0;
  ACE_NEW_RETURN (allocator,
                  LOCKED_ALLOCATOR,
                  0);

  return allocator;
}

ACE_Allocator *
TAO_Default_Resource_Factory::input_cdr_buffer_allocator (void)
{
  ACE_Allocator *allocator = 0;
  ACE_NEW_RETURN (allocator,
                  LOCKED_ALLOCATOR,
                  0);

  return allocator;
}

ACE_Allocator *
TAO_Default_Resource_Factory::input_cdr_msgblock_allocator (void)
{
  ACE_Allocator *allocator = 0;
  ACE_NEW_RETURN (allocator,
                  LOCKED_ALLOCATOR,
                  0);

  return allocator;
}

int
TAO_Default_Resource_Factory::input_cdr_allocator_type_locked (void)
{
  return 1;
}

ACE_Allocator*
TAO_Default_Resource_Factory::output_cdr_dblock_allocator (void)
{
  ACE_Allocator *allocator = 0;
  ACE_NEW_RETURN (allocator, NULL_LOCK_ALLOCATOR, 0);
  return allocator;
}

ACE_Allocator *
TAO_Default_Resource_Factory::output_cdr_buffer_allocator (void)
{
  ACE_Allocator *allocator = 0;
  ACE_NEW_RETURN (allocator, NULL_LOCK_ALLOCATOR, 0);
  return allocator;
}

ACE_Allocator*
TAO_Default_Resource_Factory::output_cdr_msgblock_allocator (void)
{
  ACE_Allocator *allocator = 0;
  ACE_NEW_RETURN (allocator, NULL_LOCK_ALLOCATOR, 0);
  return allocator;
}

TAO_Resource_Factory::Caching_Strategy
TAO_Default_Resource_Factory::connection_caching_strategy_type (void) const
{
  return this->connection_caching_type_;
}

int
TAO_Default_Resource_Factory::cache_maximum (void) const
{
  return this->cache_maximum_;
}

int
TAO_Default_Resource_Factory::purge_percentage (void) const
{
  return this->purge_percentage_;
}

ACE_Lock *
TAO_Default_Resource_Factory::create_cached_connection_lock (void)
{
  ACE_Lock *the_lock = 0;

  if (this->cached_connection_lock_type_ == TAO_NULL_LOCK)
    ACE_NEW_RETURN (the_lock,
                    ACE_Lock_Adapter<ACE_SYNCH_NULL_MUTEX>,
                    0);
  else
    ACE_NEW_RETURN (the_lock,
                    ACE_Lock_Adapter<TAO_SYNCH_MUTEX>,
                    0);

  return the_lock;
}

TAO_Flushing_Strategy *
TAO_Default_Resource_Factory::create_flushing_strategy (void)
{
  TAO_Flushing_Strategy *strategy = 0;
  if (this->flushing_strategy_type_ == TAO_LEADER_FOLLOWER_FLUSHING)
    ACE_NEW_RETURN (strategy,
                    TAO_Leader_Follower_Flushing_Strategy,
                    0);
  else if (this->flushing_strategy_type_ == TAO_REACTIVE_FLUSHING)
    ACE_NEW_RETURN (strategy,
                    TAO_Reactive_Flushing_Strategy,
                    0);
  else
    ACE_NEW_RETURN (strategy,
                    TAO_Block_Flushing_Strategy,
                    0);
  return strategy;
}

TAO_Connection_Purging_Strategy *
TAO_Default_Resource_Factory::create_purging_strategy (void)
{
  TAO_Connection_Purging_Strategy *strategy = 0;

  if (this->connection_caching_type_ == TAO_Resource_Factory::LRU)
    {
      ACE_NEW_RETURN (strategy,
                      TAO_LRU_Connection_Purging_Strategy (
                                           this->cache_maximum ()),
                      0);
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - ")
                  ACE_TEXT ("no usable purging strategy ")
                  ACE_TEXT ("was found.\n")));
    }

  return strategy;
}

TAO_LF_Strategy *
TAO_Default_Resource_Factory::create_lf_strategy (void)
{
  TAO_LF_Strategy *strategy = 0;

  ACE_NEW_RETURN (strategy,
                  TAO_LF_Strategy_Complete,
                  0);

  return strategy;
}

void
TAO_Default_Resource_Factory::report_option_value_error (
                                 const char* option_name,
                                 const char* option_value)
{
  ACE_DEBUG((LM_DEBUG,
             ACE_TEXT ("Default_Resource_Factory - unknown argument")
             ACE_TEXT (" <%s> for <%s>\n"),
             option_value, option_name));
}

void
TAO_Default_Resource_Factory::disable_factory (void)
{
  this->factory_disabled_ = 1;
  if (this->options_processed_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Warning: Resource_Factory options ignored\n")
                  ACE_TEXT ("Default Resource Factory is disabled\n")));
    }
}

// ****************************************************************

// Notice that the Service Configurator/Repository will not deallocate
// the *payload* object, i.e. the default resource factory.  The
// TAO_Singleton_Manager will deallocate it.  This is necessary since
// the ORB Core requires that a resource factory exist as it is being
// finalized.  Resource factories that are statically or dynamically
// loaded (i.e. registered with the Service Repository) should
// generally enable the ACE_Service_Type::DELETE_OBJ service
// descriptor flag.
ACE_STATIC_SVC_DEFINE (TAO_Default_Resource_Factory,
                       ACE_TEXT ("Resource_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Resource_Factory),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ /* | ACE_Service_Type::DELETE_OBJ */ /* SEE ABOVE */,
                       0)

// @@ See below for an explanation of why this macro is not used for
//    the TAO_Default_Resource_Factory.
ACE_FACTORY_DEFINE (TAO, TAO_Default_Resource_Factory)

#if 0
// A custom factory definition is used instead of the commonly used
// one created by the ACE_FACTORY_DEFINE macro below.  This is
// necessary to get around dynamic unloading issues.  The default
// resource factory must exist long enough for the ORB to make the
// resource factory reclaim the reactor, for example.  The easiest
// (and probably best) way to do that is register the
// TAO_Default_Resource_Factory for destruction with the
// TAO_Singleton_Manager.
extern "C" ACE_Service_Object *
_make_TAO_Default_Resource_Factory (ACE_Service_Object_Exterminator *gobbler)
{
  // The TAO_Singleton_Manager will "gobble" the payload
  // (TAO_Default_Resource_Factory) object.  Set the gobbler function
  // invoked by the Service_Type implementation to be to be nil.
  *gobbler = 0;

  TAO_Default_Resource_Factory *rf = 0;
  ACE_NEW_RETURN (rf,
                  TAO_Default_Resource_Factory,
                  0);

  // Register for destruction with the TAO_Singleton_Manager.
  //
  // Note that the instance of the TAO_Default_Resource_Factory just
  // created is not a Singleton.  The TAO_Singleton_Manager is useful
  // for more than just Singleton managment.
  if (TAO_Singleton_Manager::at_exit (rf) != 0)
    {
      delete rf;
      return 0;
    }

  return rf;
}
#endif  /* 0 */

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class auto_ptr<TAO_Protocol_Factory>;
template class ACE_Auto_Basic_Ptr<TAO_Protocol_Factory>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate auto_ptr<TAO_Protocol_Factory>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Protocol_Factory>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
