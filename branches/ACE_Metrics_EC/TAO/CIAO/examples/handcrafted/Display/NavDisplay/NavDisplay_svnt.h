// $Id$

// ===========================================================
//
// @file NavDisplay_svnt.h
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

#ifndef CIAO_GLUE_SESSION_NAVDISPLAY_SVNT_H
#define CIAO_GLUE_SESSION_NAVDISPLAY_SVNT_H
#include "ace/pre.h"

#include "NavDisplayS.h"        // Source in the skeletons for component
                                // client-view equivalent interfaces
#include "NavDisplayEC.h"       // Source in the executor mapping
                                // that component implementations use
#include "ciao/Container_Base.h" //Source in the container interface definitions
#include "tao/LocalObject.h"
#include "tao/PortableServer/Key_Adapters.h"
#include "ace/Active_Map_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO_GLUE_HUDisplay
{

  //////////////////////////////////////////////////////////////////
  // Component specific context implementation
  class NAVDISPLAY_SVNT_Export NavDisplay_Context :
    public virtual HUDisplay::CCM_NavDisplay_Context,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    // We will allow the the servant glue code we generate to access
    // our states.
    friend class NavDisplay_Servant;

    // Ctor.
    NavDisplay_Context (::Components::CCMHome_ptr home,
                        ::CIAO::Session_Container *c,
                        NavDisplay_Servant *sv);

    // Dtor.
    virtual ~NavDisplay_Context ();

    // Operations for [component name] event source, and
    // receptacles defined in CCM_[component name]_Context.

    HUDisplay::position_ptr
    get_connection_GPSLocation (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

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

    // Simplex [receptacle name] connection management operations
    void
    connect_GPSLocation (HUDisplay::position_ptr c
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::AlreadyConnected,
                       ::Components::InvalidConnection));

    HUDisplay::position_ptr
    disconnect_GPSLocation (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::NoConnection));

    // Simplex [receptacle name] connection
    HUDisplay::position_var ciao_uses_GPSLocation_;

  protected:
    /// Cached component home reference.
    ::Components::CCMHome_var home_;

    /// session container
    ::CIAO::Session_Container *container_;

    /// Reference back to owner.
    NavDisplay_Servant *servant_;

    /// @@ Cached component reference.
    HUDisplay::NavDisplay_var component_;

  };

  //////////////////////////////////////////////////////////////////
  // Component Servant Glue code implementation
  class NAVDISPLAY_SVNT_Export NavDisplay_Servant
    : public virtual POA_HUDisplay::NavDisplay, // full skeleton name here
      public virtual PortableServer::RefCountServantBase
  {
  public:
    // Ctor.
    NavDisplay_Servant (HUDisplay::CCM_NavDisplay_ptr executor,
                        ::Components::CCMHome_ptr home,
                        ::CIAO::Session_Container *c);

    // Dtor.
    ~NavDisplay_Servant (void);

    // Operations for provides interfaces.

    // Operations for receptacles interfaces.

    // Simplex [receptacle name] connection management operations
    virtual void
    connect_GPSLocation (HUDisplay::position_ptr c
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::AlreadyConnected,
                       ::Components::InvalidConnection));

    virtual HUDisplay::position_ptr
    disconnect_GPSLocation (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::NoConnection));

    virtual HUDisplay::position_ptr
    get_connection_GPSLocation (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations for consumers interfaces.
    // First we need to generate the event sink specific servant
    class NAVDISPLAY_SVNT_Export tickConsumer_Refresh_Servant
      : public virtual POA_HUDisplay::tickConsumer, // full skeleton name here
        public virtual PortableServer::RefCountServantBase
    {
    public:
      // Constructor and destructor.
      tickConsumer_Refresh_Servant (HUDisplay::CCM_NavDisplay_ptr executor,
                                    HUDisplay::CCM_NavDisplay_Context_ptr c);

      ~tickConsumer_Refresh_Servant ();

      virtual void push_tick (HUDisplay::tick_ptr evt
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      // Inherit from ::Compopnents::EventBConsumerBase
      virtual void push_event (::Components::EventBase_ptr ev
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         ::Components::BadEventType));

    // get_component implementation.
      virtual CORBA::Object_ptr
      _get_component (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

    protected:
      // Executor
      HUDisplay::CCM_NavDisplay_var executor_;

      // Context object.
      HUDisplay::CCM_NavDisplay_Context_var ctx_;
    };

    virtual HUDisplay::tickConsumer_ptr
    get_consumer_Refresh (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations for emits interfaces.

    // Operations for publishes interfaces.

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
    virtual ::Components::Cookie_ptr
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
                Components::Cookie_ptr ck
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

    virtual ::Components::Cookie_ptr
    subscribe (const char * publisher_name,
               Components::EventConsumerBase_ptr subscriber
               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName,
                       Components::InvalidConnection,
                       Components::ExceededConnectionLimit));

    virtual ::Components::EventConsumerBase_ptr
    unsubscribe (const char * publisher_name,
                 Components::Cookie_ptr ck
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
    HUDisplay::CCM_NavDisplay_var executor_;

    // My Run-time Context.
    NavDisplay_Context *context_;

    // Managing container.
    ::CIAO::Session_Container *container_;

    // Cached provided interfaces.
    HUDisplay::tickConsumer_var consumes_Refresh_;
  };


  //////////////////////////////////////////////////////////////////
  // Component Home Glue code implementation

  // Foreach component home
  class NAVDISPLAY_SVNT_Export NavDisplayHome_Servant :
    public virtual POA_HUDisplay::NavDisplayHome, // full skeleton name here
    public virtual PortableServer::RefCountServantBase
  {
  public:
    // Ctor.
    NavDisplayHome_Servant (HUDisplay::CCM_NavDisplayHome_ptr exe,
                            CIAO::Session_Container *c);

    // Dtor.
    ~NavDisplayHome_Servant (void);

    // Factory operations

    // Finder operations

    // Operations for KeylessHome interface
    virtual ::Components::CCMObject_ptr
    create_component (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CreateFailure));

    // Operations for Implicit Home interface
    virtual HUDisplay::NavDisplay_ptr
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
    HUDisplay::NavDisplay_ptr
     _ciao_activate_component (HUDisplay::CCM_NavDisplay_ptr exe
                               ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
       ACE_THROW_SPEC ((CORBA::SystemException));

    void
    _ciao_passivate_component (HUDisplay::NavDisplay_ptr comp
                               ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // My Executor.
    HUDisplay::CCM_NavDisplayHome_var executor_;

    // My Container
    CIAO::Session_Container *container_;

    // Components this home manages.
    ACE_Hash_Map_Manager_Ex <PortableServer::ObjectId,
                             NavDisplay_Servant*,
                             TAO_ObjectId_Hash,
                             ACE_Equal_To<PortableServer::ObjectId>,
                             ACE_SYNCH_MUTEX> component_map_;
  };

  extern "C" NAVDISPLAY_SVNT_Export ::PortableServer::Servant
  createNavDisplayHome_Servant (::Components::HomeExecutorBase_ptr p,
                                CIAO::Session_Container *c
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS);
}

#if defined (__ACE_INLINE__)
# include "NavDisplay_svnt.inl"
#endif /* __ACE_INLINE__ */


#include "ace/post.h"
#endif /* CIAO_GLUE_SESSION_NAVDISPLAY_SVNT_H */
