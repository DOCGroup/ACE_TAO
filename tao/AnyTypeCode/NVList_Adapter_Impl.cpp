// -*- C++ -*-
// $Id$

#include "tao/AnyTypeCode/NVList_Adapter_Impl.h"
#include "tao/AnyTypeCode/NVList.h"
#include "tao/SystemException.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_NVList_Adapter_Impl::create_list (
      CORBA::Long count,
      CORBA::NVList_ptr &new_list)
{
  ACE_ASSERT (CORBA::ULong (count) <= UINT_MAX);
  // Create an empty list
  ACE_NEW_THROW_EX (new_list,
                    CORBA::NVList,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  // If count is greater than 0, create a list of NamedValues.
  if (count != 0)
    {
      new_list->max_ = (CORBA::ULong) count;

      for (CORBA::Long i = 0; i < count; ++i)
        {
          CORBA::NamedValue_ptr nv = 0;
          ACE_NEW_THROW_EX (nv,
                            CORBA::NamedValue,
                            CORBA::NO_MEMORY (
                              CORBA::SystemException::_tao_minor_code (
                                0,
                                ENOMEM),
                              CORBA::COMPLETED_NO));

          new_list->values_.enqueue_tail (nv);
        }
    }
}

void
TAO_NVList_Adapter_Impl::create_named_value (CORBA::NamedValue_ptr &nv)
{
  ACE_NEW_THROW_EX (nv,
                    CORBA::NamedValue,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
}

int
TAO_NVList_Adapter_Impl::Initializer (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_NVList_Adapter_Impl);
}

ACE_STATIC_SVC_DEFINE (TAO_NVList_Adapter_Impl,
                       ACE_TEXT ("TAO_NVList_Adapter"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_NVList_Adapter_Impl),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_AnyTypeCode, TAO_NVList_Adapter_Impl)
TAO_END_VERSIONED_NAMESPACE_DECL
