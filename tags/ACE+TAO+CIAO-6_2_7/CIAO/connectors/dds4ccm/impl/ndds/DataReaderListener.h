/**
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDSDATAREADERLISTENER_T_H_
#define DDSDATAREADERLISTENER_T_H_

#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "dds4ccm/idl/ndds/ndds_dcpsC.h"
#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_DataReaderListener_i :
      public ::DDSDataReaderListener
    {
    public:
      /// Constructor
      DDS_DataReaderListener_i (::DDS::DataReaderListener_ptr p,
                                ::DDS::DataReader_ptr typed_dr);

      /// Destructor
      virtual ~DDS_DataReaderListener_i (void);

      virtual void on_requested_deadline_missed(::DDSDataReader* reader,
        const ::DDS_RequestedDeadlineMissedStatus& status);

      virtual void on_liveliness_changed(::DDSDataReader* reader,
        const ::DDS_LivelinessChangedStatus& status);

      virtual void on_requested_incompatible_qos(::DDSDataReader* reader,
        const ::DDS_RequestedIncompatibleQosStatus& status);

      virtual void on_sample_rejected(::DDSDataReader* reader,
        const ::DDS_SampleRejectedStatus& status);

      virtual void on_data_available(::DDSDataReader* reader);

      virtual void on_sample_lost(::DDSDataReader* reader,
          const ::DDS_SampleLostStatus& status);

      virtual void on_subscription_matched(::DDSDataReader* reader,
          const ::DDS_SubscriptionMatchedStatus& status);

      ::DDS::DataReaderListener_ptr get_datareaderlistener (void);

      void set_dds_entity (::DDS::DataReader_ptr typed_dr);
    private:
      ::DDS::DataReaderListener_var impl_;
      ::DDS::DataReader_var dr_;
    };
  }
}

#endif /* DDSDATAREADERLISTENER_T_H_ */
