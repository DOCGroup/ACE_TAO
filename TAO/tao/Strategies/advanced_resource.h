// $Id$

#ifndef TAO_ADVANCED_RESOURCE_H
#define TAO_ADVANCED_RESOURCE_H
#include "ace/pre.h"

#include "strategies_export.h"
#include "tao/default_resource.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "tao/ORB_Core.h"

class TAO_Connection_Purging_Strategy;

class TAO_Strategies_Export TAO_Resource_Factory_Changer
{
public:
  TAO_Resource_Factory_Changer (void);
};

class TAO_Strategies_Export TAO_Advanced_Resource_Factory : public TAO_Default_Resource_Factory
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
  TAO_Advanced_Resource_Factory (void);
  // Constructor.

  virtual ~TAO_Advanced_Resource_Factory (void);
  // Destructor.

  // = Service Configurator hooks.
  virtual int init (int argc, ACE_TCHAR* argv[]);
  // Dynamic linking hook

  // = Member Accessors
  enum
  {
    TAO_ALLOCATOR_NULL_LOCK,
    TAO_ALLOCATOR_THREAD_LOCK
  };

  // = Type of Reactor
  enum
  {
    TAO_REACTOR_SELECT_MT, // Use ACE_Token
    TAO_REACTOR_SELECT_ST, // Use ACE_Noop_Token
    TAO_REACTOR_FL,
    TAO_REACTOR_TK,
    TAO_REACTOR_WFMO,
    TAO_REACTOR_MSGWFMO,
    TAO_REACTOR_TP
  };

  // = Resource Retrieval
  virtual int init_protocol_factories (void);
  virtual ACE_Allocator* input_cdr_dblock_allocator (void);
  virtual ACE_Allocator* input_cdr_buffer_allocator (void);
  virtual ACE_Allocator* input_cdr_msgblock_allocator (void);
  virtual int input_cdr_allocator_type_locked (void);
  virtual TAO_ProtocolFactorySet *get_protocol_factories (void);

  virtual TAO_Connection_Purging_Strategy *create_purging_strategy (void);
  virtual TAO_LF_Strategy *create_lf_strategy (void);

protected:

  virtual ACE_Reactor_Impl *allocate_reactor_impl (void) const;
  // Obtain the reactor implementation

  void report_option_value_error (const ACE_TCHAR* option_name,
                                  const ACE_TCHAR* option_value);

  void report_unsupported_error (const ACE_TCHAR* option_name);

  TAO_ProtocolFactorySet protocol_factories_;
  // list of loaded protocol factories.

  int reactor_registry_type_;
  // The type of reactor registry.

  int reactor_type_;
  // Flag indicating which kind of reactor we should use.

  int cdr_allocator_type_;
  // The type of CDR allocators.

  virtual int load_default_protocols (void);

};

#if defined (__ACE_INLINE__)
#include "advanced_resource.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Strategies, TAO_Advanced_Resource_Factory)
ACE_FACTORY_DECLARE (TAO_Strategies, TAO_Advanced_Resource_Factory)

static TAO_Resource_Factory_Changer TAO_changer;

#include "ace/post.h"
#endif /* TAO_ADVANCED_RESOURCE_H */
