// -*- C++ -*-
#include "tao/AnyTypeCode/NVList_Adapter_Impl.h"
#include "tao/AnyTypeCode/NVList.h"
#include "tao/SystemException.h"
#include "ace/CORBA_macros.h"

#include <memory>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_NVList_Adapter_Impl::create_list (
      CORBA::Long count,
      CORBA::NVList_ptr &new_list)
{
  ACE_ASSERT (CORBA::ULong (count) <= UINT_MAX);
  // Create an empty list
  CORBA::NVList_ptr list {};
  ACE_NEW_THROW_EX (list,
                    CORBA::NVList,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        0,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  std::unique_ptr<CORBA::NVList, void (*) (CORBA::NVList *)> list_owner (
    list,
    [] (CORBA::NVList *value) { value->_decr_refcount (); });

  // If count is greater than 0, create a list of NamedValues.
  if (count != 0)
    {
      list->max_ = static_cast<CORBA::ULong> (count);

      for (CORBA::Long i {}; i < count; ++i)
        {
          CORBA::NamedValue_ptr nv {};
          ACE_NEW_THROW_EX (nv,
                            CORBA::NamedValue,
                            CORBA::NO_MEMORY (
                              CORBA::SystemException::_tao_minor_code (
                                0,
                                ENOMEM),
                              CORBA::COMPLETED_NO));

          std::unique_ptr<CORBA::NamedValue, void (*) (CORBA::NamedValue *)> nv_owner (
            nv,
            [] (CORBA::NamedValue *value) { value->_decr_refcount (); });

          if (list->values_.enqueue_tail (nv) == -1)
            {
              throw CORBA::NO_MEMORY (
                CORBA::SystemException::_tao_minor_code (0, ENOMEM),
                CORBA::COMPLETED_NO);
            }

          nv_owner.release ();
        }
    }

  new_list = list_owner.release ();
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
TAO_NVList_Adapter_Impl::Initializer ()
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
