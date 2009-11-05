/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Task.h"
#include "ace/Reactor.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      template <typename NDDS_TYPE, typename RAWLISTENER>
      class DataReaderHandler_T : 
        public ACE_Event_Handler
      {
        public:
          DataReaderHandler_T (
              typename RAWLISTENER::_ptr_type listen,
              typename NDDS_TYPE::data_reader * reader);
          ~DataReaderHandler_T ();

          virtual int handle_input (ACE_HANDLE fc = ACE_INVALID_HANDLE);
          //virtual int handle_signal (int );
          virtual int handle_output (ACE_HANDLE fc = ACE_INVALID_HANDLE);
          virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
        private:
          //ACE_Reactor ar_;
          typename RAWLISTENER::_var_type listener_;
          typename NDDS_TYPE::data_reader * reader_;
      };

      template <typename NDDS_TYPE, typename RAWLISTENER, typename PORTSTATUSLISTENER>
      class DataReaderListener_T :
        public virtual ::DDS::DataReaderListener
      {
      public:
        // Constructor
        DataReaderListener_T (
                      typename RAWLISTENER::_ptr_type listen, 
                      typename PORTSTATUSLISTENER::_ptr_type psl, 
                      ACE_Atomic_Op <TAO_SYNCH_MUTEX, bool> &enabled,
                      CORBA::ORB_ptr orb);

        // Destructor
        virtual ~DataReaderListener_T (void);

        virtual void on_data_available( ::DDS::DataReader *rdr);

        virtual void on_requested_deadline_missed (::DDS::DataReader_ptr the_reader,
                                               const ::DDS::RequestedDeadlineMissedStatus & status);

        virtual void on_sample_lost (::DDS::DataReader_ptr the_reader,
                                 const ::DDS::SampleLostStatus & status);
        bool enabled () const;
        void enabled (bool enable);

      private:
        DataReaderListener_T<NDDS_TYPE, RAWLISTENER, PORTSTATUSLISTENER> (const DataReaderListener_T<NDDS_TYPE, RAWLISTENER, PORTSTATUSLISTENER> &);
        DataReaderListener_T<NDDS_TYPE, RAWLISTENER, PORTSTATUSLISTENER> & operator = (const DataReaderListener_T<NDDS_TYPE, RAWLISTENER, PORTSTATUSLISTENER> &);
        typename RAWLISTENER::_var_type listener_;
        typename PORTSTATUSLISTENER::_var_type portlistener_;
        ACE_Atomic_Op <TAO_SYNCH_MUTEX, bool> &enable_;
        CORBA::ORB_var orb_;
      };
    }
  }
}

#include "dds4ccm/impl/ndds/DataReaderListener_T.cpp"
