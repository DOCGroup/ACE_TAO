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

#include "ace/Strategies_T.h"
#include "ace/Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Resource_Factory.h"
#include "tao/IIOP_Acceptor.h"
#include "tao/POA.h"

// ****************************************************************

class TAO_Default_Reactor : public ACE_Reactor
{
  // = TITLE
  //
  //   Force TAO to use Select Reactor.
public:
  TAO_Default_Reactor (int nolock = 0);
  virtual ~TAO_Default_Reactor (void);
};

// ****************************************************************

class TAO_Export TAO_Pre_Allocated_Resources
{
  //
  // = TITLE
  //   Container for pre-allocated resources.
  //
  // = DESCRIPTION
  //   Structure containing resources which can be pre-allocated by
  //   the default Resource Factory without intervention from the
  //   application.
  //
public:
  TAO_Pre_Allocated_Resources (void);
  // Constructor

  ~TAO_Pre_Allocated_Resources (void);
  // Destructor

  ACE_Thread_Manager tm_;
  // The Thread Manager

  TAO_Connector_Registry cr_;
  // The Connector Registry!

  TAO_IIOP_Connector c_;
  // The Connector, HACK to create the first connector which happens
  // to be IIOP.

  TAO_NULL_CREATION_STRATEGY null_creation_strategy_;
  // This no-op creation strategy is necessary for using the
  // <Strategy_Connector> with the <Cached_Connect_Strategy>.

  TAO_NULL_ACTIVATION_STRATEGY null_activation_strategy_;
  // This no-op activation strategy prevents the cached connector from
  // calling the service handler's <open> method multiple times.

  TAO_IIOP_Acceptor a_;
  // The Acceptor

  TAO_ORB_Parameters orbparams_;
  // ORB Parameters
};

// ****************************************************************

class TAO_Export TAO_App_Allocated_Resources
{
  // = TITLE
  //   Container for application allocated resources.
  //
  // = DESCRIPTION
  //   Structure containing resources which can only be allocated
  //   after obtaining information from the application such as
  //   arguments, etc.
  //
public:
  TAO_App_Allocated_Resources (void);
  // Constructor necessary because we have pointers.  It's inlined
  // here rather than in the .i file because it's easier than trying
  // to re-order header files in corba.h to eliminate the "used
  // before declared inline" warnings/errors on certain compilers.

  ~TAO_App_Allocated_Resources (void);
  // Destructor is also necessary because we now allocate some of
  // the objects held here.

  // = Resources

  TAO_Default_Reactor *r_;
  // The Reactor.

  TAO_Object_Adapter *object_adapter_;
  // Object Adapter.

  TAO_CACHED_CONNECT_STRATEGY *cached_connect_strategy_;
  // The Cached Connect Strategy

  TAO_POA *poa_;
  // Pointer to application-created POA.

  ACE_Allocator *input_cdr_dblock_allocator_;
  ACE_Allocator *input_cdr_buffer_allocator_;
  // The allocators for the input CDR streams.

  ACE_Allocator *output_cdr_dblock_allocator_;
  ACE_Allocator *output_cdr_buffer_allocator_;
  // The allocators for the output CDR streams.
};

// ****************************************************************

class TAO_Default_Resource_Factory : public TAO_Resource_Factory
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

  // = Member Accessors
  enum
  {
    TAO_GLOBAL,
    TAO_TSS
  };

  // = Type of Reactor
  enum
  {
    TAO_TOKEN,     // Use ACE_Token as Select_Reactor's internal lock
    TAO_NULL_LOCK  // Use ACE_Noop_Token as Select_Reactor's internal lock
  };

  // = Range of values for <{resource source specifier}>.
  virtual void resource_source (int which_source);
  // Set the resource source specifier.
  virtual int resource_source (void);
  // Get the resource source specifier.

  virtual void poa_source (int which_source);
  // Set the POA source specifier.
  virtual int poa_source (void);
  // Get the POA source specifier.

  int cdr_allocator_source (void);
  // Modify and get the source for the CDR allocators

  // = Resource Retrieval
  virtual ACE_Reactor *get_reactor (void);
  virtual ACE_Thread_Manager *get_thr_mgr (void);
  virtual TAO_Connector *get_connector (void);
  virtual TAO_Connector_Registry *get_connector_registry (void);
  virtual TAO_CACHED_CONNECT_STRATEGY *get_cached_connect_strategy (void);
  virtual TAO_NULL_CREATION_STRATEGY *get_null_creation_strategy (void);
  virtual TAO_NULL_ACTIVATION_STRATEGY *get_null_activation_strategy (void);
  virtual TAO_Acceptor *get_acceptor (void);
  virtual TAO_POA *get_root_poa (void);
  virtual TAO_Object_Adapter *object_adapter (void);
  virtual TAO_GLOBAL_Collocation_Table *get_global_collocation_table (void);
  virtual int reactor_lock (void);
  virtual ACE_Allocator* input_cdr_dblock_allocator (void);
  virtual ACE_Allocator* input_cdr_buffer_allocator (void);
  virtual ACE_Allocator* output_cdr_dblock_allocator (void);
  virtual ACE_Allocator* output_cdr_buffer_allocator (void);
  virtual ACE_Data_Block *create_input_cdr_data_block (size_t size);

protected:

  int resource_source_;
  // Flag indicating whether resources should be global or
  // thread-specific.

  int poa_source_;
  // Flag indicating whether the POA should be global or
  // thread-specific.  If not set specifically, this takes on the
  // value of <resource_source_>.

  int collocation_table_source_;
  // Flag indicating whether the collocation table should be global
  // thread-specific.  It defaults to TAO_GLOBAL if not set
  // specifically.

  int reactor_lock_;
  // Flag indicating wether we should provide a lock-freed reactor
  // or not.

  int cdr_allocator_source_;
  // The source for the CDR allocator. Even with a TSS resource
  // factory the user may be interested in global allocators for the
  // CDR streams, for instance to keep the buffers around after the
  // upcall and/or pass them to another thread.

  // = Typedefs for the singleton types used to store our orb core
  // information.
  typedef ACE_Singleton<TAO_Pre_Allocated_Resources, ACE_SYNCH_MUTEX>
          GLOBAL_PRE_ALLOCATED;
  typedef ACE_TSS_Singleton<TAO_Pre_Allocated_Resources, ACE_SYNCH_MUTEX>
          TSS_PRE_ALLOCATED;

  typedef ACE_Singleton<TAO_App_Allocated_Resources, ACE_SYNCH_MUTEX>
          GLOBAL_APP_ALLOCATED;
  typedef ACE_TSS_Singleton<TAO_App_Allocated_Resources, ACE_SYNCH_MUTEX>
          TSS_APP_ALLOCATED;
  typedef ACE_Singleton<TAO_GLOBAL_Collocation_Table, ACE_SYNCH_MUTEX>
          GLOBAL_Collocation_Table;
};

#if defined (__ACE_INLINE__)
#include "tao/default_resource.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE (TAO_Default_Resource_Factory)
ACE_FACTORY_DECLARE (TAO, TAO_Default_Resource_Factory)

#endif /* TAO_DEFAULT_CLIENT_H */
