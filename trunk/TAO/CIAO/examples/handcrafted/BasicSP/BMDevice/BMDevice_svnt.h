// $Id$

// ===========================================================
/**
 *
 * @file BMDevice_svnt.h
 *
 * Internals of the BMDevice component that would mostly be
 * generated.
 *
 * @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
// ===========================================================

#ifndef CIAO_GLUE_SESSION_BMDEVICE_SVNT_H
#define CIAO_GLUE_SESSION_BMDEVICE_SVNT_H
#include "ace/pre.h"

#include "BMDeviceS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "BMDeviceEC.h"
#include "ciao/Container_Base.h"
#include "tao/LocalObject.h"
#include "tao/PortableServer/Key_Adapters.h"

namespace CIAO_GLUE_BasicSP
{
  /**
   *
   * @brief
   *
   * The BMDevice communicates with just one component in BasicSP and
   * that is the airframe which is a BM_ClosedEDComponent, as per
   * Boeing's OEP terminology.
   */
  class BMDEVICE_SVNT_Export BMDevice_Context :
    public virtual BasicSP::CCM_BMDevice_Context,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    friend class BMDevice_Servant;

    // Ctor.
    BMDevice_Context (::Components::CCMHome_ptr home,
                      ::CIAO::Session_Container *c,
                      BMDevice_Servant *sv);

    // Dtor.
    virtual ~BMDevice_Context ();

    /// Template methods...
    // Operations for [component name] event source, and
    // receptacles defined in CCM_[component name]_Context.

    virtual void push_data_available (BasicSP::DataAvailable *av
                                      ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Template operations for ::Components::CCMContext
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

    // Operations for publishes interfaces.
    ::Components::Cookie *
    subscribe_data_available (BasicSP::DataAvailableConsumer *c
                              ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::ExceededConnectionLimit));

    BasicSP::DataAvailableConsumer_ptr
    unsubscribe_data_available (::Components::Cookie *ck
                                ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::InvalidConnection));

    ACE_Active_Map_Manager<BasicSP::DataAvailableConsumer_var> ciao_publishes_Ready_map_;

  protected:
    /// Cached component home reference.
    ::Components::CCMHome_var home_;

    /// session container
    ::CIAO::Session_Container *container_;

    /// Reference back to owner.
    BMDevice_Servant *servant_;

    /// @@ Cached component reference.
    BasicSP::BMDevice_var component_;
  };

  class BMDEVICE_STUB_Export BMDevice_Servant
    : public virtual POA_BasicSP::BMDevice, // full skeleton name here
      public virtual PortableServer::RefCountServantBase
  {
  public:
    // Ctor.
    BMDevice_Servant (BasicSP::CCM_BMDevice_ptr executor,
                      ::Components::CCMHome_ptr home,
                      ::CIAO::Session_Container *c);

    // Dtor.
    ~BMDevice_Servant (void);

    // Operations for provides interfaces.
    virtual BasicSP::ReadData_ptr
    provide_data_read (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // First we need to generate the event sink specific servant
    class BMDEVICE_SVNT_Export TimeOutConsumer_timeout_Servant
      : public virtual POA_BasicSP::TimeOutConsumer, // full skeleton name here
        public virtual PortableServer::RefCountServantBase
    {
    public:
      // Constructor and destructor.
      TimeOutConsumer_timeout_Servant (BasicSP::CCM_BMDevice_ptr executor,
                                       BasicSP::CCM_BMDevice_Context_ptr c);

      ~TimeOutConsumer_timeout_Servant (void);

      virtual void push_TimeOut (BasicSP::TimeOut *evt
                                 ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

      // Inherit from ::Compopnents::EventBConsumerBase
      virtual void push_event (Components::EventBase *ev
                               ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         ::Components::BadEventType));

      // get_component implementation.
      virtual CORBA::Object_ptr
      _get_component (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

    protected:
      // Consumer Executor
      BasicSP::CCM_BMDevice_var executor_;

      // Context object.
      BasicSP::CCM_BMDevice_Context_var ctx_;
    };


    virtual BasicSP::TimeOutConsumer_ptr
    get_consumer_timeout (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));


    // Operations for publishes interfaces.
    virtual ::Components::Cookie *
    subscribe_data_available (BasicSP::DataAvailableConsumer_ptr c
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::ExceededConnectionLimit));

    virtual BasicSP::DataAvailableConsumer_ptr
    unsubscribe_data_available (::Components::Cookie *ck
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       ::Components::InvalidConnection));

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
    BasicSP::CCM_BMDevice_var executor_;

    // My Run-time Context.
    BMDevice_Context *context_;

    // Managing container.
    ::CIAO::Session_Container *container_;

    // Cached provided interfaces.
    BasicSP::TimeOutConsumer_var consumes_timeout_;

    // Cached provided interfaces.
    BasicSP::ReadData_var provide_ReadData_;

  };


  //////////////////////////////////////////////////////////////////
  // Component Home Glue code implementation

  // Foreach component home
  class BMDEVICE_SVNT_Export BMDeviceHome_Servant :
    public virtual POA_BasicSP::BMDeviceHome, // full skeleton name here
    public virtual PortableServer::RefCountServantBase
  {
  public:
    // Ctor.
    BMDeviceHome_Servant (BasicSP::CCM_BMDeviceHome_ptr exe,
                            CIAO::Session_Container *c);

    // Dtor.
    ~BMDeviceHome_Servant (void);

    // Factory operations

    // Finder operations

    // Operations for KeylessHome interface
    virtual ::Components::CCMObject_ptr
    create_component (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CreateFailure));

    // Operations for Implicit Home interface
    virtual BasicSP::BMDevice_ptr
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
    BasicSP::BMDevice_ptr
     _ciao_activate_component (BasicSP::CCM_BMDevice_ptr exe
                               ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
       ACE_THROW_SPEC ((CORBA::SystemException));

    void
    _ciao_passivate_component (BasicSP::BMDevice_ptr comp
                               ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // My Executor.
    BasicSP::CCM_BMDeviceHome_var executor_;

    // My Container
    CIAO::Session_Container *container_;

    // Components this home manages.
    ACE_Hash_Map_Manager_Ex <PortableServer::ObjectId,
                             BMDevice_Servant*,
                             TAO_ObjectId_Hash,
                             ACE_Equal_To<PortableServer::ObjectId>,
                             ACE_SYNCH_MUTEX> component_map_;
  };

  extern "C" BMDEVICE_SVNT_Export ::PortableServer::Servant
  createBMDeviceHome_Servant (::Components::HomeExecutorBase_ptr p,
                              CIAO::Session_Container *c
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);
}



#include "ace/post.h"
#endif /* CIAO_GLUE_SESSION_NAVDISPLAY_SVNT_H */
