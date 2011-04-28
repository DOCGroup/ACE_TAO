// $Id$
/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Eventhandler on orb thread, handling the
 * received data from DDS.
 */

#ifndef DATAREADERHANDLER_T_H_
#define DATAREADERHANDLER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Event_Handler.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename CCM_TYPE, typename TYPED_READER, typename SEQ_TYPE>
    class DataReaderListener_T;

    template <typename CCM_TYPE, typename TYPED_READER, typename SEQ_TYPE>
    class DataReaderHandler_T :
      public ACE_Event_Handler
    {
      typedef typename CIAO::DDS4CCM::DataReaderListener_T
        <CCM_TYPE, TYPED_READER, SEQ_TYPE> DataReaderListener_type;

      public:
        DataReaderHandler_T (
            DataReaderListener_type* drl,
            ::DDS::DataReader_ptr dr);
        virtual ~DataReaderHandler_T ();

        virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
      private:
        DataReaderListener_type* drl_;
        ::DDS::DataReader_var dr_;
    };

    template <typename CCM_TYPE, typename TYPED_READER, typename SEQ_TYPE>
    class DataReaderStateListener_T;

    template <typename CCM_TYPE, typename TYPED_READER, typename SEQ_TYPE>
    class DataReaderStateHandler_T :
      public ACE_Event_Handler
    {
      typedef typename CIAO::DDS4CCM::DataReaderStateListener_T
        <CCM_TYPE, TYPED_READER, SEQ_TYPE> DataReaderStateListener_type;

      public:
        DataReaderStateHandler_T (
            DataReaderStateListener_type* drl,
            ::DDS::DataReader_ptr dr);
        virtual ~DataReaderStateHandler_T ();

        virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
      private:
        DataReaderStateListener_type* drl_;
        ::DDS::DataReader_var dr_;
    };
  }
}

#include "dds4ccm/impl/DataReaderHandler_T.cpp"

#endif /* DATAREADERHANDLER_T_H_ */
