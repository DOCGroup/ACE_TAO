// -*- C++ -*-

//=============================================================================
/**
 *  @file    DDSStateUpdate_T.h
 *
 *  $Id$
 *
 * @author   Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef _DDS_STATE_UPDATE_T_H_
#define _DDS_STATE_UPDATE_T_H_

#include <string>
#include "ccpp.h"
#include "StatefulObject.h"
#include "ReplicatedApplicationC.h"

/**
 * @class DDSStateUpdate_T
 *
 * @brief Implementation of the StatefulObject interface that uses
 *        DDS to disseminate state information to replicas
 */
template <typename STATE_TYPE,
	  typename TOPIC_TYPE, 
	  typename TOPIC_TYPE_SUPPORT,
	  typename TOPIC_DATA_WRITER,
          typename TOPIC_DATA_READER,
	  typename TOPIC_SEQUENCE>
class DDSStateUpdate_T : public StatefulObject
{
public:
  /// ctor
  DDSStateUpdate_T (const std::string & topic_name,
		    const std::string & id,
		    DDS::DomainParticipant_ptr participant,
		    DDS::Publisher_ptr publisher,
		    DDS::Subscriber_ptr subscriber,
		    ReplicatedApplication_ptr application);

  /// dtor
  virtual ~DDSStateUpdate_T ();

  bool init ();
  bool fini ();

  /// triggers state dissemination using DDS
  virtual void set_state (const CORBA::Any & state_value);  

protected:
  TOPIC_TYPE state_;

private:
  // helper methods for DDS initialization
  bool create_topic ();
  bool create_datawriter ();
  bool create_datareader ();

private:
  /// name of the topic to be registered
  std::string topic_name_;

  /// this id is part of every topic to check where the data comes from
  std::string id_;

  // DDS entities needed for data dissemination
  DDS::DomainParticipant_var participant_;
  DDS::Publisher_var pub_;
  DDS::Subscriber_var sub_;
  DDS::Topic_var topic_;
  DDS::InstanceHandle_t instance_handle_;
  typename TOPIC_DATA_WRITER::_var_type datawriter_;
  typename TOPIC_DATA_READER::_var_type datareader_;
  DDS::DataReaderListener_var listener_;
};

#include "DDSStateUpdate_T.cpp"

#endif /* _DDS_STATE_UPDATE_T_H_ */
