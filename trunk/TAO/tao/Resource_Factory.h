// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Resource_Factory.h
//
// = AUTHOR
//   Chris Cleeland
//   Carlos O'Ryan
//
// ============================================================================

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
#include "ace/Containers_T.h"

class TAO_Acceptor_Registry;
class TAO_Connector_Registry;
class TAO_Reactor_Registry;
class TAO_Priority_Mapping;

// ****************************************************************

class TAO_Export TAO_Cached_Connector_Lock : public ACE_Adaptive_Lock
{
  // TITLE
  //   This lock class determines the type underlying lock
  //   when it gets constructed.
public:
  TAO_Cached_Connector_Lock (TAO_ORB_Core *orb_core = 0);
  ~TAO_Cached_Connector_Lock (void);
};

// ****************************************************************

class TAO_Export TAO_Protocol_Item
{
public:
  TAO_Protocol_Item (const ACE_CString &name);
  // creator method, the protocol name can only be set when the
  // object is created.

  ~TAO_Protocol_Item (void);
  // destructor that deallocates the factory object if the
  // Protocol_Item retains ownership.

  const ACE_CString &protocol_name (void);
  // return a reference to the character representation of the protocol
  // factories name.

  TAO_Protocol_Factory *factory (void);
  // return a pointer to the protocol factory.

  void factory (TAO_Protocol_Factory *factory, int owner = 0);
  // set the factory pointer's value.

private:
  // Prohibited
  ACE_UNIMPLEMENTED_FUNC (TAO_Protocol_Item (const TAO_Protocol_Item&))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Protocol_Item&))

private:
  ACE_CString name_;
  // protocol factory name.

  TAO_Protocol_Factory *factory_;
  // pointer to factory object.

  int factory_owner_;
  // whether we own (and therefore have to delete) the factory object.
};

// typedefs for containers containing the list of loaded protocol
// factories.
typedef ACE_Unbounded_Set<TAO_Protocol_Item*>
        TAO_ProtocolFactorySet;

typedef ACE_Unbounded_Set_Iterator<TAO_Protocol_Item*>
        TAO_ProtocolFactorySetItor;

// ****************************************************************

class TAO_Export TAO_Resource_Factory : public ACE_Service_Object
{
  // = TITLE
  //   Factory which manufacturers resources for use by the ORB Core.
  //
  // = DESCRIPTION
  //   This class is a factory/repository for critical ORB Core
  //   resources.
  //
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

  virtual int use_tss_resources (void) const;
  // @@ Backwards compatibility, return 1 if the ORB core should use
  //    TSS resources

  virtual int use_locked_data_blocks (void) const;
  // @@ Backwards compatibility, return 1 if the ORB core should use
  //    Locked_Data_Blocks

  virtual TAO_Reactor_Registry *get_reactor_registry (void);
  // Create the reactor holder, an strategy to control the number of
  // reactors in the ORB

  virtual ACE_Reactor *get_reactor (void);
  // Return an <ACE_Reactor> to be utilized.

  virtual void reclaim_reactor (ACE_Reactor *reactor);
  // Reclaim reactor resources (e.g. deallocate, etc).

  virtual TAO_Acceptor_Registry *get_acceptor_registry (void);
  // return a reference to the acceptor registry.

  virtual TAO_Connector_Registry *get_connector_registry (void);
  // Return an Connector to be utilized.

  virtual ACE_Allocator* input_cdr_dblock_allocator (void);
  virtual ACE_Allocator* input_cdr_buffer_allocator (void);
  // Access the input CDR allocators.

  virtual ACE_Allocator* output_cdr_dblock_allocator (void);
  virtual ACE_Allocator* output_cdr_buffer_allocator (void);
  // Access the output CDR allocators.

  virtual TAO_ProtocolFactorySet *get_protocol_factories (void);
  // The protocol factory list is implemented in this class since
  // a) it will be a global resource and
  // b) it is initialized at start up and then not altered.
  // Returns a container holding the list of loaded protocols.

  virtual int init_protocol_factories (void);
  // this method will loop through the protocol list and
  // using the protocol name field this method will
  // retrieve a pointer to the associated protocol factory
  // from the service configurator.  It is assumed
  // that only one thread will call this method at ORB initialization.
  // NON-THREAD-SAFE

  virtual Caching_Strategy connection_caching_strategy_type (void) const;
  // This accesses the connection caching strategy we use for managing
  // purging of unused entries from the connection cache on demnad.

  virtual double purge_percentage (void) const;
  // This denotes the amount of entries to remove from the connection
  // cache.

  virtual TAO_Priority_Mapping *get_priority_mapping (void);
  // Configure the priority mapping for the ORB

  virtual int get_parser_names (char **&names,
                                int &number_of_names);
};

#include "ace/post.h"
#endif /* TAO_RESOURCE_FACTORY_H */
