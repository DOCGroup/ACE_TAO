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

#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager.h"
#include "tao/Pluggable.h"
#include "tao/Protocol_Factory.h"
#include "tao/IIOP_Connector.h"
#include "tao/corbafwd.h"
#include "tao/orbconf.h"
#include "ace/Containers_T.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Connector_Registry.h"

class TAO_Client_Connection_Handler;
class TAO_POA;
class TAO_Object_Adapter;

// ****************************************************************

class TAO_Cached_Connector_Lock : public ACE_Adaptive_Lock
{
  // TITLE
  //   This lock class determines the type underlying lock
  //   when it gets constructed.
public:
  TAO_Cached_Connector_Lock (void);
  ~TAO_Cached_Connector_Lock (void);
};

typedef ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler,
                                    TAO_SOCK_CONNECTOR,
                                    TAO_Cached_Connector_Lock>
        TAO_CACHED_CONNECT_STRATEGY;

// ****************************************************************

class TAO_Collocation_Table_Lock : public ACE_Adaptive_Lock
{
  // TITLE
  //   This lock class determines the type underlying lock
  //   when it gets constructed.
public:
  TAO_Collocation_Table_Lock (void);
  ~TAO_Collocation_Table_Lock (void);
};

typedef ACE_Hash_Map_Manager<ACE_INET_Addr, TAO_Object_Adapter *, TAO_Collocation_Table_Lock> TAO_GLOBAL_Collocation_Table;

// ****************************************************************

typedef ACE_NOOP_Creation_Strategy<TAO_Client_Connection_Handler>
        TAO_NULL_CREATION_STRATEGY;

typedef ACE_NOOP_Concurrency_Strategy<TAO_Client_Connection_Handler>
        TAO_NULL_ACTIVATION_STRATEGY;

// ****************************************************************
class TAO_Export TAO_Protocol_Item 
{
public:
  TAO_Protocol_Item (const ACE_CString &name);
  // creator method, the protocol name can only be set when the
  // object is created.

  const ACE_CString &protocol_name (void);
  // return a reference to the character representation of the protocol
  // factories name.

  const TAO_Protocol_Factory *factory (void);
  // return a pointer to the protocol factory.

  void factory (TAO_Protocol_Factory *factory);
  // set the factory pointer's valus.

private:
  ACE_CString name_;
  // protocol factory name.

  TAO_Protocol_Factory *factory_;
  // pointer to factory object.
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
  // = Initialization and termination methods.
  TAO_Resource_Factory (void);
  virtual ~TAO_Resource_Factory (void);

  // = Resource Retrieval

  virtual ACE_Reactor *get_reactor (void);
  // Return an <ACE_Reactor> to be utilized.

  virtual ACE_Thread_Manager *get_thr_mgr (void);
  // Return an <ACE_Thread_Manager> to be utilized.

  virtual TAO_Acceptor_Registry *get_acceptor_registry (void);
  // return a reference to the acceptor registry.

  virtual TAO_Connector_Registry *get_connector_registry (void);
  // Return an Connector to be utilized.

  virtual TAO_CACHED_CONNECT_STRATEGY *get_cached_connect_strategy (void);
  // Return an Cached Connect Strategy to be utilized.

  virtual TAO_NULL_CREATION_STRATEGY *get_null_creation_strategy (void);
  // This no-op creation strategy is necessary for using the
  // <Strategy_Connector> with the <Cached_Connect_Strategy>.

  virtual TAO_NULL_ACTIVATION_STRATEGY *get_null_activation_strategy (void);
  // This no-op activation strategy prevents the cached connector from
  // calling the service handler's <open> method multiple times.

  virtual TAO_POA *get_root_poa (void);
  // Return a root poa to be utilized.

  virtual TAO_Object_Adapter *object_adapter (void);
  // Return a object adapter to be utilized.

  virtual TAO_GLOBAL_Collocation_Table *get_global_collocation_table (void);
  // Get the global collocation table.  Return the pointer to the
  // global collocation table if we are using one, otherwise, return
  // 0.

  virtual ACE_Allocator* input_cdr_dblock_allocator (void);
  virtual ACE_Allocator* input_cdr_buffer_allocator (void);
  // Access the input CDR allocators.

  virtual ACE_Allocator* output_cdr_dblock_allocator (void);
  virtual ACE_Allocator* output_cdr_buffer_allocator (void);
  // Access the output CDR allocators.

  virtual ACE_Data_Block *create_input_cdr_data_block (size_t size);
  // The Message Blocks used for input CDRs must have appropiate
  // locking strategies.

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
};

#endif /* TAO_RESOURCE_FACTORY_H */
