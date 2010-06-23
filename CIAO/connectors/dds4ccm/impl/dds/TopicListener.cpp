// $Id$

#include "TopicListener.h"
#include "Topic.h"

#include "ndds/InconsistentTopicStatus.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/dds/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_TopicListener_i<DDS_TYPE, CCM_TYPE>::CCM_DDS_TopicListener_i (
        ::DDS::TopicListener_ptr p)
      : impl_ (::DDS::TopicListener::_duplicate (p))
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_TopicListener_i<DDS_TYPE, CCM_TYPE>::~CCM_DDS_TopicListener_i (void)
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_TopicListener_i<DDS_TYPE, CCM_TYPE>::on_inconsistent_topic (
      ::DDSTopic* the_topic,
      const ::DDS_InconsistentTopicStatus & status)
    {
      ::DDS::Topic_var dds_topic = ::DDS::Topic::_nil ();
      ACE_NEW (dds_topic,
               Topic_type (the_topic));
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS::InconsistentTopicStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_inconsistent_topic (dds_topic.in (), ddsstatus);
#else
      this->impl_->on_inconsistent_topic (dds_topic.in (), status);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::TopicListener_ptr
    CCM_DDS_TopicListener_i<DDS_TYPE, CCM_TYPE>::get_topiclistener (void)
    {
      return ::DDS::TopicListener::_duplicate (this->impl_.in ());
    }
  }
}

