// $Id$

#include "Publisher.h"
#include "PublisherListener.h"
#include "Topic.h"
#include "DataWriter.h"
#include "StatusCondition.h"
#include "Utils.h"
#include "Duration_t.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_Publisher_i::RTI_Publisher_i (DDSPublisher *p)
        : pub_ (p)
      {
        CIAO_TRACE ("RTI_Publisher_i::RTI_Publisher_i");
      }

      // Implementation skeleton destructor
      RTI_Publisher_i::~RTI_Publisher_i (void)
      {
        CIAO_TRACE ("RTI_Publisher_i::~RTI_Publisher_i");
      }

      ::DDS::DataWriter_ptr
      RTI_Publisher_i::create_datawriter (::DDS::Topic_ptr a_topic,
                                          const ::DDS::DataWriterQos & qos,
                                          ::DDS::DataWriterListener_ptr a_listener,
                                          ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("RTI_Publisher_i::create_datawriter");

        RTI_Topic_i * topic = dynamic_cast < RTI_Topic_i * > (a_topic);

        if (!topic)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_Publisher_i::create_datawriter - "
                         "Error: Unable to cast provided topic to its servant.\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
          }

        DDSTopic *rti_topic = topic->get_topic ();

        DDSDataWriter *rti_dw = this->pub_->create_datawriter (rti_topic,
                                                               DDS_DATAWRITER_QOS_DEFAULT,
                                                               0,
                                                               mask);

        if (rti_dw == 0)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_Publisher_i::create_datawriter - "
                         "Error: RTI Topic returned a nil datawriter.\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
          }

        ::DDS::DataWriter_var retval = new RTI_DataWriter_i (rti_dw);

        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::delete_datawriter (::DDS::DataWriter_ptr a_datawriter)
      {
        CIAO_TRACE ("RTI_Publisher_i::delete_datawriter");

        RTI_DataWriter_i *top = dynamic_cast< RTI_DataWriter_i *> (a_datawriter);

        if (top == 0)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_Publisher_i::delete_datawriter - "
                         "Unable to cast provided object reference to servant.\n"));
            return ::DDS::RETCODE_BAD_PARAMETER;
          }

        CIAO_DEBUG ((LM_TRACE, CLINFO "RTI_Publisher_i::delete_datawriter - "
                     "Successfully casted provided object reference to servant.\n"));

        DDS_ReturnCode_t retval = this->pub_->delete_datawriter (top->get_datawriter ());

        if (retval != DDS_RETCODE_OK)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_Publisher_i::delete_datawriter - "
                         "Error: RTI delete_datawriter returned non-ok error code %c\n",
                         translate_retcode (retval)));
          }
        else CIAO_DEBUG ((LM_INFO, CLINFO "RTI_Publisher_i::delete_datawriter - "
                          "Provided datawriter successfully deleted\n"));

        return retval;
      }

      ::DDS::DataWriter_ptr
      RTI_Publisher_i::lookup_datawriter (const char * topic_name)
      {
        DDSDataWriter* dw = this->pub_->lookup_datawriter (topic_name);
        ::DDS::DataWriter_var retval = new RTI_DataWriter_i (dw);
        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::delete_contained_entities (void)
      {
        return this->pub_->delete_contained_entities ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::set_qos (const ::DDS::PublisherQos & qos)
      {
        CIAO_TRACE ("RTI_Publisher_i::set_qos");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::get_qos (::DDS::PublisherQos & qos)
      {
        CIAO_TRACE ("RTI_Publisher_i::get_qos");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::set_listener (::DDS::PublisherListener_ptr a_listener,
                                     ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("RTI_Publisher_i::set_listener");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::PublisherListener_ptr
      RTI_Publisher_i::get_listener (void)
      {
        DDSPublisherListener* pl = this->pub_->get_listener ();
        ::DDS::PublisherListener_var retval = new RTI_PublisherListener_i (pl);
        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::suspend_publications (void)
      {
        return this->pub_->suspend_publications ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::resume_publications (void)
      {
        return this->pub_->resume_publications ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::begin_coherent_changes (void)
      {
        return this->pub_->begin_coherent_changes ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::end_coherent_changes (void)
      {
        return this->pub_->end_coherent_changes ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::wait_for_acknowledgments (const ::DDS::Duration_t & max_wait)
      {
        DDS_Duration_t rti_dds_duration;
        rti_dds_duration <<= max_wait;
        return this->pub_->wait_for_acknowledgments (rti_dds_duration);
      }

      ::DDS::DomainParticipant_ptr
      RTI_Publisher_i::get_participant (void)
      {
        DDSDomainParticipant* p = this->pub_->get_participant ();
        ::DDS::DomainParticipant_var retval = new RTI_DomainParticipant_i (p);
        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::set_default_datawriter_qos (const ::DDS::DataWriterQos & qos)
      {
        CIAO_TRACE ("RTI_Publisher_i::set_default_datawriter_qos");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::get_default_datawriter_qos (::DDS::DataWriterQos & qos)
      {
        CIAO_TRACE ("RTI_Publisher_i::get_default_datawriter_qos");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::copy_from_topic_qos (::DDS::DataWriterQos & a_datawriter_qos,
                                            const ::DDS::TopicQos & a_topic_qos)
      {
        CIAO_TRACE ("RTI_Publisher_i::copy_from_topic_qos");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::enable (void)
      {
        return this->pub_->enable ();
      }

      ::DDS::StatusCondition_ptr
      RTI_Publisher_i::get_statuscondition (void)
      {
        DDSStatusCondition* sc = this->pub_->get_statuscondition ();
        ::DDS::StatusCondition_var retval = new RTI_StatusCondition_i (sc);
        return retval._retn ();
      }

      ::DDS::StatusMask
      RTI_Publisher_i::get_status_changes (void)
      {
        return this->pub_->get_status_changes ();
      }

      ::DDS::InstanceHandle_t
      RTI_Publisher_i::get_instance_handle (void)
      {
        CIAO_TRACE ("RTI_Publisher_i::get_instance_handle");
        throw CORBA::NO_IMPLEMENT ();
      }

      DDSPublisher *
      RTI_Publisher_i::get_publisher (void)
      {
        return this->pub_;
      }
    }
  }
}

