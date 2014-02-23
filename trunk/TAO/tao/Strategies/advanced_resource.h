// -*- C++ -*-
// $Id$

#ifndef TAO_ADVANCED_RESOURCE_H
#define TAO_ADVANCED_RESOURCE_H
#include /**/ "ace/pre.h"

#include "tao/Strategies/strategies_export.h"
#include "tao/default_resource.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB_Core.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Connection_Purging_Strategy;

class TAO_Strategies_Export TAO_Resource_Factory_Changer
{
public:
  TAO_Resource_Factory_Changer (void);
};

/**
 * @class TAO_Advanced_Resource_Factory
 *
 * @brief TAO's default resource factory
 *
 * Using a <{resource source specifier}> as a discriminator, the
 * factory can return resource instances which are, e.g., global,
 * stored in thread-specific storage, stored in shared memory,
 * etc.
 *
 */
class TAO_Strategies_Export TAO_Advanced_Resource_Factory :
  public TAO_Default_Resource_Factory
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_Advanced_Resource_Factory (void);

  /// Destructor.
  virtual ~TAO_Advanced_Resource_Factory (void);

  // = Service Configurator hooks.
  /// Dynamic linking hook
  virtual int init (int argc, ACE_TCHAR* argv[]);

  /// Type of lock used for the allocators
  enum Allocator_Lock_Type
  {
    TAO_ALLOCATOR_NULL_LOCK,
    TAO_ALLOCATOR_THREAD_LOCK
  };

  /// Type of Reactor
  enum
  {
    /// Use ACE_Token
    TAO_REACTOR_SELECT_MT = 1,

    /// Use ACE_Noop_Token
    TAO_REACTOR_SELECT_ST = 2,
    TAO_REACTOR_WFMO      = 3,
    TAO_REACTOR_MSGWFMO   = 4,
    TAO_REACTOR_TP        = 5,
    TAO_REACTOR_DEV_POLL  = 6
  };

  /// Thread queueing Strategy
  enum
  {
    /// Not set, use LIFO.
    TAO_THREAD_QUEUE_NOT_SET,

    /// FIFO, first-in-first-out.
    TAO_THREAD_QUEUE_FIFO,

    /// LIFO, last-in-first-out (default).
    TAO_THREAD_QUEUE_LIFO
  };

  /**
   * @name Resource Retrieval
   */
  //@{
  virtual int init_protocol_factories (void);
  virtual ACE_Allocator* input_cdr_dblock_allocator (void);
  virtual ACE_Allocator* input_cdr_buffer_allocator (void);
  virtual ACE_Allocator* input_cdr_msgblock_allocator (void);
  virtual ACE_Allocator* amh_response_handler_allocator (void);
  virtual ACE_Allocator* ami_response_handler_allocator (void);
  virtual int input_cdr_allocator_type_locked (void);
  virtual TAO_ProtocolFactorySet *get_protocol_factories (void);
  //@}

  virtual TAO_Connection_Purging_Strategy *create_purging_strategy (void);
  virtual TAO_LF_Strategy *create_lf_strategy (void);

protected:

  /// Obtain the reactor implementation
  virtual ACE_Reactor_Impl *allocate_reactor_impl (void) const;

  void report_option_value_error (const ACE_TCHAR* option_name,
                                  const ACE_TCHAR* option_value);

  void report_unsupported_error (const ACE_TCHAR* option_name);

  /// List of loaded protocol factories.
  TAO_ProtocolFactorySet protocol_factories_;

  /// The type of reactor registry.
  int reactor_registry_type_;

  /// Flag indicating which kind of reactor we should use.
  int reactor_type_;

  /// The type of queueing strategy to use for multi-threaded
  /// select reactors, TAO_REACTOR_SELECT_MT and TAO_REACTOR_TP.
  int threadqueue_type_;

  /// The type of CDR allocators.
  Allocator_Lock_Type cdr_allocator_type_;

  /// Type of lock used by AMH response handler allocator.
  Allocator_Lock_Type amh_response_handler_allocator_lock_type_;

  /// Type of lock used by AMI response handler allocator.
  Allocator_Lock_Type ami_response_handler_allocator_lock_type_;

  virtual int load_default_protocols (void);

};

static TAO_Resource_Factory_Changer TAO_changer;


ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Strategies, TAO_Advanced_Resource_Factory)
ACE_FACTORY_DECLARE (TAO_Strategies, TAO_Advanced_Resource_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ADVANCED_RESOURCE_H */
