// $Id$

#include "Publisher_T.h"
#include "PublisherListener_T.h"
#include "Topic_T.h"
#include "DataWriter_T.h"
#include "StatusCondition.h"
#include "Utils.h"
#include "DDSDataWriterListener_T.h"

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
    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::CCM_DDS_Publisher_T (DDSPublisher * dw)
      : impl_ (dw)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::CCM_DDS_Publisher_T");
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::~CCM_DDS_Publisher_T (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::~CCM_DDS_Publisher_T");
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::DataWriter_ptr
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::create_datawriter (::DDS::Topic_ptr a_topic,
                                        const ::DDS::DataWriterQos & qos,
                                        ::DDS::DataWriterListener_ptr a_listener,
                                        ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::create_datawriter");

#if (CIAO_DDS4CCM_NDDS==1)
      ACE_UNUSED_ARG (qos);

      Topic_type * topic = dynamic_cast < Topic_type * > (a_topic);

      if (!topic)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::create_datawriter - "
                       "Error: Unable to cast provided topic to its servant.\n"));
          throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
        }

      DDSDataWriterListener *ccm_dds_drl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_drl,
                            DataWriterListener_type (a_listener, 0),
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
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Publisher_T"
                        "<DDS_TYPE, CCM_TYPE>::create_datawriter - "
                        "Error: RTI Topic returned a nil datawriter.\n"));
          delete ccm_dds_drl;
          throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }

      ::DDS::DataWriter_var retval = ::DDS::DataWriter::_nil ();
      ACE_NEW_THROW_EX (retval,
                        DataWriter_type (ccm_dds_dw),
                        CORBA::NO_MEMORY ());

      ccm_dds_dw->enable ();
      return retval._retn ();
#else
      return this->impl ()->create_datawriter (a_topic,
                                               qos,
                                               a_listener,
                                               mask);
#endif
    }

#if (CIAO_DDS4CCM_NDDS==1)
    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::DataWriter_ptr
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::create_datawriter_with_profile (::DDS::Topic_ptr a_topic,
                                        const char* library_name,
                                        const char *profile_name,
                                        ::DDS::DataWriterListener_ptr a_listener,
                                        ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::create_datawriter");

      Topic_type * topic = dynamic_cast < Topic_type * > (a_topic);

      if (!topic)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Publisher_T"
                        "<DDS_TYPE, CCM_TYPE>::create_datawriter_with_profile - "
                        "Error: Unable to cast provided topic to its servant.\n"));
          throw CCM_DDS::InternalError (::DDS::RETCODE_BAD_PARAMETER, 0);
        }

      DDSDataWriterListener *ccm_dds_drl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_drl,
                            DataWriterListener_type (a_listener, 0),
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
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Publisher_T"
                        "<DDS_TYPE, CCM_TYPE>::create_datawriter_with_profile - "
                        "Error: RTI Topic returned a nil datawriter.\n"));
          delete ccm_dds_drl;
          throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }
      else
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO "CCM_DDS_Publisher_T"
                        "<DDS_TYPE, CCM_TYPE>::create_datareader_with_profile - "
                        "Successfully created datawriter with profile <%C#%C>.\n",
                        library_name,
                        profile_name));
        }

      ::DDS::DataWriter_var retval = ::DDS::DataWriter::_nil ();
      ACE_NEW_THROW_EX (retval,
                        DataWriter_type (ccm_dds_dw),
                        CORBA::NO_MEMORY ());
      ccm_dds_dw->enable ();

      return retval._retn ();
    }
#endif

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::delete_datawriter (::DDS::DataWriter_ptr a_datawriter)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::delete_datawriter");

#if (CIAO_DDS4CCM_NDDS==1)
      DataWriter_type *top = dynamic_cast< DataWriter_type * > (a_datawriter);

      if (top == 0)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Publisher_T"
                        "<DDS_TYPE, CCM_TYPE>::delete_datawriter - "
                        "Unable to cast provided object reference to servant.\n"));
          return ::DDS::RETCODE_BAD_PARAMETER;
        }

      DDS4CCM_DEBUG (9, (LM_TRACE, CLINFO "CCM_DDS_Publisher_T"
                    "<DDS_TYPE, CCM_TYPE>::delete_datawriter - "
                    "Successfully casted provided object reference to servant.\n"));

      DDS_ReturnCode_t const retval = this->impl ()->delete_datawriter (top->get_impl ());

      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "CCM_DDS_Publisher_T"
                        "<DDS_TYPE, CCM_TYPE>::delete_datawriter - "
                        "Error: RTI delete_datawriter returned non-ok error code %C\n",
                        translate_retcode (retval)));
        }
      else
        {
          DDS4CCM_DEBUG (6, (LM_INFO, CLINFO "CCM_DDS_Publisher_T"
                        "<DDS_TYPE, CCM_TYPE>::delete_datawriter - "
                        "Provided datawriter successfully deleted\n"));
        }

      return retval;
#else
      return this->impl ()->delete_datawriter (a_datawriter);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::DataWriter_ptr
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::lookup_datawriter (const char * impl_name)
    {
      ::DDS::DataWriter_var retval = ::DDS::DataWriter::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSDataWriter* dw = this->impl ()->lookup_datawriter (impl_name);
      ACE_NEW_THROW_EX (retval,
                        DataWriter_type (dw),
                        CORBA::NO_MEMORY ());
#else
      ::DDS::DataWriter_var dw = this->impl ()->lookup_datawriter (impl_name);
      ACE_NEW_THROW_EX (retval,
                        DataWriter_type (dw.in ()),
                        CORBA::NO_MEMORY ());
#endif
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::delete_contained_entities (void)
    {
      return this->impl ()->delete_contained_entities ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::set_qos (const ::DDS::PublisherQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::set_qos");
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_PublisherQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->impl ()->set_qos (ccm_dds_qos);
#else
      return this->impl ()->set_qos (qos);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::get_qos (::DDS::PublisherQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::get_qos");
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

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::set_listener (::DDS::PublisherListener_ptr a_listener,
                                   ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::set_listener");

#if (CIAO_DDS4CCM_NDDS==1)
      PublisherListener_type * ccm_dds_impl_list  = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_impl_list,
                            PublisherListener_type (a_listener),
                            CORBA::NO_MEMORY ());
        }
      return this->impl ()->set_listener (ccm_dds_impl_list, mask);
#else
      return this->impl ()->set_listener (a_listener, mask);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::PublisherListener_ptr
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::get_listener (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::get_listener");

#if (CIAO_DDS4CCM_NDDS==1)
      DDSPublisherListener *ccm_dds_pub_list = this->impl ()->get_listener ();
      PublisherListener_type * list_proxy =
        dynamic_cast <PublisherListener_type *> (ccm_dds_pub_list);
      if (!list_proxy)
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO "CCM_DDS_Publisher_T"
                                    "<DDS_TYPE, CCM_TYPE>::get_listener - "
                                    "DDS returned a NIL listener.\n"));
          return ::DDS::PublisherListener::_nil ();
        }
      return list_proxy->get_publisher_listener ();
#else
      return this->impl ()->get_listener ();
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::suspend_publications (void)
    {
      return this->impl ()->suspend_publications ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::resume_publications (void)
    {
      return this->impl ()->resume_publications ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::begin_coherent_changes (void)
    {
      return this->impl ()->begin_coherent_changes ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::end_coherent_changes (void)
    {
      return this->impl ()->end_coherent_changes ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::wait_for_acknowledgments (
      const ::DDS::Duration_t & max_wait)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_Duration_t ccm_dds_dds_duration;
      ccm_dds_dds_duration <<= max_wait;
      return this->impl ()->wait_for_acknowledgments (ccm_dds_dds_duration);
#else
      return this->impl ()->wait_for_acknowledgments (max_wait);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::DomainParticipant_ptr
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::get_participant (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::get_participant");
      ::DDS::DomainParticipant_var retval = ::DDS::DomainParticipant::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSDomainParticipant* p = this->impl ()->get_participant ();
      ACE_NEW_THROW_EX (retval,
                        DomainParticipant_type (p),
                        CORBA::NO_MEMORY ());
#else
      ::DDS::DomainParticipant_var p = this->impl ()->get_participant ();
      ACE_NEW_THROW_EX (retval,
                        DomainParticipant_type (p.in ()),
                        CORBA::NO_MEMORY ());
#endif
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::set_default_datawriter_qos (const ::DDS::DataWriterQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::set_default_datawriter_qos");
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDS_DataWriterQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->impl()->set_default_datawriter_qos (ccm_dds_qos);
#else
      return this->impl()->set_default_datawriter_qos (qos);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::get_default_datawriter_qos (::DDS::DataWriterQos & qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::get_default_datawriter_qos");
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

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::copy_from_topic_qos (::DDS::DataWriterQos & a_dataimpl_qos,
                                              const ::DDS::TopicQos & a_impl_qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::copy_from_topic_qos");
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

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::enable (void)
    {
      return this->impl ()->enable ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::StatusCondition_ptr
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::get_statuscondition (void)
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

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::StatusMask
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::get_status_changes (void)
    {
      return this->impl ()->get_status_changes ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::InstanceHandle_t
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::get_instance_handle (void)
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

    template <typename DDS_TYPE, typename CCM_TYPE>
    DDSPublisher *
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::get_impl (void)
    {
      return this->impl_;
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::set_impl (DDSPublisher * dw)
    {
      this->impl_ = dw;
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    DDSPublisher *
    CCM_DDS_Publisher_T<DDS_TYPE, CCM_TYPE>::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}

