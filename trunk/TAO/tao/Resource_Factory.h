// -*- C++ -*-

//=============================================================================
/**
 *  @file   Resource_Factory.h
 *
 *  $Id$
 *
 *  @author Chris CleelandCarlos O'Ryan
 */
//=============================================================================


#ifndef TAO_RESOURCE_FACTORY_H
#define TAO_RESOURCE_FACTORY_H
#include "ace/pre.h"

#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Export.h"
#include "tao/Pluggable.h"
#include "tao/Protocol_Factory.h"
#include "tao/orbconf.h"
#include "ace/Unbounded_Set.h"

class TAO_Acceptor_Registry;
class TAO_Connector_Registry;
class TAO_Reactor_Registry;

class TAO_Flushing_Strategy;
class TAO_Connection_Purging_Strategy;
class TAO_LF_Strategy;

// ****************************************************************

class TAO_Export TAO_Protocol_Item
{
public:
  /// creator method, the protocol name can only be set when the
  /// object is created.
  TAO_Protocol_Item (const ACE_CString &name);

  /// destructor that deallocates the factory object if the
  /// Protocol_Item retains ownership.
  ~TAO_Protocol_Item (void);

  /// return a reference to the character representation of the protocol
  /// factories name.
  const ACE_CString &protocol_name (void);

  /// return a pointer to the protocol factory.
  TAO_Protocol_Factory *factory (void);

  /// set the factory pointer's value.
  void factory (TAO_Protocol_Factory *factory, int owner = 0);

private:
  // Prohibited
  ACE_UNIMPLEMENTED_FUNC (TAO_Protocol_Item (const TAO_Protocol_Item&))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Protocol_Item&))

private:
  /// protocol factory name.
  ACE_CString name_;

  /// pointer to factory object.
  TAO_Protocol_Factory *factory_;

  /// whether we own (and therefore have to delete) the factory object.
  int factory_owner_;
};

// typedefs for containers containing the list of loaded protocol
// factories.
typedef ACE_Unbounded_Set<TAO_Protocol_Item*>
        TAO_ProtocolFactorySet;

typedef ACE_Unbounded_Set_Iterator<TAO_Protocol_Item*>
        TAO_ProtocolFactorySetItor;

// ****************************************************************

/**
 * @class TAO_Resource_Factory
 *
 * @brief Factory which manufacturers resources for use by the ORB Core.
 *
 * This class is a factory/repository for critical ORB Core
 * resources.
 */
class TAO_Export TAO_Resource_Factory : public ACE_Service_Object
{
public:

  enum Caching_Strategy
  {
    // Least Recently Used
    LRU,

    // Least Frequently Used
    LFU,

    // First In First Out
    FIFO,

    // Dont use any strategy.
    NOOP
  };

  // = Initialization and termination methods.
  TAO_Resource_Factory (void);
  virtual ~TAO_Resource_Factory (void);

  // = Resource Retrieval

  /// @@ Backwards compatibility, return 1 if the ORB core should use
  ///    TSS resources
  virtual int use_tss_resources (void) const;

  /// @@ Backwards compatibility, return 1 if the ORB core should use
  ///    Locked_Data_Blocks
  virtual int use_locked_data_blocks (void) const;

  /// Create the reactor holder, an strategy to control the number of
  /// reactors in the ORB
  virtual TAO_Reactor_Registry *get_reactor_registry (void);

  /// Return an <ACE_Reactor> to be utilized.
  virtual ACE_Reactor *get_reactor (void);

  /// Reclaim reactor resources (e.g. deallocate, etc).
  virtual void reclaim_reactor (ACE_Reactor *reactor);

  /// return a reference to the acceptor registry.
  virtual TAO_Acceptor_Registry *get_acceptor_registry (void);

  /// Return an Connector to be utilized.
  virtual TAO_Connector_Registry *get_connector_registry (void);

  /// Access the input CDR allocators.
  virtual ACE_Allocator* input_cdr_dblock_allocator (void);
  virtual ACE_Allocator* input_cdr_buffer_allocator (void);
  virtual ACE_Allocator* input_cdr_msgblock_allocator (void);

  // Return 1 when the input CDR allocator uses a lock else 0.
  virtual int input_cdr_allocator_type_locked (void);

  /// Access the output CDR allocators.
  virtual ACE_Allocator* output_cdr_dblock_allocator (void);
  virtual ACE_Allocator* output_cdr_buffer_allocator (void);
  virtual ACE_Allocator* output_cdr_msgblock_allocator (void);

  /**
   * The protocol factory list is implemented in this class since
   * a) it will be a global resource and
   * b) it is initialized at start up and then not altered.
   * Returns a container holding the list of loaded protocols.
   */
  virtual TAO_ProtocolFactorySet *get_protocol_factories (void);

  /**
   * this method will loop through the protocol list and
   * using the protocol name field this method will
   * retrieve a pointer to the associated protocol factory
   * from the service configurator.  It is assumed
   * that only one thread will call this method at ORB initialization.
   * NON-THREAD-SAFE
   */
  virtual int init_protocol_factories (void);

  /// This accesses the connection caching strategy we use for managing
  /// purging of unused entries from the connection cache on demnad.
  virtual Caching_Strategy connection_caching_strategy_type (void) const;

  /// This denotes the maximum number of connections that can be cached.
  virtual int cache_maximum (void) const;

  /// This denotes the amount of entries to remove from the connection
  /// cache.
  virtual int purge_percentage (void) const;

  virtual int get_parser_names (char **&names,
                                int &number_of_names);

  /// Creates the lock for the lock needed in the Cache Map
  virtual ACE_Lock *create_cached_connection_lock (void);

  /// Creates the flushing strategy.  The new instance is owned by the
  /// caller.
  virtual TAO_Flushing_Strategy *create_flushing_strategy (void) = 0;

  /// Creates the connection purging strategy.
  virtual TAO_Connection_Purging_Strategy *create_purging_strategy (void) = 0;

  /// Creates the leader followers strategy.  The new instance is owned by the
  /// caller.
  virtual TAO_LF_Strategy *create_lf_strategy (void) = 0;

  /// Disables the factory.  When a new factory is installed and used,
  /// this function should be called on the previously used (default)
  /// factory.  This should result in proper error reporting if the
  /// user attempts to set options on an unused factory.
  virtual void disable_factory (void) = 0;

protected:
  /**
   * Loads the default protocols. This method is used so that the
   * advanced_resource.cpp can call the one in default_resource.cpp
   * without calling unnecessary functions.
   */
  virtual int load_default_protocols (void);

};

#include "ace/post.h"
#endif /* TAO_RESOURCE_FACTORY_H */
