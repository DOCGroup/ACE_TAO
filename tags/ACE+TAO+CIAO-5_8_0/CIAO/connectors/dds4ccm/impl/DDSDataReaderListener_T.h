/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDSDATAREADERLISTENER_T_H_
#define DDSDATAREADERLISTENER_T_H_

#include "tao/LocalObject.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm_dds_impl_export.h"
#include "dds4ccm/impl/dds4ccm_conf.h"
#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#if (CIAO_DDS4CCM_NDDS==1)
# include "ndds/ndds_cpp.h"
#endif

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::DataReaderListener DDSDataReaderListener;
typedef ::DDS::DataReader DDSDataReader;
typedef ::DDS::RequestedDeadlineMissedStatus DDS_RequestedDeadlineMissedStatus;
typedef ::DDS::LivelinessChangedStatus DDS_LivelinessChangedStatus;
typedef ::DDS::RequestedIncompatibleQosStatus DDS_RequestedIncompatibleQosStatus;
typedef ::DDS::SampleRejectedStatus DDS_SampleRejectedStatus;
typedef ::DDS::SubscriptionMatchedStatus DDS_SubscriptionMatchedStatus;
typedef ::DDS::SampleLostStatus DDS_SampleLostStatus;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class CCM_DDS_DataReaderListener_T :
      public virtual ::DDSDataReaderListener
    {
    typedef DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> DataReader_type;
    public:
      /// Constructor
      CCM_DDS_DataReaderListener_T (::DDS::DataReaderListener_ptr p,
                                    DataReader_type *typed_dr);

      /// Destructor
      virtual ~CCM_DDS_DataReaderListener_T (void);

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
    private:
      ::DDS::DataReaderListener_var impl_;
      DataReader_type *typed_dr_;
    };
  }
}

#include "dds4ccm/impl/DDSDataReaderListener_T.cpp"

#endif /* DDSDATAREADERLISTENER_T_H_ */
