// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/Topic.h"
#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::TopicListener_T<DDS_TYPE, CCM_TYPE>::TopicListener_T (
      typename CCM_TYPE::context_type::_ptr_type context)
      : context_ (CCM_TYPE::context_type::_duplicate (context)),
        info_out_connector_status_ (CCM_DDS::ConnectorStatusListener::_duplicate (context->get_connection_error_listener ()))
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
  if (!CORBA::is_nil (this->info_out_connector_status_))
    {
      this->info_out_connector_status_->on_inconsistent_topic (the_topic, status);
    }
}

