// $Id$ -*- C++ -*-

//    The generated filename for files using this template shoule be
//       [idl-basename]GS.i        GS --> GlueSession

// @@ Notice: [ciao module name] can expand to either CIAO_GLUE or
//    CIAO_GLUE_[module name] as defined in the header file.

//////////////////////////////////////////////////////////////////
// Facet Glue Code implementation
// @@ We are assuming that these facets are declared under the same
//    module as the component (thus, we are placing this glue code
//    here under the same namespace.  If they are not, we will
//    either be generating them in separate namespaces, or include
//    some other CIDL generated files to get the glue code
//    implementation.

#foreach [facet type] in (all facet interface types in the original IDL)
// Constructor and destructor.
ACE_INLINE
[ciao module name]::[facet type]_Servant::[facet type]_Servant (CCM_[facet type]_ptr executor)
  : executor_ (CCM_[facet type]::_duplicate (executor))
{
}

ACE_INLINE
[ciao module name]::[facet type]_Servant::~[facet tyep]_Servant ()
{
}


# foreach [operation] in (all facet operations)
ACE_INLINE [operation return_type]
[ciao module name]::[facet type]_Servant::[operation] ([operation args])
{
  // Simply relay to executor.  May not need to return the result...
  return this->operation ([operation args]);
}
# end foreach [operation]
#end foreach [facet type]


//////////////////////////////////////////////////////////////////
// EventConsumer Glue Code implementation
// @@ We are assuming that these consumers are declared under the same
//    module as the component (thus, we are placing this glue code
//    here under the same namespace.  If they are not, we will
//    either be generating them in separate namespaces, or include
//    some other CIDL generated files to get the glue code
//    implementation.

#foreach [event type] in (all eventtype-s defined in the original IDL)
// Constructor and destructor.
ACE_INLINE
[ciao module name]::[event type]Consumer_Servant::[event type]Consumer_Servant (CCM_[event type]Consumer_ptr executor)
  : executor_ (CCM_[event type]Consumer::_duplicate (executor))
{
}

ACE_INLINE
[ciao module name]::[event type]Consumer_Servant::~[event type]Consumer_Servant ()
{
}

# foreach [type] in ([event type] and all its parent eventtype, if any)
ACE_INLINE void
[ciao module name]::[event type]Consumer_Servant::push_[type]  ([type]_ptr evt
                                                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->executor_->push_[type] (evt ACE_ENV_ARG);
}
# end [type]

// Inherit from ::Compopnents::EventBConsumerBase
ACE_INLINE void
[ciao module name]::[event type]Consumer_Servant::push_event (EventBase_ptr ev)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Components::BadEventType))
{
  // ??
}
#end foreach [facet name]
