// $Id$

#include "dds4ccm/impl/ndds/DDSTopicListener_T.h"
#include "dds4ccm/impl/ndds/Topic_T.h"

#include "dds4ccm/impl/ndds/convertors/InconsistentTopicStatus.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    CCM_DDS_TopicListener_T<DDS_TYPE, VENDOR_TYPE>::CCM_DDS_TopicListener_T (
        ::DDS::TopicListener_ptr p)
      : impl_ (::DDS::TopicListener::_duplicate (p))
    {
    }

    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    CCM_DDS_TopicListener_T<DDS_TYPE, VENDOR_TYPE>::~CCM_DDS_TopicListener_T (void)
    {
    }

    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    void
    CCM_DDS_TopicListener_T<DDS_TYPE, VENDOR_TYPE>::on_inconsistent_topic (
      ::DDSTopic* the_topic,
      const ::DDS_InconsistentTopicStatus & status)
    {
      DDS4CCM_TRACE ("CCM_DDS_TopicListener_T<DDS_TYPE, VENDOR_TYPE>::on_inconsistent_topic");

      ::DDS::Topic_var dds_topic;
      ACE_NEW (dds_topic,
               Topic_type (the_topic));
      ::DDS::InconsistentTopicStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_inconsistent_topic (dds_topic.in (), ddsstatus);
    }

    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::TopicListener_ptr
    CCM_DDS_TopicListener_T<DDS_TYPE, VENDOR_TYPE>::get_topiclistener (void)
    {
      return ::DDS::TopicListener::_duplicate (this->impl_.in ());
    }
  }
}

