// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   default_resource.h
//
// = AUTHOR
//   Chris Cleeland
//   Carlos O'Ryan
//
// ============================================================================

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

class TAO_Export TAO_Default_Resource_Factory : public TAO_Resource_Factory
{
  // = TITLE
  //   TAO's default resource factory
  //
  // = DESCRIPTION
  //   Using a <{resource source specifier}> as a discriminator, the
  //   factory can return resource instances which are, e.g., global,
  //   stored in thread-specific storage, stored in shared memory,
  //   etc.
  //
public:
  // = Initialization and termination methods.
  TAO_Default_Resource_Factory (void);
  // Constructor.

  virtual ~TAO_Default_Resource_Factory (void);
  // Destructor.

  // = Service Configurator hooks.
  virtual int init (int argc, char* argv[]);
  // Dynamic linking hook

  int parse_args (int argc, char* argv[]);
  // Parse svc.conf arguments

  int get_parser_names (char **&names,
                        int &number_of_names);
  // = Member Accessors
  enum
  {
    TAO_ALLOCATOR_THREAD_LOCK
  };

  // = Type of Reactor
  enum
  {
    TAO_REACTOR_SELECT_MT // Use ACE_Token
  };

  // = Reactor mappings strategy
  enum
  {
    TAO_SINGLE_REACTOR,
    TAO_REACTOR_PER_PRIORITY
  };

  int cdr_allocator_source (void);
  // Modify and get the source for the CDR allocators

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
  virtual ACE_Allocator* output_cdr_dblock_allocator (void);
  virtual ACE_Allocator* output_cdr_buffer_allocator (void);
  virtual TAO_ProtocolFactorySet *get_protocol_factories (void);
  
  virtual int init_protocol_factories (void);
  
  virtual TAO_Resource_Factory::Caching_Strategy connection_caching_strategy_type (void) const;
  virtual double purge_percentage (void) const;
  virtual TAO_Priority_Mapping *get_priority_mapping (void);
  virtual ACE_Lock *create_cached_connection_lock (void);

protected:
  virtual ACE_Reactor_Impl *allocate_reactor_impl (void) const;
  // Obtain the reactor implementation

  int add_to_ior_parser_names (const char *);
  // Add a Parser name to the list of Parser names.

protected:
  int use_tss_resources_;
  // Flag indicating whether resources should be global or
  // thread-specific.

  int use_locked_data_blocks_;
  // The type of data blocks that the ORB should use

  int parser_names_count_;
  // The number of the different types of Parsers.

  char **parser_names_;
  // Array consisting of the names of the parsers

  int index_;
  // Index of the current element in the parser_names_ array

  TAO_ProtocolFactorySet protocol_factories_;
  // list of loaded protocol factories.

  TAO_Resource_Factory::Caching_Strategy connection_caching_type_;
  // Specifies the typeof caching strategy we should use for
  // connection management.

  double purge_percentage_;
  // Specifies the percentage of entries which should get purged on
  // demand.

  int reactor_mask_signals_;
  // If <0> then we create reactors with signal handling disabled.

  int sched_policy_;
  // The scheduling policy used to initialize the priority mapping
  // strategy.

  int priority_mapping_type_;
  // The type of priority mapping class created by this factory.

  int dynamically_allocated_reactor_;
  // Flag that is set to 1 if the reactor obtained from the
  // get_reactor() method is dynamically allocated.  If this flag is
  // set to 1, then the reclaim_reactor() method with call the delete
  // operator on the given reactor.  This flag is necessary to make
  // sure that a reactor not allocated by the default resource factory
  // is not reclaimed by the default resource factory.  Such a
  // situation can occur when a resource factory derived from the
  // default one overrides the get_reactor() method but does not
  // override the reclaim_reactor() method.

  virtual int load_default_protocols (void);

private:
  enum Lock_Type
  {
    TAO_NULL_LOCK,
    TAO_THREAD_LOCK
  };

  Lock_Type cached_connection_lock_type_;
  // Type of lock used by the cached connector.
};

#if defined (__ACE_INLINE__)
#include "tao/default_resource.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE (TAO_Default_Resource_Factory)
ACE_FACTORY_DECLARE (TAO, TAO_Default_Resource_Factory)

#include "ace/post.h"
#endif /* TAO_DEFAULT_CLIENT_H */
