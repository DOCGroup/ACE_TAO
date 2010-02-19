// $Id$

#include "Publisher.h"
#include "PublisherListener.h"
#include "Topic.h"
#include "DataWriter.h"
#include "StatusCondition.h"
#include "Utils.h"
#include "Duration_t.h"
#include "InstanceHandle_t.h"
#include "DataWriterListener.h"
#include "DataWriterQos.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      RTI_Publisher_i::RTI_Publisher_i (void)
        : impl_ (0)
      {
        CIAO_TRACE ("RTI_Publisher_i::RTI_Publisher_i");
      }

      RTI_Publisher_i::~RTI_Publisher_i (void)
      {
        CIAO_TRACE ("RTI_Publisher_i::~RTI_Publisher_i");
      }

      ::DDS::DataWriter_ptr
      RTI_Publisher_i::create_datawriter (::DDS::Topic_ptr a_topic,
                                          const ::DDS::DataWriterQos & /*qos*/,
                                          ::DDS::DataWriterListener_ptr a_listener,
                                          ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("RTI_Publisher_i::create_datawriter");

        RTI_Topic_i * topic = dynamic_cast < RTI_Topic_i * > (a_topic);

        if (!topic)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_Publisher_i::create_datawriter - "
                         "Error: Unable to cast provided topic to its servant.\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
          }

        ::DDS::DataWriter_var retval = ::DDS::DataWriter::_nil ();
        ACE_NEW_THROW_EX (retval,
                          RTI_DataWriter_i (),
                          CORBA::NO_MEMORY ());

        DDSDataWriterListener *rti_drl = 0;
        if (!CORBA::is_nil (a_listener))
          {
            ACE_NEW_THROW_EX (rti_drl,
                              RTI_DataWriterListener_i (a_listener),
                              CORBA::NO_MEMORY ());
          }
        DDS_DataWriterQos rti_qos = DDS_DATAWRITER_QOS_DEFAULT;
        DDSDataWriter *rti_dw = this->impl ()->create_datawriter (
                                                              topic->get_impl (),
                                                              rti_qos,
                                                              rti_drl,
                                                              mask);

        if (!rti_dw)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_Publisher_i::create_datawriter - "
                         "Error: RTI Topic returned a nil datawriter.\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
          }

        rti_dw->enable ();
        RTI_DataWriter_i *dw = dynamic_cast< RTI_DataWriter_i * > (retval.in ());
        dw->set_impl (rti_dw);

        return retval._retn ();
      }

      ::DDS::DataWriter_ptr
      RTI_Publisher_i::create_datawriter_with_profile (::DDS::Topic_ptr a_topic,
                                          const char* library_name,
                                          const char *profile_name,
                                          ::DDS::DataWriterListener_ptr a_listener,
                                          ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("RTI_Publisher_i::create_datawriter");

        RTI_Topic_i * topic = dynamic_cast < RTI_Topic_i * > (a_topic);

        if (!topic)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_Publisher_i::create_datawriter_with_profile - "
                         "Error: Unable to cast provided topic to its servant.\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
          }

        ::DDS::DataWriter_var retval = ::DDS::DataWriter::_nil ();
        ACE_NEW_THROW_EX (retval,
                          RTI_DataWriter_i (),
                          CORBA::NO_MEMORY ());

        DDSDataWriterListener *rti_drl = 0;
        if (!CORBA::is_nil (a_listener))
          {
            ACE_NEW_THROW_EX (rti_drl,
                              RTI_DataWriterListener_i (a_listener),
                              CORBA::NO_MEMORY ());
          }
        DDSDataWriter *rti_dw = this->impl ()->create_datawriter_with_profile (
                                                                topic->get_impl (),
                                                                library_name,
                                                                profile_name,
                                                                rti_drl,
                                                                mask);

        if (!rti_dw)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_Publisher_i::create_datawriter_with_profile - "
                         "Error: RTI Topic returned a nil datawriter.\n"));
            throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
          }

        rti_dw->enable ();
        RTI_DataWriter_i *dw = dynamic_cast< RTI_DataWriter_i * > (retval.in ());
        dw->set_impl (rti_dw);

        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::delete_datawriter (::DDS::DataWriter_ptr a_datawriter)
      {
        CIAO_TRACE ("RTI_Publisher_i::delete_datawriter");

        RTI_DataWriter_i *top = dynamic_cast< RTI_DataWriter_i *> (a_datawriter);

        if (top == 0)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_Publisher_i::delete_datawriter - "
                         "Unable to cast provided object reference to servant.\n"));
            return ::DDS::RETCODE_BAD_PARAMETER;
          }

        CIAO_DEBUG (9, (LM_TRACE, CLINFO "RTI_Publisher_i::delete_datawriter - "
                     "Successfully casted provided object reference to servant.\n"));

        DDS_ReturnCode_t const retval = this->impl ()->delete_datawriter (top->get_impl ());

        if (retval != DDS_RETCODE_OK)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO "RTI_Publisher_i::delete_datawriter - "
                         "Error: RTI delete_datawriter returned non-ok error code %C\n",
                         translate_retcode (retval)));
          }
        else CIAO_DEBUG (6, (LM_INFO, CLINFO "RTI_Publisher_i::delete_datawriter - "
                          "Provided datawriter successfully deleted\n"));

        return retval;
      }

      ::DDS::DataWriter_ptr
      RTI_Publisher_i::lookup_datawriter (const char * impl_name)
      {
        ::DDS::DataWriter_var retval = ::DDS::DataWriter::_nil ();
        ACE_NEW_THROW_EX (retval,
                          RTI_DataWriter_i (),
                          CORBA::NO_MEMORY ());
        DDSDataWriter* dw = this->impl ()->lookup_datawriter (impl_name);
        RTI_DataWriter_i *rti_dw = dynamic_cast< RTI_DataWriter_i * > (retval.in ());
        rti_dw->set_impl (dw);
        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::delete_contained_entities (void)
      {
        return this->impl ()->delete_contained_entities ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::set_qos (const ::DDS::PublisherQos & /*qos*/)
      {
        CIAO_TRACE ("RTI_Publisher_i::set_qos");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::get_qos (::DDS::PublisherQos & /* qos*/)
      {
        CIAO_TRACE ("RTI_Publisher_i::get_qos");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::set_listener (::DDS::PublisherListener_ptr a_listener,
                                     ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("RTI_Publisher_i::set_listener");

        RTI_PublisherListener_i *rti_impl_list  = 0;
       if (!CORBA::is_nil (a_listener))
          {
            ACE_NEW_THROW_EX (rti_impl_list,
                              RTI_PublisherListener_i (a_listener),
                              CORBA::NO_MEMORY ());
          }
        return this->impl ()->set_listener (rti_impl_list, mask);
      }

      ::DDS::PublisherListener_ptr
      RTI_Publisher_i::get_listener (void)
      {
        CIAO_TRACE ("RTI_Publisher_i::get_listener");

        DDSPublisherListener *rti_pub_list = this->impl ()->get_listener ();
        RTI_PublisherListener_i *list_proxy = dynamic_cast <RTI_PublisherListener_i *> (rti_pub_list);
        if (!list_proxy)
          {
            CIAO_DEBUG (6, (LM_DEBUG, "RTI_Publisher_i::get_listener - "
                                      "DDS returned a NIL listener.\n"));
            return ::DDS::PublisherListener::_nil ();
          }
        return list_proxy->get_publisher_listener ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::suspend_publications (void)
      {
        return this->impl ()->suspend_publications ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::resume_publications (void)
      {
        return this->impl ()->resume_publications ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::begin_coherent_changes (void)
      {
        return this->impl ()->begin_coherent_changes ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::end_coherent_changes (void)
      {
        return this->impl ()->end_coherent_changes ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::wait_for_acknowledgments (const ::DDS::Duration_t & max_wait)
      {
        DDS_Duration_t rti_dds_duration;
        rti_dds_duration <<= max_wait;
        return this->impl ()->wait_for_acknowledgments (rti_dds_duration);
      }

      ::DDS::DomainParticipant_ptr
      RTI_Publisher_i::get_participant (void)
      {
        CIAO_TRACE ("RTI_Publisher_i::get_participant");
        ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
        ACE_NEW_THROW_EX (retval,
                          RTI_DomainParticipant_i (),
                          CORBA::NO_MEMORY ());

        DDSDomainParticipant* p = this->impl ()->get_participant ();
        RTI_DomainParticipant_i *rti_dp = dynamic_cast < RTI_DomainParticipant_i *> (retval.in ());
        rti_dp->set_impl (p);
        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::set_default_datawriter_qos (const ::DDS::DataWriterQos & /*qos*/)
      {
        CIAO_TRACE ("RTI_Publisher_i::set_default_datawriter_qos");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::get_default_datawriter_qos (::DDS::DataWriterQos & /*qos*/)
      {
        CIAO_TRACE ("RTI_Publisher_i::get_default_datawriter_qos");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::copy_from_topic_qos (::DDS::DataWriterQos & /*a_dataimpl_qos*/,
                                            const ::DDS::TopicQos & /*a_impl_qos*/)
      {
        CIAO_TRACE ("RTI_Publisher_i::copy_from_topic_qos");
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_Publisher_i::enable (void)
      {
        return this->impl ()->enable ();
      }

      ::DDS::StatusCondition_ptr
      RTI_Publisher_i::get_statuscondition (void)
      {
        ::DDS::StatusCondition_var retval = ::DDS::StatusCondition::_nil ();
        ACE_NEW_THROW_EX (retval,
                          RTI_StatusCondition_i (),
                          CORBA::NO_MEMORY ());

        DDSStatusCondition* sc = this->impl ()->get_statuscondition ();
        RTI_StatusCondition_i *rti_sc = dynamic_cast < RTI_StatusCondition_i *> (retval.in ());
        rti_sc->set_impl (sc);
        return retval._retn ();
      }

      ::DDS::StatusMask
      RTI_Publisher_i::get_status_changes (void)
      {
        return this->impl ()->get_status_changes ();
      }

      ::DDS::InstanceHandle_t
      RTI_Publisher_i::get_instance_handle (void)
      {
        ::DDS_InstanceHandle_t const rtihandle = this->impl ()->get_instance_handle ();
        ::DDS::InstanceHandle_t handle;
        handle <<= rtihandle;
        return handle;
      }

      DDSPublisher *
      RTI_Publisher_i::get_impl (void)
      {
        return this->impl_;
      }

      void
      RTI_Publisher_i::set_impl (DDSPublisher * dw)
      {
        this->impl_ = dw;
      }

      DDSPublisher *
      RTI_Publisher_i::impl (void)
      {
        if (!this->impl_)
          {
            throw ::CORBA::BAD_INV_ORDER ();
          }
        return this->impl_;
      }

    }
  }
}

