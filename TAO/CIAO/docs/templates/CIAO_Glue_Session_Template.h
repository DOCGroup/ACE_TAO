// $Id$

// ===========================================================
//
// @file CIAO_Glue_Session_Template.h
//
//    This is a pseudo-meta generic servant implementations template
//    for CIAO's CIDL compiler.  It demonstrates how a servant
//    implementation for a session component should look like.
//
//    The generated filename for files using this template shoule be
//       [idl-basename]_svnt.h
//
// @author Nanbor Wang <nanbor@cs.wustl.edu>
//
// ===========================================================

#ifndef CIAO_GLUE_SESSION_[idl-basename]_SVNT_H
#define CIAO_GLUE_SESSION_[idl-basename]_SVNT_H
#include "ace/pre.h"

#include "[idl-name]S.h"        // Source in the skeletons for component
                                // client-view equivalent interfaces
#include "[idl-name]EC.h"       // Source in the executor mapping
                                // that component implementations use
#include "ciao/Container_Base.h" //Source in the container interface definitions
#include "tao/LocalObject.h"
#include "tao/PortableServer/Key_Adapters.h"
#include "ace/Active_Map_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/// @@@ Notice that all component and interface names need to be
/// fully qualified as we are creating a new namespace for the CIAO's
/// container glue code.
/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

##if component is defined withing a [module name]
namespace CIAO_GLUE_[module_name]
##else
namespace CIAO_GLUE
##endif
{

  //////////////////////////////////////////////////////////////////
  // Facet Glue Code implementation
  // @@ We are assuming that these facets are declared under the same
  //    module as the component (thus, we are placing this glue code
  //    here under the same namespace.  If they are not, we will
  //    either be generating them in separate namespaces, or include
  //    some other CIDL generated files to get the glue code
  //    implementation.

##foreach [facet type] in (all facet interface types in the original IDL)
  class [SERVANT]_Export [facet type]_Servant :
    : public virtual POA_[facet type], // full skeleton name here
      public virtual PortableServer::RefCountServantBase
  {
  public:
    // Constructor and destructor.
    [facet type]_Servant (CCM_[facet type]_ptr executor,
                          ::Components::CCMContext_ptr ctx);
    ~[facet tyep]_Servant ();

##  foreach [operation] in (all facet operations)
    // Generate operation decls.
##  end foreach [operation]

    // get_component implementation.
    virtual CORBA::Object_ptr
    _get_component (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    // Facet executor.
    CCM_[facet type]_var executor_;

    // Context object.
    ::Components::CCMContext_var ctx_;
  };
##end foreach [facet type]


  //////////////////////////////////////////////////////////////////
  // Component specific context implementation
  class [SERVANT]_Export [component name]_Context :
    public virtual CCM_[component name]_Context,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    // We will allow the the servant glue code we generate to access
    // our states.
    friend class [component name]_Servant;

    // Ctor.
    [component name]_Context (::Components::CCMHome_ptr home,
                              ::CIAO::Session_Container *c,
                              [component name]_Servant *sv);

    // Dtor.
    virtual ~[component name]_Context ();

    // Operations for [component name] event source, and
    // receptacles defined in CCM_[component name]_Context.

##foreach [receptacle name] with [uses type] in (list of all 'uses' interfaces) generate:
##  if [receptacle name] is a simplex receptacle ('uses')
    [uses type]_ptr
    get_connection_[receptacle name] (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
##  else ([receptacle name] is a multiplex ('uses multiple') receptacle)
    // [receptacle name]Connections typedef'ed as a sequence of
    // struct [receptacle name]Connection.
    [receptacle name]Connections *
    get_connections_[receptacle name] (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
      ACE_THROW_SPEC ((CORBA::SystemException));
##  endif [receptacle name]
##end foreach [receptacle name] with [uses type]

##foreach [event name] with [eventtype] in (list of all event sources) generate:
    void push_[event name] ([eventtype] *ev
                            ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
##end foreach [event name] with [eventtype]

    // Operations for ::Components::CCMContext
    virtual ::Components::Principal_ptr
    get_caller_principal (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Components::CCMHome_ptr
    get_CCM_home (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual CORBA::Boolean
    get_rollback_only (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::IllegalState));

    virtual ::Components::Transaction::UserTransaction_ptr
    get_user_transaction (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::IllegalState));

    virtual CORBA::Boolean
    is_caller_in_role (const char * role
                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void
    set_rollback_only (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::IllegalState));

    // Operations for ::Components::SessionContext interface
    virtual CORBA::Object_ptr
    get_CCM_object (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::IllegalState));
  protected:
    // We need to generate, in protected section, stuff that manage
    // connections and consumers of this component.

##foreach [receptacle name] with [uses type] in (list of all 'uses' interfaces) generate:
##  if [receptacle name] is a simplex receptacle ('uses')
    // Simplex [receptacle name] connection management operations
    void
    connect_[receptacle name] ([uses type]_ptr c
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::AlreadyConnected,
                       ::Components::InvalidConnection));

    [uses type]_ptr
    disconnect_[receptacle name] (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::NoConnection));

    // Simplex [receptacle name] connection
    [uses type]_var ciao_uses_[receptacle name]_;

##  else ([receptacle name] is a multiplex ('uses multiple') receptacle)
    // Multiplex [receptacle name] connection management operations
    ::Components::Cookie *
    connect_[receptacle name] ([uses type]_ptr c
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::ExceedConnectionLimit,
                       ::Components::InvalidConnection));

    [uses type]_ptr
    disconnect_[receptacle name] (::Components::Cookie *ck
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::InvalidConnection));

    // Multiplex [receptacle name] connections

    ACE_Active_Map_Manager<[uses type]_var> ciao_muses_[receptacle name]_;
##  endif [receptacle name]
##end foreach [receptacle name] with [uses type]

    // Operations for emits interfaces.
##foreach [emit name] with [eventtype] in (list of all emitters) generate:
    void
    connect_[emit name] ([eventtype]Consumer_ptr c
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::AlreadyConnected));

    [eventtype]Consumer_ptr
    disconnect_[emit name] (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::NoConnection));

    [eventtype]Consumer_var ciao_emits_[emit name]_consumer_;
##end foreach [emit name] with [eventtype]

    // Operations for publishes interfaces.
##foreach [publish name] with [eventtype] in (list of all publishers) generate:
      ::Components::Cookie *
      subscribe_[publish name] ([eventtype]Consumer_ptr c
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::ExceededConnectionLimit));

      [eventtype]Consumer_ptr
      unsubscribe_[publish name] (::Components::Cookie *ck
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::InvalidConnection));

    ACE_Active_Map_Manager<[eventtype]Consumer_var> ciao_publishes_[publish name]_map_;
##end foreach [publish name] with [eventtype]

  protected:
    /// Cached component home reference.
    ::Components::CCMHome_var home_;

    /// session container
    ::CIAO::Session_Container *container_;

    /// Reference back to owner.
    [component name]_Servant *servant_;

    /// @@ Cached component reference.
    [component name]_var component_;

  };

  //////////////////////////////////////////////////////////////////
  // Component Servant Glue code implementation
  class [SERVANT]_Export [component name]_Servant
    : public virtual POA_[component name], // full skeleton name here
      public virtual PortableServer::RefCountServantBase
  {
  public:
    // Ctor.
    [component name]_Servant (CCM_[component name]_ptr executor,
                              ::Components::CCMHome_ptr home,
                              ::CIAO::Session_Container *c);

    // Dtor.
    ~[component name]_Servant (void);

##foreach [operation] in all supported interfaces of own component and all inherited components and attribute accessors/mutators

    // Generate the [operation] here.

##end

    // Operations for provides interfaces.
##foreach [facet name] with [facet type] in (list of all provided interfaces) generate:
    virtual [facet type]_ptr
    provide_[facet name] (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
##end foreach [facet name] with [facet type]

    // Operations for receptacles interfaces.

##foreach [receptacle name] with [uses type] in (list of all 'uses' interfaces) generate:
##  if [receptacle name] is a simplex receptacle ('uses')
    // Simplex [receptacle name] connection management operations
    virtual void
    connect_[receptacle name] ([uses type]_ptr c
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::AlreadyConnected,
                       ::Components::InvalidConnection));

    virtual [uses type]_ptr
    disconnect_[receptacle name] (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::NoConnection));

    virtual [uses type]_ptr
    get_connection_[receptacle name] (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
##  else ([receptacle name] is a multiplex ('uses multiple') receptacle)
    // Multiplex [receptacle name] connection management operations
    virtual ::Components::Cookie *
    connect_[receptacle name] ([uses type]_ptr c
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::ExceedConnectionLimit,
                       ::Components::InvalidConnection));

    virtual [uses type]_ptr
    disconnect_[receptacle name] (::Components::Cookie *ck
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::InvalidConnection));

    virtual [receptacle name]Connections *
    get_connections_[receptacle name] (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
##  endif [receptacle name]
##end foreach [receptacle name] with [uses type]

    // Operations for consumers interfaces.
##foreach [consumer name] with [eventtype] in (list of all consumers) generate:

    // First we need to generate the event sink specific servant
    class [SERVANT]_Export [eventtype]Consumer_[consumer name]_Servant
      : public virtual POA_[eventtype]Consumer, // full skeleton name here
        public virtual PortableServer::RefCountServantBase
    {
    public:
      // Constructor and destructor.
      [event type]Consumer_[consumer name]_Servant (CCM_[component name]_ptr executor,
                                                    CCM_[component name]_Context_ptr c);

      ~[event type]Consumer_[consumer name]_Servant ();

##  foreach [type] in ([eventtype] and all its parent eventtype, if any)
      virtual void push_[type] ([type] *evt
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));
##  end [type]

      // Inherit from ::Compopnents::EventConsumerBase
      virtual void push_event (::Components::EventBase *ev
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         ::Components::BadEventType));

    // get_component implementation.
      virtual CORBA::Object_ptr
      _get_component (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

    protected:
      // Executor
      CCM_[component name]_var executor_;

      // Context object.
      CCM_[component name]_Context_var ctx_;
    };

    virtual [eventtype]Consumer_ptr
    get_consumer_[consumer name] (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
##end foreach [consumer name] with [eventtype]

    // Operations for emits interfaces.
##foreach [emit name] with [eventtype] in (list of all emitters) generate:
    virtual void
    connect_[emit name] ([eventtype]Consumer_ptr c
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException
                       ::Components::AlreadyConnected));

    virtual [eventtype]Consumer_ptr
    disconnect_[emit name] (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::NoConnection));
##end foreach [emit name] with [eventtype]

    // Operations for publishes interfaces.
##foreach [publish name] with [eventtype] in (list of all publishers) generate:
    virtual ::Components::Cookie *
    subscribe_[publish name] ([eventtype]Consumer_ptr c
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::ExceededConnectionLimit));

    virtual [eventtype]Consumer_ptr
    unsubscribe_[publish name] (::Components::Cookie *ck
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::InvalidConnection));
##end foreach [publish name] with [eventtype]

    // Operations for Navigation interface
    virtual CORBA::Object_ptr
    provide_facet (const char * name
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException ,
                       Components::InvalidName)) ;

    virtual ::Components::FacetDescriptions *
    get_all_facets (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Components::FacetDescriptions *
    get_named_facets (const Components::NameList & names
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException ,
                       Components::InvalidName));

    virtual CORBA::Boolean
    same_component (CORBA::Object_ptr object_ref
                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations for Receptacles interface
    virtual ::Components::Cookie *
    connect (const char * name,
             CORBA::Object_ptr connection
             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName,
                       Components::InvalidConnection,
                       Components::AlreadyConnected,
                       Components::ExceededConnectionLimit));

    virtual CORBA::Object_ptr
    disconnect (const char * name,
                Components::Cookie *ck
                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName,
                       Components::InvalidConnection,
                       Components::CookieRequired,
                       Components::NoConnection));

    virtual ::Components::ConnectionDescriptions *
    get_connections (const char * name
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName));

    virtual ::Components::ReceptacleDescriptions *
    get_all_receptacles (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Components::ReceptacleDescriptions *
    get_named_receptacles (const Components::NameList & names
                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName));

    // Operations for Events interface
    virtual ::Components::EventConsumerBase_ptr
    get_consumer (const char * sink_name
                  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName));

    virtual ::Components::Cookie *
    subscribe (const char * publisher_name,
               Components::EventConsumerBase_ptr subscriber
               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName,
                       Components::InvalidConnection,
                       Components::ExceededConnectionLimit));

    virtual ::Components::EventConsumerBase_ptr
    unsubscribe (const char * publisher_name,
                 Components::Cookie *ck
                 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName,
                       Components::InvalidConnection));

    virtual void
    connect_consumer (const char * emitter_name,
                      Components::EventConsumerBase_ptr consumer
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName,
                       Components::AlreadyConnected,
                       Components::InvalidConnection));

    virtual ::Components::EventConsumerBase_ptr
    disconnect_consumer (const char * source_name
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName,
                       Components::NoConnection));

    virtual ::Components::ConsumerDescriptions *
    get_all_consumers (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Components::ConsumerDescriptions *
    get_named_consumers (const Components::NameList & names
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName));

    virtual ::Components::EmitterDescriptions *
    get_all_emitters (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Components::EmitterDescriptions *
    get_named_emitters (const Components::NameList & names
                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName));

    virtual ::Components::PublisherDescriptions *
    get_all_publishers (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Components::PublisherDescriptions *
    get_named_publishers (const Components::NameList & names
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName));

    // Operations for CCMObject interface
    virtual ::CORBA::IRObject_ptr
    get_component_def (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Components::CCMHome_ptr
    get_ccm_home (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Components::PrimaryKeyBase *
    get_primary_key (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::NoKeyAvailable));

    virtual void
    configuration_complete (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidConfiguration));

    virtual void
    remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::RemoveFailure));

    virtual ::Components::ComponentPortDescription *
    get_all_ports (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // get_component implementation.
    virtual CORBA::Object_ptr
    _get_component (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // CIAO specific operations.

    // Activate the object in the container_
    void
    _ciao_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void
    _ciao_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    // My Executor.
    CCM_[component name]_var executor_;

    // My Run-time Context.
    [component name]_Context *context_;

    // Managing container.
    ::CIAO::Session_Container *container_;

    // Cached provided interfaces.
##foreach [facet name] with [facet type] in (list of all provided interfaces) generate:
    [facet type]_var provide_[facet name]_;
##end foreach [facet name] with [facet type]

##foreach [consumer name] with [eventtype] in (list of all consumers) generate:
    [eventtype]Consumer_var consumes_[consumer name]_;
##end foreach [consumer name] with [eventtype]

  };


  //////////////////////////////////////////////////////////////////
  // Component Home Glue code implementation

  // Foreach component home
  class [SERVANT]_Export [home name]_Servant :
    public virtual POA_[home name], // full skeleton name here
    public virtual PortableServer::RefCountServantBase
  {
  public:
    // Ctor.
    [home name]_Servant (CCM_[home name]_ptr exe,
                         CIAO::Session_Container *c);

    // Dtor.
    ~[home name]_Servant (void);

    // User defined and inherited operations
##foreach [operation] in (all explicit operations in [home basename] including its parents)

    // The operation decl here.

## end foreach opeartion

    // Factory operations
##foreach [factory name]  in (all factory operations in [home basename] including its parents)
    // for factory operations inherit from parent home(s), they should return
    // the corresponding component types their homes manage
    virtual [component name]_ptr
    [factory name] (.... ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::CreateFailure,
                       ....));
##end foreach [factory name]

    // Finder operations
##foreach [finder name]  in (all finder operations in [home basename] including its parents)
    // for finder operations inherit from parent home(s), they should return
    // the corresponding component types their homes manage
    virtual [component name]_ptr
    [finder name] (.... ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::FinderFailure,
                       ....));
##end foreach [finder name]

##  if [home name] is a keyless home

    // Operations for KeylessHome interface
    virtual ::Components::CCMObject_ptr
    create_component (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CreateFailure));

##  else [home basename] is keyed home with [key type]

    // We do not support key'ed home at the moment but we might
    // as well generate the mapping.
    virtual [component name]_ptr create ([key type] *key
                                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::CreationFailure,
                       ::Components::DuplicateKeyValue,
                       ::Components::InvalidKey));

    virtual [component name]_ptr
    find_by_primary_key ([key type] *key
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::FinderFailure,
                       ::Components::UnknownKeyValue,
                       ::Components::InvalidKey));

    virtual void remove ([key type] *key
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::RemoveFailure,
                       ::Components::UnknownKeyValue,
                       ::Components::InvalidKey));

    virtual [key type] *
    get_primary_key ([component name]_ptr comp
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

##  endif (keyed or keyless home)

    // Operations for Implicit Home interface
    virtual [component name]_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CreateFailure));

    // Operations for CCMHome interface
    virtual ::CORBA::IRObject_ptr
    get_component_def (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::CORBA::IRObject_ptr
    get_home_def (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void remove_component (Components::CCMObject_ptr comp
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::RemoveFailure));

  protected:
    // Helper method for factory operations.
     [component name]_ptr
     _ciao_activate_component (CCM_[component name]_ptr exe
                               ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
       ACE_THROW_SPEC ((CORBA::SystemException));

    void
    _ciao_passivate_component ([component name]_ptr comp
                               ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // My Executor.
    CCM_[home name]_var executor_;

    // My Container
    CIAO::Session_Container *container_;

    // Components this home manages.
    ACE_Hash_Map_Manager_Ex <PortableServer::ObjectId,
                             [component name]_Servant*,
                             TAO_ObjectId_Hash,
                             ACE_Equal_To<PortableServer::ObjectId>,
                             ACE_SYNCH_MUTEX> component_map_;
  };

}

extern "C" [SERVANT]_Export ::PortableServer::Servant
create[home name]_Servant (::Components::HomeExecutorBase_ptr p,
                           CIAO::Session_Container *c
                           ACE_ENV_ARG_DECL_WITH_DEFAULTS);

#if defined (__ACE_INLINE__)
# include "[idl-name]_svnt.inl"
#endif /* __ACE_INLINE__ */


#include "ace/post.h"
#endif /* CIAO_GLUE_SESSION_[idl-basename]_SVNT_H */
