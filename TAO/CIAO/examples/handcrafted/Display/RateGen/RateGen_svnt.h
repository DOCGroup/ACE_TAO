// $Id$

// ===========================================================
//
// @file RateGen_svnt.h
//
//    This is a pseudo-meta generic servant implementations template
//    for CIAO's CIDL compiler.  It demonstrates how a servant
//    implementation for a session component should look like.
//
// @author Nanbor Wang <nanbor@cs.wustl.edu>
//
// ===========================================================

#ifndef CIAO_GLUE_SESSION_RATEGEN_SVNT_H
#define CIAO_GLUE_SESSION_RATEGEN_SVNT_H
#include "ace/pre.h"

#include "RateGenS.h"        // Source in the skeletons for component
                                // client-view equivalent interfaces
#include "RateGenEC.h"       // Source in the executor mapping
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
  class RATEGEN_SVNT_Export RateGen_Context :
    public virtual HUDisplay::CCM_RateGen_Context,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    // We will allow the the servant glue code we generate to access
    // our states.
    friend class RateGen_Servant;

    // Ctor.
    RateGen_Context (::Components::CCMHome_ptr home,
                     ::CIAO::Session_Container *c,
                     RateGen_Servant *sv);

    // Dtor.
    virtual ~RateGen_Context ();

    void push_Pulse (HUDisplay::tick *ev
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations for ::Components::CCMContext
    virtual ::Components::Principal_ptr get_caller_principal (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::Components::CCMHome_ptr get_CCM_home (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual CORBA::Boolean get_rollback_only (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::IllegalState));

    virtual ::Components::Transaction::UserTransaction_ptr get_user_transaction (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::IllegalState));

    virtual CORBA::Boolean is_caller_in_role (const char * role
                                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void set_rollback_only (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::IllegalState));

    // Operations for ::Components::SessionContext interface
    virtual CORBA::Object_ptr get_CCM_object (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::IllegalState));

  protected:
    // We need to generate, in protected section, stuff that manage
    // connections and consumers of this component.

    // Operations for publishes interfaces.
    ::Components::Cookie_ptr
    subscribe_Pulse (HUDisplay::tickConsumer_ptr c
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::ExceededConnectionLimit));

    HUDisplay::tickConsumer_ptr
    unsubscribe_Pulse (::Components::Cookie_ptr ck
                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::InvalidConnection));

    ACE_Active_Map_Manager<HUDisplay::tickConsumer_var> ciao_publishes_Pulse_map_;

  protected:
    /// Cached component home reference.
    ::Components::CCMHome_var home_;

    /// session container
    ::CIAO::Session_Container *container_;

    /// Reference back to owner.
    RateGen_Servant *servant_;

    /// @@ Cached component reference.
    HUDisplay::RateGen_var component_;
  };

  //////////////////////////////////////////////////////////////////
  // Component Servant Glue code implementation
  class RATEGEN_SVNT_Export RateGen_Servant
    : public virtual POA_HUDisplay::RateGen, // full skeleton name here
      public virtual PortableServer::RefCountServantBase
  {
  public:
    // Ctor.
    RateGen_Servant (HUDisplay::CCM_RateGen_ptr executor,
                              ::Components::CCMHome_ptr home,
                              ::CIAO::Session_Container *c);

    // Dtor.
    ~RateGen_Servant (void);

    // Operations from supported interfaces and inherited components

    virtual void start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void stop (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual CORBA::Boolean active (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual CORBA::Long hertz (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void hertz (CORBA::Long hertz
                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations for provides interfaces.

    // Operations for receptacles interfaces.

    // Operations for consumers interfaces.

    // Operations for emits interfaces.

    // Operations for publishes interfaces.
    virtual ::Components::Cookie_ptr
    subscribe_Pulse (HUDisplay::tickConsumer_ptr c
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::ExceededConnectionLimit));

    virtual HUDisplay::tickConsumer_ptr
    unsubscribe_Pulse (::Components::Cookie_ptr ck
                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::InvalidConnection));

    // Operations for Navigation interface
    virtual CORBA::Object_ptr provide_facet (const char * name
                                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException ,
                       Components::InvalidName)) ;
    virtual ::Components::FacetDescriptions * get_all_facets (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
    virtual ::Components::FacetDescriptions * get_named_facets (const Components::NameList & names
                                                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException ,
                       Components::InvalidName));
    virtual CORBA::Boolean same_component (CORBA::Object_ptr object_ref
                                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations for Receptacles interface
    virtual ::Components::Cookie_ptr connect (const char * name,
                                            CORBA::Object_ptr connection
                                            ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName,
                       Components::InvalidConnection,
                       Components::AlreadyConnected,
                       Components::ExceededConnectionLimit));
    virtual CORBA::Object_ptr disconnect (const char * name,
                                          Components::Cookie_ptr ck
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName,
                       Components::InvalidConnection,
                       Components::CookieRequired,
                       Components::NoConnection));
    virtual ::Components::ConnectionDescriptions * get_connections (const char * name
                                                                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName));

    virtual ::Components::ReceptacleDescriptions * get_all_receptacles (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
    virtual ::Components::ReceptacleDescriptions * get_named_receptacles (const Components::NameList & names
                                                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName));

    // Operations for Events interface
    virtual ::Components::EventConsumerBase_ptr get_consumer (const char * sink_name
                                                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName));
    virtual ::Components::Cookie_ptr subscribe (const char * publisher_name,
                                                Components::EventConsumerBase_ptr subscriber
                                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName,
                       Components::InvalidConnection,
                       Components::ExceededConnectionLimit));
    virtual ::Components::EventConsumerBase_ptr unsubscribe (const char * publisher_name,
                                                             Components::Cookie_ptr ck
                                                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName,
                       Components::InvalidConnection));
    virtual void connect_consumer (const char * emitter_name,
                                   Components::EventConsumerBase_ptr consumer
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName,
                       Components::AlreadyConnected,
                       Components::InvalidConnection));
    virtual ::Components::EventConsumerBase_ptr disconnect_consumer (const char * source_name
                                                                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName,
                       Components::NoConnection));
    virtual ::Components::ConsumerDescriptions * get_all_consumers (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
    virtual ::Components::ConsumerDescriptions * get_named_consumers (const Components::NameList & names
                                                                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName));
    virtual ::Components::EmitterDescriptions * get_all_emitters (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
    virtual ::Components::EmitterDescriptions * get_named_emitters (const Components::NameList & names
                                                                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName));
    virtual ::Components::PublisherDescriptions * get_all_publishers (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
    virtual ::Components::PublisherDescriptions * get_named_publishers (const Components::NameList & names
                                                                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidName));

    // Operations for CCMObject interface
    virtual ::CORBA::IRObject_ptr get_component_def (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
    virtual ::Components::CCMHome_ptr get_ccm_home (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
    virtual ::Components::PrimaryKeyBase * get_primary_key (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::NoKeyAvailable));
    virtual void configuration_complete (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::InvalidConfiguration));
    virtual void remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::RemoveFailure));
    virtual ::Components::ComponentPortDescription * get_all_ports (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // get_component implementation.
    virtual CORBA::Object_ptr _get_component (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
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
    HUDisplay::CCM_RateGen_var executor_;

    // My Run-time Context.
    RateGen_Context *context_;

    // Managing container.
    ::CIAO::Session_Container *container_;
  };

  //////////////////////////////////////////////////////////////////
  // Component Home Glue code implementation

  // Foreach component home
  class RATEGEN_SVNT_Export RateGenHome_Servant :
    public virtual POA_HUDisplay::RateGenHome, // full skeleton name here
    public virtual PortableServer::RefCountServantBase
  {
  public:
    // Ctor.
    RateGenHome_Servant (HUDisplay::CCM_RateGenHome_ptr exe,
                         CIAO::Session_Container *c);

    // Dtor.
    ~RateGenHome_Servant (void);

    // User defined and inherited operations

    // Factory operations.
    virtual HUDisplay::RateGen_ptr new_RateGen (CORBA::Long hertz
                                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::CreateFailure));

    // Finder operations.


    // Operations for KeylessHome interface
    virtual ::Components::CCMObject_ptr
    create_component (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CreateFailure));

    // Operations for Implicit Home interface
    virtual HUDisplay::RateGen_ptr
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

    virtual void
    remove_component (Components::CCMObject_ptr comp
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::RemoveFailure));

  protected:
    // Helper method for factory operations.
     HUDisplay::RateGen_ptr
     _ciao_activate_component (HUDisplay::CCM_RateGen_ptr exe
                               ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
       ACE_THROW_SPEC ((CORBA::SystemException));

    void
    _ciao_passivate_component (HUDisplay::RateGen_ptr comp
                               ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // My Executor.
    HUDisplay::CCM_RateGenHome_var executor_;

    // My Container
    CIAO::Session_Container *container_;

    // Components this home manages.
    ACE_Hash_Map_Manager_Ex <PortableServer::ObjectId,
                             RateGen_Servant*,
                             TAO_ObjectId_Hash,
                             ACE_Equal_To<PortableServer::ObjectId>,
                             ACE_SYNCH_MUTEX> component_map_;
  };
}

extern "C" RATEGEN_SVNT_Export ::PortableServer::Servant
createRateGenHome_Servant (::Components::HomeExecutorBase_ptr p,
                           CIAO::Session_Container *c
                           ACE_ENV_ARG_DECL_WITH_DEFAULTS);

#if defined (__ACE_INLINE__)
# include "RateGen_svnt.inl"
#endif /* __ACE_INLINE__ */


#include "ace/post.h"
#endif /* CIAO_GLUE_SESSION_RATEGEN_SVNT_H */
