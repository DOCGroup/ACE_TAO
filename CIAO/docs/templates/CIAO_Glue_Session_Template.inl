// $Id$ -*- C++ -*-

//    The generated filename for files using this template shoule be
//       [idl-basename]GS.i        GS --> GlueSession

// @@ Notice: [ciao module name] can expand to either CIAO_GLUE or
//    CIAO_GLUE_[module name] as defined in the header file.

/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/// @@@ Notice that all component and interface names need to be
/// fully qualified as we are creating a new namespace for the CIAO's
/// container glue code.
/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

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
// Constructor and destructor.
ACE_INLINE
[ciao module name]::[facet type]_Servant::[facet type]_Servant (CCM_[facet type]_ptr executor,
                                                                ::Components::CCMContext_ptr c)
  : executor_ (CCM_[facet type]::_duplicate (executor)),
    ctx_ (Components::CCMContext::_duplicate (c))
{
}

ACE_INLINE
[ciao module name]::[facet type]_Servant::~[facet tyep]_Servant ()
{
}


##  foreach [operation] in (all facet operations)

// This is only a guideline...  we always relay the operation to underlying
// executor.

ACE_INLINE [operation return_type]
[ciao module name]::[facet type]_Servant::[operation] ([operation args])
{
  // Simply relay to executor.  May not need to return the result...
  return this->executor_->operation ([operation args]);
}
##  end foreach [operation]

##end foreach [facet type]


//////////////////////////////////////////////////////////////////
// Component specific context implementation
//////////////////////////////////////////////////////////////////

ACE_INLINE
[ciao module name]::[component name]_Context::[component name]_Context (::Components::CCMHome_ptr home,
                                                                        ::CIAO::Session_Container *c,
                                                                        [ciao module name]::[component name]_Servant *sv)
  : home_ (::Components::CCMHome::_duplicate (home)),
    container_ (c),
    servant_ (sv)
{

}

ACE_INLINE
[ciao module name]::[component name]_Context::~[component name]_Context ()
{
}

// Operations for emits interfaces.
##foreach [emit name] with [eventtype] in (list of all emitters) generate:

ACE_INLINE void
[ciao module name]::[component name]_Context::connect_[emit name] ([eventtype]Consumer_ptr c)
{
  if (CORBA::is_nil (c))
    throw CORBA::BAD_PARAM ();

  if (! CORBA::is_nil (this->ciao_emits_[emit name]_consumer_.in ()))
    throw ::Components::AlreadyConnected ();

  this->ciao_emits_[emit name]_consumer_ = c;
}

ACE_INLINE [eventtype]Consumer_ptr
[ciao module name]::[component name]_Context::disconnect_[emit name] ()
{
  if (CORBA::is_nil (this->ciao_emits_[emit name]_consumer_.in ()))
    throw ::Components::NoConnection ();

  return this->ciao_emits_[emit name]_consumer_._retn ();
}

##end foreach [emit name] with [eventtype]

// Operations for ::Components::CCMContext
ACE_INLINE ::Components::Principal_ptr
[ciao module name]::[component name]_Context::get_caller_principal ()
{
  // @@ We don't support Security in CIAO yet.
  throw CORBA::NO_IMPLEMENT ();
}

ACE_INLINE ::Components::CCMHome_ptr
[ciao module name]::[component name]_Context::get_CCM_home ()
{
  return ::Components::CCMHome::_duplicate (this->home_.in ());
}

ACE_INLINE CORBA::Boolean
[ciao module name]::[component name]_Context::get_rollback_only ()
{
  // @@ We don't support Transaction in CIAO yet.
  throw CORBA::NO_IMPLEMENT ();
}

ACE_INLINE ::Components::Transaction::UserTransaction_ptr
[ciao module name]::[component name]_Context::get_user_transaction ()
{
  // @@ We don't support Transaction in CIAO yet.
  throw CORBA::NO_IMPLEMENT ();
}

ACE_INLINE CORBA::Boolean
[ciao module name]::[component name]_Context::is_caller_in_role (const char * role)
{
  ACE_UNUSED_ARG (role);

  // @@ We don't support Transaction in CIAO yet.
  throw CORBA::NO_IMPLEMENT ();
}

ACE_INLINE void
[ciao module name]::[component name]_Context::set_rollback_only ()
{
  // @@ We don't support Transaction in CIAO yet.
  throw CORBA::NO_IMPLEMENT ();
}

//////////////////////////////////////////////////////////////////
// Component Servant Glue code implementation
//////////////////////////////////////////////////////////////////

##foreach [operation] in all supported interfaces of own component and all inherited components and attribute accessors/mutators

// This is only a guideline...  we always relay the operation to underlying
// executor.

ACE_INLINE [operation return_type]
[ciao module name]::[component name]_Servant::[operation] ([operation args])
{
  // Simply relay to executor.  May not need to return the result...
  return this->executor_->operation ([operation args]);
}

##end

##foreach [receptacle name] with [uses type] in (list of all 'uses' interfaces) generate:
##  if [receptacle name] is a simplex receptacle ('uses')
// Simplex [receptacle name] connection management operations
ACE_INLINE void
[ciao module name]::[component name]_Servant::connect_[receptacle name] ([uses type]_ptr c)
{
  this->context_->connect_[receptacle name] (c);
}

ACE_INLINE [uses type]_ptr
[ciao module name]::[component name]_Servant::disconnect_[receptacle name] ()
{
  return this->context_->disconnect_[receptacle name] ();
}

ACE_INLINE [uses type]_ptr
[ciao module name]::[component name]_Servant::get_connection_[receptacle name] ()
{
  return this->context_->get_connection_[receptacle name] ();
}
##  else ([receptacle name] is a multiplex ('uses multiple') receptacle)

// Multiplex [receptacle name] connection management operations
ACE_INLINE ::Components::Cookie *
[ciao module name]::[component name]_Servant::connect_[receptacle name] ([uses type]_ptr c)
{
  return this->context_->connect_[receptacle name] (c);
}

ACE_INLINE [uses type]_ptr
[ciao module name]::[component name]_Servant::disconnect_[receptacle name] (::Components::Cookie *ck)
{
  return this->context_->disconnect_[receptacle name] (ck);
}

ACE_INLINE [receptacle name]Connections *
[ciao module name]::[component name]_Servant::get_connections_[receptacle name] ()
{
  return this->context_->get_connections_[receptacle name] ();
}

##  endif [receptacle name]
##end foreach [receptacle name] with [uses type]

//////////////////////////////////////////////////////////////////
// EventConsumer Glue Code implementation
// @@ We are assuming that these consumers are declared under the same
//    module as the component (thus, we are placing this glue code
//    here under the same namespace.  If they are not, we will
//    either be generating them in separate namespaces, or include
//    some other CIDL generated files to get the glue code
//    implementation.
//////////////////////////////////////////////////////////////////

##foreach [consumer name] with [eventtype] in (list of all consumers) generate:

// Constructor and destructor.
ACE_INLINE
[ciao module name]::[component name]_Servant::[eventtype]Consumer_[consumer name]_Servant::[eventtype]Consumer_[consumer name]_Servant
  (CCM_[component name]_ptr executor,
   CCM_[component name]_Context_ptr c)
    : executor_ (CCM_[component name]::_duplicate (executor)),
      ctx_ (CCM_[component name]_Context::_duplicate (c))
{
}

ACE_INLINE
[ciao module name]::[component name]_Servant::[eventtype]Consumer_[consumer name]_Servant::~[eventtype]Consumer_[consumer name]_Servant ()
{
}

ACE_INLINE CORBA::Object_ptr
[ciao module name]::[component name]_Servant::[eventtype]Consumer_[consumer name]_Servant::_get_component ()
{
  return this->ctx_->get_CCM_object ();
}

ACE_INLINE void
[ciao module name]::[component name]_Servant::[eventtype]Consumer_[consumer name]_Servant::push_[eventtype]
  ([eventtype] *evt)
{
  this->executor_->push_[consumer name] (evt);
}

##  foreach [type] in (all parents of eventtype, if any, not including EventConsumerBase)
ACE_INLINE void
[ciao module name]::[component name]_Servant::[eventtype]Consumer_[consumer name]_Servant::push_[type]
  ([type] *evt)
{
  // @@ Problem, there's no way to handle this case.

  //  this->executor_->push_[consumer name] (evt ACE_ENV_ARG);
  throw ::CORBA::BAD_PARAM ();
}
##  end [type]

##end foreach [consumer name]

// Operations for emits interfaces.
##foreach [emit name] with [eventtype] in (list of all emitters) generate:
ACE_INLINE void
[ciao module name]::[component name]_Servant::connect_[emit name] ([eventtype]Consumer_ptr c)
{
  this->context_->connect_[emit name] (c);
}

ACE_INLINE [eventtype]Consumer_ptr
[ciao module name]::[component name]_Servant::disconnect_[emit name] ()
{
  return this->context_->disconnect_[emit name] ();
}
##end foreach [emit name] with [eventtype]

// Operations for publishes interfaces.
##foreach [publish name] with [eventtype] in (list of all publishers) generate:
ACE_INLINE ::Components::Cookie *
[ciao module name]::[component name]_Servant::subscribe_[publish name] ([eventtype]Consumer_ptr c)
{
  return this->context_->subscribe_[publish name] (c);
}

ACE_INLINE [eventtype]Consumer_ptr
[ciao module name]::[component name]_Servant::unsubscribe_[publish name] (::Components::Cookie *ck)
{
  return this->context_->unsubscribe_[publish name] (ck);
}
##end foreach [publish name] with [eventtype]

//////////////////////////////////////////////////////////////////
// Component Home Glue code implementation
//////////////////////////////////////////////////////////////////

ACE_INLINE
[ciao module name]::[home name]_Servant::[home name]_Servant (CCM_[home name]_ptr exe,
                                                              CIAO::Session_Container *c)
  : executor_ (CCM_[home name]::_duplicate (exe)),
    container_ (c)
{
}

ACE_INLINE
[ciao module name]::[home name]_Servant::~[home name]_Servant (void)
{
}

##foreach [operation] in (all explicit operations in [home basename] including its parents)

// The operation impl here.  This can simply be relayed to the underlying executor...

## end foreach opeartion

##foreach [factory name]  in (all factory operations in [home basename] including its parents)
// for factory operations inherit from parent home(s), they should return
// the corresponding component types their homes manage
ACE_INLINE [component name]_ptr
[ciao module name]::[home name]_Servant::[factory name] (....)
{
  Components::EnterpriseComponent_var _ciao_ec =
    this->executor_->[factory name] (....);

  CCM_[component name]_var _ciao_comp
    = CCM_[component name]::_narrow (_ciao_ec.in ());

  return this->_ciao_activate_component (_ciao_comp.in ());
}
##end foreach [factory name]

##foreach [finder name]  in (all finder operations in [home basename] including its parents)
// for finder operations inherit from parent home(s), they should return
// the corresponding component types their homes manage
ACE_INLINE [component name]_ptr
[ciao module name]::[home name]_Servant::[finder name] (....)
{
  Components::EnterpriseComponent_var com =
    this->executor_->[finder name] (....);

  // Do we create a new object reference referring to the same object,
  // or do we try to create a different objref referring to the same object?
  return this->_ciao_create_helper (com);
}
##end foreach [finder name]

##  if [home name] is a keyless home

// Operations for KeylessHome interface
ACE_INLINE ::Components::CCMObject_ptr
[ciao module name]::[home name]_Servant::create_component ()
{
  // Simply forward to the create method.
  return this->create ();
}

##  else [home basename] is keyed home with [key type]

// We do not support key'ed home at the moment but we might
// as well generate the mapping.
ACE_INLINE [component name]_ptr
[ciao module name]::[home name]_Servant::create ([key type] *key)
{
  // @@ TO-DO when we suppor keyed home.

  throw CORBA::NO_IMPLEMENT ();
}

ACE_INLINE [component name]_ptr
[ciao module name]::[home name]_Servant::find_by_primary_key ([key type] *key)
{
  // @@ TO-DO when we suppor keyed home.

  throw CORBA::NO_IMPLEMENT ();
}


ACE_INLINE void
[ciao module name]::[home name]_Servant::remove ([key type] *key)
{
  // @@ TO-DO when we suppor keyed home.

  throw CORBA::NO_IMPLEMENT ();
}

ACE_INLINE [key type] *
[ciao module name]::[home name]_Servant::get_primary_key ([component name]_ptr comp)
{
  // @@ TO-DO when we suppor keyed home.

  throw CORBA::NO_IMPLEMENT ();
}

##  endif (keyed or keyless home)


// Operations for CCMHome interface
ACE_INLINE ::CORBA::IRObject_ptr
[ciao module name]::[home name]_Servant::get_component_def ()
{
  // @@ TO-DO.  Contact IfR?

  throw CORBA::NO_IMPLEMENT ();
}

ACE_INLINE CORBA::IRObject_ptr
[ciao module name]::[home name]_Servant::get_home_def ()
{
  // @@ TO-DO.  Contact IfR?

  throw CORBA::NO_IMPLEMENT ();
}
