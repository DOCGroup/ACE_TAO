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
#include "ace/pre.h"

#include "tao/Resource_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"

class TAO_Object_Adapter;
class TAO_IOR_Parser;

/**
 * @class TAO_Default_Resource_Factory
 *
 * @brief TAO's default resource factory
 *
 * Using a <{resource source specifier}> as a discriminator, the
 * factory can return resource instances which are, e.g., global,
 * stored in thread-specific storage, stored in shared memory,
 * etc.
 */
class TAO_Export TAO_Default_Resource_Factory : public TAO_Resource_Factory
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_Default_Resource_Factory (void);

  /// Destructor.
  virtual ~TAO_Default_Resource_Factory (void);

  // = Service Configurator hooks.
  /// Dynamic linking hook
  virtual int init (int argc, char* argv[]);

  /// Parse svc.conf arguments
  int parse_args (int argc, char* argv[]);

  /// = Member Accessors
  int get_parser_names (char **&names,
                        int &number_of_names);
  enum
  {
    TAO_ALLOCATOR_THREAD_LOCK
  };

  // = Reactor mappings strategy
  enum
  {
    TAO_SINGLE_REACTOR,
    TAO_REACTOR_PER_PRIORITY
  };

  /// Modify and get the source for the CDR allocators
  int cdr_allocator_source (void);

  // = Resource Retrieval
  virtual int use_tss_resources (void) const;
  virtual int use_locked_data_blocks (void) const;
  virtual TAO_Reactor_Registry *get_reactor_registry (void);
  virtual ACE_Reactor *get_reactor (void);
  virtual void reclaim_reactor (ACE_Reactor *);
  virtual TAO_Acceptor_Registry  *get_acceptor_registry (void);
  virtual TAO_Connector_Registry *get_connector_registry (void);
  virtual ACE_Allocator* input_cdr_dblock_allocator (void);
  virtual ACE_Allocator* input_cdr_buffer_allocator (void);
  virtual int input_cdr_allocator_type_locked (void);
  virtual ACE_Allocator* output_cdr_dblock_allocator (void);
  virtual ACE_Allocator* output_cdr_buffer_allocator (void);
  virtual TAO_ProtocolFactorySet *get_protocol_factories (void);

  virtual int init_protocol_factories (void);

  virtual TAO_Resource_Factory::Caching_Strategy connection_caching_strategy_type (void) const;
  virtual double purge_percentage (void) const;
  virtual TAO_Priority_Mapping *get_priority_mapping (void);
  virtual ACE_Lock *create_cached_connection_lock (void);
  virtual TAO_Flushing_Strategy *create_flushing_strategy (void);

protected:
  /// Obtain the reactor implementation
  virtual ACE_Reactor_Impl *allocate_reactor_impl (void) const;

  /// Add a Parser name to the list of Parser names.
  int add_to_ior_parser_names (const char *);

protected:
  /// Flag indicating whether resources should be global or
  /// thread-specific.
  int use_tss_resources_;

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

  /// Specifies the typeof caching strategy we should use for
  /// connection management.
  TAO_Resource_Factory::Caching_Strategy connection_caching_type_;

  /// Specifies the percentage of entries which should get purged on
  /// demand.
  double purge_percentage_;

  /// If <0> then we create reactors with signal handling disabled.
  int reactor_mask_signals_;

  /// The scheduling policy used to initialize the priority mapping
  /// strategy.
  int sched_policy_;

  /// The type of priority mapping class created by this factory.
  int priority_mapping_type_;

  /**
   * Flag that is set to 1 if the reactor obtained from the
   * get_reactor() method is dynamically allocated.  If this flag is
   * set to 1, then the reclaim_reactor() method with call the delete
   * operator on the given reactor.  This flag is necessary to make
   * sure that a reactor not allocated by the default resource factory
   * is not reclaimed by the default resource factory.  Such a
   * situation can occur when a resource factory derived from the
   * default one overrides the get_reactor() method but does not
   * override the reclaim_reactor() method.
   */
  int dynamically_allocated_reactor_;

  virtual int load_default_protocols (void);

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
    TAO_REACTIVE_FLUSHING,
    TAO_BLOCKING_FLUSHING
  };

  /// Type of flushing strategy configured
  int flushing_strategy_type_;
};

#if defined (__ACE_INLINE__)
#include "tao/default_resource.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_Default_Resource_Factory)
ACE_FACTORY_DECLARE (TAO, TAO_Default_Resource_Factory)

#include "ace/post.h"
#endif /* TAO_DEFAULT_CLIENT_H */
