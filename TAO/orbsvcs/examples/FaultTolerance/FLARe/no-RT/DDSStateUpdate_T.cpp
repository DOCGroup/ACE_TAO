// -*- C++ -*-

//=============================================================================
/**
 *  @file    DDSStateUpdate_T.cpp
 *
 *  $Id$
 *
 * @author   Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef _DDS_STATE_UPDATE_T_CPP_
#define _DDS_STATE_UPDATE_T_CPP_

#include "ccpp_dds_dcps.h"

#include "DDSStateReaderListener_T.h"

template <typename STATE_TYPE,
	  typename TOPIC_TYPE, 
	  typename TOPIC_TYPE_SUPPORT,
	  typename TOPIC_DATA_WRITER,
          typename TOPIC_DATA_READER,
          typename TOPIC_SEQUENCE>
DDSStateUpdate_T<STATE_TYPE,
                 TOPIC_TYPE,
		 TOPIC_TYPE_SUPPORT,
		 TOPIC_DATA_WRITER,
		 TOPIC_DATA_READER,
                 TOPIC_SEQUENCE>::DDSStateUpdate_T (
    const std::string & topic_name,
    const std::string & id,
    DDS::DomainParticipant_ptr participant,
    DDS::Publisher_ptr publisher,
    DDS::Subscriber_ptr subscriber,
    ReplicatedApplication_ptr application)
  : topic_name_ (topic_name),
    id_ (id),
    participant_ (DDS::DomainParticipant::_duplicate (participant)),
    pub_ (DDS::Publisher::_duplicate (publisher)),
    sub_ (DDS::Subscriber::_duplicate (subscriber)),
    topic_ (DDS::Topic::_nil ()),
    datawriter_ (TOPIC_DATA_WRITER::_nil ()),
    datareader_ (TOPIC_DATA_READER::_nil ()),
    listener_ (new DDSStateReaderListener_T <TOPIC_TYPE,
	                                     TOPIC_DATA_READER,
                                             TOPIC_SEQUENCE,
	                                     STATE_TYPE> (id, application))    
{
  state_.id = id.c_str ();
  this->init ();
}

template <typename STATE_TYPE,
	  typename TOPIC_TYPE, 
	  typename TOPIC_TYPE_SUPPORT,
	  typename TOPIC_DATA_WRITER,
          typename TOPIC_DATA_READER,
          typename TOPIC_SEQUENCE>
DDSStateUpdate_T<STATE_TYPE,
                 TOPIC_TYPE,
		 TOPIC_TYPE_SUPPORT,
		 TOPIC_DATA_WRITER,
		 TOPIC_DATA_READER,
                 TOPIC_SEQUENCE>::~DDSStateUpdate_T ()
{
  this->fini ();
}

template <typename STATE_TYPE,
	  typename TOPIC_TYPE, 
	  typename TOPIC_TYPE_SUPPORT,
	  typename TOPIC_DATA_WRITER,
          typename TOPIC_DATA_READER,
          typename TOPIC_SEQUENCE>
bool
DDSStateUpdate_T<STATE_TYPE,
                 TOPIC_TYPE,
		 TOPIC_TYPE_SUPPORT,
		 TOPIC_DATA_WRITER,
		 TOPIC_DATA_READER,
                 TOPIC_SEQUENCE>::init ()
{
  if (!this->create_topic ())
    throw (DDSFailure ("Could not create topic\n"));

  if (!this->create_datawriter ())
    throw (DDSFailure ("Could not create datawriter\n"));

  if (!this->create_datareader ())
    throw (DDSFailure ("Could not create datareader\n"));
 
  return true;
}

template <typename STATE_TYPE,
	  typename TOPIC_TYPE, 
	  typename TOPIC_TYPE_SUPPORT,
	  typename TOPIC_DATA_WRITER,
          typename TOPIC_DATA_READER,
          typename TOPIC_SEQUENCE>
bool
DDSStateUpdate_T<STATE_TYPE,
                 TOPIC_TYPE,
		 TOPIC_TYPE_SUPPORT,
		 TOPIC_DATA_WRITER,
		 TOPIC_DATA_READER,
                 TOPIC_SEQUENCE>::fini ()
{
  DDS::ReturnCode_t status =
    this->pub_->delete_datawriter (this->datawriter_.in ());

  if (status != DDS::RETCODE_OK)
    {
      return false;
    }

  status = this->participant_->delete_topic (this->topic_.in ());

  if (status != DDS::RETCODE_OK)
    {
      return false;
    }
    
  return true;
}

template <typename STATE_TYPE,
	  typename TOPIC_TYPE, 
	  typename TOPIC_TYPE_SUPPORT,
	  typename TOPIC_DATA_WRITER,
          typename TOPIC_DATA_READER,
          typename TOPIC_SEQUENCE>
void
DDSStateUpdate_T<STATE_TYPE,
                 TOPIC_TYPE,
		 TOPIC_TYPE_SUPPORT,
		 TOPIC_DATA_WRITER,
		 TOPIC_DATA_READER,
                 TOPIC_SEQUENCE>::set_state (
  const CORBA::Any & state_value)
{
  // extract value from any
  STATE_TYPE value;

  // update value on state topic sample
  if (state_value >>= value)
    state_.value = value;

  ACE_DEBUG ((LM_TRACE, ACE_TEXT ("DDSStateUpdate_T::set_state writes sample " 
				  "with id %s\n"), state_.id.in ()));
  
  // publish value
  DDS::ReturnCode_t ret =
    this->datawriter_->write (this->state_, this->instance_handle_);
          
  if (ret != DDS::RETCODE_OK)
    {
      ACE_DEBUG ((
          LM_ERROR,
          ACE_TEXT ("StateUpdate::publish_update: write returned %d.\n"),
          ret));
    }
}

template <typename STATE_TYPE,
	  typename TOPIC_TYPE, 
	  typename TOPIC_TYPE_SUPPORT,
	  typename TOPIC_DATA_WRITER,
          typename TOPIC_DATA_READER,
          typename TOPIC_SEQUENCE>
bool
DDSStateUpdate_T<STATE_TYPE,
                 TOPIC_TYPE,
		 TOPIC_TYPE_SUPPORT,
		 TOPIC_DATA_WRITER,
		 TOPIC_DATA_READER,
                 TOPIC_SEQUENCE>::create_topic ()
{
  DDS::TypeSupport_var ts = new TOPIC_TYPE_SUPPORT ();
  CORBA::String_var data_type_name = ts->get_type_name ();
  
  DDS::ReturnCode_t status = ts->register_type (this->participant_.in (),
						data_type_name.in ());
                                                   
  if (status != DDS::RETCODE_OK)
    {
      return false;
    }

  DDS::TopicQos default_topic_qos;
  status = this->participant_->get_default_topic_qos (default_topic_qos);

  if (status != DDS::RETCODE_OK)
    {
      return false;
    }

  this->topic_ =
    this->participant_->create_topic (topic_name_.c_str (),
                                      data_type_name.in (),
                                      default_topic_qos,
                                      DDS::TopicListener::_nil (),
                                      DDS::ANY_STATUS);
                               
  if (CORBA::is_nil (this->topic_.in ()))
    {
      return false;
    }

  return true;
}

template <typename STATE_TYPE,
	  typename TOPIC_TYPE, 
	  typename TOPIC_TYPE_SUPPORT,
	  typename TOPIC_DATA_WRITER,
          typename TOPIC_DATA_READER,
          typename TOPIC_SEQUENCE>
bool
DDSStateUpdate_T<STATE_TYPE,
                 TOPIC_TYPE,
		 TOPIC_TYPE_SUPPORT,
		 TOPIC_DATA_WRITER,
		 TOPIC_DATA_READER,
                 TOPIC_SEQUENCE>::create_datawriter ()
{
  DDS::DataWriterQos dw_qos;
  DDS::ReturnCode_t status =
    pub_->get_default_datawriter_qos (dw_qos);

  // state synchronization should be reliable
  dw_qos.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;

  if (status != DDS::RETCODE_OK)
    {
      return false;
    }

  DDS::DataWriter_var datawriter_base = 
    pub_->create_datawriter (this->topic_.in (),
			     dw_qos,
			     DDS::DataWriterListener::_nil (),
			     DDS::ANY_STATUS);
                       
  if (CORBA::is_nil (datawriter_base.in ()))
    {
      return false;
    }
  
  datawriter_ = TOPIC_DATA_WRITER::_narrow (datawriter_base.in ());
  
  if (CORBA::is_nil (datawriter_.in ()))
    {
      return false;
    }
    
  // Since there is no key, we need only one instance handle for all writes.  
  instance_handle_ =
    datawriter_->register_instance (state_);

  return true;
}

template <typename STATE_TYPE,
	  typename TOPIC_TYPE, 
	  typename TOPIC_TYPE_SUPPORT,
	  typename TOPIC_DATA_WRITER,
          typename TOPIC_DATA_READER,
          typename TOPIC_SEQUENCE>
bool
DDSStateUpdate_T<STATE_TYPE,
                 TOPIC_TYPE,
		 TOPIC_TYPE_SUPPORT,
		 TOPIC_DATA_WRITER,
		 TOPIC_DATA_READER,
                 TOPIC_SEQUENCE>::create_datareader ()
{
  DDS::DataReaderQos dr_qos;
  DDS::ReturnCode_t status =
    sub_->get_default_datareader_qos (dr_qos);

  // state synchronization should be reliable
  dr_qos.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;

  if (status != DDS::RETCODE_OK)
    {
      return false;
    }

  DDS::DataReader_var datareader_base = 
    sub_->create_datareader (topic_.in (),
			     dr_qos,
			     listener_.in (),
			     DDS::ANY_STATUS);
                       
  if (CORBA::is_nil (datareader_base.in ()))
    {
      return false;
    }
  
  datareader_ = TOPIC_DATA_READER::_narrow (datareader_base.in ());
  
  if (CORBA::is_nil (datareader_.in ()))
    {
      return false;
    }

  ACE_DEBUG ((LM_TRACE, ACE_TEXT ("created DR for %s\n"), topic_name_.c_str ()));
    
  return true;
}

#endif /* _DDS_STATE_UPDATE_T_CPP_ */
