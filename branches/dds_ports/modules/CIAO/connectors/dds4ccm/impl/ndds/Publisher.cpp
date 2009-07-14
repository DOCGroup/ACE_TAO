// $Id$

#include "Publisher.h"
#include "Topic.h"
#include "DataWriter.h"
#include "Utils.h"

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
        
        if (topic == 0)
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
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::DataWriter_ptr 
      RTI_Publisher_i::lookup_datawriter (const char * topic_name)
      {
        CIAO_TRACE ("RTI_Publisher_i::lookup_datawriter");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t 
      RTI_Publisher_i::delete_contained_entities (void)
      {
        CIAO_TRACE ("RTI_Publisher_i::delete_contained_entities");
        throw CORBA::NO_IMPLEMENT ();
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
        CIAO_TRACE ("RTI_Publisher_i::get_listener");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t 
      RTI_Publisher_i::suspend_publications (void)
      {
        CIAO_TRACE ("RTI_Publisher_i::suspend_publications");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t 
      RTI_Publisher_i::resume_publications (void)
      {
        CIAO_TRACE ("RTI_Publisher_i::resume_publications");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t 
      RTI_Publisher_i::begin_coherent_changes (void)
      {
        CIAO_TRACE ("RTI_Publisher_i::begin_coherent_changes");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t 
      RTI_Publisher_i::end_coherent_changes (void)
      {
        CIAO_TRACE ("RTI_Publisher_i::end_coherent_changes");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t 
      RTI_Publisher_i::wait_for_acknowledgments (const ::DDS::Duration_t & max_wait)
      {
        CIAO_TRACE ("RTI_Publisher_i::wait_for_acknowledgments");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::DomainParticipant_ptr 
      RTI_Publisher_i::get_participant (void)
      {
        CIAO_TRACE ("RTI_Publisher_i::get_participant");
        throw CORBA::NO_IMPLEMENT ();
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
        CIAO_TRACE ("RTI_Publisher_i::enable");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::StatusCondition_ptr 
      RTI_Publisher_i::get_statuscondition (void)
      {
        CIAO_TRACE ("RTI_Publisher_i::get_statuscondition");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::StatusMask 
      RTI_Publisher_i::get_status_changes (void)
      {
        CIAO_TRACE ("RTI_Publisher_i::get_status_changes");
        throw CORBA::NO_IMPLEMENT ();
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

