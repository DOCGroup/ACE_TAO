// -*- C++ -*-

//=============================================================================
/**
 *  @file    DDSStateReader_T.cpp
 *
 *  $Id$
 *
 *  @author   Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef _DDS_STATE_READER_T_CPP_
#define _DDS_STATE_READER_T_CPP_

#include "DDSStateReaderListener_T.h"

template <typename TOPIC_TYPE, 
	  typename TOPIC_TYPE_SUPPORT,
	  typename TOPIC_DATA_READER, 
	  typename STATE_TYPE>
DDSStateReader_T<TOPIC_TYPE, 
		 TOPIC_TYPE_SUPPORT, 
		 TOPIC_DATA_READER, 
		 STATE_TYPE>::DDSStateReader_T (
    const std::string & topic_name,
    const std::string & id,
    DDS::DomainParticipant_ptr participant,
    ReplicatedApplication_ptr application)
  : topic_name_ (topic_name),
    participant_ (DDS::DomainParticipant::_duplicate (participant)),
    sub_ (DDS::Subscriber::_nil ()),
    topic_ (DDS::Topic::_nil ()),
    datareader_ (TOPIC_DATA_READER::_nil ()),
    listener_ (new DDSStateReaderListener_T <TOPIC_TYPE,
	                                     TOPIC_DATA_READER,
	                                     STATE_TYPE> (id, application))
{
  this->init ();
}

template <typename TOPIC_TYPE, 
	  typename TOPIC_TYPE_SUPPORT,
	  typename TOPIC_DATA_READER, 
	  typename STATE_TYPE>
DDSStateReader_T<TOPIC_TYPE, 
		 TOPIC_TYPE_SUPPORT, 
		 TOPIC_DATA_READER, 
		 STATE_TYPE>::~DDSStateReader_T ()
{
  this->fini ();
}

template <typename TOPIC_TYPE, 
	  typename TOPIC_TYPE_SUPPORT,
	  typename TOPIC_DATA_READER, 
	  typename STATE_TYPE>
bool
DDSStateReader_T<TOPIC_TYPE, 
               TOPIC_TYPE_SUPPORT, 
               TOPIC_DATA_READER, 
               STATE_TYPE>::init ()
{
  return this->create_subscriber ()
    && this->create_topic ()
    && this->create_datareader ();
}

template <typename TOPIC_TYPE, 
	  typename TOPIC_TYPE_SUPPORT,
	  typename TOPIC_DATA_READER, 
	  typename STATE_TYPE>
bool
DDSStateReader_T<TOPIC_TYPE, 
		 TOPIC_TYPE_SUPPORT, 
		 TOPIC_DATA_READER, 
		 STATE_TYPE>::fini ()
{
  DDS::ReturnCode_t status =
    this->sub_->delete_datareader (this->datareader_.in ());

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

template <typename TOPIC_TYPE, 
	  typename TOPIC_TYPE_SUPPORT,
	  typename TOPIC_DATA_READER, 
	  typename STATE_TYPE>
bool
DDSStateReader_T<TOPIC_TYPE, 
		 TOPIC_TYPE_SUPPORT, 
		 TOPIC_DATA_READER, 
		 STATE_TYPE>::create_topic ()
{
  DDS::TypeSupport_var svnt = new TOPIC_TYPE_SUPPORT;
  CORBA::String_var data_type_name = svnt->get_type_name ();
  
  DDS::ReturnCode_t status = svnt->register_type (participant_.in (),
                                                  data_type_name.in ());
                                                   
  if (status != DDS::RETCODE_OK)
    {
      return false;
    }

  DDS::TopicQos default_topic_qos;
  status = participant_->get_default_topic_qos (default_topic_qos);
  
  if (status != DDS::RETCODE_OK)
    {
      return false;
    }

  topic_ =
    participant_->create_topic (topic_name_.c_str (),
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

template <typename TOPIC_TYPE, 
	  typename TOPIC_TYPE_SUPPORT,
	  typename TOPIC_DATA_READER, 
	  typename STATE_TYPE>
bool
DDSStateReader_T<TOPIC_TYPE, 
		 TOPIC_TYPE_SUPPORT, 
		 TOPIC_DATA_READER, 
		 STATE_TYPE>::create_datareader ()
{
  DDS::DataReaderQos dr_qos;
  DDS::ReturnCode_t status =
    sub_->get_default_datareader_qos (dr_qos);

  if (status != DDS::RETCODE_OK)
    {
      return false;
    }

  DDS::DataReader_var datareader_base = 
    sub_->create_datareader (this->topic_.in (),
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
    
  return true;
}

#endif /* _DDS_STATE_READER_T_CPP_ */
