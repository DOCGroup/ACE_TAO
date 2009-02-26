// -*- C++ -*-

//=============================================================================
/**
 *  @file    DDSStateReaderListener_T.h
 *
 *  $Id$
 *
 * @author   Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef _DDS_STATE_READER_LISTENER_T_H_
#define _DDS_STATE_READER_LISTENER_T_H_

#include "ccpp.h"
#include "ReplicatedApplicationC.h"

/**
 * @class DDSStateReaderListener_T
 *
 * @brief this listener receives data from DDS and posts it to the replicated
 *        application.
 */
template <typename DATA_TYPE>
class DDSStateReaderListener_T : public DDS::DataReaderListener
{
public:
  /// ctor
  DDSStateReaderListener_T (const std::string & id,
			                      ReplicatedApplication_ptr application);

  /// dtor
  virtual ~DDSStateReaderListener_T (void);

  virtual void
  on_requested_deadline_missed (
    DDS::DataReader_ptr reader,
    const DDS::RequestedDeadlineMissedStatus & status)
    throw (CORBA::SystemException);

  virtual void on_requested_incompatible_qos (
    DDS::DataReader_ptr reader,
    const DDS::RequestedIncompatibleQosStatus & status)
    throw (CORBA::SystemException);

  virtual void on_liveliness_changed (
    DDS::DataReader_ptr reader,
    const DDS::LivelinessChangedStatus & status)
    throw (CORBA::SystemException);

  virtual void on_subscription_matched (
    DDS::DataReader_ptr reader,
    const DDS::SubscriptionMatchedStatus & status)
    throw (CORBA::SystemException);

  virtual void on_sample_rejected (
    DDS::DataReader_ptr reader,
    const DDS::SampleRejectedStatus& status)
    throw (CORBA::SystemException);

  virtual void on_data_available (
    DDS::DataReader_ptr reader)
    throw (CORBA::SystemException);

  virtual void on_sample_lost (
    DDS::DataReader_ptr reader,
    const DDS::SampleLostStatus& status)
    throw (CORBA::SystemException);

private:
  /// This id is part of every topic to check where the data comes from.
  std::string id_;

  /// Reference of the application receiving the state information.
  ReplicatedApplication_var application_;
};

#include "DDSStateReaderListener_T.cpp"

#endif /* _DDS_STATE_READER_LISTENER_T_H_ */
