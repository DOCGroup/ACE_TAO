// -*- C++ -*-
// $Id$

#include "tao/IORInterceptor/IORInterceptor_Adapter_Impl.h"
#include "tao/IORInterceptor/IORInfo.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "tao/PI/PI.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IORInterceptor_Adapter_Impl::~TAO_IORInterceptor_Adapter_Impl (void)
{
}

void
TAO_IORInterceptor_Adapter_Impl::add_interceptor (
    PortableInterceptor::IORInterceptor_ptr i)
{
  this->ior_interceptor_list_.add_interceptor (i);
}

void
TAO_IORInterceptor_Adapter_Impl::add_interceptor (
    PortableInterceptor::IORInterceptor_ptr i,
    const CORBA::PolicyList& policies)
{
  this->ior_interceptor_list_.add_interceptor (i, policies);
}

void
TAO_IORInterceptor_Adapter_Impl::destroy_interceptors (void)
{
  this->ior_interceptor_list_.destroy_interceptors ();

  delete this;
}

void
TAO_IORInterceptor_Adapter_Impl::establish_components (TAO_Root_POA* poa)
{
  size_t const interceptor_count = this->ior_interceptor_list_.size ();

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

  PortableInterceptor::IORInfo_var info = tao_info;

  // Release the POA during IORInterceptor calls to avoid potential
  // deadlocks.
  TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*poa);
  ACE_UNUSED_ARG (non_servant_upcall);

  for (size_t i = 0; i < interceptor_count; ++i)
    {
      try
        {
          this->ior_interceptor_list_.interceptor (i)->establish_components (
            info.in ());
        }
      catch (const ::CORBA::Exception& ex)
        {
          // According to the Portable Interceptors specification,
          // IORInterceptor::establish_components() must not throw an
          // exception.  If it does, then the ORB is supposed to
          // ignore it and continue processing the remaining
          // IORInterceptors.
          if (TAO_debug_level > 1)
            {
              CORBA::String_var name =
                this->ior_interceptor_list_.interceptor (i)->name ();
              // @@ What do we do if we get an exception here?

              if (name.in () != 0)
                {
                  TAOLIB_DEBUG ((LM_WARNING,
                              "(%P|%t) Exception thrown while processing "
                              "IORInterceptor \"%C\">\n",
                              name.in ()));
                }

              ex._tao_print_exception (
                "Ignoring exception in "
                "IORInterceptor::establish_components");
            }
        }
    }

  tao_info->components_established ();

  this->components_established (info.in ());

  // The IORInfo instance is no longer valid.  Invalidate it to
  // prevent the user from peforming "illegal" operations.
  tao_info->invalidate ();
}

void
TAO_IORInterceptor_Adapter_Impl::components_established (
  PortableInterceptor::IORInfo_ptr info)
{
  // Iterate over the registered IOR interceptors so that they may be
  // given the opportunity to add tagged components to the profiles
  // for this servant.
  size_t const interceptor_count = this->ior_interceptor_list_.size ();

  // All the establish_components() interception points have been
  // invoked. Now call the components_established() interception point
  // on all the IORInterceptors.
  for (size_t j = 0; j < interceptor_count; ++j)
    {
      try
        {
          PortableInterceptor::IORInterceptor_ptr ior_interceptor =
            this->ior_interceptor_list_.interceptor (j);

          PortableInterceptor::IORInterceptor_3_0_var ior_3_interceptor =
            PortableInterceptor::IORInterceptor_3_0::_narrow (ior_interceptor);
          if (!CORBA::is_nil (ior_3_interceptor.in ()))
            {
              ior_3_interceptor->components_established (info);
            }
        }
      catch (const ::CORBA::Exception&)
        {
          throw ::CORBA::OBJ_ADAPTER (CORBA::OMGVMCID | 6,
                                      CORBA::COMPLETED_NO);
        }
    }
}

void
TAO_IORInterceptor_Adapter_Impl::adapter_state_changed (
      const TAO::ObjectReferenceTemplate_Array &array_obj_ref_template,
      PortableInterceptor::AdapterState state)
{
  size_t const interceptor_count = this->ior_interceptor_list_.size ();

  if (interceptor_count == 0)
    return;

  PortableInterceptor::ObjectReferenceTemplateSeq seq_obj_ref_template;

  seq_obj_ref_template.length (
    static_cast <CORBA::ULong> (array_obj_ref_template.size()));

  for (size_t counter = 0; counter < array_obj_ref_template.size(); ++counter)
    {
      PortableInterceptor::ObjectReferenceTemplate *member =
        array_obj_ref_template[counter];

      CORBA::add_ref (member);

      seq_obj_ref_template[counter] = member;
    }

  for (size_t i = 0; i < interceptor_count; ++i)
    {
      PortableInterceptor::IORInterceptor_ptr ior_interceptor =
        this->ior_interceptor_list_.interceptor (i);

      PortableInterceptor::IORInterceptor_3_0_var ior_3_interceptor =
        PortableInterceptor::IORInterceptor_3_0::_narrow (ior_interceptor);
      if (!CORBA::is_nil (ior_3_interceptor.in ()))
        {
          ior_3_interceptor->adapter_state_changed (seq_obj_ref_template, state);
        }
    }
}

void
TAO_IORInterceptor_Adapter_Impl::adapter_manager_state_changed (
      const char * id,
      PortableInterceptor::AdapterState state)
{
  /// Whenever the POAManager state is changed, the
  /// adapter_manager_state_changed method is to be invoked on all the IOR
  ///  Interceptors.
  size_t const interceptor_count = this->ior_interceptor_list_.size ();

  if (interceptor_count == 0)
    return;

  for (size_t i = 0; i < interceptor_count; ++i)
    {
      PortableInterceptor::IORInterceptor_ptr ior_interceptor =
        this->ior_interceptor_list_.interceptor (i);

      PortableInterceptor::IORInterceptor_3_0_var ior_3_interceptor =
        PortableInterceptor::IORInterceptor_3_0::_narrow (ior_interceptor);
      if (!CORBA::is_nil (ior_3_interceptor.in ()))
        {
          ior_3_interceptor->adapter_manager_state_changed (id, state);
        }
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
