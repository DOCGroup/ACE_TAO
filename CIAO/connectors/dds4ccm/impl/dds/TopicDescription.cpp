// $Id$

#include "TopicDescription.h"
#include "DomainParticipant.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"
#include "dds4ccm/impl/dds/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_TopicDescription_i<DDS_TYPE, CCM_TYPE>::CCM_DDS_TopicDescription_i (
      DDSTopicDescription * rc) : impl_ (rc)
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_TopicDescription_i<DDS_TYPE, CCM_TYPE>::~CCM_DDS_TopicDescription_i (void)
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    char *
    CCM_DDS_TopicDescription_i<DDS_TYPE, CCM_TYPE>::get_type_name (void)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      return CORBA::string_dup (this->impl ()->get_type_name ());
#else
      return this->impl ()->get_type_name ();
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    char *
    CCM_DDS_TopicDescription_i<DDS_TYPE, CCM_TYPE>::get_name (void)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      return CORBA::string_dup (this->impl ()->get_name ());
#else
      return this->impl ()->get_name ();
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::DomainParticipant_ptr
    CCM_DDS_TopicDescription_i<DDS_TYPE, CCM_TYPE>::get_participant (void)
    {
      ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSDomainParticipant* p = this->impl ()->get_participant ();
      if (p)
        {
          ACE_NEW_THROW_EX (retval,
                            DomainParticipant_type (p),
                            CORBA::NO_MEMORY ());
        }
#else
      ::DDS::DomainParticipant_var p = this->impl ()->get_participant ();
      if (! ::CORBA::is_nil (p.in ()))
        {
          ACE_NEW_THROW_EX (retval,
                            DomainParticipant_type (p.in ()),
                            CORBA::NO_MEMORY ());
        }
#endif
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    DDSTopicDescription *
    CCM_DDS_TopicDescription_i<DDS_TYPE, CCM_TYPE>::get_impl (void)
    {
      return this->impl_;
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_TopicDescription_i<DDS_TYPE, CCM_TYPE>::set_impl (DDSTopicDescription * rc)
    {
      this->impl_ = rc;
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    DDSTopicDescription *
    CCM_DDS_TopicDescription_i<DDS_TYPE, CCM_TYPE>::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}

