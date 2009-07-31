// $Id$

#include "Subscriber.h"
#include "SubscriberListener.h"
#include "Topic.h"
#include "DataReader.h"
#include "DataReaderListener.h"
#include "Utils.h"
#include "StatusCondition.h"
#include "InstanceHandle_t.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_Subscriber_i::RTI_Subscriber_i (DDSSubscriber *s)
        : impl_ (s)
      {
        CIAO_TRACE ("RTI_Subscriber_i::RTI_Subscriber_i");
      }

      // Implementation skeleton destructor
      RTI_Subscriber_i::~RTI_Subscriber_i (void)
      {
        CIAO_TRACE ("RTI_Subscriber_i::~RTI_Subscriber_i");
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::enable (void)
      {
        return this->impl_->enable ();
      }

      ::DDS::StatusCondition_ptr
      RTI_Subscriber_i::get_statuscondition (void)
      {
        DDSStatusCondition* sc = this->impl_->get_statuscondition ();
        ::DDS::StatusCondition_var retval = new RTI_StatusCondition_i (sc);
        return retval._retn ();
      }

      ::DDS::StatusMask
      RTI_Subscriber_i::get_status_changes (void)
      {
        return this->impl_->get_status_changes ();
      }

      ::DDS::InstanceHandle_t
      RTI_Subscriber_i::get_instance_handle (void)
      {
        ::DDS_InstanceHandle_t const rtihandle = this->impl_->get_instance_handle ();
        ::DDS::InstanceHandle_t handle;
        handle <<= rtihandle;
        return handle;
      }

      ::DDS::DataReader_ptr
      RTI_Subscriber_i::create_datareader (
        ::DDS::TopicDescription_ptr a_topic,
        const ::DDS::DataReaderQos & qos,
        ::DDS::DataReaderListener_ptr a_listener,
        ::DDS::StatusMask mask)
      {
        RTI_Topic_i * topic = dynamic_cast < RTI_Topic_i * > (a_topic);
        if (!topic)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_Subscriber_i::create_datareader - "
                         "Error: Unable to cast provided topic to its servant.\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
          }
        DDSTopic *rti_topic = topic->get_topic ();
//        DDSDataReaderListener *rti_drl = drl->get_datareaderlistener ();
// todo leak
        DDSDataReaderListener *rti_drl = new RTI_DataReaderListener_i (a_listener);
        DDSDataReader *rti_dr = this->impl_->create_datareader (rti_topic,
                                                               DDS_DATAREADER_QOS_DEFAULT,
                                                               rti_drl,
                                                               mask);
        if (!rti_dr)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_Subscriber_i::create_datareader - "
                         "Error: RTI Topic returned a nil datareader.\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
          }

        ::DDS::DataReader_var retval = new RTI_DataReader_i (rti_dr);

        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::delete_datareader (
        ::DDS::DataReader_ptr a_datareader)
      {
        RTI_DataReader_i *top = dynamic_cast< RTI_DataReader_i *> (a_datareader);
        if (!top)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_Subscriber_i::delete_datareader - "
                         "Unable to cast provided object reference to servant.\n"));
            return ::DDS::RETCODE_BAD_PARAMETER;
          }

        CIAO_DEBUG ((LM_TRACE, CLINFO "RTI_Subscriber_i::delete_datareader - "
                     "Successfully casted provided object reference to servant.\n"));

        DDS_ReturnCode_t retval = this->impl_->delete_datareader (top->get_datareader ());

        if (retval != DDS_RETCODE_OK)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_Subscriber_i::delete_datareader - "
                         "Error: Returned non-ok error code %c\n",
                         translate_retcode (retval)));
          }
        else CIAO_DEBUG ((LM_INFO, CLINFO "RTI_Subscriber_i::delete_datareader - "
                          "Successfully deleted\n"));

        return retval;
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::delete_contained_entities (void)
      {
        return this->impl_->delete_contained_entities ();
      }

      ::DDS::DataReader_ptr
      RTI_Subscriber_i::lookup_datareader (
        const char * impl_name)
      {
        DDSDataReader* dr = this->impl_->lookup_datareader (impl_name);
        ::DDS::DataReader_var retval = new RTI_DataReader_i (dr);
        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::get_datareaders (
        ::DDS::DataReaderSeq & readers,
        ::DDS::SampleStateMask sample_states,
        ::DDS::ViewStateMask view_states,
        ::DDS::InstanceStateMask instance_states)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::notify_datareaders (void)
      {
        return this->impl_->notify_datareaders ();
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::set_qos (
        const ::DDS::SubscriberQos & qos)
      {
        DDS_SubscriberQos rti_impl_qos;
/*        rti_impl_qos.presentation = qos.presentation;
        rti_impl_qos.partition = qos.partition;
        rti_impl_qos.group_data = qos.group_data;
        rti_impl_qos.entity_factory = qos.entity_factory;*/
        return this->impl_->set_qos (rti_impl_qos);
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::get_qos (
        ::DDS::SubscriberQos & qos)
      {
        DDS_SubscriberQos rti_impl_qos;
        DDS_ReturnCode_t const rti_retcode = this->impl_->get_qos (rti_impl_qos);
        /*qos.presentation = rti_impl_qos.presentation;
        qos.partition = rti_impl_qos.partition;
        qos.group_data = rti_impl_qos.group_data;
        qos.entity_factory = rti_impl_qos.entity_factory;*/
        return rti_retcode;
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::set_listener (
        ::DDS::SubscriberListener_ptr a_listener,
        ::DDS::StatusMask mask)
      {
        RTI_SubscriberListener_i* rti_impl_list = new RTI_SubscriberListener_i (a_listener);
        return this->impl_->set_listener (rti_impl_list, mask);
      }

      ::DDS::SubscriberListener_ptr
      RTI_Subscriber_i::get_listener (void)
      {
//        DDSSubscriberListener* rti_impl_list = this->impl_->get_listener ();
//        ::DDS::SubscriberListener_var retval = new RTI_SubscriberListener_i (rti_impl_list);
//        return retval._retn ();
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::begin_access (void)
      {
        return this->impl_->begin_access ();
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::end_access (void)
      {
        return this->impl_->end_access ();
      }

      ::DDS::DomainParticipant_ptr
      RTI_Subscriber_i::get_participant (void)
      {
        DDSDomainParticipant* p = this->impl_->get_participant ();
        ::DDS::DomainParticipant_var retval = new RTI_DomainParticipant_i (p);
        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::set_default_datareader_qos (
        const ::DDS::DataReaderQos & qos)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::get_default_datareader_qos (
        ::DDS::DataReaderQos & qos)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      ::DDS::ReturnCode_t
      RTI_Subscriber_i::copy_from_topic_qos (
        ::DDS::DataReaderQos & a_datareader_qos,
        const ::DDS::TopicQos & a_impl_qos)
      {
        throw CORBA::NO_IMPLEMENT ();
        // Add your implementation here
      }

      DDSSubscriber *
      RTI_Subscriber_i::get_subscriber (void)
      {
        return this->impl_;
      }
    }
  }
}

