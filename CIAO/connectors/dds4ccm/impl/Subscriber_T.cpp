// $Id$

#include "Subscriber_T.h"
#include "SubscriberListener_T.h"
#include "Topic_T.h"
#include "ContentFilteredTopic_T.h"
#include "DataReader_T.h"
#include "DDSDataReaderListener_T.h"
#include "Utils.h"
#include "StatusCondition.h"

#include "ndds/InstanceHandle_t.h"
#include "ndds/DataReaderQos.h"
#include "ndds/SubscriberQos.h"
#include "ndds/TopicQos.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::CCM_DDS_Subscriber_T (DDSSubscriber * sub)
      : impl_ (sub)
    {
      DDS4CCM_TRACE ("CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::CCM_DDS_Subscriber_T");
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~CCM_DDS_Subscriber_T (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~CCM_DDS_Subscriber_T");
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::enable (void)
    {
      return this->impl ()->enable ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::StatusCondition_ptr
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_statuscondition (void)
    {
      ::DDS::StatusCondition_var retval;
      DDSStatusCondition* sc = this->impl ()->get_statuscondition ();
      if (sc)
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_StatusCondition_i (sc),
                            ::CORBA::NO_MEMORY ());
        }
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::StatusMask
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_status_changes (void)
    {
      return this->impl ()->get_status_changes ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::InstanceHandle_t
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_instance_handle (void)
    {
      ::DDS_InstanceHandle_t const rtihandle = this->impl ()->get_instance_handle ();
      ::DDS::InstanceHandle_t handle;
      handle <<= rtihandle;
      return handle;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    DDSDataReader *
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_datareader (
                  DDSContentFilteredTopic * topic,
                  DDSDataReaderListener * ccm_dds_drl,
                  ::DDS::StatusMask mask,
                   const ::DDS::DataReaderQos & qos)
    {
      ACE_UNUSED_ARG (qos);
      DDS_DataReaderQos ccm_dds_qos = DDS_DATAREADER_QOS_DEFAULT;
      return this->impl ()->create_datareader (topic,
                                               ccm_dds_qos,
                                               ccm_dds_drl,
                                               mask);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    DDSDataReader *
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_datareader (
                  DDSTopic * topic,
                  DDSDataReaderListener * ccm_dds_drl,
                  ::DDS::StatusMask mask,
                   const ::DDS::DataReaderQos & qos)
    {
      ACE_UNUSED_ARG (qos);
      DDS_DataReaderQos ccm_dds_qos = DDS_DATAREADER_QOS_DEFAULT;
      return this->impl ()->create_datareader (topic,
                                               ccm_dds_qos,
                                               ccm_dds_drl,
                                               mask);
    }

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    DDSDataReader *
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_datareader_with_profile (
                  DDSContentFilteredTopic * topic,
                  const char * library_name,
                  const char * profile_name,
                  DDSDataReaderListener * ccm_dds_drl,
                  ::DDS::StatusMask mask)
    {
      return this->impl ()->create_datareader_with_profile (topic,
                                                            library_name,
                                                            profile_name,
                                                            ccm_dds_drl,
                                                            mask);
    }
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    DDSDataReader *
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_datareader_with_profile (
                  DDSTopic * topic,
                  const char * library_name,
                  const char * profile_name,
                  DDSDataReaderListener * ccm_dds_drl,
                  ::DDS::StatusMask mask)
    {
      return this->impl ()->create_datareader_with_profile (topic,
                                                            library_name,
                                                            profile_name,
                                                            ccm_dds_drl,
                                                            mask);
    }
#endif

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::DataReader_ptr
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_datareader (
      ::DDS::TopicDescription_ptr a_topic,
      const ::DDS::DataReaderQos & qos,
      ::DDS::DataReaderListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_datareader");
      ::DDS::DataReader_var retval;
      DDSDataReaderListener *ccm_dds_drl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_drl,
                            DataReaderListener_type (a_listener, 0),
                            ::CORBA::NO_MEMORY ());
        }

      DDSDataReader * ccm_dds_dr = 0;
      Topic_type* topic = dynamic_cast < Topic_type * > (a_topic);

      if (!topic)
        {
          ContentFilteredTopic_type * cf_topic =
            dynamic_cast < ContentFilteredTopic_type * > (a_topic);
          if (!cf_topic)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, CLINFO
                          "CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, "
                          "VENDOR_TYPE>::create_datareader - Error: Unable to "
                          "cast provided topic to one of its servant.\n"));
              delete ccm_dds_drl;
              return ::DDS::DataReader::_nil ();
            }
          else
            ccm_dds_dr = this->create_datareader (cf_topic->get_impl (),
                                                  ccm_dds_drl,
                                                  mask,
                                                  qos);
        }
      else
        ccm_dds_dr = this->create_datareader (topic->get_impl (),
                                              ccm_dds_drl,
                                              mask,
                                              qos);

      if (!ccm_dds_dr)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, CLINFO "CCM_DDS_Subscriber_T"
                       "<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_datareader - "
                       "Error: RTI Topic returned a nil datareader.\n"));
          delete ccm_dds_drl;
          return ::DDS::DataReader::_nil ();
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, CLINFO
                        "CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::create_datareader - "
                        "Successfully created datareader.\n"));
        }

      ccm_dds_dr->enable ();
      ACE_NEW_THROW_EX (retval,
                        DataReader_type (ccm_dds_dr),
                        ::CORBA::NO_MEMORY ());
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::DataReader_ptr
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_datareader_with_profile (
      ::DDS::TopicDescription_ptr a_topic,
      const char * library_name,
      const char * profile_name,
      ::DDS::DataReaderListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::create_datareader_with_profile");
      DDSDataReaderListener *ccm_dds_drl = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_drl,
                            DataReaderListener_type (a_listener, 0),
                            ::CORBA::NO_MEMORY ());
        }

      DDSDataReader * ccm_dds_dr = 0;
      Topic_type * topic = dynamic_cast < Topic_type * > (a_topic);

      if (!topic)
        {
          ContentFilteredTopic_type * cf_topic =
            dynamic_cast < ContentFilteredTopic_type * > (a_topic);
          if (!cf_topic)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, CLINFO
                            "CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, "
                            "VENDOR_TYPE>::create_datareader_with_profile - "
                            "Error: Unable to cast provided topic to one "
                            "of its servant.\n"));
              delete ccm_dds_drl;
              return ::DDS::DataReader::_nil ();
            }
          else
            {
              ccm_dds_dr = this->create_datareader_with_profile (cf_topic->get_impl (),
                                                           library_name,
                                                           profile_name,
                                                           ccm_dds_drl,
                                                           mask);
            }
        }
      else
        {
          ccm_dds_dr = this->create_datareader_with_profile (topic->get_impl (),
                                                             library_name,
                                                             profile_name,
                                                             ccm_dds_drl,
                                                             mask);
        }

      if (!ccm_dds_dr)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, CLINFO
                        "CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::create_datareader_with_profile - "
                        "Error: RTI Topic returned a nil datareader.\n"));
          delete ccm_dds_drl;
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, CLINFO
                        "CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::create_datareader_with_profile - "
                        "Successfully created datareader with profile <%C#%C>.\n",
                        library_name,
                        profile_name));
        }

      ::DDS::DataReader_var retval;
      ACE_NEW_THROW_EX (retval,
                        DataReader_type (ccm_dds_dr),
                        ::CORBA::NO_MEMORY ());

      ccm_dds_dr->enable ();

      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::delete_datareader (
      ::DDS::DataReader_ptr a_datareader)
    {
      DataReader_type *dr = dynamic_cast< DataReader_type *> (a_datareader);
      if (!dr)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_ERROR, CLINFO
                        "CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::delete_datareader -Unable to cast "
                        "provided object reference to servant.\n"));
          return ::DDS::RETCODE_BAD_PARAMETER;
        }

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_CAST_SUCCESSFUL, (LM_TRACE, CLINFO
                    "CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, "
                    "VENDOR_TYPE>::delete_datareader - "
                    "Successfully casted provided object reference "
                    "to servant.\n"));

      DDS_ReturnCode_t const retval = this->impl ()->delete_datareader (dr->get_impl ());

      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                        "CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::delete_datareader - "
                        "Error: Returned non-ok error code %C\n",
                        translate_retcode (retval)));
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, CLINFO
                        "CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::delete_datareader - "
                        "Datareader successfully  deleted\n"));
        }

      return retval;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::delete_contained_entities (void)
    {
      return this->impl ()->delete_contained_entities ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::DataReader_ptr
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::lookup_datareader (
      const char * impl_name)
    {
      ::DDS::DataReader_var retval;
      DDSDataReader* dr = this->impl ()->lookup_datareader (impl_name);
      if (dr)
        {
          ACE_NEW_THROW_EX (retval,
                            DataReader_type (dr),
                            ::CORBA::NO_MEMORY ());
        }
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_datareaders (
      ::DDS::DataReaderSeq & readers,
      ::DDS::SampleStateMask sample_states,
      ::DDS::ViewStateMask view_states,
      ::DDS::InstanceStateMask instance_states)
    {
      CIAO_TRACE ("CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_qos");
      ::DDSDataReaderSeq dds_readers;

      ::DDS_ReturnCode_t retcode =
        this->impl ()->get_datareaders (dds_readers,
                                        sample_states,
                                        view_states,
                                        instance_states);
      if (retcode == DDS_RETCODE_OK)
        {
          readers.length (dds_readers.length ());
          for (::DDS_Long i = 0; i < dds_readers.length (); ++i)
            {
              ::DDS::DataReader_var rdr;
              ACE_NEW_THROW_EX (rdr,
                                DataReader_type (dds_readers[i]),
                                ::CORBA::NO_MEMORY ());
              readers [i] = rdr._retn ();
            }
        }
      return retcode;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::notify_datareaders (void)
    {
      return this->impl ()->notify_datareaders ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_qos (
      const ::DDS::SubscriberQos & qos)
    {
      CIAO_TRACE ("CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_qos");
      ::DDS_SubscriberQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->impl ()->get_qos (ccm_dds_qos);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_qos (
      ::DDS::SubscriberQos & qos)
    {
      CIAO_TRACE ("CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_qos");
      ::DDS_SubscriberQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode = this->impl ()->get_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_listener (
      ::DDS::SubscriberListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_listener");

      // Delete the previously set listener
      DDSSubscriberListener *listener = this->impl ()->get_listener ();
      delete listener;

      SubscriberListener_type * ccm_dds_impl_list = 0;
      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (ccm_dds_impl_list,
                            SubscriberListener_type (a_listener),
                            ::CORBA::NO_MEMORY ());
        }
      return this->impl ()->set_listener (ccm_dds_impl_list, mask);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::SubscriberListener_ptr
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_listener (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_listener");

      DDSSubscriberListener *ccm_dds_impl_list = this->impl ()->get_listener ();
      SubscriberListener_type *list_proxy =
        dynamic_cast <SubscriberListener_type *> (ccm_dds_impl_list);
      if (!list_proxy)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, CLINFO
                        "CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, "
                        "VENDOR_TYPE>::get_listener - "
                        "DDS returned a NIL listener.\n"));
          return ::DDS::SubscriberListener::_nil ();
        }
      return list_proxy->get_subscriber_listener ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::begin_access (void)
    {
      return this->impl ()->begin_access ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::end_access (void)
    {
      return this->impl ()->end_access ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::DomainParticipant_ptr
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_participant (void)
    {
      ::DDS::DomainParticipant_var retval;
      DDSDomainParticipant* p = this->impl ()->get_participant ();
      if (p)
        {
          ACE_NEW_THROW_EX (retval,
                            DomainParticipant_type (p),
                            ::CORBA::NO_MEMORY ());
        }
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_default_datareader_qos (
      const ::DDS::DataReaderQos & qos)
    {
      CIAO_TRACE ("CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_default_datareader_qos");
      ::DDS_DataReaderQos ccm_dds_qos;
      ccm_dds_qos <<= qos;
      return this->impl ()->set_default_datareader_qos (ccm_dds_qos);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_default_datareader_qos (
      ::DDS::DataReaderQos & qos)
    {
      CIAO_TRACE ("CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_default_datareader_qos");
      ::DDS_DataReaderQos ccm_dds_qos;
      ::DDS::ReturnCode_t retcode = this->impl ()->get_default_datareader_qos (ccm_dds_qos);
      qos <<= ccm_dds_qos;
      return retcode;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::copy_from_topic_qos (
      ::DDS::DataReaderQos & a_datareader_qos,
      const ::DDS::TopicQos & a_impl_qos)
    {
      DDS4CCM_TRACE ("CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::copy_from_topic_qos");
      ::DDS_DataReaderQos ccm_dds_qos;
      ::DDS_TopicQos ccm_dds_topic_qos;

      ccm_dds_qos <<= a_datareader_qos;
      ccm_dds_topic_qos <<= a_impl_qos;
      ::DDS::ReturnCode_t retcode =
          this->impl()->copy_from_topic_qos (ccm_dds_qos,
                                             ccm_dds_topic_qos);
      a_datareader_qos <<= ccm_dds_qos;
      return retcode;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    DDSSubscriber *
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_impl (void)
    {
      return this->impl_;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    void
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_impl (DDSSubscriber * sub)
    {
      this->impl_ = sub;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    DDSSubscriber *
    CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}

