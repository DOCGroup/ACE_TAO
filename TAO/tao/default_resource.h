// -*- C++ -*-

//=============================================================================
/**
 *  @file   default_resource.h
 *
 *  @author Chris Cleeland
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_DEFAULT_RESOURCE_H
#define TAO_DEFAULT_RESOURCE_H

#include /**/ "ace/pre.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Resource_Factory.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor_Impl;
ACE_END_VERSIONED_NAMESPACE_DECL

#include "ace/Timer_Queuefwd.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Object_Adapter;
class TAO_IOR_Parser;
class TAO_LF_Strategy;
class TAO_Codeset_Descriptor_Base;
class TAO_Time_Policy_Manager;
class TAO_RSF_Timer_Queue_Ptr;

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
 */
class TAO_Export TAO_Codeset_Parameters
{
public:
  TAO_Codeset_Parameters ();
  ~TAO_Codeset_Parameters ();

  /// The native codeset (getter)
  const ACE_TCHAR* native ();

  /// The native codeset (setter)
  void native (const ACE_TCHAR* n);

  /// Add a new codeset
  void add_translator (const ACE_TCHAR* name);

  typedef ACE_Unbounded_Queue_Iterator<ACE_TCHAR*> iterator;

  /// Iterate through the registered translators
  iterator translators ();

  /// Apply the parameters to the said descriptor
  void apply_to (TAO_Codeset_Descriptor_Base *csd);

private:
  TAO_Codeset_Parameters (const TAO_Codeset_Parameters &) = delete;
  TAO_Codeset_Parameters &operator= (const TAO_Codeset_Parameters &) = delete;

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
  TAO_Default_Resource_Factory ();

  /// Destructor.
  ~TAO_Default_Resource_Factory () override;

  /**
   * @name Service Configurator Hooks
   */
  //@{
  /// Dynamic linking hook
  int init (int argc, ACE_TCHAR *argv[]) override;

  /// Parse svc.conf arguments
  int parse_args (int argc, ACE_TCHAR* argv[]);
  //@}

  /**
   * @name Member Accessors
   */
  //@{
  int get_parser_names (char **&names,
                        int &number_of_names) override;
  enum
  {
    TAO_ALLOCATOR_THREAD_LOCK
  };

  /// Modify and get the source for the CDR allocators
  int cdr_allocator_source ();

  // = Resource Retrieval
  int use_locked_data_blocks () const override;
  ACE_Reactor *get_reactor () override;
  void reclaim_reactor (ACE_Reactor *) override;
  TAO_Acceptor_Registry  *get_acceptor_registry () override;
  TAO_Connector_Registry *get_connector_registry () override;
  void use_local_memory_pool (bool) override;
  ACE_Allocator* input_cdr_dblock_allocator () override;
  ACE_Allocator* input_cdr_buffer_allocator () override;
  ACE_Allocator* input_cdr_msgblock_allocator () override;
  int input_cdr_allocator_type_locked () override;
  ACE_Allocator* output_cdr_dblock_allocator () override;
  ACE_Allocator* output_cdr_buffer_allocator () override;
  ACE_Allocator* output_cdr_msgblock_allocator () override;
  ACE_Allocator* amh_response_handler_allocator () override;
  ACE_Allocator* ami_response_handler_allocator () override;
  TAO_ProtocolFactorySet *get_protocol_factories () override;

  int init_protocol_factories () override;

  TAO_Codeset_Manager * codeset_manager () override;

  int cache_maximum () const override;
  int purge_percentage () const override;
  int max_muxed_connections () const override;
  ACE_Lock *create_cached_connection_lock () override;
  int locked_transport_cache () override;
  TAO_Flushing_Strategy *create_flushing_strategy () override;
  TAO_Connection_Purging_Strategy *create_purging_strategy () override;
  TAO_Resource_Factory::Resource_Usage resource_usage_strategy () const override;
  TAO_LF_Strategy *create_lf_strategy () override;
  TAO_GIOP_Fragmentation_Strategy*
    create_fragmentation_strategy (TAO_Transport * transport,
                                   CORBA::ULong max_message_size) const override;
  void disable_factory () override;
  bool drop_replies_during_shutdown () const override;
 //@}

protected:
  friend class TAO_RSF_Timer_Queue_Ptr;

#if (TAO_HAS_TIME_POLICY == 1)
  TAO_Time_Policy_Manager* time_policy_manager () const;
#endif

  ACE_Timer_Queue * create_timer_queue () const;

  void destroy_timer_queue (ACE_Timer_Queue *tmq) const;

  /// Obtain the reactor implementation
  virtual ACE_Reactor_Impl *allocate_reactor_impl () const;

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

  int load_default_protocols () override;

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

/**
 * @class TAO_RSF_Timer_Queue_Ptr
 *
 * @brief A simple auto_ptr like class to manage timer queues dynamically
 *        allocated by a time policy.
 */
class TAO_Export TAO_RSF_Timer_Queue_Ptr
{
public:
  TAO_RSF_Timer_Queue_Ptr (TAO_Default_Resource_Factory const &, ACE_Timer_Queue*);
  ~TAO_RSF_Timer_Queue_Ptr ();

  ACE_Timer_Queue* get ();
  void release ();

private:
  TAO_Default_Resource_Factory const & resource_factory_;
  ACE_Timer_Queue * timer_queue_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/default_resource.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_DEFAULT_CLIENT_H */
