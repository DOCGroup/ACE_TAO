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

#include "tao/Resource_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Singleton.h"
#include "ace/Service_Config.h"

class TAO_Object_Adapter;

// ****************************************************************

class TAO_Export TAO_Allocated_Resources
{
  // = TITLE
  //   Container for the resources allocated by the factory.
  //
  // = DESCRIPTION
  //
public:
  TAO_Allocated_Resources (void);
  // Constructor necessary because we have pointers.  It's inlined
  // here rather than in the .i file because it's easier than trying
  // to re-order header files in corba.h to eliminate the "used
  // before declared inline" warnings/errors on certain compilers.

  ~TAO_Allocated_Resources (void);
  // Destructor is also necessary because we now allocate some of
  // the objects held here.

  // = Resources

  ACE_Allocator *input_cdr_dblock_allocator_;
  ACE_Allocator *input_cdr_buffer_allocator_;
  // The allocators for the input CDR streams.
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
    TAO_REACTOR_SELECT_MT, // Use ACE_Token
    TAO_REACTOR_SELECT_ST, // Use ACE_Noop_Token
    TAO_REACTOR_FL,
    TAO_REACTOR_XT,
    TAO_REACTOR_WFMO,
    TAO_REACTOR_MSGWFMO,
    TAO_REACTOR_TP
  };

  int cdr_allocator_source (void);
  // Modify and get the source for the CDR allocators

  // = Resource Retrieval
  virtual int use_tss_resources (void) const;
  virtual ACE_Reactor *get_reactor (void);
  virtual TAO_Acceptor_Registry  *get_acceptor_registry (void);
  virtual TAO_Connector_Registry *get_connector_registry (void);
  virtual ACE_Allocator* input_cdr_dblock_allocator (void);
  virtual ACE_Allocator* input_cdr_buffer_allocator (void);
  virtual ACE_Allocator* output_cdr_dblock_allocator (void);
  virtual ACE_Allocator* output_cdr_buffer_allocator (void);
  virtual ACE_Data_Block *create_input_cdr_data_block (size_t size);

  virtual TAO_ProtocolFactorySet *get_protocol_factories (void);
  virtual int init_protocol_factories (void);

protected:
  ACE_Reactor_Impl *allocate_reactor_impl (void) const;
  // Obtain the reactor implementation

protected:
  int use_tss_resources_;
  // Flag indicating whether resources should be global or
  // thread-specific.

  int reactor_type_;
  // Flag indicating which kind of reactor we should use.

  int cdr_allocator_source_;
  // The source for the CDR allocator. Even with a TSS resource
  // factory the user may be interested in global allocators for the
  // CDR streams, for instance to keep the buffers around after the
  // upcall and/or pass them to another thread.

  TAO_ProtocolFactorySet protocol_factories_;
  // list of loaded protocol factories.

  // = Typedefs for the singleton types used to store our orb core
  // information.
  typedef ACE_Singleton<TAO_Allocated_Resources, ACE_SYNCH_MUTEX>
          GLOBAL_ALLOCATED;
  typedef ACE_TSS_Singleton<TAO_Allocated_Resources, ACE_SYNCH_MUTEX>
          TSS_ALLOCATED;
};

#if defined (__ACE_INLINE__)
#include "tao/default_resource.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE (TAO_Default_Resource_Factory)
ACE_FACTORY_DECLARE (TAO, TAO_Default_Resource_Factory)

#endif /* TAO_DEFAULT_CLIENT_H */
