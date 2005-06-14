// $Id$

#include "IORInterceptor_Adapter_Impl.h"
#include "IORInfo.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "tao/PI/PI.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"

ACE_RCSID (IORInterceptor,
           IORInterceptor_Adapter_Impl,
           "$Id$")


TAO_IORInterceptor_Adapter_Impl::~TAO_IORInterceptor_Adapter_Impl (void)
{
}

void
TAO_IORInterceptor_Adapter_Impl::add_interceptor (
    PortableInterceptor::IORInterceptor_ptr i
    ACE_ENV_ARG_DECL
  )
{
  int retval =
    this->ior_interceptor_list_.add_interceptor (i
                                                 ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

  if (retval == -1)
    {
      ACE_THROW (PortableInterceptor::ORBInitInfo::DuplicateName ());
    }
}

void
TAO_IORInterceptor_Adapter_Impl::destroy_interceptors (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC (())
{
  TAO::IORInterceptor_List::TYPE & i =
    this->ior_interceptor_list_.interceptors ();

  const size_t len = i.size ();
  size_t ilen = len;

  ACE_TRY
    {
      for (size_t k = 0; k < len; ++k)
        {
          // Destroy the interceptors in reverse order in case the
          // array list is only partially destroyed and another
          // invocation occurs afterwards.
          --ilen;

          i[k]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Since Interceptor::destroy() can throw an exception,
          // decrease the size of the interceptor array incrementally
          // since some interceptors may not have been destroyed yet.
          // Note that this size reduction is fast since no memory is
          // actually deallocated.
          i.size (ilen);
        }
    }
  ACE_CATCHALL
    {
      // Exceptions should not be propagated beyond this call.
      if (TAO_debug_level > 3)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Exception in ")
                      ACE_TEXT ("IORInterceptor_Adapter_Impl")
                      ACE_TEXT ("::destroy_interceptors () \n")));
        }
    }
  ACE_ENDTRY;
  ACE_CHECK;

  delete this;
}

TAO::IORInterceptor_List *
TAO_IORInterceptor_Adapter_Impl::interceptor_list (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return &this->ior_interceptor_list_;
}

void
TAO_IORInterceptor_Adapter_Impl::establish_components (
  TAO_Root_POA* poa
  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO::IORInterceptor_List::TYPE & interceptors =
    this->ior_interceptor_list_.interceptors ();

  const size_t interceptor_count = interceptors.size ();

  if (interceptor_count == 0)
    return;

  TAO_IORInfo *tao_info = 0;
  ACE_NEW_THROW_EX (tao_info,
                    TAO_IORInfo (poa),
                    CORBA::NO_MEMORY (
                       CORBA::SystemException::_tao_minor_code (
                          TAO::VMCID,
                          ENOMEM),
                       CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::IORInfo_var info = tao_info;

  // Release the POA during IORInterceptor calls to avoid potential
  // deadlocks.
  TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*poa);
  ACE_UNUSED_ARG (non_servant_upcall);

  for (size_t i = 0; i < interceptor_count; ++i)
    {
      ACE_TRY
        {
          interceptors[i]->establish_components (info.in ()
                                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // According to the Portable Interceptors specification,
          // IORInterceptor::establish_components() must not throw an
          // exception.  If it does, then the ORB is supposed to
          // ignore it and continue processing the remaining
          // IORInterceptors.
          if (TAO_debug_level > 1)
            {
              CORBA::String_var name = interceptors[i]->name (
                ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
              // @@ What do we do if we get an exception here?

              if (name.in () != 0)
                {
                  ACE_DEBUG ((LM_WARNING,
                              "(%P|%t) Exception thrown while processing "
                              "IORInterceptor \"%s\">\n",
                              ACE_TEXT_CHAR_TO_TCHAR (name.in ())));
                }

              ACE_PRINT_TAO_EXCEPTION (ACE_ANY_EXCEPTION,
                                       "Ignoring exception in "
                                       "IORInterceptor::establish_components");
            }
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }

  tao_info->components_established ();

  this->components_established (info.in ()
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // The IORInfo instance is no longer valid.  Invalidate it to
  // prevent the user from peforming "illegal" operations.
  tao_info->invalidate ();
}

void
TAO_IORInterceptor_Adapter_Impl::components_established (
  PortableInterceptor::IORInfo_ptr info
  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Iterate over the registered IOR interceptors so that they may be
  // given the opportunity to add tagged components to the profiles
  // for this servant.
  TAO::IORInterceptor_List::TYPE & interceptors =
    this->ior_interceptor_list_.interceptors ();

  const size_t interceptor_count = interceptors.size ();

  // All the establish_components() interception points have been
  // invoked. Now call the components_established() interception point
  // on all the IORInterceptors.
  for (size_t j = 0; j < interceptor_count; ++j)
    {
      ACE_TRY
        {
          interceptors[j]->components_established (
            info
            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          ACE_THROW (CORBA::OBJ_ADAPTER (CORBA::OMGVMCID | 6,
                                         CORBA::COMPLETED_NO));
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }
}

void
TAO_IORInterceptor_Adapter_Impl::adapter_state_changed (
      const TAO::ObjectReferenceTemplate_Array &array_obj_ref_template,
      PortableInterceptor::AdapterState state
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO::IORInterceptor_List::TYPE interceptors =
    this->ior_interceptor_list_.interceptors();

  const size_t interceptor_count = interceptors.size ();

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
      interceptors[i]->adapter_state_changed (seq_obj_ref_template,
                                              state
                                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_IORInterceptor_Adapter_Impl::adapter_manager_state_changed (
      PortableInterceptor::AdapterManagerId id,
      PortableInterceptor::AdapterState state
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  /// Whenever the POAManager state is changed, the
  /// adapter_manager_state_changed method is to be invoked on all the IOR
  ///  Interceptors.
  TAO::IORInterceptor_List::TYPE & interceptors =
    this->ior_interceptor_list_.interceptors ();

  const size_t interceptor_count = interceptors.size ();

  if (interceptor_count == 0)
    return;

  for (size_t i = 0; i < interceptor_count; ++i)
    {
      interceptors[i]->adapter_manager_state_changed (
        id,
        state
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}
