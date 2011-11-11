// -*- C++ -*-
// $Id: default_resource.cpp 93496 2011-03-07 09:37:27Z johnnyw $

#include "tao/default_resource.h"

#include "tao/debug.h"
#include "tao/IIOP_Factory.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Connector_Registry.h"
#include "tao/Reactive_Flushing_Strategy.h"
#include "tao/Block_Flushing_Strategy.h"
#include "tao/Leader_Follower_Flushing_Strategy.h"
#include "tao/LRU_Connection_Purging_Strategy.h"
#include "tao/LF_Strategy_Complete.h"
#include "tao/Codeset_Descriptor_Base.h"
#include "tao/Codeset_Manager_Factory_Base.h"
#include "tao/Codeset_Manager.h"
#include "tao/Null_Fragmentation_Strategy.h"
#include "tao/On_Demand_Fragmentation_Strategy.h"
#include "tao/MMAP_Allocator.h"
#include "tao/Load_Protocol_Factory_T.h"

#include "ace/TP_Reactor.h"
#include "ace/Malloc.h"
#include "ace/Reactor.h"
#include "ace/Malloc_T.h"
#include "ace/Local_Memory_Pool.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Codeset_Parameters::TAO_Codeset_Parameters (void)
  : translators_ ()
  , native_ (0)
{
}

TAO_Codeset_Parameters::~TAO_Codeset_Parameters (void)
{
  for (TAO_Codeset_Parameters::iterator i =
         this->translators ();
       !i.done ();
       i.advance ())
    {
      ACE_TCHAR** element = 0;
      if (i.next (element))
        ACE_OS::free (*element);
    }

  ACE_OS::free (this->native_);
}

const ACE_TCHAR*
TAO_Codeset_Parameters::native (void)
{
  return (this->native_);
}

void
TAO_Codeset_Parameters::apply_to (TAO_Codeset_Descriptor_Base *csd)
{
  if (csd == 0)
    return;

  if (this->native () != 0)
      csd->ncs (this->native ());

  ACE_TCHAR** element = 0;
  for (TAO_Codeset_Parameters::iterator i = this->translators ();
       !i.done ();
       i.advance ())
    {
      if (i.next (element))
        csd->add_translator (*element);
    }
}

void
TAO_Codeset_Parameters::native (const ACE_TCHAR* n)
{
  ACE_OS::free (this->native_);
  this->native_ = ACE_OS::strdup (n);
}

void
TAO_Codeset_Parameters::add_translator (const ACE_TCHAR*name)
{
  this->translators_.enqueue_tail (ACE_OS::strdup (name));
}

TAO_Codeset_Parameters::iterator
TAO_Codeset_Parameters::translators (void)
{
  return this->translators_.begin ();
}

TAO_Default_Resource_Factory::TAO_Default_Resource_Factory (void)
  : use_locked_data_blocks_ (1)
  , parser_names_count_ (0)
  , parser_names_ (0)
  , protocol_factories_ ()
  , connection_purging_type_ (TAO_CONNECTION_PURGING_STRATEGY)
  , cache_maximum_ (TAO_CONNECTION_CACHE_MAXIMUM)
  , purge_percentage_ (TAO_PURGE_PERCENT)
  , max_muxed_connections_ (0)
  , reactor_mask_signals_ (1)
  , dynamically_allocated_reactor_ (false)
  , options_processed_ (0)
  , factory_disabled_ (0)
#if TAO_USE_OUTPUT_CDR_MMAP_MEMORY_POOL == 1
  , output_cdr_allocator_type_ (MMAP_ALLOCATOR)
#else
  , output_cdr_allocator_type_ (DEFAULT)
#endif
#if TAO_USE_LOCAL_MEMORY_POOL == 1
  , use_local_memory_pool_ (true)
#else
  , use_local_memory_pool_ (false)
#endif
  , cached_connection_lock_type_ (TAO_THREAD_LOCK)
#if defined (TAO_USE_BLOCKING_FLUSHING)
  , flushing_strategy_type_ (TAO_BLOCKING_FLUSHING)
#elif defined (TAO_USE_REACTIVE_FLUSHING)
  , flushing_strategy_type_ (TAO_REACTIVE_FLUSHING)
#else
  , flushing_strategy_type_ (TAO_LEADER_FOLLOWER_FLUSHING)
#endif
  , char_codeset_parameters_ ()
  , wchar_codeset_parameters_ ()
  , resource_usage_strategy_ (TAO_Resource_Factory::TAO_EAGER)
  , drop_replies_ (true)
{
#if TAO_USE_LAZY_RESOURCE_USAGE_STRATEGY == 1
  this->resource_usage_strategy_ =
    TAO_Resource_Factory::TAO_LAZY;
#endif /*TAO_USE_LAZY_RESOURCE_USAGE_STRATEGY*/


}

TAO_Default_Resource_Factory::~TAO_Default_Resource_Factory (void)
{
  const TAO_ProtocolFactorySetItor end = this->protocol_factories_.end ();

  for (TAO_ProtocolFactorySetItor iterator =
         this->protocol_factories_.begin ();
       iterator != end;
       ++iterator)
    {
      delete *iterator;
    }

  this->protocol_factories_.reset ();

  for (int i = 0; i < this->parser_names_count_; ++i)
    CORBA::string_free (this->parser_names_[i]);

  delete [] this->parser_names_;
}


int
TAO_Default_Resource_Factory::init (int argc, ACE_TCHAR *argv[])
{
  ACE_TRACE ("TAO_Default_Resource_Factory::init");

  // If this factory has already been disabled then
  // print a warning and exit because any options
  // are useless
  if (this->factory_disabled_)
    {
      ACE_DEBUG ((LM_WARNING,
                  ACE_TEXT ("TAO (%P|%t) - Warning: Resource_Factory options ")
                  ACE_TEXT ("ignored. ")
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
                            ACE_TEXT("-ORBIORParser")) == 0)
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

  for (curarg = 0; curarg < argc; ++curarg)
    {
      if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBReactorMaskSignals")) == 0)
      {
        ++curarg;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name, ACE_TEXT("0")) == 0)
              this->reactor_mask_signals_ = 0;
            else if (ACE_OS::strcasecmp (name, ACE_TEXT("1")) == 0)
              this->reactor_mask_signals_= 1;
            else
              this->report_option_value_error (ACE_TEXT("-ORBReactorMaskSignals"), name);
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBProtocolFactory")) == 0)
      {
        TAO_ProtocolFactorySet *pset = this->get_protocol_factories ();
        ++curarg;
        if (curarg < argc)
          {
            TAO_Protocol_Item *item = 0;
            ACE_NEW_RETURN (item,
                            TAO_Protocol_Item (ACE_TEXT_ALWAYS_CHAR(argv[curarg])),
                            -1);
            if (pset->insert (item) == -1)
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) Unable to add protocol factories ")
                          ACE_TEXT ("for %s: %m\n"),
                          argv[curarg]));
          }
      }

    /// CodeSet Translators
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBNativeCharCodeSet")) == 0)
      {
        ++curarg;
        if (curarg < argc)
            this->char_codeset_parameters_.native (argv[curarg]);
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBNativeWCharCodeSet")) == 0)
      {
        ++curarg;
        if (curarg < argc)
            this->wchar_codeset_parameters_.native (argv[curarg]);
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBCharCodesetTranslator")) == 0)
      {
        ++curarg;
        if (curarg < argc)
            this->char_codeset_parameters_.add_translator (argv[curarg]);
      }

    /// CodeSet Translators
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBWCharCodesetTranslator")) == 0)
      {
        ++curarg;
        if (curarg < argc)
            this->wchar_codeset_parameters_.add_translator (argv[curarg]);
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBConnectionPurgingStrategy")) == 0)
      {
        ++curarg;

        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_TEXT("lru")) == 0)
              this->connection_purging_type_ =
                TAO_Resource_Factory::LRU;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_TEXT("lfu")) == 0)
              this->connection_purging_type_ =
                TAO_Resource_Factory::LFU;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_TEXT("fifo")) == 0)
              this->connection_purging_type_ =
                TAO_Resource_Factory::FIFO;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_TEXT("null")) == 0)
              this->connection_purging_type_ =
                  TAO_Resource_Factory::NOOP;
            else
              this->report_option_value_error (ACE_TEXT("-ORBConnectionPurgingStrategy"), name);
          }
      }
   else if (ACE_OS::strcasecmp (argv[curarg],
                                ACE_TEXT("-ORBConnectionCacheMax")) == 0)
      {
        ++curarg;
        if (curarg < argc)
            this->cache_maximum_ = ACE_OS::atoi (argv[curarg]);
        else
          this->report_option_value_error (ACE_TEXT("-ORBConnectionCacheMax"), argv[curarg]);
      }

   else if (ACE_OS::strcasecmp (argv[curarg],
                                ACE_TEXT("-ORBConnectionCachePurgePercentage")) == 0)
      {
        ++curarg;
        if (curarg < argc)
            this->purge_percentage_ = ACE_OS::atoi (argv[curarg]);
        else
          this->report_option_value_error (ACE_TEXT("-ORBConnectionCachePurgePercentage"),
                                           argv[curarg]);
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBIORParser")) == 0)
      {
        ++curarg;

        if (curarg < argc)
          {
            this->add_to_ior_parser_names (ACE_TEXT_ALWAYS_CHAR(argv[curarg]));
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBConnectionCacheLock")) == 0)
      {
        ++curarg;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_TEXT("thread")) == 0)
              this->cached_connection_lock_type_ = TAO_THREAD_LOCK;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_TEXT("null")) == 0)
              {
                // @@ Bug 940 :This is a sort of hack now. We need to put
                // this in a common place once we get the common
                // switch that is documented in bug 940...
                this->use_locked_data_blocks_  = 0;
                this->cached_connection_lock_type_ = TAO_NULL_LOCK;
              }
            else
              this->report_option_value_error (ACE_TEXT("-ORBConnectionCacheLock"), name);
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBResourceUsage")) == 0)
      {
        ++curarg;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_TEXT("eager")) == 0)
              this->resource_usage_strategy_ = TAO_EAGER;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_TEXT("lazy")) == 0)
              {
                this->resource_usage_strategy_ = TAO_LAZY;
              }
            else
              this->report_option_value_error (ACE_TEXT("-ORBResourceUsage"), name);
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBFlushingStrategy")) == 0)
      {
        ++curarg;
        /*
         * Hook to specialize TAO's Flushing strategy implementations
         */
//@@ FLUSHING_STRATEGY_SPL_COMMENT_HOOK_START
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_TEXT("leader_follower")) == 0)
              this->flushing_strategy_type_ = TAO_LEADER_FOLLOWER_FLUSHING;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_TEXT("reactive")) == 0)
              this->flushing_strategy_type_ = TAO_REACTIVE_FLUSHING;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_TEXT("blocking")) == 0)
              this->flushing_strategy_type_ = TAO_BLOCKING_FLUSHING;
            else
              this->report_option_value_error (ACE_TEXT("-ORBFlushingStrategy"), name);
          }
//@@ FLUSHING_STRATEGY_SPL_COMMENT_HOOK_END
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT ("-ORBMuxedConnectionMax")) == 0)
      {
        ++curarg;
        if (curarg < argc)
            this->max_muxed_connections_ =
              ACE_OS::atoi (argv[curarg]);
        else
          this->report_option_value_error (ACE_TEXT("-ORBMuxedConnectionMax"),
                                           argv[curarg]);
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_TEXT("-ORBDropRepliesDuringShutdown")) == 0)
      {
        ++curarg;
        if (curarg < argc)
          {
            int tmp = ACE_OS::atoi (argv[curarg]);

            if (tmp == 0)
              this->drop_replies_ = false;
            else
              this->drop_replies_ = true;
          }
        else
          this->report_option_value_error (ACE_TEXT("-ORBDropRepliesDuringShutdown"),
                                           argv[curarg]);
      }
    else if (0 == ACE_OS::strcasecmp (argv[curarg],
                                      ACE_TEXT("-ORBOutputCDRAllocator")))
      {
        ++curarg;

        if (curarg < argc)
          {
            ACE_TCHAR const * const current_arg = argv[curarg];

            if (ACE_OS::strcasecmp (current_arg,
                                    ACE_TEXT("mmap")) == 0)
              {
#if TAO_HAS_SENDFILE == 1
                this->output_cdr_allocator_type_ = MMAP_ALLOCATOR;
#else
                ACE_DEBUG ((LM_WARNING,
                            ACE_TEXT ("MMAP allocator unsupport on this platform")));
#endif  /* TAO_HAS_SENDFILE==1 */
              }
            else if (ACE_OS::strcasecmp (current_arg,
                                         ACE_TEXT("local_memory_pool")) == 0
                     && this->output_cdr_allocator_type_ != DEFAULT)
              {
                this->output_cdr_allocator_type_ = LOCAL_MEMORY_POOL;
              }
            else if (ACE_OS::strcasecmp (current_arg,
                                         ACE_TEXT("default")) == 0)
              {
                this->output_cdr_allocator_type_ = DEFAULT;
              }
            else
              {
                this->report_option_value_error (
                  ACE_TEXT("-ORBOutputCDRAllocator"), current_arg);
              }
          }
      }
    else if (0 == ACE_OS::strcasecmp (argv[curarg],
                                      ACE_TEXT("-ORBZeroCopyWrite")))
      {
#if TAO_HAS_SENDFILE == 1
        this->output_cdr_allocator_type_ = MMAP_ALLOCATOR;
#else
        ACE_DEBUG ((LM_WARNING,
                    ACE_TEXT ("Zero copy writes unsupported on this platform\n")));
#endif  /* TAO_HAS_SENDFILE==1 */
      }
    else if (ACE_OS::strncmp (argv[curarg],
                              ACE_TEXT ("-ORB"),
                              4) == 0)
      {
        // Can we assume there is an argument after the option?
        // ++curarg;
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - Default_Resource_Factory - ")
                    ACE_TEXT ("unknown option <%s>\n"),
                    argv[curarg]));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - Default_Resource_Factory - ")
                    ACE_TEXT ("ignoring option <%s>\n"),
                    argv[curarg]));
      }
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

  this->parser_names_count_ = 0;
#if (TAO_HAS_DDL_PARSER == 1)
  ++this->parser_names_count_;
#endif
#if (TAO_HAS_FILE_PARSER == 1)
  ++this->parser_names_count_;
#endif
#if (TAO_HAS_CORBALOC_PARSER == 1)
  ++this->parser_names_count_;
#endif
#if (TAO_HAS_CORBANAME_PARSER == 1)
  ++this->parser_names_count_;
#endif
#if (TAO_HAS_HTTP_PARSER == 1)
  ++this->parser_names_count_;
#endif
#if (TAO_HAS_MCAST_PARSER == 1)
  ++this->parser_names_count_;
#endif
  ACE_NEW_RETURN (this->parser_names_,
                  char *[this->parser_names_count_],
                  -1);

  CORBA::ULong index = 0;

#if (TAO_HAS_DDL_PARSER == 1)
  this->parser_names_[index] = CORBA::string_dup ("DLL_Parser");
  ++index;
#endif

#if (TAO_HAS_FILE_PARSER == 1)
  this->parser_names_[index] = CORBA::string_dup ("FILE_Parser");
  ++index;
#endif

#if (TAO_HAS_CORBALOC_PARSER == 1)
  this->parser_names_[index] = CORBA::string_dup ("CORBALOC_Parser");
  ++index;
#endif

#if (TAO_HAS_CORBANAME_PARSER == 1)
  this->parser_names_[index] = CORBA::string_dup ("CORBANAME_Parser");
  ++index;
#endif

#if (TAO_HAS_MCAST_PARSER == 1)
  this->parser_names_[index] = CORBA::string_dup ("MCAST_Parser");
  ++index;
#endif

#if (TAO_HAS_HTTP_PARSER == 1)
  this->parser_names_[index] = CORBA::string_dup ("HTTP_Parser");
  ++index;
#endif

  names = this->parser_names_;

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
#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)
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

      if (TAO::details::load_protocol_factory <TAO_IIOP_Protocol_Factory> (
          this->protocol_factories_, "IIOP_Factory") == -1)
        return -1;
#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */

  return 0;
}

int
TAO_Default_Resource_Factory::init_protocol_factories (void)
{
  const TAO_ProtocolFactorySetItor end = protocol_factories_.end ();
  TAO_ProtocolFactorySetItor factory = protocol_factories_.begin ();

  if (factory == end)
    {
      return this->load_default_protocols ();
    }

  for (; factory != end; ++factory)
    {
      const ACE_CString &name = (*factory)->protocol_name ();
      (*factory)->factory (
        ACE_Dynamic_Service<TAO_Protocol_Factory>::instance (name.c_str ()));
      if ((*factory)->factory () == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("TAO (%P|%t) - Unable to load ")
                             ACE_TEXT ("protocol <%C>, %p\n"),
                             name.c_str (),
                             ACE_TEXT ("")),
                            -1);
        }

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Loaded protocol <%C>\n"),
                      name.c_str ()));
        }
    }

  return 0;
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

TAO_Acceptor_Registry *
TAO_Default_Resource_Factory::get_acceptor_registry (void)
{
  TAO_Acceptor_Registry *ar = 0;

  ACE_NEW_RETURN(ar,
                 TAO_Acceptor_Registry,
                 0);

  return ar;
}

TAO_Connector_Registry *
TAO_Default_Resource_Factory::get_connector_registry (void)
{
  TAO_Connector_Registry *cr = 0;

  ACE_NEW_RETURN(cr,
                 TAO_Connector_Registry,
                 0);

  return cr;
}

ACE_Reactor_Impl*
TAO_Default_Resource_Factory::allocate_reactor_impl (void) const
{
  ACE_Reactor_Impl *impl = 0;
  /*
   * Hook to specialize TAO's reactor implementation.
   */
//@@ TAO_REACTOR_SPL_COMMENT_HOOK_START
  ACE_NEW_RETURN (impl,
                  ACE_TP_Reactor (ACE::max_handles (),
                                  1,
                                  (ACE_Sig_Handler*)0,
                                  (ACE_Timer_Queue*)0,
                                  this->reactor_mask_signals_,
                                  ACE_Select_Reactor_Token::LIFO),
                  0);
//@@ TAO_REACTOR_SPL_COMMENT_HOOK_END
  return impl;
}

ACE_Reactor *
TAO_Default_Resource_Factory::get_reactor (void)
{
  ACE_Reactor *reactor = 0;
  ACE_NEW_RETURN (reactor,
                  ACE_Reactor (this->allocate_reactor_impl (), 1),
                  0);

  if (reactor->initialized () == 0)
    {
      delete reactor;
      reactor = 0;
    }
  else
    this->dynamically_allocated_reactor_ = true;

  return reactor;
}

void
TAO_Default_Resource_Factory::reclaim_reactor (ACE_Reactor *reactor)
{
  if (this->dynamically_allocated_reactor_)
    delete reactor;
}


typedef ACE_Malloc<ACE_LOCAL_MEMORY_POOL,TAO_SYNCH_MUTEX> LOCKED_MALLOC;
typedef ACE_Allocator_Adapter<LOCKED_MALLOC> LOCKED_ALLOCATOR_POOL;
typedef ACE_New_Allocator LOCKED_ALLOCATOR_NO_POOL;

void
TAO_Default_Resource_Factory::use_local_memory_pool (bool flag)
{
  this->use_local_memory_pool_ = flag;

  if (this->output_cdr_allocator_type_ == DEFAULT)
    this->output_cdr_allocator_type_ = LOCAL_MEMORY_POOL;
}

ACE_Allocator *
TAO_Default_Resource_Factory::input_cdr_dblock_allocator (void)
{
  ACE_Allocator *allocator = 0;
  if (use_local_memory_pool_)
  {
    ACE_NEW_RETURN (allocator,
                    LOCKED_ALLOCATOR_POOL,
                    0);
  }
  else
  {
    ACE_NEW_RETURN (allocator,
                    LOCKED_ALLOCATOR_NO_POOL,
                    0);
  }

  return allocator;
}

ACE_Allocator *
TAO_Default_Resource_Factory::input_cdr_buffer_allocator (void)
{
  ACE_Allocator *allocator = 0;
  if (use_local_memory_pool_)
  {
    ACE_NEW_RETURN (allocator,
                    LOCKED_ALLOCATOR_POOL,
                    0);
  }
  else
  {
    ACE_NEW_RETURN (allocator,
                    LOCKED_ALLOCATOR_NO_POOL,
                    0);
  }

  return allocator;
}

ACE_Allocator *
TAO_Default_Resource_Factory::input_cdr_msgblock_allocator (void)
{
  ACE_Allocator *allocator = 0;
  if (use_local_memory_pool_)
  {
    ACE_NEW_RETURN (allocator,
                    LOCKED_ALLOCATOR_POOL,
                    0);
  }
  else
  {
    ACE_NEW_RETURN (allocator,
                    LOCKED_ALLOCATOR_NO_POOL,
                    0);
  }

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
  if (use_local_memory_pool_)
  {
    ACE_NEW_RETURN (allocator,
                    LOCKED_ALLOCATOR_POOL,
                    0);
  }
  else
  {
    ACE_NEW_RETURN (allocator,
                    LOCKED_ALLOCATOR_NO_POOL,
                    0);
  }

  return allocator;
}

ACE_Allocator *
TAO_Default_Resource_Factory::output_cdr_buffer_allocator (void)
{
  ACE_Allocator *allocator = 0;

  switch (this->output_cdr_allocator_type_)
    {
    case LOCAL_MEMORY_POOL:
      ACE_NEW_RETURN (allocator,
                      LOCKED_ALLOCATOR_POOL,
                      0);

      break;

#if TAO_HAS_SENDFILE == 1
    case MMAP_ALLOCATOR:
      ACE_NEW_RETURN (allocator,
                      TAO_MMAP_Allocator,
                      0);

      break;
#endif  /* TAO_HAS_SENDFILE==1 */

    case DEFAULT:
    default:
      ACE_NEW_RETURN (allocator,
                      LOCKED_ALLOCATOR_NO_POOL,
                      0);

      break;
    }

  return allocator;
}

ACE_Allocator*
TAO_Default_Resource_Factory::output_cdr_msgblock_allocator (void)
{
  ACE_Allocator *allocator = 0;
  if (use_local_memory_pool_)
  {
    ACE_NEW_RETURN (allocator,
                    LOCKED_ALLOCATOR_POOL,
                    0);
  }
  else
  {
    ACE_NEW_RETURN (allocator,
                    LOCKED_ALLOCATOR_NO_POOL,
                    0);
  }

  return allocator;
}

ACE_Allocator*
TAO_Default_Resource_Factory::amh_response_handler_allocator (void)
{
  ACE_Allocator *allocator = 0;
  if (use_local_memory_pool_)
  {
    ACE_NEW_RETURN (allocator,
                    LOCKED_ALLOCATOR_POOL,
                    0);
  }
  else
  {
    ACE_NEW_RETURN (allocator,
                    LOCKED_ALLOCATOR_NO_POOL,
                    0);
  }

  return allocator;
}

ACE_Allocator*
TAO_Default_Resource_Factory::ami_response_handler_allocator (void)
{
  ACE_Allocator *allocator = 0;
  if (use_local_memory_pool_)
  {
    ACE_NEW_RETURN (allocator,
                    LOCKED_ALLOCATOR_POOL,
                    0);
  }
  else
  {
    ACE_NEW_RETURN (allocator,
                    LOCKED_ALLOCATOR_NO_POOL,
                    0);
  }

  return allocator;
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

int
TAO_Default_Resource_Factory::max_muxed_connections (void) const
{
  return this->max_muxed_connections_;
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

int
TAO_Default_Resource_Factory::locked_transport_cache (void)
{
  if (this->cached_connection_lock_type_ == TAO_NULL_LOCK)
    return 0;

  return 1;
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

  if (this->connection_purging_type_ == TAO_Resource_Factory::LRU)
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

auto_ptr<TAO_GIOP_Fragmentation_Strategy>
TAO_Default_Resource_Factory::create_fragmentation_strategy (
  TAO_Transport * transport,
  CORBA::ULong max_message_size) const
{
  auto_ptr<TAO_GIOP_Fragmentation_Strategy> strategy (0);

  TAO_GIOP_Fragmentation_Strategy * tmp = 0;

  // Minimum GIOP message size is 24 (a multiple of 8):
  //   12   GIOP Message Header
  //    4   GIOP Fragment Header (request ID)
  // +  8   Smallest payload, including padding.
  //  ---
  //   24
  static CORBA::ULong const min_message_size = 24;

  // GIOP fragments are supported in GIOP 1.1 and better, but TAO only
  // supports them in 1.2 or better since GIOP 1.1 fragments do not
  // have a fragment message header.
  if (transport) // No transport.  Cannot fragment.
    {
      if (max_message_size < min_message_size
          || (TAO_DEF_GIOP_MAJOR == 1 && TAO_DEF_GIOP_MINOR < 2))
        {
          // No maximum was set by the user.
          ACE_NEW_RETURN (tmp,
                          TAO_Null_Fragmentation_Strategy,
                          strategy);

        }
      else
        {
          ACE_NEW_RETURN (tmp,
                          TAO_On_Demand_Fragmentation_Strategy (
                            transport,
                            max_message_size),
                          strategy);
        }
    }

  ACE_auto_ptr_reset (strategy, tmp);

  return strategy;
}

void
TAO_Default_Resource_Factory::report_option_value_error (
                                 const ACE_TCHAR* option_name,
                                 const ACE_TCHAR* option_value)
{
  ACE_DEBUG((LM_DEBUG,
             ACE_TEXT ("TAO (%P|%t) - Default_Resource_Factory - unknown ")
             ACE_TEXT ("argument <%s> for <%s>\n"),
             option_value, option_name));
}

void
TAO_Default_Resource_Factory::disable_factory (void)
{
  this->factory_disabled_ = 1;
  if (this->options_processed_)
    {
      ACE_DEBUG ((LM_WARNING,
                  ACE_TEXT ("TAO (%P|%t) - Warning: Resource_Factory options ignored\n")
                  ACE_TEXT ("Default Resource Factory is disabled\n")));
    }
}

TAO_Codeset_Manager *
TAO_Default_Resource_Factory::codeset_manager(void)
{
  TAO_Codeset_Manager_Factory_Base *factory =
    ACE_Dynamic_Service<TAO_Codeset_Manager_Factory_Base>::instance ("TAO_Codeset");

  if (factory == 0)
    {
      if (TAO_debug_level >= 2)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - Default_Resource_Factory")
                    ACE_TEXT (" - unable to find codeset manager factory.\n")));
      return 0;
    }

  TAO_Codeset_Manager* mgr = factory->create ();

  if (mgr == 0)
    {
      if (TAO_debug_level >= 2)
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("TAO (%P|%t) - Default_Resource_Factory")
                    ACE_TEXT (" - unable to create codeset manager.\n")));
      return 0;
    }


  ACE_Auto_Ptr<TAO_Codeset_Manager> safemgr (mgr);

  if (TAO_debug_level >= 1)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - Default_Resource_Factory - codeset manager=%@\n"),
                mgr));

  this->char_codeset_parameters_.apply_to (mgr->char_codeset_descriptor());
  this->wchar_codeset_parameters_.apply_to (mgr->wchar_codeset_descriptor());

  return safemgr.release ();

}

TAO_Resource_Factory::Resource_Usage
TAO_Default_Resource_Factory::resource_usage_strategy (void) const
{
  return this->resource_usage_strategy_;
}

bool
TAO_Default_Resource_Factory::drop_replies_during_shutdown (void) const
{
  return this->drop_replies_;
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Default_Resource_Factory,
                       ACE_TEXT ("Resource_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Resource_Factory),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Resource_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

