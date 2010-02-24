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
    namespace RTI
    {
      RTI_TopicListener_i::RTI_TopicListener_i (::DDS::TopicListener_ptr p)
        : impl_ (::DDS::TopicListener::_duplicate (p))
      {
      }

      RTI_TopicListener_i::~RTI_TopicListener_i (void)
      {
      }

      void
      RTI_TopicListener_i::on_inconsistent_topic (
        ::DDSTopic* the_topic,
        const ::DDS_InconsistentTopicStatus & status)
      {
#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
        ::DDS::InconsistentTopicStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::Topic_var dds_topic = ::DDS::Topic::_nil ();
        ACE_NEW (dds_topic, RTI_Topic_i ());
        RTI_Topic_i *tp = dynamic_cast < RTI_Topic_i *> (dds_topic.in ());
        tp->set_impl (the_topic);
        this->impl_->on_inconsistent_topic (dds_topic.in (), ddsstatus);
#else   
        this->impl_->on_inconsistent_topic (the_topic, status);
#endif     
      }

      ::DDS::TopicListener_ptr
      RTI_TopicListener_i::get_topiclistener (void)
      {
        return ::DDS::TopicListener::_duplicate (this->impl_.in ());
      }
    }
  }
}

