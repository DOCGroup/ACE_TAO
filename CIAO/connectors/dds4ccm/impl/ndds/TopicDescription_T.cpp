// $Id$

#include "TopicDescription_T.h"
#include "DomainParticipant_T.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"
#include "dds4ccm/impl/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE>
    DDS_TopicDescription_T<DDS_TYPE>::DDS_TopicDescription_T (
      DDSTopicDescription * rc) : impl_ (rc)
    {
    }

    template <typename DDS_TYPE>
    DDS_TopicDescription_T<DDS_TYPE>::~DDS_TopicDescription_T (void)
    {
    }

    template <typename DDS_TYPE>
    char *
    DDS_TopicDescription_T<DDS_TYPE>::get_type_name (void)
    {
      return CORBA::string_dup (this->impl ()->get_type_name ());
    }

    template <typename DDS_TYPE>
    char *
    DDS_TopicDescription_T<DDS_TYPE>::get_name (void)
    {
      return CORBA::string_dup (this->impl ()->get_name ());
    }

    template <typename DDS_TYPE>
    ::DDS::DomainParticipant_ptr
    DDS_TopicDescription_T<DDS_TYPE>::get_participant (void)
    {
      ::DDS::DomainParticipant_var retval;
      DDSDomainParticipant* p = this->impl ()->get_participant ();
      if (p)
        {
          ACE_NEW_THROW_EX (retval,
                            DomainParticipant_type (p),
                            ::CORBA::NO_MEMORY ());
        }
      return retval._retn ();
    }

    template <typename DDS_TYPE>
    DDSTopicDescription *
    DDS_TopicDescription_T<DDS_TYPE>::get_impl (void)
    {
      return this->impl_;
    }

    template <typename DDS_TYPE>
    void
    DDS_TopicDescription_T<DDS_TYPE>::set_impl (DDSTopicDescription * rc)
    {
      this->impl_ = rc;
    }

    template <typename DDS_TYPE>
    DDSTopicDescription *
    DDS_TopicDescription_T<DDS_TYPE>::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}

