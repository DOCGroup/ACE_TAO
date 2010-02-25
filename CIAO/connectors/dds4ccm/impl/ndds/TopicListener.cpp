// $Id$

#include "TopicListener.h"
#include "Topic.h"
#include "InconsistentTopicStatus.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    CCM_DDS_TopicListener_i::CCM_DDS_TopicListener_i (::DDS::TopicListener_ptr p)
      : impl_ (::DDS::TopicListener::_duplicate (p))
    {
    }

    CCM_DDS_TopicListener_i::~CCM_DDS_TopicListener_i (void)
    {
    }

    void
    CCM_DDS_TopicListener_i::on_inconsistent_topic (
      ::DDSTopic* the_topic,
      const ::DDS_InconsistentTopicStatus & status)
    {
      ::DDS::Topic_var dds_topic = ::DDS::Topic::_nil ();
      ACE_NEW (dds_topic,
               CCM_DDS_Topic_i (the_topic));
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
      ::DDS::InconsistentTopicStatus ddsstatus;
      ddsstatus <<= status;
      this->impl_->on_inconsistent_topic (dds_topic.in (), ddsstatus);
#else
      this->impl_->on_inconsistent_topic (dds_topic.in (), status);
#endif
    }

    ::DDS::TopicListener_ptr
    CCM_DDS_TopicListener_i::get_topiclistener (void)
    {
      return ::DDS::TopicListener::_duplicate (this->impl_.in ());
    }
  }
}

