// $Id$
#include "BMDisplay_svnt.h"
#include "Cookies.h"

BasicSP::ReadData_ptr
CIAO_GLUE_BasicSP::BMDisplay_Context::get_connection_comp_data (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return BasicSP::ReadData::_duplicate (this->ciao_uses_comp_data_.in ());
}

// Simplex [receptacle name] connection management operations
void
CIAO_GLUE_BasicSP::BMDisplay_Context::connect_comp_data (BasicSP::ReadData_ptr c
                                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Components::AlreadyConnected,
                   ::Components::InvalidConnection))
{
  if (! CORBA::is_nil (this->ciao_uses_comp_data_.in ()))
    ACE_THROW (::Components::AlreadyConnected ());

  if (CORBA::is_nil (c))
    ACE_THROW (::Components::InvalidConnection ());

  // When do we throw InvalidConnection exception?
  this->ciao_uses_comp_data_ = BasicSP::ReadData::_duplicate (c);
}

BasicSP::ReadData_ptr
CIAO_GLUE_BasicSP::BMDisplay_Context::disconnect_comp_data (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Components::NoConnection))
{
  if (CORBA::is_nil (this->ciao_uses_comp_data_.in ()))
    ACE_THROW (::Components::NoConnection ());

  return this->ciao_uses_comp_data_._retn ();
}

// Operations for ::Components::SessionContext interface
CORBA::Object_ptr
CIAO_GLUE_BasicSP::BMDisplay_Context::get_CCM_object (ACE_ENV_SINGLE_ARG_DECL)
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

      this->component_ = BasicSP::BMDisplay::_narrow (obj.in ()
                                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (CORBA::is_nil (this->component_.in ()))
        ACE_THROW_RETURN (CORBA::INTERNAL (), 0); // This should not happen...
    }
  return BasicSP::BMDisplay::_duplicate (this->component_.in ());
}


CIAO_GLUE_BasicSP::BMDisplay_Context::BMDisplay_Context (::Components::CCMHome_ptr home,
                                                             ::CIAO::Session_Container *c,
                                                             CIAO_GLUE_BasicSP::BMDisplay_Servant *sv)
  : home_ (::Components::CCMHome::_duplicate (home)),
    container_ (c),
    servant_ (sv)
{

}


CIAO_GLUE_BasicSP::BMDisplay_Context::~BMDisplay_Context ()
{
}

// Operations for emits interfaces.

// Operations for ::Components::CCMContext
 ::Components::Principal_ptr
CIAO_GLUE_BasicSP::BMDisplay_Context::get_caller_principal (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ We don't support Security in CIAO yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

 ::Components::CCMHome_ptr
CIAO_GLUE_BasicSP::BMDisplay_Context::get_CCM_home (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ::Components::CCMHome::_duplicate (this->home_.in ());
}

 CORBA::Boolean
CIAO_GLUE_BasicSP::BMDisplay_Context::get_rollback_only (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::IllegalState))
{
  // @@ We don't support Transaction in CIAO yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

 ::Components::Transaction::UserTransaction_ptr
CIAO_GLUE_BasicSP::BMDisplay_Context::get_user_transaction (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::IllegalState))
{
  // @@ We don't support Transaction in CIAO yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

 CORBA::Boolean
CIAO_GLUE_BasicSP::BMDisplay_Context::is_caller_in_role (const char * role
                                                            ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (role);

  // @@ We don't support Transaction in CIAO yet.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

 void
CIAO_GLUE_BasicSP::BMDisplay_Context::set_rollback_only (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::IllegalState))
{
  // @@ We don't support Transaction in CIAO yet.
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}


//////////////////////////////////////////////////////////////////
// Component Servant Glue code implementation
//////////////////////////////////////////////////////////////////

CIAO_GLUE_BasicSP::BMDisplay_Servant::BMDisplay_Servant (BasicSP::CCM_BMDisplay_ptr exe,
                                                             ::Components::CCMHome_ptr h,
                                                             ::CIAO::Session_Container *c)
  : executor_ (BasicSP::CCM_BMDisplay::_duplicate (exe)),
    container_ (c)
{
  this->context_ = new CIAO_GLUE_BasicSP::BMDisplay_Context (h, c, this);

  ACE_TRY_NEW_ENV
    {
      Components::SessionComponent_var scom =
        Components::SessionComponent::_narrow (exe
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (! CORBA::is_nil (scom.in ()))
        scom->set_session_context (this->context_
                                   ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      // @@ Ignore any exceptions?  What happens if
      // set_session_context throws an CCMException?
    }
  ACE_ENDTRY;
}

CIAO_GLUE_BasicSP::BMDisplay_Servant::~BMDisplay_Servant (void)
{
  ACE_TRY_NEW_ENV
    {
      Components::SessionComponent_var scom =
        Components::SessionComponent::_narrow (this->executor_.in ()
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (! CORBA::is_nil (scom.in ()))
        scom->ccm_remove (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      // @@ Ignore any exceptions?  What happens if
      // set_session_context throws an CCMException?
    }
  ACE_ENDTRY;
  this->context_->_remove_ref ();
}

// Operations for provides interfaces.

// Operations for consumers interfaces.

// EventConsumer Glue Code implementation
// Inherit from ::Compopnents::EventBConsumerBase
void
CIAO_GLUE_BasicSP::BMDisplay_Servant::DataAvailableConsumer_data_ready_Servant::push_event (::Components::EventBase *ev
                                                                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Components::BadEventType))
{
  BasicSP::DataAvailable_var ev_type = BasicSP::DataAvailable::_downcast (ev);
  if (ev_type != 0)
    {
      this->push_DataAvailable (ev_type.in ()
                       ACE_ENV_ARG_PARAMETER);
      return;
    }

  // @@ This include the case when we receive a parent eventtype of [eventtype]

  ACE_THROW (::Components::BadEventType ());
}

// get_component implementation.
BasicSP::DataAvailableConsumer_ptr
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_consumer_data_ready (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (this->consumes_data_ready_.in ()))
    {
      CIAO_GLUE_BasicSP::BMDisplay_Servant::DataAvailableConsumer_data_ready_Servant *svt =
        new CIAO_GLUE_BasicSP::BMDisplay_Servant::DataAvailableConsumer_data_ready_Servant (this->executor_.in (),
                                                                                   this->context_);
      PortableServer::ServantBase_var safe_servant (svt);

      CORBA::Object_var obj = this->container_->install_servant (svt
                                                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      BasicSP::DataAvailableConsumer_var eco
        = BasicSP::DataAvailableConsumer::_narrow (obj.in ()
                                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      this->consumes_data_ready_ = eco;
    }

  return BasicSP::DataAvailableConsumer::_duplicate (this->consumes_data_ready_.in ());
}

// Operations for Navigation interface
CORBA::Object_ptr
CIAO_GLUE_BasicSP::BMDisplay_Servant::provide_facet (const char * name
                                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException ,
                   Components::InvalidName))
{
  ACE_UNUSED_ARG (name);
  ACE_THROW_RETURN (::Components::InvalidName (), 0);
}

::Components::FacetDescriptions *
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_all_facets (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::FacetDescriptions_var collection
      = new ::Components::FacetDescriptions (0); // #99 = number of all provided
                                                   // facets including those inherited
                                                   // from parent component(s).
  return collection._retn ();

}

::Components::FacetDescriptions *
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_named_facets (const Components::NameList & names
                                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  ACE_UNUSED_ARG (names);
  ACE_THROW_RETURN (::Components::InvalidName (), 0);
}

CORBA::Boolean
CIAO_GLUE_BasicSP::BMDisplay_Servant::same_component (CORBA::Object_ptr object_ref
                                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (object_ref))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);

  CORBA::Object_var the_other = object_ref->_get_component (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Object_var me = this->context_->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return me->_is_equivalent (object_ref
                             ACE_ENV_ARG_PARAMETER);
}

// Operations for Receptacles interface
::Components::Cookie *
CIAO_GLUE_BasicSP::BMDisplay_Servant::connect (const char * name,
                                                  CORBA::Object_ptr connection
                                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::InvalidConnection,
                   Components::AlreadyConnected,
                   Components::ExceededConnectionLimit))
{
  // @@ We can omit this if clause if there's no receptacle in this component.
  if (name == 0)
    ACE_THROW_RETURN (Components::InvalidName (), 0);

  if (ACE_OS_String::strcmp (name, "comp_data") == 0)
    {
      BasicSP::ReadData_var _ciao_conn =
        BasicSP::ReadData::_narrow (connection
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (CORBA::is_nil (_ciao_conn.in ()))
        ACE_THROW_RETURN (::Components::InvalidConnection (), 0);

      this->connect_comp_data (_ciao_conn.in ()
                                 ACE_ENV_ARG_PARAMETER);
      return 0;
    }

  ACE_THROW_RETURN (Components::InvalidName (), 0);
}

CORBA::Object_ptr
CIAO_GLUE_BasicSP::BMDisplay_Servant::disconnect (const char * name,
                                                     Components::Cookie *ck
                                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::InvalidConnection,
                   Components::CookieRequired,
                   Components::NoConnection))
{
  ACE_UNUSED_ARG (ck);
  // @@ We can omit this if clause if there's no receptacle in this component.
  if (name == 0)
    ACE_THROW_RETURN (Components::InvalidName (), 0);

  if (ACE_OS_String::strcmp (name, "comp_data") == 0)
    return this->disconnect_comp_data (ACE_ENV_SINGLE_ARG_PARAMETER);

  ACE_THROW_RETURN (Components::InvalidName (), 0);
}

::Components::ConnectionDescriptions *
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_connections (const char * name
                                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  // @@ We can omit this if clause if there's no receptacle in this component.
  if (name == 0)
    ACE_THROW_RETURN (Components::InvalidName (), 0);

  if (ACE_OS_String::strcmp (name, "comp_data") == 0)
    {
      ::Components::ConnectionDescriptions_var retv
          = new ::Components::ConnectionDescriptions;
      retv->length (1);

      ::Components::ConnectionDescription_var x
          = new OBV_Components::ConnectionDescription;
      x ->ck (0);
      x ->objref (this->get_connection_comp_data (ACE_ENV_SINGLE_ARG_PARAMETER));
      ACE_CHECK_RETURN (0);

      //      retv[0] = x._retn ();

      return retv._retn ();
    }

  ACE_THROW_RETURN (Components::InvalidName (), 0);
}

::Components::ReceptacleDescriptions *
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_all_receptacles (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::ReceptacleDescriptions_var retv =
      new ::Components::ReceptacleDescriptions (1); // #99 is number of receptacles
                                                      // this component has.
  retv->length (1);
  CORBA::ULong i = 0;

  retv[i] = new OBV_Components::ReceptacleDescription;

  retv[i]->Name ((const char *) "comp_data");
  retv[i]->type_id ((const char *) "IDL:BasicSP/ReadData:1.0");
  retv[i]->is_multiple (0);
  retv[i]->connections (*this->get_connections ("comp_data"
                                                ACE_ENV_ARG_PARAMETER));
  return retv._retn ();
}

::Components::ReceptacleDescriptions *
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_named_receptacles (const Components::NameList & names
                                                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  ::Components::ReceptacleDescriptions_var retv =
      new ::Components::ReceptacleDescriptions (names.length ());
  retv->length (names.length ());

  CORBA::ULong i = 0;
  for (; i < names.length (); ++i)
    {
      retv[i] = new ::OBV_Components::ReceptacleDescription;
      if (ACE_OS_String::strcmp (names[i].in (), "comp_data") == 0)
        {
          retv[i]->Name ((const char *) "comp_data");
          retv[i]->type_id ((const char *) "IDL:BasicSP/ReadData:1.0");
          retv[i]->is_multiple (0);
          retv[i]->connections (*this->get_connections ("comp_data"
                                                        ACE_ENV_ARG_PARAMETER));
        }
      else
        ACE_THROW_RETURN (::Components::InvalidName (), 0);
    }
  return retv._retn ();
}

// Operations for Events interface
::Components::EventConsumerBase_ptr
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_consumer (const char * sink_name
                                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  // @@ We can omit this if clause if there's no event sinks in this component.
  if (sink_name == 0)
    ACE_THROW_RETURN (Components::InvalidName (), 0);

  if (ACE_OS_String::strcmp (sink_name, "data_ready") == 0)
    return this->get_consumer_data_ready (ACE_ENV_SINGLE_ARG_PARAMETER);

  ACE_THROW_RETURN (Components::InvalidName (), 0);
}

::Components::Cookie *
CIAO_GLUE_BasicSP::BMDisplay_Servant::subscribe (const char * ,
                                                    Components::EventConsumerBase_ptr
                                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::InvalidConnection,
                   Components::ExceededConnectionLimit))
{
  ACE_THROW_RETURN (Components::InvalidName (), 0);
}

::Components::EventConsumerBase_ptr
CIAO_GLUE_BasicSP::BMDisplay_Servant::unsubscribe (const char * ,
                                                      Components::Cookie *
                                                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::InvalidConnection))
{
  ACE_THROW_RETURN (Components::InvalidName (), 0);
}

void
CIAO_GLUE_BasicSP::BMDisplay_Servant::connect_consumer (const char * ,
                                                           Components::EventConsumerBase_ptr
                                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::AlreadyConnected,
                   Components::InvalidConnection))
{
  ACE_THROW (Components::InvalidName ());
}

::Components::EventConsumerBase_ptr
CIAO_GLUE_BasicSP::BMDisplay_Servant::disconnect_consumer (const char *
                                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::NoConnection))
{
  ACE_THROW_RETURN (Components::InvalidName (), 0);
}

::Components::ConsumerDescriptions *
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_all_consumers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::ConsumerDescriptions_var retv =
      new ::Components::ConsumerDescriptions (1); // #99 is the number of consumers
                                                    // this component has.
  retv->length (1);

  CORBA::ULong i = 0;
  retv[i] = new OBV_Components::ConsumerDescription;
  retv[i]->Name ("data_ready");
  retv[i]->type_id ("IDL:BasicSP/DataAvailableConsumer:1.0");
  BasicSP::DataAvailableConsumer_var c
    = this->get_consumer_data_ready (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  retv[i]->consumer (c.in ());

  return retv._retn ();
}

::Components::ConsumerDescriptions *
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_named_consumers (const Components::NameList & names
                                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  ::Components::ConsumerDescriptions_var retv =
      new ::Components::ConsumerDescriptions (names.length ());
  retv->length (names.length ());

  CORBA::ULong i = 0;
  for (; i < names.length (); ++i)
    {
      retv[i] = new OBV_Components::ConsumerDescription;

      if (ACE_OS_String::strcmp (names[i].in (), "data_ready") == 0)
        {
          retv[i]->Name ("data_ready");
          retv[i]->type_id ("IDL:BasicSP/DataAvailableConsumer:1.0");
          BasicSP::DataAvailableConsumer_var c =
            this->get_consumer_data_ready (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

          retv[i]->consumer (c.in ());
        }
      else
        ACE_THROW_RETURN (::Components::InvalidName (), 0);
    }
  return retv._retn ();
}

::Components::EmitterDescriptions *
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_all_emitters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::EmitterDescriptions_var retv =
      new ::Components::EmitterDescriptions (0); // #99 is the number of emitters
                                                 // this component has.
  return retv._retn ();
}

::Components::EmitterDescriptions *
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_named_emitters (const Components::NameList & names
                                                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  ::Components::EmitterDescriptions_var retv =
      new ::Components::EmitterDescriptions (names.length ());
  retv->length (names.length ());

  CORBA::ULong i = 0;
  for (; i < names.length (); ++i)
    {
      ACE_THROW_RETURN (::Components::InvalidName (), 0);
    }
  return retv._retn ();
}

::Components::PublisherDescriptions *
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_all_publishers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ to-do

  // Need to add interfaces in the Context class to gather the information.
  // Or we can just relay it to the Context object.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

::Components::PublisherDescriptions *
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_named_publishers (const Components::NameList & names
                                                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  ACE_UNUSED_ARG (names);
  // @@ to-do

  // Need to add interfaces in the Context class to gather the information.
  // Or we can just relay it to the Context object.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

// Operations for CCMObject interface
::CORBA::IRObject_ptr
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_component_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ to-do: Connect to an IfR?
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

::Components::CCMHome_ptr
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_ccm_home (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->context_->get_CCM_home (ACE_ENV_SINGLE_ARG_PARAMETER);
}

::Components::PrimaryKeyBase *
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_primary_key (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::NoKeyAvailable))
{
  // This is a keyless component.
  ACE_THROW_RETURN (::Components::NoKeyAvailable (), 0);
}

void
CIAO_GLUE_BasicSP::BMDisplay_Servant::configuration_complete (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidConfiguration))
{
  // @@ to-do
  // No-op.  Don't know how to pass this info to monolithic executor.
}

void
CIAO_GLUE_BasicSP::BMDisplay_Servant::remove (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  // @@ to-do
  // Need to figure out what to do here.  E.g., tear down the all the connections
  // this component has?
}

::Components::ComponentPortDescription *
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_all_ports (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::ComponentPortDescription_var retv =
    new OBV_Components::ComponentPortDescription;

  ::Components::FacetDescriptions_var facets_desc
      = this->get_all_facets (ACE_ENV_SINGLE_ARG_PARAMETER);
  ::Components::ReceptacleDescriptions_var receptacle_desc
      = get_all_receptacles (ACE_ENV_SINGLE_ARG_PARAMETER);
  ::Components::ConsumerDescriptions_var consumer_desc
      = this->get_all_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ::Components::EmitterDescriptions_var emitter_desc
      = this->get_all_emitters (ACE_ENV_SINGLE_ARG_PARAMETER);
  ::Components::PublisherDescriptions_var publisher_desc
      = this->get_all_publishers (ACE_ENV_SINGLE_ARG_PARAMETER);

  retv->facets (facets_desc.in());
  retv->receptacles (receptacle_desc.in());
  retv->consumers (consumer_desc.in());
  retv->emitters (emitter_desc.in());
  retv->publishers (publisher_desc.in());

  return retv._retn();
}

// get_component implementation.
CORBA::Object_ptr
CIAO_GLUE_BasicSP::BMDisplay_Servant::_get_component (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::SessionContext_var sc =
      ::Components::SessionContext::_narrow (this->context_);

  if (! CORBA::is_nil(sc.in ()))
    return sc->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);

  // @@ Do we need to try the following case here?  We are afterall implementing
  // a session component here.
  ::Components::EntityContext_var ec =
      ::Components::EntityContext::_narrow (this->context_);

  if (! CORBA::is_nil(ec.in ()))
    return ec->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);

  ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
}

void
CIAO_GLUE_BasicSP::BMDisplay_Servant::_ciao_activate (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::SessionComponent_var temp =
      ::Components::SessionComponent::_narrow (this->executor_.in ()
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (! CORBA::is_nil (temp.in ()))
    temp->ccm_activate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
CIAO_GLUE_BasicSP::BMDisplay_Servant::_ciao_passivate (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::SessionComponent_var temp =
      ::Components::SessionComponent::_narrow (this->executor_.in ()
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (! CORBA::is_nil (temp.in ()))
    temp->ccm_passivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

 void
CIAO_GLUE_BasicSP::BMDisplay_Servant::connect_comp_data (BasicSP::ReadData_ptr c
                                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Components::AlreadyConnected,
                   ::Components::InvalidConnection))
{
  this->context_->connect_comp_data (c
                                       ACE_ENV_ARG_PARAMETER);
}

 BasicSP::ReadData_ptr
CIAO_GLUE_BasicSP::BMDisplay_Servant::disconnect_comp_data (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Components::NoConnection))
{
  return this->context_->disconnect_comp_data (ACE_ENV_SINGLE_ARG_PARAMETER);
}

BasicSP::ReadData_ptr
CIAO_GLUE_BasicSP::BMDisplay_Servant::get_connection_comp_data (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->context_->get_connection_comp_data (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// Constructor and destructor.

CIAO_GLUE_BasicSP::BMDisplay_Servant::DataAvailableConsumer_data_ready_Servant::DataAvailableConsumer_data_ready_Servant
(BasicSP::CCM_BMDisplay_ptr executor,
 BasicSP::CCM_BMDisplay_Context_ptr c)
  : executor_ (BasicSP::CCM_BMDisplay::_duplicate (executor)),
    ctx_ (BasicSP::CCM_BMDisplay_Context::_duplicate (c))
{
}


CIAO_GLUE_BasicSP::BMDisplay_Servant::DataAvailableConsumer_data_ready_Servant::~DataAvailableConsumer_data_ready_Servant ()
{
}

 CORBA::Object_ptr
CIAO_GLUE_BasicSP::BMDisplay_Servant::DataAvailableConsumer_data_ready_Servant::_get_component (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->ctx_->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);
}

 void
CIAO_GLUE_BasicSP::BMDisplay_Servant::DataAvailableConsumer_data_ready_Servant::push_DataAvailable
(BasicSP::DataAvailable *evt
 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->executor_->push_data_ready (evt
                                    ACE_ENV_ARG_PARAMETER);
}


//////////////////////////////////////////////////////////////////
// Component Home Glue code implementation
//////////////////////////////////////////////////////////////////

BasicSP::BMDisplay_ptr
CIAO_GLUE_BasicSP::BMDisplayHome_Servant::_ciao_activate_component (BasicSP::CCM_BMDisplay_ptr exe
                                                                       ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_var hobj
    = this->container_->get_objref (this
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ::Components::CCMHome_var home
      = ::Components::CCMHome::_narrow (hobj.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CIAO_GLUE_BasicSP::BMDisplay_Servant *svt =
    new CIAO_GLUE_BasicSP::BMDisplay_Servant (exe,
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

  BasicSP::BMDisplay_var ho
    = BasicSP::BMDisplay::_narrow (objref.in ()
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
CIAO_GLUE_BasicSP::BMDisplayHome_Servant::_ciao_passivate_component (BasicSP::BMDisplay_ptr comp
                                                                        ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var oid;

  this->container_->uninstall_component (comp,
                                         oid.out ()
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CIAO_GLUE_BasicSP::BMDisplay_Servant *servant = 0;
  if (this->component_map_.unbind (oid.in (), servant) == 0)
    {
      PortableServer::ServantBase_var safe (servant);
      servant->_ciao_passivate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
  // What happen if unbind failed?

}

// Operations for Implicit Home interface
BasicSP::BMDisplay_ptr
CIAO_GLUE_BasicSP::BMDisplayHome_Servant::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CreateFailure))
{
  if (this->executor_.in () == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

  Components::EnterpriseComponent_var _ciao_ec =
    this->executor_->create (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  BasicSP::CCM_BMDisplay_var _ciao_comp
    = BasicSP::CCM_BMDisplay::_narrow (_ciao_ec.in ()
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->_ciao_activate_component (_ciao_comp.in ()
                                         ACE_ENV_ARG_PARAMETER);
}

// Operations for CCMHome interface
void
CIAO_GLUE_BasicSP::BMDisplayHome_Servant::remove_component (Components::CCMObject_ptr comp
                                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  BasicSP::BMDisplay_var _ciao_comp
    = BasicSP::BMDisplay::_narrow (comp
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (_ciao_comp.in ()))
    ACE_THROW (CORBA::INTERNAL ()); // What is the right exception to throw here?

  // @@ It seems to me that we need to make sure this is a component
  // generated by this home before calling remove on this component.
  _ciao_comp->remove (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;


  // Removing the object reference?  get the servant from the POA with
  // the objref, and call remove() on the component, deactivate the
  // component, and then remove-ref the servant?
  this->_ciao_passivate_component (_ciao_comp.in ()
                                   ACE_ENV_ARG_PARAMETER);
}


CIAO_GLUE_BasicSP::BMDisplayHome_Servant::BMDisplayHome_Servant (BasicSP::CCM_BMDisplayHome_ptr exe,
                                                                     CIAO::Session_Container *c)
  : executor_ (BasicSP::CCM_BMDisplayHome::_duplicate (exe)),
    container_ (c)
{
}


CIAO_GLUE_BasicSP::BMDisplayHome_Servant::~BMDisplayHome_Servant (void)
{
}

// Operations for KeylessHome interface
 ::Components::CCMObject_ptr
CIAO_GLUE_BasicSP::BMDisplayHome_Servant::create_component (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CreateFailure))
{
  // Simply forward to the create method.
  return this->create (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// Operations for CCMHome interface
 ::CORBA::IRObject_ptr
CIAO_GLUE_BasicSP::BMDisplayHome_Servant::get_component_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ TO-DO.  Contact IfR?

  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::IRObject_ptr
CIAO_GLUE_BasicSP::BMDisplayHome_Servant::get_home_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ TO-DO.  Contact IfR?

  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}


extern "C" BMDISPLAY_SVNT_Export ::PortableServer::Servant
createBMDisplayHome_Servant (::Components::HomeExecutorBase_ptr p,
                              CIAO::Session_Container *c
                              ACE_ENV_ARG_DECL)
{
  if (p == 0)
    return 0;

  BasicSP::CCM_BMDisplayHome_var x
    = BasicSP::CCM_BMDisplayHome::_narrow (p
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (CORBA::is_nil (x.in ()))
    return 0;

  return new CIAO_GLUE_BasicSP::BMDisplayHome_Servant (x.in (),
                                                          c);
}
