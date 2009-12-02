// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/Topic.h"
#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::TopicListener_T<DDS_TYPE, CCM_TYPE>::TopicListener_T (
  ::CCM_DDS::ConnectorStatusListener_ptr error_listener) :
    error_listener_ (::CCM_DDS::ConnectorStatusListener::_duplicate (error_listener))
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
  if (!CORBA::is_nil (this->error_listener_.in ()))
    {
      this->error_listener_->on_inconsistent_topic (the_topic, status);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::TopicListener_T<DDS_TYPE, CCM_TYPE>::get_mask (void)
{
  return DDS_INCONSISTENT_TOPIC_STATUS;
}

