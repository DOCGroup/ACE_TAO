// -*- C++ -*-

//=============================================================================
/**
 *  @file   default_resource.h
 *
 *  $Id$
 *
 *  @author Chris Cleeland
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_DEFAULT_RESOURCE_H
#define TAO_DEFAULT_RESOURCE_H

#include /**/ "ace/pre.h"
#include "ace/Copy_Disabled.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Resource_Factory.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor_Impl;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Object_Adapter;
class TAO_IOR_Parser;
class TAO_LF_Strategy;
class TAO_Codeset_Descriptor_Base;

/**
 * @class TAO_Codeset_Parameters
 *
 * @brief A simple storage class for the native codeset and any
 * translators that must be configured when creating an instance of
 * codeset manager.
 *
 * The Resource Factory uses two instances of this class during its
 * initialization, to capture any native codeset or translators
 * settings. The RF later uses these parameters when creating
 * instances of the codeset manager.
 *
 * Perhaps, the best would be to place the responsibility for codeset
 * manager's configuration with the the codeset manager factory?
 *
 */
class TAO_Export TAO_Codeset_Parameters
  : public ACE_Copy_Disabled
{
public:
  TAO_Codeset_Parameters (void);
  ~TAO_Codeset_Parameters (void);

  /// The native codeset (getter)
  const ACE_TCHAR* native (void);

  /// The native codeset (setter)
  void native (const ACE_TCHAR* n);

  /// Add a new codeset
  void add_translator (const ACE_TCHAR* name);

  typedef ACE_Unbounded_Queue_Iterator<ACE_TCHAR*> iterator;

  /// Iterate through the registered translators
  iterator translators (void);

  /// Apply the parameters to the said descriptor
  void apply_to (TAO_Codeset_Descriptor_Base *csd);

private:
  ACE_Unbounded_Queue<ACE_TCHAR*> translators_;
  ACE_TCHAR* native_;
};



/**
 * @class TAO_Default_Resource_Factory
 *
 * @brief TAO's default resource factory
 *
 * Using a <{resource source specifier}> as a discriminator, the
 * factory can return resource instances which are, e.g., global,
 * stored in thread-specific storage, stored in shared memory,
 * etc.
 *
 * @note When using an ORB created by a dynamically loaded object, it
 *       is generally necessary to pre-load a "Resource_Factory" prior
 *       to initializing the ORB.  In the case of the
 *       TAO_Default_Resource_Factory, this can be done by adding the
 *       following Service Configurator directive to your `svc.conf'
 *       file before your the directive that loads the object that
 *       initialized your ORB:
 * @par
 *           static Resource_Factory ""
 * @par
 *       Alternatively, explicitly pre-load the Resource_Factory using
 *       the following in your code:
 * @par
 *           ACE_Service_Config::process_directive (
 *             ACE_TEXT ("static Resource_Factory \"\"") );
 * @par
 *       In both cases, place the appropriate resource factory
 *       arguments, if any, between the quotes immediately following
 *       "Resource_Factory."
 */
class TAO_Export TAO_Default_Resource_Factory
  : public TAO_Resource_Factory
{
public:

  /// Constructor.
  TAO_Default_Resource_Factory (void);

  /// Destructor.
  virtual ~TAO_Default_Resource_Factory (void);

  /**
   * @name Service Configurator Hooks
   */
  //@{
  /// Dynamic linking hook
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Parse svc.conf arguments
  int parse_args (int argc, ACE_TCHAR* argv[]);
  //@}

  /**
   * @name Member Accessors
   */
  //@{

  int get_parser_names (char **&names,
                        int &number_of_names);
  enum
  {
    TAO_ALLOCATOR_THREAD_LOCK
  };

  /// Modify and get the source for the CDR allocators
  int cdr_allocator_source (void);

  // = Resource Retrieval
  virtual int use_locked_data_blocks (void) const;
  virtual ACE_Reactor *get_reactor (void);
  virtual void reclaim_reactor (ACE_Reactor *);
  virtual TAO_Acceptor_Registry  *get_acceptor_registry (void);
  virtual TAO_Connector_Registry *get_connector_registry (void);
  virtual void use_local_memory_pool (bool);
  virtual ACE_Allocator* input_cdr_dblock_allocator (void);
  virtual ACE_Allocator* input_cdr_buffer_allocator (void);
  virtual ACE_Allocator* input_cdr_msgblock_allocator (void);
  virtual int input_cdr_allocator_type_locked (void);
  virtual ACE_Allocator* output_cdr_dblock_allocator (void);
  virtual ACE_Allocator* output_cdr_buffer_allocator (void);
  virtual ACE_Allocator* output_cdr_msgblock_allocator (void);
  virtual ACE_Allocator* amh_response_handler_allocator (void);
  virtual ACE_Allocator* ami_response_handler_allocator (void);
  virtual TAO_ProtocolFactorySet *get_protocol_factories (void);

  virtual int init_protocol_factories (void);

  virtual TAO_Codeset_Manager * codeset_manager (void);

  virtual int cache_maximum (void) const;
  virtual int purge_percentage (void) const;
  virtual int max_muxed_connections (void) const;
  virtual ACE_Lock *create_cached_connection_lock (void);
  virtual int locked_transport_cache (void);
  virtual TAO_Flushing_Strategy *create_flushing_strategy (void);
  virtual TAO_Connection_Purging_Strategy *create_purging_strategy (void);
  TAO_Resource_Factory::Resource_Usage resource_usage_strategy (void) const;
  virtual TAO_LF_Strategy *create_lf_strategy (void);
  virtual auto_ptr<TAO_GIOP_Fragmentation_Strategy>
    create_fragmentation_strategy (TAO_Transport * transport,
                                   CORBA::ULong max_message_size) const;
  virtual void disable_factory (void);
  virtual bool drop_replies_during_shutdown (void) const;
  //@}

protected:

  /// Obtain the reactor implementation
  virtual ACE_Reactor_Impl *allocate_reactor_impl (void) const;

  /// Add a Parser name to the list of Parser names.
  int add_to_ior_parser_names (const char *);

  void report_option_value_error (const ACE_TCHAR* option_name,
                                  const ACE_TCHAR* option_value);

protected:

  /// The type of data blocks that the ORB should use
  int use_locked_data_blocks_;

  /// The number of the different types of Parsers.
  int parser_names_count_;

  /// Array consisting of the names of the parsers
  char **parser_names_;

  /// Index of the current element in the parser_names_ array
  int index_;

  /// list of loaded protocol factories.
  TAO_ProtocolFactorySet protocol_factories_;

  /// Specifies the typeof purging strategy we should use for cleaning
  /// up unused connections
  TAO_Resource_Factory::Purging_Strategy connection_purging_type_;

  /// Specifies the maximum number of connections which should get cached
  /// in the ORB.
  int cache_maximum_;

  /// Specifies the percentage of entries which should get purged on
  /// demand.
  int purge_percentage_;

  /// Specifies the limit on the number of muxed connections
  /// allowed per-property for the ORB. A value of 0 indicates no
  /// limit
  int max_muxed_connections_;

  /// If 0 then we create reactors with signal handling disabled.
  int reactor_mask_signals_;

  /**
   * Flag that is set to true if the reactor obtained from the
   * get_reactor() method is dynamically allocated.  If this flag is
   * set to true, then the reclaim_reactor() method with call the delete
   * operator on the given reactor.  This flag is necessary to make
   * sure that a reactor not allocated by the default resource factory
   * is not reclaimed by the default resource factory.  Such a
   * situation can occur when a resource factory derived from the
   * default one overrides the get_reactor() method but does not
   * override the reclaim_reactor() method.
   */
  bool dynamically_allocated_reactor_;

  virtual int load_default_protocols (void);

  /// This flag is used to determine whether options have been
  /// processed via the init() function.  It is necessary to
  /// properly report errors when the default factory is replaced.
  int options_processed_;

  /// This flag specifies whether the factory has been disabled.
  /// If it has been disabled we should print warnings if options
  /// were processed before (or later).
  int factory_disabled_;

  enum Output_CDR_Allocator_Type
    {
      LOCAL_MEMORY_POOL,
#if TAO_HAS_SENDFILE == 1
      MMAP_ALLOCATOR,
#endif  /* TAO_HAS_SENDFILE == 1*/
      DEFAULT
    };

  /// Type of allocator to use for output CDR buffers.
  Output_CDR_Allocator_Type output_cdr_allocator_type_;

  /// This flag is used to determine whether the CDR allocators
  /// should use the local memory pool or not.
  bool use_local_memory_pool_;

private:
  enum Lock_Type
  {
    TAO_NULL_LOCK,
    TAO_THREAD_LOCK
  };

  /// Type of lock used by the cached connector.
  Lock_Type cached_connection_lock_type_;

  enum Flushing_Strategy_Type
  {
    TAO_LEADER_FOLLOWER_FLUSHING,
    TAO_REACTIVE_FLUSHING,
    TAO_BLOCKING_FLUSHING
  };

  /// Type of flushing strategy configured
  Flushing_Strategy_Type flushing_strategy_type_;

  /// Initialization options. To be used later when creating a codeset
  /// manager instance (s)
  TAO_Codeset_Parameters char_codeset_parameters_;
  TAO_Codeset_Parameters wchar_codeset_parameters_;

  /// Resource usage strategy
  Resource_Usage resource_usage_strategy_;

  /// Flag to indicate whether replies should be dropped during ORB
  /// shutdown.
  bool drop_replies_;
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_Default_Resource_Factory)
ACE_FACTORY_DECLARE (TAO, TAO_Default_Resource_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_DEFAULT_CLIENT_H */
