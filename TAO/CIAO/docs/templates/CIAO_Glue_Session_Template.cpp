// $Id$

//    The generated filename for files using this template shoule be
//       [idl-basename]GS.cpp       GS --> GlueSession

// @@ Notice: [ciao module name] can expand to either CIAO_GLUE or
//    CIAO_GLUE_[module name] as defined in the header file.

#include "[idl-basename]GS.h"

#if !defined (__ACE_INLINE__)
# include "[idl-basename]GS.inl"
#endif /* __ACE_INLINE__ */

//////////////////////////////////////////////////////////////////
// Facet Glue Code implementation
// @@ We are assuming that these facets are declared under the same
//    module as the component (thus, we are placing this glue code
//    here under the same namespace.  If they are not, we will
//    either be generating them in separate namespaces, or include
//    some other CIDL generated files to get the glue code
//    implementation.
//////////////////////////////////////////////////////////////////

##foreach [facet type] in (all facet interface types in the original IDL)

// get_component implementation.
CORBA::Object_ptr
[ciao module name]::[facet type]_Servant::_get_component (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::SessionContext_var sc =
      ::Components::SessionContext::_narrow (this->ctx_);

  if (! CORBA::is_nil(sc.in ()))
    return sc->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);

  ::Components::EntityContext_var ec =
      ::Components::EntityContext::_narrow (this->ctx_);

  if (! CORBA::is_nil(ec.in ()))
    return ec->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);

  ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO::Utils::Servant_Var<[facet type]>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO::Utils::Servant_Var<[facet type]>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

##end foreach [facet type]



//////////////////////////////////////////////////////////////////
// Component specific context implementation
//////////////////////////////////////////////////////////////////

void *
[ciao module name]::[component name]_Context::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;

  if (type == ACE_reinterpret_cast (
              ptr_arith_t,
              &CCM_[component name]_Context::_tao_class_id)
            )
    {
      retv = ACE_reinterpret_cast (void*, this); // Does this look right?
    }
  else if (type == ACE_reinterpret_cast (
              ptr_arith_t,
              &::Components::SessionContext::_tao_class_id)
            )
    {
      retv =
        ACE_reinterpret_cast (
            void *,
            ACE_static_cast (
                Components::SessionContext_ptr,
                this
              )
          );
    }
  else if (type == ACE_reinterpret_cast (
              ptr_arith_t,
              &::Components::CCMContext::_tao_class_id)
            )
    {
      retv =
        ACE_reinterpret_cast (
            void *,
            ACE_static_cast (
                Components::CCMContext_ptr,
                this
              )
          );
    }
  else if (type == ACE_reinterpret_cast (
               ptr_arith_t,
               &CORBA::Object::_tao_class_id)
             )
    {
      retv =
        ACE_reinterpret_cast (
            void *,
            ACE_static_cast (CORBA::Object_ptr, this)
          );
    }

  if (retv != 0)
    {
      this->_add_ref ();
    }

  return retv;
}

const char*
[ciao module name]::[component name]_Context::_interface_repository_id (void) const
{
  // replay to
  return CCM_[component]_Context::_interface_repository_id ();
}

##foreach [receptacle name] with [uses type] in (list of all 'uses' interfaces) generate:
##  if ([receptacle name] is a multiplex ('uses multiple') receptacle)
    // [receptacle name]Connections typedef'ed as a sequence of
    // struct [receptacle name]Connection.
[receptacle name]Connections *
[ciao module name]::[component name]_Context::get_connections_[receptacle name] (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Strategized locking here.

  [receptacle name]Connections_var retv =
    new [receptacle name]Connections (this->ciao_muses_[receptacle name]_.current_size ());

  CORBA::Long i = 0;
  CIAO::Active_Objref_Map::iterator end = this->ciso_muses_[receptacle name]_.end ();
  for (CIAO::Active_Objref_Map::iterator iter = this->ciso_muses_[receptacle name]_.begin ();
       iter != end;
       ++iter)
    {
      CIAO::Active_Objref_Map::ENTRY &entry = *iter;
      retv[i].objref = [uses type]::_narrow (entry.in_id_.in ());
      retv[i].ck = new CIAO::Map_Key_Cookie (entry.ext_id_);
      ++i;
    }

  return retv._retn ();
}
##  endif [receptacle name]
##end foreach [receptacle name] with [uses type]

##foreach [event name] with [eventtype] in (list of all event sources) generate:
void
[ciao module name]::[component name]_Context::push_[event name] ([eventtype]_ptr ev ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
##  if [event name] belongs to an 'emits' port
  this->ciao_emits_[event name]_consumer_->push_[event name] (ev
                                                              ACE_ENV_ARG_PARAMETER);
##  else [event name] belongs to a 'publishes' port
  CIAO::Active_Objref_Map::iterator end = this->ciao_publishes_[event name]_map_.end ();
  for (CIAO::Active_Objref_Map::iterator iter = this->ciao_publishes_[event name]_map_.begin ();
       iter != end;
       ++iter)
    {
      CIAO::Active_Objref_Map::ENTRY &entry = *iter;
      [eventtype]Consumer_var c = [eventtype]Consumer::_narrow (entry.in_id_.in ());
      c->push_[event name] (ev
                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
##  endif [event name]
}

##end foreach [event name] with [eventtype]


// Operations for ::Components::SessionContext interface
CORBA::Object_ptr
[ciao module name]::[component name]_Context::get_CCM_object (ACE_ENV_SINGLE_ARG_DECL)
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

      this->component_ = [component name]::_narrow (obj.in ()
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (CORBA::is_nil (this->component_.in ()))
        ACE_THROW_RETURN (CORBA::INTERNAL (), 0); // This should not happen...
    }
  return [component name]::_duplicate (this->component_.in ());
}

//////////////////////////////////////////////////////////////////
// Component Servant Glue code implementation
//////////////////////////////////////////////////////////////////

##foreach [operation] in all supported interfaces of own component and all inherited components

    // Generate the [operation] here.


##end

// Operations for provides interfaces.
##foreach [facet name] with [facet type] in (list of all provided interfaces) generate:

[facet type]_ptr
[ciao module name]::[component name]_Servant::provide_[facet name] (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (this->provide_[facet name]_.in ()))
    {
      CCM_[facet type]_var fexe = this->executor_->get_[facet name] (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (CORBA::is_nil (fexe.in ()))
        ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

      // @@ What's the auto_ptr for servant?
      TAO::Utils::Servant_Var<[facet type]_Servant> svt =
        new [facet type]_Servant (fexe.in (),
                                  this->context_);

      CORBA::Object_var obj = this->container_->_install_servant (svc.in ()
                                                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      [facet type]_var fo = [facet type]::_narrow (obj
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      this->provide_[facet name]_ = fo;
    }

  return [facet type]::_duplicate (this->provide_[facet name]_.in ());
}
##end foreach [facet name] with [facet type]

// Operations for consumers interfaces.
##foreach [consumer name] with [eventtype] in (list of all consumers) generate:

// EventConsumer Glue Code implementation

// Inherit from ::Compopnents::EventBConsumerBase
void
[ciao module name]::[component name]_Servant::[eventtype]Consumer_[consumer name]_Servant::push_event (EventBase_ptr ev
                                                                                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Components::BadEventType))
{
  [eventtype] *ev_type = [eventtype]::_downcast (ev);
  if (ev_type != 0)
    {
      this->push_[eventtype] (ev ACE_ENV_ARG);
      return;
    }

  // @@ This include the case when we receive a parent eventtype of [eventtype]

  ACE_THROW (::Components::BadEventType ());
}

// get_component implementation.
[eventtype]Consumer_ptr
[ciao module name]::[component name]_Servant::get_consumer_[consumer name] (ACE_ENV_SINGLE_ARG)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
    [eventtype]Consumer_var consumes_[consumer name]_;

  if (CORBA::is_nil (this->consumes_[consumer name]_.in ()))
    {
      TAO::Utils::Servant_Var<[eventtype]Consumer_[consumer name]_Servant> svt =
        new [eventtype]Consumer_[consumer name]_Servant (this->executor_,
                                                         this->context_);

      CORBA::Object_var obj = this->container_->_install_servant (svt.in ()
                                                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      [eventtype]Consumer_var eco = [eventtype]Consumer::_narrow (obj
                                                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      this->consumes_[consumer name]_ = eco;
    }

  return [eventtype]Consumer::_duplicate (this->consumes_[consumer name]_.in ());
}

##end foreach [consumer name] with [eventtype]

// Operations for Navigation interface
CORBA::Object_ptr
[ciao module name]::[component name]_Servant::provide_facet (const char * name
                                                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException ,
                   Components::InvalidName))
{
  if (name == 0)
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);

  // We simply iterate thru all the facets this component provides
  // now.  We can also use a hash map along with perfect hashing
  // (i.e., gperf) if we need faster performance since all the provided
  // interfaces are static to a component definition.

##foreach [facet name] with [facet type] in (list of all provided interfaces) generate:
  if (ACE_OS_String::strcmp (name, "[facet name]") == 0)
    return this->provide_[facet name] (ACE_ENV_SINGLE_ARG_PARAMETER);
##end foreach [facet name] with [facet type]

  ACE_THROW_RETURN (::Components::InvalidName (), 0);
}

::Components::FacetDescriptions *
[ciao module name]::[component name]_Servant::get_all_facets (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::Components::FacetDescriptions_var collection
      = new ::Components::FacetDescriptions (#99); // #99 = number of all provided
                                                   // facets including those inherited
                                                   // from parent component(s).

  ::Components::FacetDescription_var x;
  CORBA::ULong i = 0;

##foreach [facet name] with [facet type] in (list of all provided interfaces) generate:

  x = new ::OBV_Components::FacetDescription;

  x->Name ((const char *)"[facet name]");
  x->type_id ((const char *) "[facet type's repo id]"); //
  x->facet_ref (this->provide_[facet name] (ACE_ENV_SINGLE_ARG_PARAMETER));

  collection[i] = x;
  ++i;
##end foreach [facet name] with [facet type]

  return collection._retn ();

}

::Components::FacetDescriptions *
[ciao module name]::[component name]_Servant::get_named_facets (const Components::NameList & names
                                                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  ::Components::FacetDescriptions_var collection
      = new ::Components::FacetDescriptions (names.length ());

  ::Components::FacetDescription_var x;
  CORBA::ULong i = 0;
  for (; i < names.length (); ++i)
    {
      // We probably need a more efficient way, e.g., a hash map, to handle all these.
##foreach [facet name] with [facet type] in (list of all provided interfaces) generate:
      if (ACE_OS_String::strcmp (names[i].in (), "[facet name]") == 0)
        {
          x = new ::OBV_Components::FacetDescription;

          x->Name ((const char *)"[facet name]");
          x->type_id ((const char *) "[facet type's repo id]"); //
          x->facet_ref (this->provide_facet ("[facet name]" ACE_ENV_ARG_PARAMETER));

          collection[i] = x;
        }
##end foreach [facet name] with [facet type]
      ACE_THROW_RETURN (::Components::InvalidName (), 0);
    }
  return collection._retn ();
}

CORBA::Boolean
[ciao module name]::[component name]_Servant::same_component (CORBA::Object_ptr object_ref
                                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_var me = this->context_->get_CCM_object (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return me->_is_equivalent (object_ref
                             ACE_ENV_ARG_PARAMETER);
}

// Operations for Receptacles interface
::Components::Cookie_ptr
[ciao module name]::[component name]_Servant::connect (const char * name,
                                                       CORBA::Object_ptr connection
                                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::InvalidConnection,
                   Components::AlreadyConnected,
                   Components::ExceededConnectionLimit))
{
  // @@ to-do
}

CORBA::Object_ptr
[ciao module name]::[component name]_Servant::disconnect (const char * name,
                                                          Components::Cookie_ptr ck
                                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::InvalidConnection,
                   Components::CookieRequired,
                   Components::NoConnection))
{
  // @@ to-do
}

::Components::ConnectionDescriptions *
[ciao module name]::[component name]_Servant::get_connections (const char * name
                                                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  // @@ to-do
}

::Components::ReceptacleDescriptions *
[ciao module name]::[component name]_Servant::get_all_receptacles (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ to-do
}

::Components::ReceptacleDescriptions *
[ciao module name]::[component name]_Servant::get_named_receptacles (const Components::NameList & names
                                                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  // @@ to-do
}

// Operations for Events interface
::Components::EventConsumerBase_ptr
[ciao module name]::[component name]_Servant::get_consumer (const char * sink_name
                                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  // @@ to-do
}

::Components::Cookie_ptr
[ciao module name]::[component name]_Servant::subscribe (const char * publisher_name,
                                                         Components::EventConsumerBase_ptr subscriber
                                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::AlreadyConnected,
                   Components::InvalidConnection))
{
  // @@ to-do
}

void
[ciao module name]::[component name]_Servant::unsubscribe (const char * publisher_name,
                                                           Components::Cookie_ptr ck
                                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::InvalidConnection))
{
  // @@ to-do
}

void
[ciao module name]::[component name]_Servant::connect_consumer (const char * emitter_name,
                                                                Components::EventConsumerBase_ptr consumer
                                                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::AlreadyConnected,
                   Components::InvalidConnection))
{
  // @@ to-do
}

::Components::EventConsumerBase_ptr
[ciao module name]::[component name]_Servant::disconnect_consumer (const char * source_name
                                                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName,
                   Components::NoConnection))
{
  // @@ to-do
}

::Components::ConsumerDescriptions *
[ciao module name]::[component name]_Servant::get_all_consumers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ to-do
}

::Components::ConsumerDescriptions *
[ciao module name]::[component name]_Servant::get_named_consumers (const Components::NameList & names
                                                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  // @@ to-do
}

::Components::EmitterDescriptions *
[ciao module name]::[component name]_Servant::get_all_emitters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ to-do
}

::Components::EmitterDescriptions *
[ciao module name]::[component name]_Servant::get_named_emitters (const Components::NameList & names
                                                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  // @@ to-do
}

::Components::PublisherDescriptions *
[ciao module name]::[component name]_Servant::get_all_publishers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ to-do
}

::Components::PublisherDescriptions *
[ciao module name]::[component name]_Servant::get_named_publishers (const Components::NameList & names
                                                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidName))
{
  // @@ to-do
}

// Operations for CCMObject interface
::CORBA::IRObject_ptr
[ciao module name]::[component name]_Servant::get_component_def (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ to-do: Connect to an IfR?
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

::Components::CCMHome_ptr
[ciao module name]::[component name]_Servant::get_ccm_home (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->context_->get_CCM_home (ACE_ENV_SINGLE_ARG_PARAMETER);
}

::Components::PrimaryKeyBase *
[ciao module name]::[component name]_Servant::get_primary_key (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::NoKeyAvailable))
{
  // This is a keyless component.
  ACE_THROW_RETURN (::Components::NoKeyAvailable (), 0);
}

void
[ciao module name]::[component name]_Servant::configuration_complete (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::InvalidConfiguration))
{
  // @@ to-do
}

void
[ciao module name]::[component name]_Servant::remove (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::RemoveFailure))
{
  // @@ to-do
}

::Components::ComponentPortDescription *
[ciao module name]::[component name]_Servant::get_all_ports (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ to-do
}

// get_component implementation.
CORBA::Object_ptr
[ciao module name]::[component name]_Servant::_get_component (ACE_ENV_SINGLE_ARG_DECL)
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

[component name]_ptr
[ciao module name]::[component name]_Servant::_ciao_activate_component (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_var obj
    = this->container_->install_servant (this
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  [component name]_var ho = [component name]::_narrow (obj
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return ho._retn ();

}
