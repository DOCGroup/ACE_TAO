/**
 * @author 
 * @author 
 *
 * $Id: ConnectorStatusListener.h 
 *
 * Wrapper facade for NDDS.
 */
 
#ifndef DDS4CCM_CONNECTORSTATUSLISTENERI_H_
#define DDS4CCM_CONNECTORSTATUSLISTENERI_H_

#include "dds4ccm/impl/ndds/dds4ccm_ndds_impl_export.h"
#include "dds4ccm/idl/dds4ccm_ConnectorStatusListenerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class DDS4CCM_NDDS_IMPL_Export CCM_DDS_ConnectorStatusListener_i
  : public virtual CCM_DDS::CCM_ConnectorStatusListener,
    public virtual ::CORBA::LocalObject
{

public:
  // Constructor 
  CCM_DDS_ConnectorStatusListener_i (void);
  
  // Destructor 
  virtual ~CCM_DDS_ConnectorStatusListener_i (void);
  virtual
  void on_inconsistent_topic( ::DDS::Topic_ptr the_topic, 
                              const DDS::InconsistentTopicStatus & status);
  virtual
  void on_requested_incompatible_qos( ::DDS::DataReader_ptr the_reader,
                                      const DDS::RequestedIncompatibleQosStatus & status);
  virtual
  void on_sample_rejected( ::DDS::DataReader_ptr the_reader, 
                           const DDS::SampleRejectedStatus & status);
  virtual
    void on_offered_deadline_missed( ::DDS::DataWriter_ptr the_writer,
                                   const DDS::OfferedDeadlineMissedStatus & status);
  virtual
  void on_offered_incompatible_qos( ::DDS::DataWriter_ptr the_writer, 
                                    const DDS::OfferedIncompatibleQosStatus & status);
  virtual
    void on_unexpected_status( ::DDS::Entity_ptr the_entity,
     ::DDS::StatusKind  status_kind);


   
};


#endif /* DDS4CCM_CONNECTORSTATUSLISTENERI_H_  */

