// $Id$

#include "tao/default_resource.h"

#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/IIOP_Factory.h"

#include "tao/Acceptor_Registry.h"
#include "tao/Connector_Registry.h"

#include "tao/Reactive_Flushing_Strategy.h"
#include "tao/Block_Flushing_Strategy.h"
#include "tao/Leader_Follower_Flushing_Strategy.h"

#include "tao/Leader_Follower.h"
#include "tao/LRU_Connection_Purging_Strategy.h"

#include "tao/LF_Strategy_Complete.h"
#include "tao/Codeset_Manager.h"

#include "ace/TP_Reactor.h"
#include "ace/Dynamic_Service.h"
#include "ace/Arg_Shifter.h"
#include "ace/Auto_Ptr.h"
#include "ace/Memory_Pool.h"
#include "ace/Malloc.h"
#include "ace/Codeset_Registry.h"

#if !defined (__ACE_INLINE__)
# include "tao/default_resource.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           default_resource,
           "$Id$")

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
  , dynamically_allocated_reactor_ (0)
  , options_processed_ (0)
  , factory_disabled_ (0)
  , cached_connection_lock_type_ (TAO_THREAD_LOCK)
  , object_key_table_lock_type_ (TAO_THREAD_LOCK)
  , corba_object_lock_type_ (TAO_THREAD_LOCK)
  , flushing_strategy_type_ (TAO_LEADER_FOLLOWER_FLUSHING)
  , codeset_manager_ (0)
  , resource_usage_strategy_ (TAO_Resource_Factory::TAO_EAGER)
{
#if TAO_USE_LAZY_RESOURCE_USAGE_STRATEGY == 1
  this->resource_usage_strategy_ =
    TAO_Resource_Factory::TAO_LAZY;
#endif /*TAO_USE_LAZY_RESOURCE_USAGE_STRATEGY*/
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

  delete this->codeset_manager_;
}

int
TAO_Default_Resource_Factory::init (int argc, ACE_TCHAR *argv[])
{
  ACE_TRACE ("TAO_Default_Resource_Factory::init");

  // If this factory has already been disabled then
  // print a warning and exit because any options
  // are useless
  if (this->factory_disabled_) {
    ACE_DEBUG ((LM_DEBUG,
                ACE_LIB_TEXT ("TAO (%P|%t) Warning: Resource_Factory options ignored\n")
                ACE_LIB_TEXT ("Default Resource Factory is disabled\n")));
    return 0;
  }
  this->options_processed_ = 1;

  this->parser_names_count_ = 0;

  int curarg = 0;

  for (curarg = 0; curarg < argc; ++curarg)
  {
    // Parse thro' and find the number of Parsers to be loaded.
    if (ACE_OS::strcasecmp (argv[curarg],
                            ACE_LIB_TEXT("-ORBIORParser")) == 0)
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
    if (ACE_OS::strcasecmp (argv[curarg],
                            ACE_LIB_TEXT("-ORBResources")) == 0)
      {
        curarg++;

        ACE_DEBUG ((LM_DEBUG,
                    ACE_LIB_TEXT ("(%P|%t) This option has been deprecated \n")));
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_LIB_TEXT("-ORBReactorMaskSignals")) == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name, ACE_LIB_TEXT("0")) == 0)
              this->reactor_mask_signals_ = 0;
            else if (ACE_OS::strcasecmp (name, ACE_LIB_TEXT("1")) == 0)
              this->reactor_mask_signals_= 1;
            else
              this->report_option_value_error (ACE_LIB_TEXT("-ORBReactorMaskSignals"), name);
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_LIB_TEXT("-ORBProtocolFactory")) == 0)
      {
        TAO_ProtocolFactorySet *pset = this->get_protocol_factories ();
        curarg++;
        if (curarg < argc)
          {
            TAO_Protocol_Item *item = 0;
            ACE_NEW_RETURN (item,
                            TAO_Protocol_Item (ACE_TEXT_ALWAYS_CHAR(argv[curarg])),
                            -1);
            if (pset->insert (item) == -1)
              ACE_ERROR ((LM_ERROR,
                          ACE_LIB_TEXT ("(%P|%t) Unable to add protocol factories for %s: %p\n"),
                          argv[curarg]));
          }
      }

    /// CodeSet Translators
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_LIB_TEXT("-ORBNativeCharCodeSet")) == 0)
    {
        curarg++;
        CONV_FRAME::CodeSetId ncs;
        if (ACE_Codeset_Registry::locale_to_registry (argv[curarg],
                                                      ncs) == 0)
          {
            char **endPtr =0;
            ncs = ACE_OS_String::strtoul(ACE_TEXT_ALWAYS_CHAR(argv[curarg]),
                                         endPtr, 0);
  }
        // Validate the CodesetId
        if (ACE_Codeset_Registry::get_max_bytes(ncs) == 0)
          {
            if (TAO_debug_level > 0)
              ACE_ERROR((LM_ERROR,
                         ACE_TEXT("(%P|%t) Invalid NativeCharCodeSet, %x\n"),
                         ncs));
            return -1;
          }
        TAO_Codeset_Manager *csm = this->get_codeset_manager();
        if (csm)
          csm->set_ncs_c(ncs);
    }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_LIB_TEXT("-ORBNativeWCharCodeSet")) == 0)
      {
        curarg++;
        CONV_FRAME::CodeSetId ncs;
        if (ACE_Codeset_Registry::locale_to_registry( argv[curarg],
                                                      ncs) == 0)
          {
            char **endPtr = 0;
            ncs = ACE_OS_String::strtoul(ACE_TEXT_ALWAYS_CHAR(argv[curarg]),
                                         endPtr, 0);
          }
        // Validate the CodesetId
        int mb = ACE_Codeset_Registry::get_max_bytes(ncs);
        if (mb == 0 || ACE_static_cast(size_t,mb) > sizeof (ACE_CDR::WChar))
          {
            if (TAO_debug_level > 0)
              ACE_ERROR((LM_ERROR,
                         ACE_TEXT("(%P|%t) Invalid NativeWCharCodeSet, %x\n"),
                         ncs));
            return -1;
          }
        TAO_Codeset_Manager *csm = this->get_codeset_manager();
        if (csm)
          csm->set_ncs_w(ncs,mb);
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_LIB_TEXT("-ORBCharCodesetTranslator")) == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            TAO_Codeset_Manager *csm = this->get_codeset_manager();
            if (csm)
              if (csm->add_char_translator(argv[curarg]) == -1)
                return -1;
          }
      }

  /// CodeSet Translators
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_LIB_TEXT("-ORBWCharCodesetTranslator")) == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            TAO_Codeset_Manager *csm = this->get_codeset_manager();
            if (csm)
              if (csm->add_wchar_translator(argv[curarg]) == -1)
                return -1;
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_LIB_TEXT("-ORBConnectionCachingStrategy")) == 0)
      {
        curarg++;

        // @@todo: This needs to be removed after a few betas. The
        // note is being written during 1.2.3 timeframe.
        ACE_DEBUG ((LM_DEBUG,
                    ACE_LIB_TEXT ("(%P|%t) This option would be deprecated \n")
                    ACE_LIB_TEXT ("(%P|%t) Please use -ORBConnectionPurgingStrategy instead \n")));

        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_LIB_TEXT("lru")) == 0)
              this->connection_purging_type_ =
                TAO_Resource_Factory::LRU;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_LIB_TEXT("lfu")) == 0)
              this->connection_purging_type_ =
                TAO_Resource_Factory::LFU;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_LIB_TEXT("fifo")) == 0)
              this->connection_purging_type_ =
                TAO_Resource_Factory::FIFO;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_LIB_TEXT("null")) == 0)
              this->connection_purging_type_ =
                  TAO_Resource_Factory::NOOP;
            else
              this->report_option_value_error (ACE_LIB_TEXT("-ORBConnectionCachingStrategy"), name);
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_LIB_TEXT("-ORBConnectionPurgingStrategy")) == 0)
      {
        curarg++;

        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_LIB_TEXT("lru")) == 0)
              this->connection_purging_type_ =
                TAO_Resource_Factory::LRU;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_LIB_TEXT("lfu")) == 0)
              this->connection_purging_type_ =
                TAO_Resource_Factory::LFU;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_LIB_TEXT("fifo")) == 0)
              this->connection_purging_type_ =
                TAO_Resource_Factory::FIFO;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_LIB_TEXT("null")) == 0)
              this->connection_purging_type_ =
                  TAO_Resource_Factory::NOOP;
            else
              this->report_option_value_error (ACE_LIB_TEXT("-ORBConnectionPurgingStrategy"), name);
          }
      }
   else if (ACE_OS::strcasecmp (argv[curarg],
                                ACE_LIB_TEXT("-ORBConnectionCacheMax")) == 0)
      {
        curarg++;
        if (curarg < argc)
            this->cache_maximum_ = ACE_OS::atoi (argv[curarg]);
        else
          this->report_option_value_error (ACE_LIB_TEXT("-ORBConnectionCacheMax"), argv[curarg]);
      }

   else if (ACE_OS::strcasecmp (argv[curarg],
                                ACE_LIB_TEXT("-ORBConnectionCachePurgePercentage")) == 0)
      {
        curarg++;
        if (curarg < argc)
            this->purge_percentage_ = ACE_OS::atoi (argv[curarg]);
        else
          this->report_option_value_error (ACE_LIB_TEXT("-ORBConnectionCachePurgePercentage"),
                                           argv[curarg]);
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_LIB_TEXT("-ORBIORParser")) == 0)
      {
        curarg++;

        if (curarg < argc)
          {
            this->add_to_ior_parser_names (ACE_TEXT_ALWAYS_CHAR(argv[curarg]));
          }
      }

    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_LIB_TEXT("-ORBConnectionCacheLock")) == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_LIB_TEXT("thread")) == 0)
              this->cached_connection_lock_type_ = TAO_THREAD_LOCK;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_LIB_TEXT("null")) == 0)
              {
                // @@ Bug 940 :This is a sort of hack now. We need to put
                // this in a common place once we get the common
                // switch that is documented in bug 940...
                this->use_locked_data_blocks_  = 0;
                this->cached_connection_lock_type_ = TAO_NULL_LOCK;
              }
            else
              this->report_option_value_error (ACE_LIB_TEXT("-ORBConnectionCacheLock"), name);
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_LIB_TEXT("-ORBObjectKeyTableLock")) == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_LIB_TEXT("thread")) == 0)
              this->object_key_table_lock_type_ = TAO_THREAD_LOCK;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_LIB_TEXT("null")) == 0)
              {
                // @@ Bug 940 :This is a sort of hack now. We need to put
                // this in a common place once we get the common
                // switch that is documented in bug 940...
                this->object_key_table_lock_type_ = TAO_NULL_LOCK;
              }
            else
              this->report_option_value_error (ACE_LIB_TEXT("-ORBObjectKeyTableLock"), name);
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_LIB_TEXT("-ORBCorbaObjectLock")) == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_LIB_TEXT("thread")) == 0)
              this->corba_object_lock_type_ = TAO_THREAD_LOCK;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_LIB_TEXT("null")) == 0)
              {
                // @@ Bug 940 :This is a sort of hack now. We need to put
                // this in a common place once we get the common
                // switch that is documented in bug 940...
                this->corba_object_lock_type_ = TAO_NULL_LOCK;
              }
            else
              this->report_option_value_error (ACE_LIB_TEXT("-ORBCorbaObjectLock"), name);
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_LIB_TEXT("-ORBResourceUsage")) == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_LIB_TEXT("eager")) == 0)
              this->resource_usage_strategy_ = TAO_EAGER;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_LIB_TEXT("lazy")) == 0)
              {
                this->resource_usage_strategy_ = TAO_LAZY;
              }
            else
              this->report_option_value_error (ACE_LIB_TEXT("-ORBResourceUsage"), name);
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_LIB_TEXT("-ORBFlushingStrategy")) == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            ACE_TCHAR* name = argv[curarg];

            if (ACE_OS::strcasecmp (name,
                                    ACE_LIB_TEXT("leader_follower")) == 0)
              this->flushing_strategy_type_ = TAO_LEADER_FOLLOWER_FLUSHING;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_LIB_TEXT("reactive")) == 0)
              this->flushing_strategy_type_ = TAO_REACTIVE_FLUSHING;
            else if (ACE_OS::strcasecmp (name,
                                         ACE_LIB_TEXT("blocking")) == 0)
              this->flushing_strategy_type_ = TAO_BLOCKING_FLUSHING;
            else
              this->report_option_value_error (ACE_LIB_TEXT("-ORBFlushingStrategy"), name);
          }
      }
    else if (ACE_OS::strcasecmp (argv[curarg],
                                 ACE_LIB_TEXT ("-ORBMuxedConnectionMax")) == 0)
      {
        curarg++;
        if (curarg < argc)
            this->max_muxed_connections_ =
              ACE_OS::atoi (argv[curarg]);
        else
          this->report_option_value_error ("-ORBMuxedConnectionMax",
                                           argv[curarg]);
      }
    else if (ACE_OS::strncmp (argv[curarg],
                              ACE_LIB_TEXT ("-ORB"),
                              4) == 0)
      {
        // Can we assume there is an argument after the option?
        // curarg++;
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("Default_Resource_Factory - ")
                    ACE_LIB_TEXT ("unknown option <%s>\n"),
                    argv[curarg]));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_LIB_TEXT ("Default_Resource_Factory - ")
                    ACE_LIB_TEXT ("ignoring option <%s>\n"),
                    argv[curarg]));
      }

  TAO_Codeset_Manager *csm = this->get_codeset_manager();
  if (csm)
    csm->configure_codeset_factories();

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
  this->parser_names_count_ = 5; // HOW MANY DO WE HAVE?
  this->parser_names_ =
    new char *[this->parser_names_count_];

  CORBA::ULong index = 0;

  this->parser_names_[index] = CORBA::string_dup ("DLL_Parser");
  index++;

  this->parser_names_[index] = CORBA::string_dup ("FILE_Parser");
  index++;

  this->parser_names_[index] = CORBA::string_dup ("CORBALOC_Parser");
  index++;

  this->parser_names_[index] = CORBA::string_dup ("CORBANAME_Parser");
  index++;

  this->parser_names_[index] = CORBA::string_dup ("MCAST_Parser");
  index++;

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
      //                        ACE_LIB_TEXT ("PP_Factory"), ...)
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
                        ACE_LIB_TEXT ("TAO (%P|%t) No %s found in ")
                        ACE_LIB_TEXT ("Service Repository. ")
                        ACE_LIB_TEXT ("Using default instance IIOP ")
                        ACE_LIB_TEXT ("Protocol Factory.\n"),
                        ACE_LIB_TEXT ("IIOP Protocol Factory")));

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
                      ACE_LIB_TEXT ("TAO (%P|%t) Unable to add ")
                      ACE_LIB_TEXT ("<%s> to protocol factory set.\n"),
                      ACE_TEXT_CHAR_TO_TCHAR (item->protocol_name ().c_str ())));

          delete item;

          if (transfer_ownership == 0)
            delete protocol_factory;

          return -1;
        }

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_LIB_TEXT ("TAO (%P|%t) Loaded default ")
                      ACE_LIB_TEXT ("protocol <IIOP_Factory>\n")));
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
                             ACE_LIB_TEXT ("TAO (%P|%t) Unable to load ")
                             ACE_LIB_TEXT ("protocol <%s>, %p\n"),
                             ACE_TEXT_CHAR_TO_TCHAR(name.c_str ()),
                             ACE_LIB_TEXT ("")),
                            -1);
        }

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_LIB_TEXT ("TAO (%P|%t) Loaded protocol <%s>\n"),
                      ACE_TEXT_CHAR_TO_TCHAR(name.c_str ())));
        }
    }

  return 0;
}

#if 0
// @@todo: Need to go at a later date!
int
TAO_Default_Resource_Factory::use_tss_resources (void) const
{
  return 0;
}
#endif /*if 0*/

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

  ACE_NEW_RETURN (impl, ACE_TP_Reactor ((ACE_Sig_Handler*)0,
                                        (ACE_Timer_Queue*)0,
                                        this->reactor_mask_signals_,
                                        ACE_Select_Reactor_Token::LIFO),
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

#if TAO_USE_LOCAL_MEMORY_POOL
typedef ACE_Malloc<ACE_LOCAL_MEMORY_POOL,TAO_SYNCH_MUTEX> LOCKED_MALLOC;
typedef ACE_Allocator_Adapter<LOCKED_MALLOC> LOCKED_ALLOCATOR;
#else
typedef ACE_New_Allocator LOCKED_ALLOCATOR;
#endif

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


ACE_Lock *
TAO_Default_Resource_Factory::create_object_key_table_lock (void)
{
  ACE_Lock *the_lock = 0;

  if (this->object_key_table_lock_type_ == TAO_NULL_LOCK)
    ACE_NEW_RETURN (the_lock,
                    ACE_Lock_Adapter<ACE_SYNCH_NULL_MUTEX>,
                    0);
  else
    ACE_NEW_RETURN (the_lock,
                    ACE_Lock_Adapter<TAO_SYNCH_MUTEX>,
                    0);

  return the_lock;
}

ACE_Lock *
TAO_Default_Resource_Factory::create_corba_object_lock (void)
{
  ACE_Lock *the_lock = 0;

  if (this->corba_object_lock_type_ == TAO_NULL_LOCK)
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
                  ACE_LIB_TEXT ("TAO (%P|%t) - ")
                  ACE_LIB_TEXT ("no usable purging strategy ")
                  ACE_LIB_TEXT ("was found.\n")));
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
                                 const ACE_TCHAR* option_name,
                                 const ACE_TCHAR* option_value)
{
  ACE_DEBUG((LM_DEBUG,
             ACE_LIB_TEXT ("Default_Resource_Factory - unknown argument")
             ACE_LIB_TEXT (" <%s> for <%s>\n"),
             option_value, option_name));
}

void
TAO_Default_Resource_Factory::disable_factory (void)
{
  this->factory_disabled_ = 1;
  if (this->options_processed_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("TAO (%P|%t) Warning: Resource_Factory options ignored\n")
                  ACE_LIB_TEXT ("Default Resource Factory is disabled\n")));
    }
}

TAO_Codeset_Manager *
TAO_Default_Resource_Factory::get_codeset_manager()
{
  if (this->codeset_manager_ == 0)
    {
      ACE_NEW_RETURN (this->codeset_manager_, TAO_Codeset_Manager, 0);
    }
  return this->codeset_manager_;
}

TAO_Resource_Factory::Resource_Usage
TAO_Default_Resource_Factory::resource_usage_strategy (void)const
{
  return this->resource_usage_strategy_;
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Default_Resource_Factory,
                       ACE_LIB_TEXT ("Resource_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Default_Resource_Factory),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_Default_Resource_Factory)

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class auto_ptr<TAO_Protocol_Factory>;
#  if defined (ACE_LACKS_AUTO_PTR) \
      || !(defined (ACE_HAS_STANDARD_CPP_LIBRARY) \
           && (ACE_HAS_STANDARD_CPP_LIBRARY != 0))
template class ACE_Auto_Basic_Ptr<TAO_Protocol_Factory>;
#  endif  /* ACE_LACKS_AUTO_PTR */

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate auto_ptr<TAO_Protocol_Factory>
#  if defined (ACE_LACKS_AUTO_PTR) \
      || !(defined (ACE_HAS_STANDARD_CPP_LIBRARY) \
           && (ACE_HAS_STANDARD_CPP_LIBRARY != 0))
#    pragma instantiate ACE_Auto_Basic_Ptr<TAO_Protocol_Factory>
#  endif  /* ACE_LACKS_AUTO_PTR */

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
