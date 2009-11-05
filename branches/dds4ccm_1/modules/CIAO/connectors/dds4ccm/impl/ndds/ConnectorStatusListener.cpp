#include "ConnectorStatusListener.h"

// Implementation skeleton constructor
CCM_DDS_ConnectorStatusListener_i::CCM_DDS_ConnectorStatusListener_i (void)
{
  printf("############################## constructor in ConnectionStatusListener\n");
}

// Implementation skeleton destructor
CCM_DDS_ConnectorStatusListener_i::~CCM_DDS_ConnectorStatusListener_i (void)
{
  printf("############################## destructor in ConnectionStatusListener\n");
}
void CCM_DDS_ConnectorStatusListener_i::on_inconsistent_topic( ::DDS::Topic_ptr the_topic, 
                            const DDS::InconsistentTopicStatus & status)
{
  printf("############################## on_inconsistent_topic in ConnectionStatusListener\n");
}
 void CCM_DDS_ConnectorStatusListener_i::on_requested_incompatible_qos( ::DDS::DataReader_ptr the_reader,
                            const DDS::RequestedIncompatibleQosStatus & status)
 {
   printf("############################## on_requested_incompatible_qosin ConnectionStatusListener\n");
 }
 void CCM_DDS_ConnectorStatusListener_i::on_sample_rejected( ::DDS::DataReader_ptr the_reader, 
                            const DDS::SampleRejectedStatus & status)
 {
   printf("############################## on_sample_rejected in ConnectionStatusListener\n");
 }
l
 void CCM_DDS_ConnectorStatusListener_i::on_offered_deadline_missed( ::DDS::DataWriter_ptr the_writer,
                             const DDS::OfferedDeadlineMissedStatus & status)
 {
   printf("############################## on_offered_deadline_missed in ConnectionStatusListener\n");
 }

 void CCM_DDS_ConnectorStatusListener_i::on_offered_incompatible_qos( ::DDS::DataWriter_ptr the_writer, 
                              const DDS::OfferedIncompatibleQosStatus & status)
 {
   printf("############################## on_offered_incompatible_qos in ConnectionStatusListener\n");
 }
 void CCM_DDS_ConnectorStatusListener_i::on_unexpected_status( ::DDS::Entity_ptr the_entity,
                            ::DDS::StatusKind  status_kind)
 {
   printf("############################## on_unexpected_status in ConnectionStatusListener\n");
 }



