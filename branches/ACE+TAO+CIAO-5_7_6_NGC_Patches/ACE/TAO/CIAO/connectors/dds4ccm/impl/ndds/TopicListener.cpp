// $Id$

#include "TopicListener.h"
#include "Topic.h"
#include "InconsistentTopicStatus.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

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
        ::DDS::InconsistentTopicStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::Topic_var dds_topic = new RTI_Topic_i ();
        RTI_Topic_i *tp = dynamic_cast < RTI_Topic_i *> (dds_topic.in ());
        tp->set_impl (the_topic);
        this->impl_->on_inconsistent_topic (dds_topic.in (), ddsstatus);
      }

      ::DDS::TopicListener_ptr
      RTI_TopicListener_i::get_topiclistener (void)
      {
        return ::DDS::TopicListener::_duplicate (this->impl_.in ());
      }
    }
  }
}

