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
#include "tao/CONV_FRAMEC.h"

class TAO_Object_Adapter;
class TAO_IOR_Parser;
class TAO_LF_Strategy;

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
  TAO_Default_Resource_Factory (void);

  /// Destructor.
  virtual ~TAO_Default_Resource_Factory (void);

  /**
   * @name Service Configurator Hooks
   */
  //@{
  /// Dynamic linking hook
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Parse svc.conf arguments
  int parse_args (int argc, ACE_TCHAR* argv[]);
  //@}

  /**
   * @name Member Accessors
   */
  //@{

  int get_parser_names (char **&names,
                        int &number_of_names);
  enum
  {
    TAO_ALLOCATOR_THREAD_LOCK
  };

  // Translator type
  enum TRANSLATOR_TYPE
  {
    CHAR_TRANSLATOR,
    WCHAR_TRANSLATOR
  };

  /// Modify and get the source for the CDR allocators
  int cdr_allocator_source (void);

  // = Resource Retrieval
  virtual int use_locked_data_blocks (void) const;
  virtual ACE_Reactor *get_reactor (void);
  virtual void reclaim_reactor (ACE_Reactor *);
  virtual TAO_Acceptor_Registry  *get_acceptor_registry (void);
  virtual TAO_Connector_Registry *get_connector_registry (void);
  virtual ACE_Allocator* input_cdr_dblock_allocator (void);
  virtual ACE_Allocator* input_cdr_buffer_allocator (void);
  virtual ACE_Allocator* input_cdr_msgblock_allocator (void);
  virtual int input_cdr_allocator_type_locked (void);
  virtual ACE_Allocator* output_cdr_dblock_allocator (void);
  virtual ACE_Allocator* output_cdr_buffer_allocator (void);
  virtual ACE_Allocator* output_cdr_msgblock_allocator (void);
  virtual TAO_ProtocolFactorySet *get_protocol_factories (void);

  virtual TAO_Codeset_Manager *get_codeset_manager ();

  virtual int init_protocol_factories (void);

  virtual int cache_maximum (void) const;
  virtual int purge_percentage (void) const;
  virtual int max_muxed_connections (void) const;
  virtual ACE_Lock *create_cached_connection_lock (void);
  virtual ACE_Lock *create_object_key_table_lock (void);
  virtual ACE_Lock *create_corba_object_lock (void);
  virtual int locked_transport_cache (void);
  virtual TAO_Flushing_Strategy *create_flushing_strategy (void);
  virtual TAO_Connection_Purging_Strategy *create_purging_strategy (void);
  TAO_Resource_Factory::Resource_Usage resource_usage_strategy (void) const;
  virtual TAO_LF_Strategy *create_lf_strategy (void);

  virtual void disable_factory (void);
  //@}

protected:

  /// Obtain the reactor implementation
  virtual ACE_Reactor_Impl *allocate_reactor_impl (void) const;

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

  /// If <0> then we create reactors with signal handling disabled.
  int reactor_mask_signals_;

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

  /// This flag is used to determine whether options have been
  /// processed via the init() function.  It is necessary to
  /// properly report errors when the default factory is replaced.
  int options_processed_;

  /// This flag specifies whether the factory has been disabled.
  /// If it has been disabled we should print warnings if options
  /// were processed before (or later).
  int factory_disabled_;

private:
  enum Lock_Type
  {
    TAO_NULL_LOCK,
    TAO_THREAD_LOCK
  };

  /// Type of lock used by the cached connector.
  Lock_Type cached_connection_lock_type_;

  /// Type of lock used by the corba object.
  Lock_Type object_key_table_lock_type_;

  /// Type of lock used by the corba object.
  Lock_Type corba_object_lock_type_;

  enum Flushing_Strategy_Type
  {
    TAO_LEADER_FOLLOWER_FLUSHING,
    TAO_REACTIVE_FLUSHING,
    TAO_BLOCKING_FLUSHING
  };

  /// Type of flushing strategy configured
  int flushing_strategy_type_;

  TAO_Codeset_Manager *codeset_manager_;

  /// Resource usage strategy
  Resource_Usage resource_usage_strategy_;
};

#if defined (__ACE_INLINE__)
#include "tao/default_resource.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_Default_Resource_Factory)
ACE_FACTORY_DECLARE (TAO, TAO_Default_Resource_Factory)

#include "ace/post.h"
#endif /* TAO_DEFAULT_CLIENT_H */
