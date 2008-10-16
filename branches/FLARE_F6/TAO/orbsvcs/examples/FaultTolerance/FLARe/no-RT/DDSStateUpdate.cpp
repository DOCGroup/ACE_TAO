// -*- C++ -*-

//=============================================================================
/**
 *  @file    DDSStateUpdate.cpp
 *
 *  $Id$
 *
 * @author   Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef _DDS_STATE_UPDATE_CPP_
#define _DDS_STATE_UPDATE_CPP_

#include "ccpp_dds_dcps.h"
#include "DDSStateUpdate.h"

template <class TOPIC_TYPE, 
	  class TOPIC_TYPE_SUPPORT,
	  class TOPIC_DATA_WRITER,
	  class STATE_TYPE>
DDSStateUpdate<TOPIC_TYPE,
	       TOPIC_TYPE_SUPPORT,
	       TOPIC_DATA_WRITER,
	       STATE_TYPE>::DDSStateUpdate (
    const std::string & topic_name,
    const std::string & id,
    DDS::DomainParticipant_ptr participant,
    DDS::Publisher_ptr publisher)
  : topic_name_ (topic_name),
    id_ (id),
    participant_ (DDS::DomainParticipant::_duplicate (participant)),
    pub_ (DDS::Publisher::_duplicate (publisher)),
    topic_ (DDS::Topic::_nil ()),
    datawriter_ (TOPIC_DATA_WRITER::_nil ())
{
  state_.id = id.c_str ();
}

template <class TOPIC_TYPE,
	  class TOPIC_TYPE_SUPPORT,
	  class TOPIC_DATA_WRITER,
	  class STATE_TYPE>
DDSStateUpdate<TOPIC_TYPE,
	       TOPIC_TYPE_SUPPORT,
	       TOPIC_DATA_WRITER,
	       STATE_TYPE>::~DDSStateUpdate ()
{
}

template <class TOPIC_TYPE,
	  class TOPIC_TYPE_SUPPORT,
	  class TOPIC_DATA_WRITER,
	  class STATE_TYPE>
bool
DDSStateUpdate<TOPIC_TYPE,
	       TOPIC_TYPE_SUPPORT,
	       TOPIC_DATA_WRITER,
	       STATE_TYPE>::init ()
{
  return this->create_topic ()
    && this->create_datawriter ();
}

template <class TOPIC_TYPE,
	  class TOPIC_TYPE_SUPPORT,
	  class TOPIC_DATA_WRITER,
	  class STATE_TYPE>
bool
DDSStateUpdate<TOPIC_TYPE,
	       TOPIC_TYPE_SUPPORT,
	       TOPIC_DATA_WRITER,
	       STATE_TYPE>::fini ()
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

template <class TOPIC_TYPE,
	  class TOPIC_TYPE_SUPPORT,
	  class TOPIC_DATA_WRITER,
	  class STATE_TYPE>
void
DDSStateUpdate<TOPIC_TYPE,
	       TOPIC_TYPE_SUPPORT,
	       TOPIC_DATA_WRITER,
	       STATE_TYPE>::set_state (
  const CORBA::Any & state_value)
{
  // extract value from any
  STATE_TYPE value;

  // update value on state topic sample
  if (state_value >>= value)
    state_.value = value;

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

template <class TOPIC_TYPE,
	  class TOPIC_TYPE_SUPPORT,
	  class TOPIC_DATA_WRITER,
	  class STATE_TYPE>
bool
DDSStateUpdate<TOPIC_TYPE,
	       TOPIC_TYPE_SUPPORT,
	       TOPIC_DATA_WRITER,
	       STATE_TYPE>::create_topic ()
{
  DDS::TypeSupport_var svnt = new TOPIC_TYPE_SUPPORT;
  CORBA::String_var data_type_name = svnt->get_type_name ();
  
  DDS::ReturnCode_t status = svnt->register_type (this->participant_.in (),
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

template <class TOPIC_TYPE,
	  class TOPIC_TYPE_SUPPORT,
	  class TOPIC_DATA_WRITER,
	  class STATE_TYPE>
bool
DDSStateUpdate<TOPIC_TYPE,
	       TOPIC_TYPE_SUPPORT,
	       TOPIC_DATA_WRITER,
	       STATE_TYPE>::create_datawriter ()
{
  DDS::DataWriterQos dw_qos;
  DDS::ReturnCode_t status =
    pub_->get_default_datawriter_qos (dw_qos);

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

#endif /* _DDS_STATE_UPDATE_CPP_ */
