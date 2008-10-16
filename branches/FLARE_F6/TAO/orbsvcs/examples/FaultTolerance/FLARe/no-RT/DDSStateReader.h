// -*- C++ -*-

//=============================================================================
/**
 *  @file    DDSStateReader.h
 *
 *  $Id$
 *
 * @author   Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef _DDS_STATE_READER_H_
#define _DDS_STATE_READER_H_

#include "ccpp.h"
#include <string>
#include "ReplicatedApplicationC.h"

/**
 * @typename DDSStateReader
 *
 * @brief This typename receives state information and passes it on to the
 *        replicated application.
 */
template <typename TOPIC_TYPE, 
	  typename TOPIC_TYPE_SUPPORT,
	  typename TOPIC_DATA_READER, 
	  typename STATE_TYPE>
class DDSStateReader
{
public:

  /// ctor
  DDSStateReader (const std::string & topic_name,
		  const std::string & id,
		  DDS::DomainParticipant_ptr participant,
		  ReplicatedApplication_ptr application);

  /// dtor
  virtual ~DDSStateReader ();

  bool init ();
  bool fini ();

private:
  // helper methods for DDS initialization
  bool create_subscriber ();
  bool create_topic ();
  bool create_datareader ();

private:
  /// name of the topic to be registered
  std::string topic_name_;

  // DDS entities needed for data dissemination
  DDS::DomainParticipant_var participant_;
  DDS::Subscriber_var sub_;
  DDS::Topic_var topic_;
  typename TOPIC_DATA_READER::_var_type datareader_;
  DDS::DataReaderListener_var listener_;
};

#include "DDSStateReader.cpp"

#endif /* _DDS_STATE_READER_H_ */
