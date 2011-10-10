// $Id$

#include "dds4ccm/impl/ndds/TopicListener.h"
#include "dds4ccm/impl/ndds/Topic.h"
#include "dds4ccm/impl/ndds/convertors/InconsistentTopicStatus.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace NDDS
  {
    DDS_TopicListener_i::DDS_TopicListener_i (
      ::DDS::Topic_ptr topic,
      ::DDS::TopicListener_ptr topic_listener)
      : topic_ (::DDS::Topic::_duplicate (topic)),
        topic_listener_ (::DDS::TopicListener::_duplicate (topic_listener))
    {
    }

    DDS_TopicListener_i::~DDS_TopicListener_i (void)
    {
    }

    void
    DDS_TopicListener_i::on_inconsistent_topic (
      ::DDSTopic*,
      const ::DDS_InconsistentTopicStatus & status)
    {
      DDS4CCM_TRACE ("DDS_TopicListener_i::on_inconsistent_topic");

      ::DDS::InconsistentTopicStatus ddsstatus;
      ddsstatus <<= status;
      this->topic_listener_->on_inconsistent_topic (this->topic_.in (), ddsstatus);
    }

    void
    DDS_TopicListener_i::set_dds_topic (::DDS::Topic_ptr topic)
    {
      this->topic_ = ::DDS::Topic::_duplicate (topic);
    }
  }
}

