/**
 * @file DataReaderListener.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_DATAREADERLISTENER_H
#define CIAO_RTI_DATAREADERLISTENER_H

#include "tao/LocalObject.h"

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/ndds/dds4ccm_ndds_impl_export.h"

#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      class DDS4CCM_NDDS_IMPL_Export RTI_DataReaderListener_i :
        public virtual ::DDS::CCM_DataReaderListener,
        public virtual ::DDSDataReaderListener
      {
      public:
        // Constructor @todo zap default argument
        RTI_DataReaderListener_i (DDSDataReaderListener *p = 0);

        // Destructor
        virtual ~RTI_DataReaderListener_i (void);

        virtual void
        on_requested_deadline_missed (
          ::DDS::DataReader_ptr the_reader,
          const ::DDS::RequestedDeadlineMissedStatus & status);

        virtual void
        on_requested_incompatible_qos (
          ::DDS::DataReader_ptr the_reader,
          const ::DDS::RequestedIncompatibleQosStatus & status);

        virtual void
        on_sample_rejected (
          ::DDS::DataReader_ptr the_reader,
          const ::DDS::SampleRejectedStatus & status);

        virtual void
        on_liveliness_changed (
          ::DDS::DataReader_ptr the_reader,
          const ::DDS::LivelinessChangedStatus & status);

        virtual void
        on_data_available (
          ::DDS::DataReader_ptr the_reader);
          
        virtual void 
          on_data_available(DDSDataReader *reader);

        virtual void
        on_subscription_matched (
          ::DDS::DataReader_ptr the_reader,
          const ::DDS::SubscriptionMatchedStatus & status);

        virtual void
        on_sample_lost (
          ::DDS::DataReader_ptr the_reader,
          const ::DDS::SampleLostStatus & status);

        DDSDataReaderListener * get_datareaderlistener (void);
      private:
        DDSDataReaderListener *drl_;
      };
    }
  }
}

#endif
