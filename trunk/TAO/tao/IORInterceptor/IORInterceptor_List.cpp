// $Id$

#include "IORInterceptor_List.h"

#if !defined (__ACE_INLINE__)
# include "IORInterceptor_List.inl"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID (IORInterceptor,
           Interceptor_List,
           "$Id$")


TAO_IORInterceptor_List::TAO_IORInterceptor_List (void)
  : interceptors_ ()
{
}

TAO_IORInterceptor_List::~TAO_IORInterceptor_List (void)
{
}

size_t
TAO_IORInterceptor_List::length (void)
{
  return this->interceptors_.size ();
}

void
TAO_IORInterceptor_List::length (size_t len)
{
  this->interceptors_.size (len);
}

PortableInterceptor::Interceptor_ptr
TAO_IORInterceptor_List::interceptor (size_t index)
{
  return this->interceptors_[index].in ();
}

void
TAO_IORInterceptor_List::add_interceptor (
  PortableInterceptor::IORInterceptor_ptr interceptor
  ACE_ENV_ARG_DECL)
{
  const size_t index = this->add_interceptor_i (interceptor
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->interceptors_[index] =
    PortableInterceptor::IORInterceptor::_duplicate (interceptor);
}

void
TAO_IORInterceptor_List::adapter_state_changed (
  const TAO::ORT_Array &array_obj_ref_template,
  PortableInterceptor::AdapterState state
  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  const size_t interceptor_count = this->interceptors_.size ();

  if (interceptor_count == 0)
    return;

  PortableInterceptor::ObjectReferenceTemplateSeq seq_obj_ref_template;

  seq_obj_ref_template.length (array_obj_ref_template.size());

  for (size_t counter = 0; counter < array_obj_ref_template.size(); ++counter)
    {
      PortableInterceptor::ObjectReferenceTemplate *member =
        array_obj_ref_template[counter];

      CORBA::add_ref (member);

      seq_obj_ref_template[counter] = member;
    }

  for (size_t i = 0; i < interceptor_count; ++i)
    {
      this->interceptors_[i]->adapter_state_changed (seq_obj_ref_template,
                                                     state
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Array_Base<PortableInterceptor::IORInterceptor_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Array_Base<PortableInterceptor::IORInterceptor_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
