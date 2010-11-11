// $Id$

#include "dds4ccm/impl/Utils.h"

#include "dds4ccm/impl/ndds/Publisher.h"
#include "dds4ccm/impl/ndds/PublisherListener.h"
#include "dds4ccm/impl/ndds/DomainParticipant.h"
#include "dds4ccm/impl/ndds/Topic.h"
#include "dds4ccm/impl/ndds/DataWriter_T.h"
#include "dds4ccm/impl/ndds/StatusCondition.h"
#include "dds4ccm/impl/ndds/DataWriterListener.h"
#include "dds4ccm/impl/ndds/TypeSupport.h"

#include "dds4ccm/impl/ndds/convertors/InstanceHandle_t.h"
#include "dds4ccm/impl/ndds/convertors/Duration_t.h"
#include "dds4ccm/impl/ndds/convertors/DataWriterQos.h"
#include "dds4ccm/impl/ndds/convertors/PublisherQos.h"
#include "dds4ccm/impl/ndds/convertors/TopicQos.h"

#include "dds4ccm/impl/logger/Log_Macros.h"
#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace NDDS
  {
    DDS_Publisher_i::DDS_Publisher_i (::DDSPublisher * p,
                                      ::DDS::DomainParticipant_ptr dp)
      : rti_entity_ (p),
        dp_ (::DDS::DomainParticipant::_duplicate (dp))
    {
      DDS4CCM_TRACE ("DDS_Publisher_i::DDS_Publisher_i");
    }

    DDS_Publisher_i::~DDS_Publisher_i (void)
    {
      DDS4CCM_TRACE ("DDS_Publisher_i::~DDS_Publisher_i");
    }

    ::DDS::DataWriter_ptr
    DDS_Publisher_i::create_datawriter (
      ::DDS::Topic_ptr a_topic,
      const ::DDS::DataWriterQos &,
      ::DDS::DataWriterListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_Publisher_i::create_datawriter");

      DDS_Topic_i * topic = dynamic_cast < DDS_Topic_i * > (a_topic);

      if (!topic)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Publisher_i::create_datawriter - "
                        "Error: Unable to cast provided topic to its servant.\n"));
          return ::DDS::DataWriter::_nil ();
        }

      DDS_DataWriterListener_i *ccm_dds_drl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_drl,
                            DDS_DataWriterListener_i (a_listener, 0),
                            ::CORBA::NO_MEMORY ());
        }
      DDS_DataWriterQos ccm_dds_qos = DDS_DATAWRITER_QOS_DEFAULT;
      DDSDataWriter *ccm_dds_dw = this->rti_entity ()->create_datawriter (
                                                            topic->get_rti_entity (),
                                                            ccm_dds_qos,
                                                            ccm_dds_drl,
                                                            mask);

      if (!ccm_dds_dw)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Publisher_i::create_datawriter - "
                        "Error: RTI Topic returned a nil datawriter.\n"));
          delete ccm_dds_drl;
          return ::DDS::DataWriter::_nil ();
        }

     ::DDS::DataWriter_var retval = DDS_TypeSupport_i::create_datawriter (ccm_dds_dw, this->dp_.in ());
     if (ccm_dds_drl)
       {
         ccm_dds_drl->set_dds_entity (retval.in ());
       }
      ccm_dds_dw->enable ();
      return retval._retn ();
    }


    ::DDS::DataWriter_ptr
    DDS_Publisher_i::create_datawriter_with_profile (::DDS::Topic_ptr a_topic,
                                        const char* library_name,
                                        const char *profile_name,
                                        ::DDS::DataWriterListener_ptr a_listener,
                                        ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_Publisher_i::create_datawriter_with_profile");

      DDS_Topic_i * topic = dynamic_cast < DDS_Topic_i * > (a_topic);

      if (!topic)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Publisher_i::create_datawriter_with_profile - "
                        "Error: Unable to cast provided topic to its servant.\n"));
          return ::DDS::DataWriter::_nil ();
        }

      DDS_DataWriterListener_i *ccm_dds_drl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_drl,
                            DDS_DataWriterListener_i (a_listener, 0),
                            ::CORBA::NO_MEMORY ());
        }
      DDSDataWriter *ccm_dds_dw = this->rti_entity ()->create_datawriter_with_profile (
                                                              topic->get_rti_entity (),
                                                              library_name,
                                                              profile_name,
                                                              ccm_dds_drl,
                                                              mask);

      if (!ccm_dds_dw)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Publisher_i::create_datawriter_with_profile - "
                        "Error: RTI Topic returned a nil datawriter.\n"));
          delete ccm_dds_drl;
          return ::DDS::DataWriter::_nil ();
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_Publisher_i::create_datawriter_with_profile - "
                        "Successfully created datawriter with profile <%C#%C>.\n",
                        library_name,
                        profile_name));
        }

     ::DDS::DataWriter_var retval = DDS_TypeSupport_i::create_datawriter (ccm_dds_dw, this->dp_.in ());
     if (ccm_dds_drl)
       {
         ccm_dds_drl->set_dds_entity (retval.in ());
       }
      ccm_dds_dw->enable ();
      return retval._retn ();
    }


    ::DDS::ReturnCode_t
    DDS_Publisher_i::delete_datawriter (::DDS::DataWriter_ptr a_datawriter)
    {
      DDS4CCM_TRACE ("DDS_Publisher_i::delete_datawriter");

      DDS_DataWriter_Base *top = dynamic_cast< DDS_DataWriter_Base * > (a_datawriter);

      if (!top)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Publisher_i::delete_datawriter - "
                        "Unable to cast provided object reference to servant.\n"));
          return ::DDS::RETCODE_BAD_PARAMETER;
        }

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_CAST_SUCCESSFUL, (LM_TRACE, DDS4CCM_INFO
                    "DDS_Publisher_i::delete_datawriter - "
                    "Successfully casted provided object reference to servant.\n"));

      DDS_ReturnCode_t const retval = this->rti_entity ()->delete_datawriter (top->get_rti_entity ());

      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Publisher_i::delete_datawriter - "
                        "Error: RTI delete_datawriter returned non-ok error code %C\n",
                        ::CIAO::DDS4CCM::translate_retcode (retval)));
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                        "DDS_Publisher_i::delete_datawriter - "
                        "Provided datawriter successfully deleted\n"));
        }

      return retval;
    }


    ::DDS::DataWriter_ptr
    DDS_Publisher_i::lookup_datawriter (const char * impl_name)
    {
      DDSDataWriter* dw = this->rti_entity ()->lookup_datawriter (impl_name);
     ::DDS::DataWriter_var retval = DDS_TypeSupport_i::create_datawriter (dw, this->dp_.in ());
      return retval._retn ();
    }


    ::DDS::ReturnCode_t
    DDS_Publisher_i::delete_contained_entities (void)
    {
      return this->rti_entity ()->delete_contained_entities ();
    }


    ::DDS::ReturnCode_t
    DDS_Publisher_i::set_qos (const ::DDS::PublisherQos & qos)
    {
      DDS4CCM_TRACE ("DDS_Publisher_i::set_qos");
      ::DDS_PublisherQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->rti_entity ()->set_qos (ccm_dds_qos);
    }


    ::DDS::ReturnCode_t
    DDS_Publisher_i::get_qos (::DDS::PublisherQos & qos)
    {
      DDS4CCM_TRACE ("DDS_Publisher_i::get_qos");
      ::DDS_PublisherQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      ::DDS::ReturnCode_t retcode = this->rti_entity ()->
              get_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }


    ::DDS::ReturnCode_t
    DDS_Publisher_i::set_listener (
      ::DDS::PublisherListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("DDS_Publisher_i::set_listener");

      // Delete the previously set listener
      DDSPublisherListener *listener = this->rti_entity ()->get_listener ();
      delete listener;

      DDS_PublisherListener_i * ccm_dds_impl_list  = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_impl_list,
                            DDS_PublisherListener_i (a_listener, this->dp_.in ()),
                            ::CORBA::NO_MEMORY ());
        }
      return this->rti_entity ()->set_listener (ccm_dds_impl_list, mask);
    }


    ::DDS::PublisherListener_ptr
    DDS_Publisher_i::get_listener (void)
    {
      DDS4CCM_TRACE ("DDS_Publisher_i::get_listener");

      DDSPublisherListener *ccm_dds_pub_list = this->rti_entity ()->get_listener ();
      DDS_PublisherListener_i * list_proxy =
        dynamic_cast <DDS_PublisherListener_i *> (ccm_dds_pub_list);
      if (!list_proxy)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_Publisher_i::get_listener - "
                        "DDS returned a NIL listener.\n"));
          return ::DDS::PublisherListener::_nil ();
        }
      return list_proxy->get_publisher_listener ();
    }


    ::DDS::ReturnCode_t
    DDS_Publisher_i::suspend_publications (void)
    {
      return this->rti_entity ()->suspend_publications ();
    }


    ::DDS::ReturnCode_t
    DDS_Publisher_i::resume_publications (void)
    {
      return this->rti_entity ()->resume_publications ();
    }


    ::DDS::ReturnCode_t
    DDS_Publisher_i::begin_coherent_changes (void)
    {
      return this->rti_entity ()->begin_coherent_changes ();
    }


    ::DDS::ReturnCode_t
    DDS_Publisher_i::end_coherent_changes (void)
    {
      return this->rti_entity ()->end_coherent_changes ();
    }


    ::DDS::ReturnCode_t
    DDS_Publisher_i::wait_for_acknowledgments (
      const ::DDS::Duration_t & max_wait)
    {
      DDS_Duration_t ccm_dds_dds_duration;
      ccm_dds_dds_duration <<= max_wait;
      return this->rti_entity ()->wait_for_acknowledgments (ccm_dds_dds_duration);
    }


    ::DDS::DomainParticipant_ptr
    DDS_Publisher_i::get_participant (void)
    {
      DDS4CCM_TRACE ("DDS_Publisher_i::get_participant");
      return ::DDS::DomainParticipant::_duplicate (this->dp_.in ());
    }


    ::DDS::ReturnCode_t
    DDS_Publisher_i::set_default_datawriter_qos (const ::DDS::DataWriterQos & qos)
    {
      DDS4CCM_TRACE ("DDS_Publisher_i::set_default_datawriter_qos");
      ::DDS_DataWriterQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->rti_entity ()->set_default_datawriter_qos (ccm_dds_qos);
    }


    ::DDS::ReturnCode_t
    DDS_Publisher_i::get_default_datawriter_qos (::DDS::DataWriterQos & qos)
    {
      DDS4CCM_TRACE ("DDS_Publisher_i::get_default_datawriter_qos");
      ::DDS_DataWriterQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode =
            this->rti_entity ()->get_default_datawriter_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }


    ::DDS::ReturnCode_t
    DDS_Publisher_i::copy_from_topic_qos (::DDS::DataWriterQos & a_dataimpl_qos,
                                              const ::DDS::TopicQos & a_impl_qos)
    {
      DDS4CCM_TRACE ("DDS_Publisher_i::copy_from_topic_qos");
      ::DDS_DataWriterQos ccm_dds_qos;
      ::DDS_TopicQos ccm_dds_topic_qos;

      ccm_dds_qos <<= a_dataimpl_qos;
      ccm_dds_topic_qos <<= a_impl_qos;
      ::DDS::ReturnCode_t const retcode =
          this->rti_entity ()->copy_from_topic_qos (ccm_dds_qos, ccm_dds_topic_qos);
      a_dataimpl_qos <<= ccm_dds_qos;
      return retcode;
    }


    ::DDS::ReturnCode_t
    DDS_Publisher_i::enable (void)
    {
      return this->rti_entity ()->enable ();
    }


    ::DDS::StatusCondition_ptr
    DDS_Publisher_i::get_statuscondition (void)
    {
      ::DDS::StatusCondition_var retval;
      DDSStatusCondition* sc = this->rti_entity ()->get_statuscondition ();
      ACE_NEW_THROW_EX (retval,
                        DDS_StatusCondition_i (sc, 0), // @todo
                        ::CORBA::NO_MEMORY ());
      return retval._retn ();
    }


    ::DDS::StatusMask
    DDS_Publisher_i::get_status_changes (void)
    {
      return this->rti_entity ()->get_status_changes ();
    }


    ::DDS::InstanceHandle_t
    DDS_Publisher_i::get_instance_handle (void)
    {
      ::DDS_InstanceHandle_t const rtihandle = this->rti_entity ()->get_instance_handle ();
      ::DDS::InstanceHandle_t handle;
      handle <<= rtihandle;
      return handle;
    }


    DDSPublisher *
    DDS_Publisher_i::get_rti_entity (void)
    {
      return this->rti_entity_;
    }

    DDSPublisher *
    DDS_Publisher_i::rti_entity (void)
    {
      if (!this->rti_entity_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->rti_entity_;
    }
  }
}

