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
// EventConsumer Glue Code implementation
// @@ We are assuming that these consumers are declared under the same
//    module as the component (thus, we are placing this glue code
//    here under the same namespace.  If they are not, we will
//    either be generating them in separate namespaces, or include
//    some other CIDL generated files to get the glue code
//    implementation.

#foreach [event type] in (all eventtype-s defined in the original IDL)

// Inherit from ::Compopnents::EventBConsumerBase
void
[ciao module name]::[event type]Consumer_Servant::push_event (EventBase_ptr ev
                                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ::Components::BadEventType))
{
# foreach [type] in ([event type] and all its parent eventtype, if any)

  [type] *ev_type = [type]::_downcast (ev);
  if (ev_type != 0)
    {
      this->push_[type] (ev ACE_ENV_ARG);
      return;
    }
# end foreach [type]

  ACE_THROW (::Components::BadEventType ());

}
#end foreach [event type]
