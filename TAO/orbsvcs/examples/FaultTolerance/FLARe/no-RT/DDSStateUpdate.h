// -*- C++ -*-

//=============================================================================
/**
 *  @file    DDSStateUpdate.h
 *
 *  $Id$
 *
 * @author   Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef _DDS_STATE_UPDATE_H_
#define _DDS_STATE_UPDATE_H_

#include "StatefulObject.h"
#include "ccpp.h"
#include <string>

/**
 * @class DDSStateUpdate
 *
 * @brief Implementation of the StatefulObject interface that uses
 *        DDS to disseminate state information to replicas
 */
template <class TOPIC_TYPE, 
	  class TOPIC_TYPE_SUPPORT,
	  class TOPIC_DATA_WRITER, 
	  class STATE_TYPE>
class DDSStateUpdate : public StatefulObject
{
public:

  /// ctor
  DDSStateUpdate (const std::string & topic_name,
		  const std::string & id,
		  DDS::DomainParticipant_ptr participant);


  /// dtor
  virtual ~DDSStateUpdate ();

  bool init ();
  bool fini ();

  /// triggers state dissemination using DDS
  virtual void set_state (const CORBA::Any & state_value);  

protected:
  TOPIC_TYPE state_;

private:
  // helper methods for DDS initialization
  bool create_publisher ();
  bool create_topic ();
  bool create_datawriter ();

private:
  /// name of the topic to be registered
  std::string topic_name_;

  /// this id is part of every topic to check where the data comes from
  std::string id_;

  /// domain id of the used domain
  DDS::DomainId_t domain_id_;

  // DDS entities needed for data dissemination
  DDS::DomainParticipant_var participant_;
  DDS::Publisher_var pub_;
  DDS::Topic_var topic_;
  DDS::InstanceHandle_t instance_handle_;
  typename TOPIC_DATA_WRITER::_var_type datawriter_;

};

#include "DDSStateUpdate.cpp"

#endif /* _DDS_STATE_UPDATE_H_ */
