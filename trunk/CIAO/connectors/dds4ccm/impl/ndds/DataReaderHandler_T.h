// $Id$
/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Eventhandler on orb thread, handling the
 * received data from DDS.
 */

#ifndef CIAO_RTI_DATAREADERHANDLER_T_H
#define CIAO_RTI_DATAREADERHANDLER_T_H

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Event_Handler.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    class DataReaderListener_T;

    template <typename DDS_TYPE, typename CCM_TYPE>
    class DataReaderHandler_T :
      public ACE_Event_Handler
    {
      public:
        DataReaderHandler_T (
            typename CIAO::DDS4CCM::DataReaderListener_T<DDS_TYPE, CCM_TYPE>* drl,
            ::DDS::DataReader_ptr dr);
        virtual ~DataReaderHandler_T ();

        virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
      private:
        typename CIAO::DDS4CCM::DataReaderListener_T<DDS_TYPE, CCM_TYPE>* drl_;
        ::DDS::DataReader_var dr_;
    };

    template <typename DDS_TYPE, typename CCM_TYPE>
    class DataReaderStateListener_T;

    template <typename DDS_TYPE, typename CCM_TYPE>
    class DataReaderStateHandler_T :
      public ACE_Event_Handler
    {
      public:
        DataReaderStateHandler_T (
            typename CIAO::DDS4CCM::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>* drl,
            ::DDS::DataReader_ptr dr);
        virtual ~DataReaderStateHandler_T ();

        virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
      private:
        typename CIAO::DDS4CCM::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>* drl_;
        ::DDS::DataReader_var dr_;
    };
  }
}

#include "dds4ccm/impl/ndds/DataReaderHandler_T.cpp"

#endif
