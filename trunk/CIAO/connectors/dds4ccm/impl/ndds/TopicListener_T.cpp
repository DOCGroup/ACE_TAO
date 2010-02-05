// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/Topic.h"
#include "dds4ccm/impl/ndds/DDSCallbackStatusHandler.h"
#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::TopicListener_T<DDS_TYPE, CCM_TYPE>::TopicListener_T (
  ::CCM_DDS::ConnectorStatusListener_ptr error_listener,
  ACE_Reactor* reactor) :
    error_listener_ (::CCM_DDS::ConnectorStatusListener::_duplicate (error_listener)),
    reactor_ (reactor)
{
  CIAO_TRACE ("CIAO::DDS4CCM::TopicListener_T::TopicListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::TopicListener_T<DDS_TYPE, CCM_TYPE>::~TopicListener_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::TopicListener_T::~TopicListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::TopicListener_T<DDS_TYPE, CCM_TYPE>::on_inconsistent_topic (
  ::DDS::Topic_ptr the_topic,
  const ::DDS::InconsistentTopicStatus & status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::TopicListener_T::on_inconsistent_topic");

  CIAO_DEBUG (10, (LM_DEBUG, CLINFO
              ACE_TEXT ("TopicListener_T::on_inconsistent_topic: ")
              ACE_TEXT ("total count <%d> - total change <%d> - "),
              status.total_count, status.total_count_change));

  if (!CORBA::is_nil (this->error_listener_.in ()))
    {
      if (this->reactor_)
        {
          ::CIAO::DDS4CCM::OnInconsistentTopicHandler* rh =
           new ::CIAO::DDS4CCM::OnInconsistentTopicHandler (
            this->error_listener_, the_topic, status);
          ACE_Event_Handler_var safe_handler (rh);
          if (this->reactor_->notify (rh) != 0)
            {
              ACE_ERROR ((LM_ERROR, CLINFO
                          ACE_TEXT ("TopicListener_T::on_inconsistent_topic: ")
                          ACE_TEXT ("failed to use reactor.\n")));
            }
        }
      else
        {
          this->error_listener_->on_inconsistent_topic (the_topic, status);
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::TopicListener_T<DDS_TYPE, CCM_TYPE>::get_mask (
  CCM_DDS::ConnectorStatusListener_ptr error_listener)
{
  CIAO_TRACE ("CIAO::DDS4CCM::TopicListener_T::get_mask");

  if (!CORBA::is_nil (error_listener) || CIAO_debug_level >= 10)
    {
      return DDS_INCONSISTENT_TOPIC_STATUS;
    }
  else
    {
      return DDS_STATUS_MASK_NONE;
    }
}

