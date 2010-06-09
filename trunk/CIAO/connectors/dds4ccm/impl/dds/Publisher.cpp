// $Id$

#include "Publisher.h"
#include "PublisherListener.h"
#include "Topic.h"
#include "DataWriter.h"
#include "StatusCondition.h"
#include "Utils.h"
#include "DataWriterListener.h"

#include "ndds/InstanceHandle_t.h"
#include "ndds/Duration_t.h"
#include "ndds/DataWriterQos.h"
#include "ndds/PublisherQos.h"
#include "ndds/TopicQos.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/dds/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    CCM_DDS_Publisher_i::CCM_DDS_Publisher_i (DDSPublisher * dw)
      : impl_ (dw)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_i::CCM_DDS_Publisher_i");
    }

    CCM_DDS_Publisher_i::~CCM_DDS_Publisher_i (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_i::~CCM_DDS_Publisher_i");
    }

    ::DDS::DataWriter_ptr
    CCM_DDS_Publisher_i::create_datawriter (::DDS::Topic_ptr a_topic,
                                        const ::DDS::DataWriterQos & qos,
                                        ::DDS::DataWriterListener_ptr a_listener,
                                        ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_i::create_datawriter");

#if (CIAO_DDS4CCM_NDDS==1)
      ACE_UNUSED_ARG (qos);

      CCM_DDS_Topic_i * topic = dynamic_cast < CCM_DDS_Topic_i * > (a_topic);

      if (!topic)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Publisher_i::create_datawriter - "
                       "Error: Unable to cast provided topic to its servant.\n"));
          throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
        }

      DDSDataWriterListener *ccm_dds_drl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_drl,
                            CCM_DDS_DataWriterListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }
      DDS_DataWriterQos ccm_dds_qos = DDS_DATAWRITER_QOS_DEFAULT;
      DDSDataWriter *ccm_dds_dw = this->impl ()->create_datawriter (
                                                            topic->get_impl (),
                                                            ccm_dds_qos,
                                                            ccm_dds_drl,
                                                            mask);

      if (!ccm_dds_dw)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Publisher_i::create_datawriter - "
                       "Error: RTI Topic returned a nil datawriter.\n"));
          delete ccm_dds_drl;
          throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }

      ::DDS::DataWriter_var retval = ::DDS::DataWriter::_nil ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_DataWriter_i (ccm_dds_dw),
                        CORBA::NO_MEMORY ());

      ccm_dds_dw->enable ();
      return retval._retn ();
#else
      return this->impl ()->create_datawriter (
                                                            a_topic,
                                                            qos,
                                                            a_listener,
                                                            mask);
#endif
    }

#if (CIAO_DDS4CCM_NDDS==1)
    ::DDS::DataWriter_ptr
    CCM_DDS_Publisher_i::create_datawriter_with_profile (::DDS::Topic_ptr a_topic,
                                        const char* library_name,
                                        const char *profile_name,
                                        ::DDS::DataWriterListener_ptr a_listener,
                                        ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_i::create_datawriter");

      CCM_DDS_Topic_i * topic = dynamic_cast < CCM_DDS_Topic_i * > (a_topic);

      if (!topic)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Publisher_i::create_datawriter_with_profile - "
                       "Error: Unable to cast provided topic to its servant.\n"));
          throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
        }

      DDSDataWriterListener *ccm_dds_drl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_drl,
                            CCM_DDS_DataWriterListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }
      DDSDataWriter *ccm_dds_dw = this->impl ()->create_datawriter_with_profile (
                                                              topic->get_impl (),
                                                              library_name,
                                                              profile_name,
                                                              ccm_dds_drl,
                                                              mask);

      if (!ccm_dds_dw)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Publisher_i::create_datawriter_with_profile - "
                       "Error: RTI Topic returned a nil datawriter.\n"));
          delete ccm_dds_drl;
          throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }
      else
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO "CCM_DDS_Publisher_i::create_datareader_with_profile - "
                       "Successfully created datawriter with profile <%C#%C>.\n",
                       library_name,
                       profile_name));
        }

      ::DDS::DataWriter_var retval = ::DDS::DataWriter::_nil ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_DataWriter_i (ccm_dds_dw),
                        CORBA::NO_MEMORY ());
      ccm_dds_dw->enable ();

      return retval._retn ();
    }
#endif

    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_i::delete_datawriter (::DDS::DataWriter_ptr a_datawriter)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_i::delete_datawriter");

#if (CIAO_DDS4CCM_NDDS==1)
      CCM_DDS_DataWriter_i *top = dynamic_cast< CCM_DDS_DataWriter_i *> (a_datawriter);

      if (top == 0)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Publisher_i::delete_datawriter - "
                       "Unable to cast provided object reference to servant.\n"));
          return ::DDS::RETCODE_BAD_PARAMETER;
        }

      DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_Publisher_i::delete_datawriter - "
                   "Successfully casted provided object reference to servant.\n"));

      DDS_ReturnCode_t const retval = this->impl ()->delete_datawriter (top->get_impl ());

      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Publisher_i::delete_datawriter - "
                       "Error: RTI delete_datawriter returned non-ok error code %C\n",
                       translate_retcode (retval)));
        }
      else
        {
          DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "CCM_DDS_Publisher_i::delete_datawriter - "
                        "Provided datawriter successfully deleted\n"));
        }

      return retval;
#else
      return this->impl ()->delete_datawriter (a_datawriter);
#endif
    }

    ::DDS::DataWriter_ptr
    CCM_DDS_Publisher_i::lookup_datawriter (const char * impl_name)
    {
      ::DDS::DataWriter_var retval = ::DDS::DataWriter::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSDataWriter* dw = this->impl ()->lookup_datawriter (impl_name);
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_DataWriter_i (dw),
                        CORBA::NO_MEMORY ());
#else
      ::DDS::DataWriter_var dw = this->impl ()->lookup_datawriter (impl_name);
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_DataWriter_i (dw.in ()),
                        CORBA::NO_MEMORY ());
#endif
      return retval._retn ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_i::delete_contained_entities (void)
    {
      return this->impl ()->delete_contained_entities ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_i::set_qos (const ::DDS::PublisherQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_i::set_qos");
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_PublisherQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->impl ()->set_qos (ccm_dds_qos);
#else
      return this->impl ()->set_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_i::get_qos (::DDS::PublisherQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_i::get_qos");
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_PublisherQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      ::DDS::ReturnCode_t retcode = this->impl()->
              get_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
#else
      return this->impl ()->get_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_i::set_listener (::DDS::PublisherListener_ptr a_listener,
                                   ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_i::set_listener");

#if (CIAO_DDS4CCM_NDDS==1)
      CCM_DDS_PublisherListener_i *ccm_dds_impl_list  = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_impl_list,
                            CCM_DDS_PublisherListener_i (a_listener),
                            CORBA::NO_MEMORY ());
        }
      return this->impl ()->set_listener (ccm_dds_impl_list, mask);
#else
      return this->impl ()->set_listener (a_listener, mask);
#endif
    }

    ::DDS::PublisherListener_ptr
    CCM_DDS_Publisher_i::get_listener (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_i::get_listener");

#if (CIAO_DDS4CCM_NDDS==1)
      DDSPublisherListener *ccm_dds_pub_list = this->impl ()->get_listener ();
      CCM_DDS_PublisherListener_i *list_proxy = dynamic_cast <CCM_DDS_PublisherListener_i *> (ccm_dds_pub_list);
      if (!list_proxy)
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, "CCM_DDS_Publisher_i::get_listener - "
                                    "DDS returned a NIL listener.\n"));
          return ::DDS::PublisherListener::_nil ();
        }
      return list_proxy->get_publisher_listener ();
#else
      return this->impl ()->get_listener ();
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_i::suspend_publications (void)
    {
      return this->impl ()->suspend_publications ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_i::resume_publications (void)
    {
      return this->impl ()->resume_publications ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_i::begin_coherent_changes (void)
    {
      return this->impl ()->begin_coherent_changes ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_i::end_coherent_changes (void)
    {
      return this->impl ()->end_coherent_changes ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_i::wait_for_acknowledgments (const ::DDS::Duration_t & max_wait)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_Duration_t ccm_dds_dds_duration;
      ccm_dds_dds_duration <<= max_wait;
      return this->impl ()->wait_for_acknowledgments (ccm_dds_dds_duration);
#else
      return this->impl ()->wait_for_acknowledgments (max_wait);
#endif
    }

    ::DDS::DomainParticipant_ptr
    CCM_DDS_Publisher_i::get_participant (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_i::get_participant");
      ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSDomainParticipant* p = this->impl ()->get_participant ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_DomainParticipant_i (p),
                        CORBA::NO_MEMORY ());
#else
      ::DDS::DomainParticipant_var p = this->impl ()->get_participant ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_DomainParticipant_i (p.in ()),
                        CORBA::NO_MEMORY ());
#endif
      return retval._retn ();
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_i::set_default_datawriter_qos (const ::DDS::DataWriterQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_i::set_default_datawriter_qos");
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_DataWriterQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->impl()->set_default_datawriter_qos (ccm_dds_qos);
#else
      return this->impl()->set_default_datawriter_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_i::get_default_datawriter_qos (::DDS::DataWriterQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_i::get_default_datawriter_qos");
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_DataWriterQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode =
            this->impl()->get_default_datawriter_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
#else
      return this->impl()->get_default_datawriter_qos (qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_i::copy_from_topic_qos (::DDS::DataWriterQos & a_dataimpl_qos,
                                              const ::DDS::TopicQos & a_impl_qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_i::copy_from_topic_qos");
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_DataWriterQos ccm_dds_qos;
      ::DDS_TopicQos ccm_dds_topic_qos;

      ccm_dds_qos <<= a_dataimpl_qos;
      ccm_dds_topic_qos <<= a_impl_qos;
      ::DDS::ReturnCode_t const retcode =
          this->impl()->copy_from_topic_qos (ccm_dds_qos, ccm_dds_topic_qos);
      a_dataimpl_qos <<= ccm_dds_qos;
      return retcode;
#else
      return this->impl()->copy_from_topic_qos (a_dataimpl_qos, a_impl_qos);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_i::enable (void)
    {
      return this->impl ()->enable ();
    }

    ::DDS::StatusCondition_ptr
    CCM_DDS_Publisher_i::get_statuscondition (void)
    {
      ::DDS::StatusCondition_var retval = ::DDS::StatusCondition::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSStatusCondition* sc = this->impl ()->get_statuscondition ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_StatusCondition_i (sc),
                        CORBA::NO_MEMORY ());
#else
      ::DDS::StatusCondition_var sc = this->impl ()->get_statuscondition ();
      ACE_NEW_THROW_EX (retval,
                        CCM_DDS_StatusCondition_i (sc.in ()),
                        CORBA::NO_MEMORY ());
#endif
      return retval._retn ();
    }

    ::DDS::StatusMask
    CCM_DDS_Publisher_i::get_status_changes (void)
    {
      return this->impl ()->get_status_changes ();
    }

    ::DDS::InstanceHandle_t
    CCM_DDS_Publisher_i::get_instance_handle (void)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_InstanceHandle_t const rtihandle = this->impl ()->get_instance_handle ();
      ::DDS::InstanceHandle_t handle;
      handle <<= rtihandle;
      return handle;
#else
      return this->impl ()->get_instance_handle ();
#endif
    }

    DDSPublisher *
    CCM_DDS_Publisher_i::get_impl (void)
    {
      return this->impl_;
    }

    void
    CCM_DDS_Publisher_i::set_impl (DDSPublisher * dw)
    {
      this->impl_ = dw;
    }

    DDSPublisher *
    CCM_DDS_Publisher_i::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}

