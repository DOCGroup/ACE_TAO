// $Id$

#include "hello_servants.h"

// @@: Denote stuff that need to be fixed later.
// @@@: Denote stuff that need to be fixed now.

CIAO_HelloWorld_Context::CIAO_HelloWorld_Context (::Components::CCMHome_ptr h,
                                                  ::CIAO::Session_Container *c,
                                                  CIAO_HelloWorld_Servant *sv)
  : home_ (::Components::CCMHome::_duplicate (h)),
    container_ (c),
    servant_ (sv)
{

}

CIAO_HelloWorld_Context::~CIAO_HelloWorld_Context ()
{

}

// Operations for ::Components::SessionContext interface
CORBA::Object_ptr
CIAO_HelloWorld_Context::get_CCM_object (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::IllegalState))
{
  // @@ How do I check for IllegalState here?  When it's not in a
  //    callback operation...
  //    ACE_THROW_RETURN (::Components::IllegalState (), 0);

  if (CORBA::is_nil (this->component_.in ()))
    {
      CORBA::Object_var obj =  this->container_->get_objref (this->servant_
                                                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      this->component_ = HelloWorld::_narrow (obj.in ()
                                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (CORBA::is_nil (this->component_.in ()))
        ACE_THROW_RETURN (CORBA::INTERNAL (), 0); // This should not happen...
    }
  return HelloWorld::_duplicate (this->component_.in ());
}

// Operations for HellowWorld attributes, event source, and
// receptable defined in CCM_HelloWorld_Context.

// Operations for ::Components::CCMContext
::Components::Principal_ptr
CIAO_HelloWorld_Context::get_caller_principal (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ We don't support Security in CIAO yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

::Components::CCMHome_ptr
CIAO_HelloWorld_Context::get_CCM_home (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ::Components::CCMHome::_duplicate (this->home_.in ());
}

CORBA::Boolean
CIAO_HelloWorld_Context::get_rollback_only (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::IllegalState))
{
  // @@ We don't support Transaction in CIAO yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

::Components::Transaction::UserTransaction_ptr
CIAO_HelloWorld_Context::get_user_transaction (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::IllegalState))
{
  // @@ We don't support Transaction in CIAO yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::Boolean
CIAO_HelloWorld_Context::is_caller_in_role (const char * role
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (role);

  // @@ We don't support Transaction in CIAO yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

void
CIAO_HelloWorld_Context::set_rollback_only (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::IllegalState))
{
  // @@ We don't support Transaction in CIAO yet.
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}


////////////////////////////////////////////////////////////////
/////////////////// CIAO_HelloWorld_Servant ////////////////////

CIAO_HelloWorld_Servant::CIAO_HelloWorld_Servant (CCM_HelloWorld_ptr exe,
                                                  ::Components::CCMHome_ptr h,
                                                  ::CIAO::Session_Container *c)
  : executor_ (CCM_HelloWorld::_duplicate (exe)),
    container_ (c)
{
  this->context_ = new CIAO_HelloWorld_Context (h, c, this);

  ACE_TRY_NEW_ENV;
    {
      Components::SessionComponent_var temp =
        Components::SessionComponent::_narrow (exe
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (! CORBA::is_nil (temp.in ()))
        temp->set_session_context (this->context_.in ()
                                   ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      // @@ Ignore any exceptions?  What happens if
      // set_session_context throws an CCMException?
    }
  ACE_ENDTRY;
}

CIAO_HelloWorld_Servant::~CIAO_HelloWorld_Servant (void)
{
  ACE_TRY_NEW_ENV;
  {
    Components::SessionComponent_var temp =
      Components::SessionComponent::_narrow (this->executor_.in ()
                                             ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (! CORBA::is_nil (temp.in ()))
      temp->ccm_remove (ACE_ENV_SINGLE_ARG_PARAMETER);
  }
  ACE_CATCHANY
    {
      // @@ Ignore any exceptions?  What happens if
      // set_session_context throws an CCMException?
    }
  ACE_ENDTRY;
}

// Operations for supported interfaces.
// Explicit opereations and attribute operations.
char *
CIAO_HelloWorld_Servant::sayhello (const char * username
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->executor_.in () == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

  return this->executor_->sayhello (username ACE_ENV_ARG_DECL);
}

// Operations for provides interfaces.

// Operations for receptacles interfaces.

// Operations for emits interfaces.

// Operations for consumers interfaces.

// Operations for publishes interfaces.

// Operations for Navigation interface
CORBA::Object_ptr
CIAO_HelloWorld_Servant::provide_facet (const char * name
                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException ,
                   Components::InvalidName))
{
  ACE_UNUSED_ARG (name);

  // @@ We should probably provide a more generic contruct
  // (Hash_Map?) to manage the navigation/receptacles/events
  // interface.

  // This component doesn't provide any facets so whatever name you
  // pass in is not a valid facet name.
  ACE_THROW_RETURN (Components::InvalidName (), 0);
}

::Components::FacetDescriptions *
CIAO_HelloWorld_Servant::get_all_facets (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::FacetDescriptions_var collection
      = new ::Components::FacetDescriptions (1);

  ::Components::FacetDescription_var x
    = new ::OBV_Components::FacetDescription;


  x->Name ((const char *)"a_name");
  x->type_id ("repo id");
  x->facet_ref (CORBA::Object::_nil ());

  CORBA::ULong i = 0;
  collection[i] = x;

  return collection._retn ();
  // No facet for this component.
  //  return new ::Components::FacetDescriptions;
}

::Components::FacetDescriptions *
CIAO_HelloWorld_Servant::get_named_facets (const Components::NameList & names
                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException ,
                   Components::InvalidName))
{
  // No facet for this component.
  if (names.length () != 0)
    ACE_THROW_RETURN (Components::InvalidName (), 0);

  // Or return an empty description sequence if you are not asking
  // for anyhing in particular.
  return new ::Components::FacetDescriptions;
}

CORBA::Boolean
CIAO_HelloWorld_Servant::same_component (CORBA::Object_ptr object_ref
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (object_ref);

  // @@@ Need to contact the POA, get the object reference, and invoke
  // _is_equivalent on the object reference.

  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

// Operations for Receptacles interface
::Components::Cookie *
CIAO_HelloWorld_Servant::connect (const char * name,
                                  CORBA::Object_ptr connection
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::InvalidConnection,
                   Components::AlreadyConnected,
                   Components::ExceededConnectionLimit))
{
  ACE_UNUSED_ARG (name);
  ACE_UNUSED_ARG (connection);
  // @@ We need to come up with a generic interface to manage
  // these connections.

  ACE_THROW_RETURN (::Components::InvalidName (), 0);
}

CORBA::Object_ptr
CIAO_HelloWorld_Servant::disconnect (const char * name,
                                     Components::Cookie * ck
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::InvalidConnection,
                   Components::CookieRequired,
                   Components::NoConnection))
{
  ACE_UNUSED_ARG (name);
  ACE_UNUSED_ARG (ck);

  ACE_THROW_RETURN (::Components::InvalidName (), 0);
}

::Components::ConnectionDescriptions *
CIAO_HelloWorld_Servant::get_connections (const char * name
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  ACE_UNUSED_ARG (name);

  ACE_THROW_RETURN (::Components::InvalidName (), 0);
}

::Components::ReceptacleDescriptions *
CIAO_HelloWorld_Servant::get_all_receptacles (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // No receptacles.  Return an empty sequence.
  return new ::Components::ReceptacleDescriptions;
}

::Components::ReceptacleDescriptions *
CIAO_HelloWorld_Servant::get_named_receptacles (const Components::NameList & names
                                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  // No facet for this component.
  if (names.length () != 0)
    ACE_THROW_RETURN (Components::InvalidName (), 0);

  // Or return an empty description sequence if you are not asking
  // for anyhing in particular.
  return new ::Components::ReceptacleDescriptions;
}

// Operations for Events interface
::Components::EventConsumerBase_ptr
CIAO_HelloWorld_Servant::get_consumer (const char * sink_name
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  ACE_UNUSED_ARG (sink_name);

  ACE_THROW_RETURN (::Components::InvalidName (), 0);
}

::Components::Cookie *
CIAO_HelloWorld_Servant::subscribe (const char * publisher_name,
                                    Components::EventConsumerBase_ptr subscriber
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::AlreadyConnected,
                   Components::InvalidConnection))
{
  ACE_UNUSED_ARG (publisher_name);
  ACE_UNUSED_ARG (subscriber);

  ACE_THROW_RETURN (::Components::InvalidName (), 0);
}

::Components::EventConsumerBase_ptr
CIAO_HelloWorld_Servant::unsubscribe (const char * publisher_name,
                                      Components::Cookie * ck
                                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::InvalidConnection))
{
  ACE_UNUSED_ARG (publisher_name);
  ACE_UNUSED_ARG (ck);

  ACE_THROW_RETURN (::Components::InvalidName (), 0);
}

void
CIAO_HelloWorld_Servant::connect_consumer (const char * emitter_name,
                                           Components::EventConsumerBase_ptr consumer
                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::AlreadyConnected,
                   Components::InvalidConnection))
{
  ACE_UNUSED_ARG (emitter_name);
  ACE_UNUSED_ARG (consumer);

  ACE_THROW (::Components::InvalidName ());
}

::Components::EventConsumerBase_ptr
CIAO_HelloWorld_Servant::disconnect_consumer (const char * source_name
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::NoConnection))
{
  ACE_UNUSED_ARG (source_name);

  ACE_THROW_RETURN (::Components::InvalidName (), 0);
}

::Components::ConsumerDescriptions *
CIAO_HelloWorld_Servant::get_all_consumers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return new ::Components::ConsumerDescriptions;
}

::Components::ConsumerDescriptions *
CIAO_HelloWorld_Servant::get_named_consumers (const Components::NameList & names
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  // No facet for this component.
  if (names.length () != 0)
    ACE_THROW_RETURN (Components::InvalidName (), 0);

  // Or return an empty description sequence if you are not asking
  // for anyhing in particular.
  return new ::Components::ConsumerDescriptions;
}

::Components::EmitterDescriptions *
CIAO_HelloWorld_Servant::get_all_emitters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return new ::Components::EmitterDescriptions;
}

::Components::EmitterDescriptions *
CIAO_HelloWorld_Servant::get_named_emitters (const Components::NameList & names
                                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  // No facet for this component.
  if (names.length () != 0)
    ACE_THROW_RETURN (Components::InvalidName (), 0);

  // Or return an empty description sequence if you are not asking
  // for anyhing in particular.
  return new ::Components::EmitterDescriptions;
}

::Components::PublisherDescriptions *
CIAO_HelloWorld_Servant::get_all_publishers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return new ::Components::PublisherDescriptions;
}

::Components::PublisherDescriptions *
CIAO_HelloWorld_Servant::get_named_publishers (const Components::NameList & names
                                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  // No facet for this component.
  if (names.length () != 0)
    ACE_THROW_RETURN (Components::InvalidName (), 0);

  // Or return an empty description sequence if you are not asking
  // for anyhing in particular.
  return new ::Components::PublisherDescriptions;
}

// Operations for CCMObject interface
::CORBA::IRObject_ptr
CIAO_HelloWorld_Servant::get_component_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

::Components::CCMHome_ptr
CIAO_HelloWorld_Servant::get_ccm_home (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@@ How?  Should we cache the home reference here?
  return this->context_->get_CCM_home (ACE_ENV_SINGLE_ARG_PARAMETER);
}

::Components::PrimaryKeyBase *
CIAO_HelloWorld_Servant::get_primary_key (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::NoKeyAvailable))
{
  // This is a keyless component.
  ACE_THROW_RETURN (::Components::NoKeyAvailable (), 0);
}

void
CIAO_HelloWorld_Servant::configuration_complete (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidConfiguration))
{
  // @@
  return;
}

void
CIAO_HelloWorld_Servant::remove (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  // @@ Tear down connections and stuff?

  return;
}

::Components::ComponentPortDescription *
CIAO_HelloWorld_Servant::get_all_ports (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // No port, no nothing.
  return new OBV_Components::ComponentPortDescription;
}


CORBA::Object_ptr
CIAO_HelloWorld_Servant::_get_component (ACE_ENV_SINGLE_ARG_DECL)
{
  ::Components::SessionContext_var sc =
      ::Components::SessionContext::_narrow (this->context_.in ());

  if (! CORBA::is_nil(sc.in ()))
    return sc->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);

  ::Components::EntityContext_var ec =
      ::Components::EntityContext::_narrow (this->context_.in ());

  if (! CORBA::is_nil(ec.in ()))
    return ec->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);

  ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
}

void
CIAO_HelloWorld_Servant::_ciao_activate (ACE_ENV_SINGLE_ARG_DECL)
{
  Components::SessionComponent_var temp =
    Components::SessionComponent::_narrow (this->executor_.in ()
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (! CORBA::is_nil (temp.in ()))
    temp->ccm_activate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
CIAO_HelloWorld_Servant::_ciao_passivate (ACE_ENV_SINGLE_ARG_DECL)
{
  Components::SessionComponent_var temp =
    Components::SessionComponent::_narrow (this->executor_.in ()
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (! CORBA::is_nil (temp.in ()))
    temp->ccm_passivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

////////////////////////////////////////////////////////////////
//////////////////// CIAO_HelloHome_Servant ////////////////////

CIAO_HelloHome_Servant::CIAO_HelloHome_Servant (CCM_HelloHome_ptr exe,
                                                CIAO::Session_Container *c)
  : executor_ (CCM_HelloHome::_duplicate (exe)),
    container_ (c)
{
}

CIAO_HelloHome_Servant::~CIAO_HelloHome_Servant (void)
{
}

// User defined and inherited operations
// (Factories, Finders, and explicit operations.)

// Operations for KeylessHome interface
::Components::CCMObject_ptr
CIAO_HelloHome_Servant::create_component (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CreateFailure))
{
  return this->create (ACE_ENV_SINGLE_ARG_PARAMETER);
}

HelloWorld_ptr
CIAO_HelloHome_Servant::_ciao_activate_component (CCM_HelloWorld_ptr exe
                                                  ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_var hobj= this->container_->get_objref (this
                                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ::Components::CCMHome_var home = ::Components::CCMHome::_narrow (hobj.in ()
                                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CIAO_HelloWorld_Servant *svt = new CIAO_HelloWorld_Servant (exe,
                                                              home.in (),
                                                              this->container_);
  PortableServer::ServantBase_var safe (svt);
  PortableServer::ObjectId_var oid;

  CORBA::Object_var objref
    = this->container_->install_component (svt,
                                           oid.out ()
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  svt->_ciao_activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  HelloWorld_var ho = HelloWorld::_narrow (objref.in ()
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (this->component_map_.bind (oid.in (), svt) == 0)
    {
      // @@ what should happen if bind fail?
      safe._retn ();
    }
  return ho._retn ();
}

void
CIAO_HelloHome_Servant::_ciao_passivate_component (HelloWorld_ptr comp
                                                   ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid;

  this->container_->uninstall_component (comp,
                                         oid.out ()
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CIAO_HelloWorld_Servant *servant = 0;
  if (this->component_map_.unbind (oid.in (), servant) == 0)
    {
      PortableServer::ServantBase_var safe (servant);
      servant->_ciao_passivate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
  // What happen if unbind failed?

}

// Operations for Implicit Home interface
::HelloWorld_ptr
CIAO_HelloHome_Servant::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CreateFailure))
{
  if (this->executor_.in () == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

  Components::EnterpriseComponent_var com =
    this->executor_->create (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CCM_HelloWorld_var hw = CCM_HelloWorld::_narrow (com.in ()
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->_ciao_activate_component (hw.in ()
                                         ACE_ENV_ARG_PARAMETER);
}

// Operations for CCMHome interface
::CORBA::IRObject_ptr
CIAO_HelloHome_Servant::get_component_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Not implemented yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

::CORBA::IRObject_ptr
CIAO_HelloHome_Servant::get_home_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Not implemented yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

void
CIAO_HelloHome_Servant::remove_component (Components::CCMObject_ptr comp
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  HelloWorld_var hw = HelloWorld::_narrow (comp
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (hw.in ()))
    ACE_THROW (CORBA::INTERNAL ()); // What is the right exception to throw here?

  hw->remove (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Removing the object reference?  get the servant from the POA with
  // the objref, and call remove() on the component, passivate the
  // component, and then remove-ref the servant?

  this->_ciao_passivate_component (hw.in ()
                                   ACE_ENV_ARG_PARAMETER);
}



extern "C" HELLO_SERVANT_Export ::PortableServer::Servant
createHelloHome_Servant (::Components::HomeExecutorBase_ptr p,
                         CIAO::Session_Container *c
                         ACE_ENV_ARG_DECL)
{
  if (p == 0)
    return 0;

  CCM_HelloHome_var x = CCM_HelloHome::_narrow (p
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (CORBA::is_nil (x.in ()))
    return 0;

  return new CIAO_HelloHome_Servant (x.in (),
                                     c);
}
