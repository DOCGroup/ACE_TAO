// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/dds/DataReaderListener_T.h"
#include "dds4ccm/impl/dds/DataWriterListener_T.h"
#include "dds4ccm/impl/dds/Writer_T.h"
#include "dds4ccm/impl/dds/Getter_T.h"
#include "dds4ccm/impl/dds/Reader_T.h"
#include "dds4ccm/impl/dds/DataReader.h"
#include "dds4ccm/impl/dds/Topic.h"
#include "dds4ccm/impl/dds/DataListenerControl_T.h"
#include "dds4ccm/impl/dds/PortStatusListener_T.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::DDS_Listen_T (void) :
  data_control_ (new CCM_DDS_DataListenerControl_T
    < ::CCM_DDS::CCM_DataListenerControl> ()),
  ccm_dds_reader_ (0)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::~DDS_Listen_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete (
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char* library_name,
  const char* profile_name)
{
  DDS4CCM_TRACE ("DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete");

  try
    {
      if (!this->ccm_dds_reader_.get_impl ())
        {
          ::DDS::DataReader_var reader;
          if (library_name && profile_name)
            {
              reader = subscriber->create_datareader_with_profile (
                    topic,
                    library_name,
                    profile_name,
                    ::DDS::DataReaderListener::_nil (),
                    0);
            }
          else
            {
              ::DDS::DataReaderQos drqos;
              reader = subscriber->create_datareader (
                    topic,
                    drqos,
                    ::DDS::DataReaderListener::_nil (),
                    0);
            }
          ::CIAO::DDS4CCM::CCM_DDS_DataReader_i *rd =
            dynamic_cast < ::CIAO::DDS4CCM::CCM_DDS_DataReader_i *> (reader.in ());
          this->ccm_dds_reader_.set_impl (rd->get_impl ());
          this->dds_read_.set_impl (&this->ccm_dds_reader_);
          this->dds_read_.set_contentfilteredtopic_data (library_name, profile_name);
        }
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Listen_T::configuration_complete: Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::activate (
  typename CCM_TYPE::listener_type::_ptr_type listener,
  ::CCM_DDS::PortStatusListener_ptr status,
  ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::activate");
  try
    {
      if (::CORBA::is_nil (this->data_listener_.in ()))
        {
          ACE_NEW_THROW_EX (this->data_listener_,
                            DataReaderListener (
                              listener,
                              status,
                              this->data_control_.in (),
                              reactor),
                            CORBA::NO_MEMORY ());
        }
      this->ccm_dds_reader_.set_listener (
        this->data_listener_.in (),
        ::CIAO::DDS4CCM::DataReaderListener_T<DDS_TYPE, CCM_TYPE>::get_mask (
          listener));
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Listen_T::activate: Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::passivate ()
{
  DDS4CCM_TRACE ("DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::passivate");
  try
    {
      this->ccm_dds_reader_.set_listener (
                ::DDS::DataReaderListener::_nil (),
                0);
      this->data_listener_= ::DDS::DataReaderListener::_nil ();
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Listen_T::passivate: Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::remove (
  ::DDS::Subscriber_ptr subscriber)
{
  DDS4CCM_TRACE ("DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::remove");
  try
    {
      DDS::ReturnCode_t retval =
        subscriber->delete_datareader (&this->ccm_dds_reader_);
      if (retval != DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
            "DDS_Listen_T::remove - "
            "Unable to delete DataReader: <%C>\n",
            ::CIAO::DDS4CCM::translate_retcode (retval)));
          throw CORBA::INTERNAL ();
        }
      this->ccm_dds_reader_.set_impl (0);
      this->dds_read_.set_impl (0);
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Listen_T::remove: Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}


template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::get_data (void)
{
  DDS4CCM_TRACE ("DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::get_data");

  return &this->dds_read_;
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataReader_ptr
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::get_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::get_dds_entity");

  return &this->ccm_dds_reader_;
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::CCM_DataListenerControl_ptr
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::get_data_control (void)
{
  DDS4CCM_TRACE ("DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::get_data_control");

  return ::CCM_DDS::CCM_DataListenerControl::_duplicate (this->data_control_.in ());
}

