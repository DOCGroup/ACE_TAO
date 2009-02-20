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
template <typename DATA_TYPE>//, 
//	        typename DATA_TYPE_SUPPORT,
//	        typename DATA_WRITER,
//          typename DATA_READER,
//	        typename DATA_TYPE_SEQUENCE>
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
  virtual ~DDSStateUpdate_T (void);

  bool init (void);
  bool fini (void);

  /// triggers state dissemination using DDS
  virtual void set_state (const CORBA::Any & state_value);  

protected:
  /// contains the value sent most recently
  DATA_TYPE state_;

private:
  // Helper methods for DDS initialization.
  bool create_topic (void);
  bool create_datawriter (void);
  bool create_datareader (void);

private:
  /// Name of the topic to be registered.
  std::string topic_name_;

  /// This id is part of every topic to check where the data comes from.
  std::string id_;

  // DDS entities needed for data dissemination.
  DDS::DomainParticipant_var participant_;
  DDS::Publisher_var pub_;
  DDS::Subscriber_var sub_;
  DDS::Topic_var topic_;
  DDS::InstanceHandle_t instance_handle_;
  typename DATA_TYPE::_data_writer_type::_var_type datawriter_;
  typename DATA_TYPE::_data_reader_type::_var_type datareader_;
  DDS::DataReaderListener_var listener_;
};

#include "DDSStateUpdate_T.cpp"

#endif /* _DDS_STATE_UPDATE_T_H_ */
