// $Id$

//    The generated filename for files using this template shoule be
//       [idl-basename]GS.cpp       GS --> GlueSession

// @@ Notice: [ciao module name] can expand to either CIAO_GLUE or
//    CIAO_GLUE_[module name] as defined in the header file.

/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/// @@@ Notice that all component and interface names need to be
/// fully qualified as we are creating a new namespace for the CIAO's
/// container glue code.
/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#include "[idl-basename]_svnt.h"
#include "Cookies.h"

#if !defined (__ACE_INLINE__)
# include "[idl-basename]_svnt.inl"
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
[ciao module name]::[facet type]_Servant::_get_component ()
{
  ::Components::SessionContext_var sc =
      ::Components::SessionContext::_narrow (this->ctx_.in ());

  if (! CORBA::is_nil(sc.in ()))
    return sc->get_CCM_object ();

  ::Components::EntityContext_var ec =
      ::Components::EntityContext::_narrow (this->ctx_.in ());

  if (! CORBA::is_nil(ec.in ()))
    return ec->get_CCM_object ();

  throw CORBA::INTERNAL ();
}

##end foreach [facet type]



//////////////////////////////////////////////////////////////////
// Component specific context implementation
//////////////////////////////////////////////////////////////////

##foreach [receptacle name] with [uses type] in (list of all 'uses' interfaces) generate:
##  if ([receptacle name] is a multiplex ('uses multiple') receptacle)
    // [receptacle name]Connections typedef'ed as a sequence of
    // struct [receptacle name]Connection.
[receptacle name]Connections *
[ciao module name]::[component name]_Context::get_connections_[receptacle name] ()
{
  // @@ Strategized locking here.

  [receptacle name]Connections_var retv =
    new [receptacle name]Connections (this->ciao_muses_[receptacle name]_.current_size ());

  CORBA::ULong i = 0;
  ACE_Active_Map_Manager<[uses type]_var>::iterator
    end = this->ciso_muses_[receptacle name]_.end ();
  for (ACE_Active_Map_Manager<[uses type]_var>::iterator
         iter = this->ciso_muses_[receptacle name]_.begin ();
       iter != end;
       ++iter)
    {
      ACE_Active_Map_Manager<[uses type]_var>::ENTRY &entry = *iter;
      retv[i]->objref = [uses type]::_narrow (entry.int_id_.in ());
      retv[i]->ck = new CIAO::Map_Key_Cookie (entry.ext_id_);
      ++i;
    }

  return retv._retn ();
}
##  endif [receptacle name]
##end foreach [receptacle name] with [uses type]

##foreach [event name] with [eventtype] in (list of all event sources) generate:
void
[ciao module name]::[component name]_Context::push_[event name] ([eventtype] *ev)
{
##  if [event name] belongs to an 'emits' port
  this->ciao_emits_[event name]_consumer_->push_[event name] (ev);

##  else [event name] belongs to a 'publishes' port
  ACE_Active_Map_Manager<[eventtype]Consumer_var>::iterator
    end = this->ciao_publishes_[event name]_map_.end ();
  for (ACE_Active_Map_Manager<[eventtype]Consumer_var>::iterator
         iter = this->ciao_publishes_[event name]_map_.begin ();
       iter != end;
       ++iter)
    {
      ACE_Active_Map_Manager<[eventtype]Consumer_var>::ENTRY &entry = *iter;
      [eventtype]Consumer_var c
        = [eventtype]Consumer::_narrow (entry.int_id_.in ());
      c->push_[eventtype] (ev);
    }
##  endif [event name]
}

##end foreach [event name] with [eventtype]


// Operations for publishes interfaces.
##foreach [publish name] with [eventtype] in (list of all publishers) generate:
::Components::Cookie *
[ciao module name]::[component name]_Context::subscribe_[publish name] ([eventtype]Consumer_ptr c)
{
  if (CORBA::is_nil (c))
    throw CORBA::BAD_PARAM ();

  [eventtype]Consumer_var sub
    = [eventtype]Consumer::_duplicate (c);

  ACE_Active_Map_Manager_Key key;
  this->ciao_publishes_[publish name]_map_.bind (sub.in (),
                                                 key);

  sub._retn ();                 // Release ownership to map.

  ::Components::Cookie_var retv = new CIAO::Map_Key_Cookie (key);
  return retv._retn ();
}

[eventtype]Consumer_ptr
[ciao module name]::[component name]_Context::unsubscribe_[publish name] (::Components::Cookie *ck)
{
  [eventtype]Consumer_var retv;

  ACE_Active_Map_Manager_Key key;
  if (ck == 0 ||
      CIAO::Map_Key_Cookie::extract (ck, key) == -1)
    throw ::Components::InvalidConnection ();

  if (this->ciao_publishes_[publish name]_map_.unbind (key,
                                                       retv) != 0)
    throw ::Components::InvalidConnection ();

  return retv._retn ();
}

##end foreach [publish name] with [eventtype]

##foreach [receptacle name] with [uses type] in (list of all 'uses' interfaces) generate:

##  if [receptacle name] is a simplex receptacle ('uses')

[uses type]_ptr
[ciao module name]::[component name]_Context::get_connection_[receptacle name] ()
{
  return [uses type]::_duplicate (this->ciao_uses_[receptacle name]_.in ());
}

// Simplex [receptacle name] connection management operations
void
[ciao module name]::[component name]_Context::connect_[receptacle name] ([uses type]_ptr c)
{
  if (! CORBA::is_nil (this->ciao_uses_[receptacle name]_.in ()))
    throw ::Components::AlreadyConnected ();

  if (CORBA::is_nil (c))
    throw ::Components::InvalidConnection ();

  // When do we throw InvalidConnection exception?
  this->ciao_uses_[receptacle name]_ = [uses type]::_duplicate (c);
}

[uses type]_ptr
[ciao module name]::[component name]_Context::disconnect_[receptacle name] ()
{
  if (CORBA::is_nil (this->ciao_uses_[receptacle name]_.in ()))
    throw ::Components::NoConnection ();

  return this->ciao_uses_[receptacle name]_._retn ();
}

##  else ([receptacle name] is a multiplex ('uses multiple') receptacle)
// Multiplex [receptacle name] connection management operations
::Components::Cookie *
[ciao module name]::[component name]_Context::connect_[receptacle name] ([uses type]_ptr c)
{
  if (CORBA::is_nil (c))
    throw ::Components::InvalidConnection ();

  [uses type]_var conn
    = [uses type]::_duplicate (c);

  ACE_Active_Map_Manager_Key key;
  this->ciao_muses_[receptacle name]_.bind (conn.in (),
                                            key);

  conn._retn ();                // Releases ownership to the map.

  ::Components::Cookie_var retv = new CIAO::Map_Key_Cookie (key);
  return retv._retn ();
}

[uses type]_ptr
[ciao module name]::[component name]_Context::disconnect_[receptacle name] (::Components::Cookie *ck)
{
  [uses type]_var retv;

  ACE_Active_Map_Manager_Key key;
  if (ck == 0 ||
      CIAO::Map_Key_Cookie::extract (ck, key) == -1)
    throw ::Components::InvalidConnection ();

  if (this->ciao_muses_[receptacle name]_.unbind (key,
                                                  retv) != 0)
    throw ::Components::InvalidConnection ();

  return retv._retn ();
}

##  endif [receptacle name]
##end foreach [receptacle name] with [uses type]

// Operations for ::Components::SessionContext interface
CORBA::Object_ptr
[ciao module name]::[component name]_Context::get_CCM_object ()
{
  // @@ How do I check for IllegalState here?  When it's not in a
  //    callback operation...
  //    ACE_THROW_RETURN (::Components::IllegalState (), 0);

  if (CORBA::is_nil (this->component_.in ()))
    {
      CORBA::Object_var obj =  this->container_->get_objref (this->servant_);

      this->component_ = [component name]::_narrow (obj.in ());

      if (CORBA::is_nil (this->component_.in ()))
        throw CORBA::INTERNAL (); // This should not happen...
    }
  return [component name]::_duplicate (this->component_.in ());
}

//////////////////////////////////////////////////////////////////
// Component Servant Glue code implementation
//////////////////////////////////////////////////////////////////

[ciao module name]::[component name]_Servant::[component name]_Servant (CCM_[component name]_ptr exe,
                                                                        ::Components::CCMHome_ptr h,
                                                                        ::CIAO::Session_Container *c)
  : executor_ (CCM_[component name]::_duplicate (exe)),
    container_ (c)
{
  this->context_ = new [ciao module name]::[component name]_Context (h, c, this);

  try
    {
      Components::SessionComponent_var scom =
        Components::SessionComponent::_narrow (exe);

      if (! CORBA::is_nil (scom.in ()))
        scom->set_session_context (this->context_);
    }
  catch (const CORBA::Exception& ex)
    {
      // @@ Ignore any exceptions?  What happens if
      // set_session_context throws an CCMException?
    }
}

[ciao module name]::[component name]_Servant::~[component name]_Servant (void)
{
  try
    {
      Components::SessionComponent_var scom =
        Components::SessionComponent::_narrow (this->executor_.in ());

      if (! CORBA::is_nil (scom.in ()))
        scom->ccm_remove ();
    }
  catch (const CORBA::Exception& ex)
    {
      // @@ Ignore any exceptions?  What happens if
      // set_session_context throws an CCMException?
    }
  this->context_->_remove_ref ();
}

// Operations for provides interfaces.
##foreach [facet name] with [facet type] in (list of all provided interfaces) generate:

[facet type]_ptr
[ciao module name]::[component name]_Servant::provide_[facet name] ()
{
  if (CORBA::is_nil (this->provide_[facet name]_.in ()))
    {
      CCM_[facet type]_var fexe = this->executor_->get_[facet name] ();

      if (CORBA::is_nil (fexe.in ()))
        throw CORBA::INTERNAL ();

      [ciao module name]::[facet type]_Servant *svt =
        new [ciao module name]::[facet type]_Servant (fexe.in (),
                                                      this->context_);
      PortableServer::ServantBase_var safe_servant (svt);

      CORBA::Object_var obj = this->container_->install_servant (svt);

      [facet type]_var fo = [facet type]::_narrow (obj.in ());

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
[ciao module name]::[component name]_Servant::[eventtype]Consumer_[consumer name]_Servant::push_event (EventBase *ev)
{
  [eventtype]_var ev_type = [eventtype]::_downcast (ev);
  if (ev_type != 0)
    {
      this->push_[eventtype] (ev_type.in ());
      return;
    }

  // @@ This include the case when we receive a parent eventtype of [eventtype]

  throw ::Components::BadEventType ();
}

// get_component implementation.
[eventtype]Consumer_ptr
[ciao module name]::[component name]_Servant::get_consumer_[consumer name] ()
{
  if (CORBA::is_nil (this->consumes_[consumer name]_.in ()))
    {
      [ciao module name]::[component name]_Servant::[eventtype]Consumer_[consumer name]_Servant *svt =
        new [ciao module name]::[component name]_Servant::[eventtype]Consumer_[consumer name]_Servant (this->executor_.in (),
                                                                                                       this->context_);
      PortableServer::ServantBase_var safe_servant (svt);

      CORBA::Object_var obj = this->container_->install_servant (svt);

      [eventtype]Consumer_var eco = [eventtype]Consumer::_narrow (obj.in ());

      this->consumes_[consumer name]_ = eco;
    }

  return [eventtype]Consumer::_duplicate (this->consumes_[consumer name]_.in ());
}

##end foreach [consumer name] with [eventtype]

// Operations for Navigation interface
CORBA::Object_ptr
[ciao module name]::[component name]_Servant::provide_facet (const char * name)
{
  if (name == 0)
    throw CORBA::BAD_PARAM ();

  // We simply iterate thru all the facets this component provides
  // now.  We can also use a hash map along with perfect hashing
  // (i.e., gperf) if we need faster performance since all the provided
  // interfaces are static to a component definition.

##foreach [facet name] with [facet type] in (list of all provided interfaces) generate:
  if (ACE_OS_String::strcmp (name, "[facet name]") == 0)
    return this->provide_[facet name] ();
##end foreach [facet name] with [facet type]

  throw ::Components::InvalidName ();
}

::Components::FacetDescriptions *
[ciao module name]::[component name]_Servant::get_all_facets ()
{
  ::Components::FacetDescriptions_var collection
      = new ::Components::FacetDescriptions (#99); // #99 = number of all provided
                                                   // facets including those inherited
                                                   // from parent component(s).
  collection->length (#99);

  ::Components::FacetDescription_var x;
  CORBA::ULong i = 0;

##foreach [facet name] with [facet type] in (list of all provided interfaces) generate:

  x = new ::OBV_Components::FacetDescription;

  x->Name ((const char *)"[facet name]");
  x->type_id ((const char *) "[facet type's repo id]"); //
  x->facet_ref (this->provide_[facet name] ());

  collection[i] = x._retn ();
  ++i;
##end foreach [facet name] with [facet type]

  return collection._retn ();

}

::Components::FacetDescriptions *
[ciao module name]::[component name]_Servant::get_named_facets (const Components::NameList & names)
{
  ::Components::FacetDescriptions_var collection
      = new ::Components::FacetDescriptions (names.length ());
  collection->length (names.length ());

  ::Components::FacetDescription_var x;
  CORBA::ULong i = 0;
  for (; i < names.length (); ++i)
    {
      x = new ::OBV_Components::FacetDescription;

      // We probably need a more efficient way, e.g., a hash map, to handle all these.
##foreach [facet name] with [facet type] in (list of all provided interfaces) generate:
      (else) if (ACE_OS_String::strcmp (names[i].in (), "[facet name]") == 0)
        {
          x->Name ((const char *)"[facet name]");
          x->type_id ((const char *) "[facet type's repo id]"); //
          x->facet_ref (this->provide_[facet name] ());
        }
##end foreach [facet name] with [facet type]
      else
        throw ::Components::InvalidName ();

      collection[i] = x._retn ();
    }
  return collection._retn ();
}

CORBA::Boolean
[ciao module name]::[component name]_Servant::same_component (CORBA::Object_ptr object_ref)
{
  if (CORBA::is_nil (object_ref))
    throw CORBA::BAD_PARAM ();

  CORBA::Object_var the_other = object_ref->_get_component ();

  CORBA::Object_var me = this->context_->get_CCM_object ();

  return me->_is_equivalent (object_ref);
}

// Operations for Receptacles interface
::Components::Cookie *
[ciao module name]::[component name]_Servant::connect (const char * name,
                                                       CORBA::Object_ptr connection)
{
  // @@ We can omit this if clause if there's no receptacle in this component.
  if (name == 0)
    throw Components::InvalidName ();

##foreach [receptacle name] with [uses type] in (list of all 'uses' interfaces) generate:
  if (ACE_OS_String::strcmp (name, "[receptacle name]") == 0)
    {
      [uses type]_var _ciao_conn =
        [uses type]::_narrow (connection);

      if (CORBA::is_nil (_ciao_conn.in ()))
        throw ::Components::InvalidConnection ();

##  if [receptacle name] is a simplex receptacle ('uses')
      this->connect_[receptacle name] (_caio_conn.in ());
      return 0;
##  else ([receptacle name] is a multiplex ('uses multiple') receptacle)
      return this->connect_[receptacle name] (_ciao_conn.in ());
##  endif [receptacle name]
    }
##end foreach [receptacle name] with [uses type]

  throw Components::InvalidName ();
}

/*
##foreach [receptacle name] with [uses type] in (list of all 'uses' interfaces) generate:
##  if [receptacle name] is a simplex receptacle ('uses')
##  else ([receptacle name] is a multiplex ('uses multiple') receptacle)
##  endif [receptacle name]
##end foreach [receptacle name] with [uses type]
*/

CORBA::Object_ptr
[ciao module name]::[component name]_Servant::disconnect (const char * name,
                                                          Components::Cookie *ck)
{
  // @@ We can omit this if clause if there's no receptacle in this component.
  if (name == 0)
    throw Components::InvalidName ();

##foreach [receptacle name] with [uses type] in (list of all 'uses' interfaces) generate:
  if (ACE_OS_String::strcmp (name, "[receptacle name]") == 0)
##  if [receptacle name] is a simplex receptacle ('uses')
      return this->disconnect_[receptacle name] ();
##  else ([receptacle name] is a multiplex ('uses multiple') receptacle)
      return this->connect_[receptacle name] (ck);
##  endif [receptacle name]
##end foreach [receptacle name] with [uses type]

  throw Components::InvalidName ();
}

::Components::ConnectionDescriptions *
[ciao module name]::[component name]_Servant::get_connections (const char * name)
{
  // @@ We can omit this if clause if there's no receptacle in this component.
  if (name == 0)
    throw Components::InvalidName ();

##foreach [receptacle name] with [uses type] in (list of all 'uses' interfaces) generate:
  if (ACE_OS_String::strcmp (name, "[receptacle name]") == 0)
    {
##  if [receptacle name] is a simplex receptacle ('uses')
      ::Components::ConnectionDescriptions_var retv =
        new ::Components::ConnectionDescriptions (1);
      retv->length (1);

      retv[0] = new OBV_Components::ConnectionDescription;
      retv[0]->ck = 0;
      retv[0]->objref = this->get_connection_[receptacle name] ();

      return retv._retn ();
##  else ([receptacle name] is a multiplex ('uses multiple') receptacle)
      // @@ Return type does not match here.  We can not return directly.
      return this->get_connections_[receptacle name] ();
##  endif [receptacle name]
    }
##end foreach [receptacle name] with [uses type]

  throw Components::InvalidName ();
}

::Components::ReceptacleDescriptions *
[ciao module name]::[component name]_Servant::get_all_receptacles ()
{
  ::Components::ReceptacleDescriptions_var retv =
      new ::Components::ReceptacleDescriptions (#99); // #99 is number of receptacles
                                                      // this component has.
  retv->length (#99);
  CORBA::ULong i = 0;

##foreach [receptacle name] with [uses type] in (list of all 'uses' interfaces) generate:
  retv[i] = new OBV_Components::ReceptacleDescription;

  retv[i]->Name ((const char *) "[receptacle name]");
  retv[i]->type_id ((const char *) "[uses type repo id]");
##  if [receptacle name] is a simplex receptacle ('uses')
  retv[i]->is_multiple (0);
##  else ([receptacle name] is a multiplex ('uses multiple') receptacle)
  retv[i]->is_multiple (1);
##  endif [receptacle name]
  retv[i]->connections (*this->get_connections ("[receptacle name]"));
  ++i;
##end foreach [receptacle name] with [uses type]

  return retv._retn ();
}

::Components::ReceptacleDescriptions *
[ciao module name]::[component name]_Servant::get_named_receptacles (const Components::NameList & names)
{
  ::Components::ReceptacleDescriptions_var retv =
      new ::Components::ReceptacleDescriptions (names.length ());
  retv->length (names.length ());

  CORBA::ULong i = 0;
  for (; i < names.length (); ++i)
    {
      retv[i] = new ::OBV_Components::ReceptacleDescription;
##foreach [receptacle name] with [uses type] in (list of all 'uses' interfaces) generate:
      (else) if (ACE_OS_String::strcmp (names[i].in (), "[receptacle name]") == 0)
        {
          retv[i]->Name ((const char *) "[receptacle name]");
          retv[i]->type_id ((const char *) "[uses type repo id]");
##  if [receptacle name] is a simplex receptacle ('uses')
          retv[i]->is_multiple (0);
##  else ([receptacle name] is a multiplex ('uses multiple') receptacle)
          retv[i]->is_multiple (1);
##  endif [receptacle name]
          retv[i]->connections (*this->get_connections ("[receptacle name]"));
        }
##end foreach [receptacle name] with [uses type]
      else
        throw ::Components::InvalidName ();
    }
  return retv._retn ();
}

// Operations for Events interface
::Components::EventConsumerBase_ptr
[ciao module name]::[component name]_Servant::get_consumer (const char * sink_name)
{
  // @@ We can omit this if clause if there's no event sinks in this component.
  if (sink_name == 0)
    throw Components::InvalidName ();

##foreach [consumer name] with [eventtype] in (list of all consumers) generate:
  if (ACE_OS_String::strcmp (sink_name, "[consumer name]") == 0)
    return this->get_consumer_[consumer name] ();
##end foreach [consumer name] with [eventtype]
  throw Components::InvalidName ();
}

::Components::Cookie *
[ciao module name]::[component name]_Servant::subscribe (const char * publisher_name,
                                                         Components::EventConsumerBase_ptr subscriber)
{
  // @@ We can omit this if clause if there's no publisher in this component.
  if (publisher_name == 0)
    throw Components::InvalidName ();

##foreach [publish name] with [eventtype] in (list of all publishers) generate:
  if (ACE_OS_String::strcmp (publisher_name, "[publish name]") == 0)
    {
      [eventtype]Consumer_var _ciao_consumer =
        [eventtype]Consumer::_narrow (subscriber);

      if (CORBA::is_nil (_ciao_consumer.in ()))
        throw Components::InvalidConnection ();

      return this->subscribe_[publish name] (_ciao_consumer.in ());
    }
##end foreach [publish name] with [eventtype]

  throw Components::InvalidName ();
}

::Components::EventConsumerBase_ptr
[ciao module name]::[component name]_Servant::unsubscribe (const char * publisher_name,
                                                           Components::Cookie *ck)
{
  // @@ We can omit this if clause if there's no publisher in this component.
  if (publisher_name == 0)
    throw Components::InvalidName ();

##foreach [publish name] with [eventtype] in (list of all publishers) generate:
  if (ACE_OS_String::strcmp (publisher_name, "[publish name]") == 0)
    {
      return this->unsubscribe_[publish name] (ck);
    }
##end foreach [publish name] with [eventtype]

  throw Components::InvalidName ();
}

void
[ciao module name]::[component name]_Servant::connect_consumer (const char * emitter_name,
                                                                Components::EventConsumerBase_ptr consumer)
{
  // @@ We can omit this if clause if there's no emitter in this component.
  if (emitter_name == 0)
    throw Components::InvalidName ();

##foreach [emit name] with [eventtype] in (list of all emitters) generate:
  if (ACE_OS_String::strcmp (emitter_name, "[emit name]") == 0)
    {
      [eventtype]Consumer_var _ciao_consumer =
        [eventtype]Consumer::_narrow (consumer);

      if (CORBA::is_nil (_ciao_consumer.in ()))
        throw Components::InvalidConnection;

      this->connect_[emit name] (_ciao_consumer.in ());
      return;
    }
##end foreach [emit name] with [eventtype]

  throw Components::InvalidName ();
}

::Components::EventConsumerBase_ptr
[ciao module name]::[component name]_Servant::disconnect_consumer (const char * source_name)
{
  // @@ We can omit this if clause if there's no emitter in this component.
  if (source_name == 0)
    throw Components::InvalidName ();

##foreach [emit name] with [eventtype] in (list of all emitters) generate:
  if (ACE_OS_String::strcmp (source_name, "[emit name]") == 0)
    {
      return this->disconnect_[emit name] ();
    }
##end foreach [emit name] with [eventtype]

  throw Components::InvalidName ();
}

::Components::ConsumerDescriptions *
[ciao module name]::[component name]_Servant::get_all_consumers ()
{
  ::Components::ConsumerDescriptions_var retv =
      new ::Components::ConsumerDescriptions (#99); // #99 is the number of consumers
                                                    // this component has.
  retv->length (#99);

  CORBA::ULong i = 0;
##foreach [consumer name] with [eventtype] in (list of all consumers) generate:
  retv[i] = new OBV_Components::ConsumerDescription;
  retv[i]->Name ("[consumer name]");
  retv[i]->type_id ("[eventtype]Consumer repo id");
  [eventtype]Consumer_var c
    = this->get_consumer_[consumer name] ();

  retv[i]->consumer (c.in ());

  i++;
##end foreach [consumer name] with [eventtype]

  return retv._retn ();
}

::Components::ConsumerDescriptions *
[ciao module name]::[component name]_Servant::get_named_consumers (const Components::NameList & names)
{
  ::Components::ConsumerDescriptions_var retv =
      new ::Components::ConsumerDescriptions (names.length ());
  retv->length (names.length ());

  CORBA::ULong i = 0;
  for (; i < names.length (); ++i)
    {
      retv[i] = new OBV_Components::ConsumerDescription;

##foreach [consumer name] with [eventtype] in (list of all consumers) generate:
      (else) if (ACE_OS_String::strcmp (names[i].in (), "[consumer name]") == 0)
        {
          retv[i]->Name ("[consumer name]");
          retv[i]->type_id ("[eventtype]Consumer repo id");
          [eventtype]Consumer_var c =
            this->get_consumer_[consumer name] ();

          retv[i]->consumer (c.in ());
        }
##end foreach [consumer name] with [eventtype]
      else
        throw ::Components::InvalidName ();
    }
  return retv._retn ();
}

::Components::EmitterDescriptions *
[ciao module name]::[component name]_Servant::get_all_emitters ()
{
  ::Components::EmitterDescriptions_var retv =
      new ::Components::EmitterDescriptions (#99); // #99 is the number of emitters
                                                    // this component has.
  retv->length (#99);

  CORBA::ULong i = 0;
##foreach [emit name] with [eventtype] in (list of all emitters) generate:
  retv[i] = new OBV_Components::EmitterDescription;
  retv[i]->Name ("[emit name]");
  retv[i]->type_id ("[eventtype]Consumer repo id");
  retv[i]->consumer ([eventtype]Consumer::_duplicate (this->context_->ciao_emits_[emit name]_consumer_));

  i++;
##end foreach [emitter name] with [eventtype]

  return retv._retn ();
}

::Components::EmitterDescriptions *
[ciao module name]::[component name]_Servant::get_named_emitters (const Components::NameList & names)
{
  ::Components::EmitterDescriptions_var retv =
      new ::Components::EmitterDescriptions (names.length ());
  retv->length (names.length ());

  CORBA::ULong i = 0;
  for (; i < names.length (); ++i)
    {
      retv[i] = new OBV_Components::EmitterDescription;

##foreach [emit name] with [eventtype] in (list of all emitters) generate:
      (else) if (ACE_OS_String::strcmp (names[i].in (), "[emit name]") == 0)
        {
          retv[i]->Name ("[emit name]");
          retv[i]->type_id ("[eventtype]Consumer repo id");
          retv[i]->consumer ([eventtype]Consumer::_duplicate (this->context_->ciao_emits_[emit name]_consumer_.in ()));
        }
##end foreach [consumer name] with [eventtype]
      else
        throw ::Components::InvalidName ();
    }
  return retv._retn ();
}

::Components::PublisherDescriptions *
[ciao module name]::[component name]_Servant::get_all_publishers ()
{
  // @@ to-do

  // Need to add interfaces in the Context class to gather the information.
  // Or we can just relay it to the Context object.
  throw CORBA::NO_IMPLEMENT ();
}

::Components::PublisherDescriptions *
[ciao module name]::[component name]_Servant::get_named_publishers (const Components::NameList & names)
{
  // @@ to-do

  // Need to add interfaces in the Context class to gather the information.
  // Or we can just relay it to the Context object.
  throw CORBA::NO_IMPLEMENT ();
}

// Operations for CCMObject interface
::CORBA::IRObject_ptr
[ciao module name]::[component name]_Servant::get_component_def ()
{
  // @@ to-do: Connect to an IfR?
  throw CORBA::NO_IMPLEMENT ();
}

::Components::CCMHome_ptr
[ciao module name]::[component name]_Servant::get_ccm_home ()
{
  return this->context_->get_CCM_home ();
}

::Components::PrimaryKeyBase *
[ciao module name]::[component name]_Servant::get_primary_key ()
{
  // This is a keyless component.
  throw ::Components::NoKeyAvailable ();
}

void
[ciao module name]::[component name]_Servant::configuration_complete ()
{
  // @@ to-do
  // No-op.  Don't know how to pass this info to monolithic executor.
}

void
[ciao module name]::[component name]_Servant::remove ()
{
  // @@ to-do
  // Need to figure out what to do here.  E.g., tear down the all the connections
  // this component has?
}

::Components::ComponentPortDescription *
[ciao module name]::[component name]_Servant::get_all_ports ()
{
  ::Components::ComponentPortDescription_var retv =
    new OBV_Components::ComponentPortDescription;

  ::Components::FacetDescriptions_var facets_desc
      = this->get_all_facets ();

  ::Components::ReceptacleDescriptions_var receptacle_desc
      = get_all_receptacles ();

  ::Components::ConsumerDescriptions_var consumer_desc
      = this->get_all_consumers ();

  ::Components::EmitterDescriptions_var emitter_desc
      = this->get_all_emitters ();

  ::Components::PublisherDescriptions_var publisher_desc
      = this->get_all_publishers ();

  retv->facets (facets_desc.in());
  retv->receptacles (receptacle_desc.in());
  retv->consumers (consumer_desc.in());
  retv->emitters (emitter_desc.in());
  retv->publishers (publisher_desc.in());

  return retv._retn();
}

// get_component implementation.
CORBA::Object_ptr
[ciao module name]::[component name]_Servant::_get_component ()
{
  ::Components::SessionContext_var sc =
      ::Components::SessionContext::_narrow (this->context_);

  if (! CORBA::is_nil(sc.in ()))
    return sc->get_CCM_object ();

  // @@ Do we need to try the following case here?  We are afterall implementing
  // a session component here.
  ::Components::EntityContext_var ec =
      ::Components::EntityContext::_narrow (this->context_);

  if (! CORBA::is_nil(ec.in ()))
    return ec->get_CCM_object ();

  throw CORBA::INTERNAL ();
}

void
[ciao module name]::[component name]_Servant::_ciao_activate ()
{
  ::Components::SessionComponent_var temp =
      ::Components::SessionComponent::_narrow (this->executor_.in ());

  if (! CORBA::is_nil (temp.in ()))
    temp->ccm_activate ();
}

void
[ciao module name]::[component name]_Servant::_ciao_passivate ()
{
  ::Components::SessionComponent_var temp =
      ::Components::SessionComponent::_narrow (this->executor_.in ());

  if (! CORBA::is_nil (temp.in ()))
    temp->ccm_passivate ();
}

//////////////////////////////////////////////////////////////////
// Component Home Glue code implementation
//////////////////////////////////////////////////////////////////

[component name]_ptr
[ciao module name]::[home name]_Servant::_ciao_activate_component (CCM_[component name]_ptr exe)
{
  CORBA::Object_var hobj
    = this->container_->get_objref (this);

  ::Components::CCMHome_var home
      = ::Components::CCMHome::_narrow (hobj.in ());

  [ciao module name]::[component name]_Servant *svt =
    new [ciao module name]::[component name]_Servant (exe,
                                                      home.in (),
                                                      this->container_);
  PortableServer::ServantBase_var safe (svt);
  PortableServer::ObjectId_var oid;

  CORBA::Object_var objref
    = this->container_->install_component (svt,
                                           oid.out ());

  svt->_ciao_activate ();

  [component name]_var ho
    = [component name]::_narrow (objref.in ());

  if (this->component_map_.bind (oid.in (), svt) == 0)
    {
      // @@ what should happen if bind fail?
      safe._retn ();
    }
  return ho._retn ();
}

void
[ciao module name]::[home name]_Servant::_ciao_passivate_component ([component name]_ptr comp)
{
  PortableServer::ObjectId_var oid;

  this->container_->uninstall_component (comp,
                                         oid.out ());

  [ciao module name]::[component name]_Servant *servant = 0;
  if (this->component_map_.unbind (oid.in (), servant) == 0)
    {
      PortableServer::ServantBase_var safe (servant);
      servant->_ciao_passivate ();
    }
  // What happen if unbind failed?

}

// Operations for Implicit Home interface
[component name]_ptr
[ciao module name]::[home name]_Servant::create ()
{
  if (this->executor_.in () == 0)
    throw CORBA::INTERNAL ();

  Components::EnterpriseComponent_var _ciao_ec =
    this->executor_->create ();

  CCM_[component name]_var _ciao_comp
    = CCM_[component name]::_narrow (_ciao_ec.in ());

  return this->_ciao_activate_component (_ciao_comp.in ());
}

// Operations for CCMHome interface
void
[ciao module name]::[home name]_Servant::remove_component (Components::CCMObject_ptr comp)
{
  [component name]_var _ciao_comp
    = [component name]::_narrow (comp);

  if (CORBA::is_nil (_ciao_comp.in ()))
    throw CORBA::INTERNAL (); // What is the right exception to throw here?

  // @@ It seems to me that we need to make sure this is a component
  // generated by this home before calling remove on this component.
  _ciao_comp->remove ();


  // Removing the object reference?  get the servant from the POA with
  // the objref, and call remove() on the component, deactivate the
  // component, and then remove-ref the servant?
  this->_ciao_passivate_component (_ciao_comp.in ());
}

extern "C" [SERVANT]_Export ::PortableServer::Servant
create[home name]_Servant (::Components::HomeExecutorBase_ptr p,
                           CIAO::Session_Container *c);
{
  if (p == 0)
    return 0;

  CCM_[home name]_var x
    = CCM_[home name]::_narrow (p);

  if (CORBA::is_nil (x.in ()))
    return 0;

  return new [ciao module name]::[home name]_Servant (x.in (),
                                                      c);
}
