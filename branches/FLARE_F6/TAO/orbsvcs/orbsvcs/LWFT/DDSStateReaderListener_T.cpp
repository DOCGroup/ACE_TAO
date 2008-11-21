// -*- C++ -*-

//=============================================================================
/**
 *  @file    DDSStateReaderListener_T.cpp
 *
 *  $Id$
 *
 * @author   Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef _DDS_STATE_READER_LISTENER_T_CPP_
#define _DDS_STATE_READER_LISTENER_T_CPP_

template <typename TOPIC_TYPE, 
	  typename TOPIC_DATA_READER, 
          typename TOPIC_SEQUENCE,
	  typename STATE_TYPE>
DDSStateReaderListener_T<TOPIC_TYPE, 
			 TOPIC_DATA_READER, 
                         TOPIC_SEQUENCE,
			 STATE_TYPE>::DDSStateReaderListener_T (
    const std::string & id,							    
    ReplicatedApplication_ptr application)
 : id_ (id),
   application_ (ReplicatedApplication::_duplicate (application))
{
}

template <typename TOPIC_TYPE, 
	  typename TOPIC_DATA_READER, 
          typename TOPIC_SEQUENCE,
	  typename STATE_TYPE>
DDSStateReaderListener_T<TOPIC_TYPE, 
			 TOPIC_DATA_READER, 
                         TOPIC_SEQUENCE,
			 STATE_TYPE>::~DDSStateReaderListener_T ()
{
}


template <typename TOPIC_TYPE, 
	  typename TOPIC_DATA_READER, 
          typename TOPIC_SEQUENCE,
	  typename STATE_TYPE>
void
DDSStateReaderListener_T<TOPIC_TYPE, 
			 TOPIC_DATA_READER, 
                         TOPIC_SEQUENCE,
			 STATE_TYPE>::on_requested_deadline_missed (
    DDS::DataReader_ptr,
    const DDS::RequestedDeadlineMissedStatus &)
  throw (CORBA::SystemException)
{
  ACE_DEBUG ((LM_TRACE, ACE_TEXT ("DDSStateReaderListener_T::")
	      ACE_TEXT ("on_requested_deadline_missed ()\n")));
}

template <typename TOPIC_TYPE, 
	  typename TOPIC_DATA_READER, 
          typename TOPIC_SEQUENCE,
	  typename STATE_TYPE>
void
DDSStateReaderListener_T<TOPIC_TYPE, 
			 TOPIC_DATA_READER, 
                         TOPIC_SEQUENCE,
			 STATE_TYPE>::on_requested_incompatible_qos (
    DDS::DataReader_ptr,
    const DDS::RequestedIncompatibleQosStatus &status)
  throw (CORBA::SystemException)
{
  ACE_DEBUG ((LM_TRACE, ACE_TEXT ("DDSStateReaderListener_T::")
	      ACE_TEXT ("on_requested_incompatible_qos ()")
	      ACE_TEXT ("count=%d id=%d\n"), 
	      status.total_count_change,
	      status.last_policy_id));
}

template <typename TOPIC_TYPE, 
	  typename TOPIC_DATA_READER, 
          typename TOPIC_SEQUENCE,
	  typename STATE_TYPE>
void
DDSStateReaderListener_T<TOPIC_TYPE, 
			 TOPIC_DATA_READER, 
                         TOPIC_SEQUENCE,
			 STATE_TYPE>::on_liveliness_changed (
    DDS::DataReader_ptr,
    const DDS::LivelinessChangedStatus &)
  throw (CORBA::SystemException)
{
}

template <typename TOPIC_TYPE, 
	  typename TOPIC_DATA_READER, 
          typename TOPIC_SEQUENCE,
	  typename STATE_TYPE>
void
DDSStateReaderListener_T<TOPIC_TYPE, 
			 TOPIC_DATA_READER, 
                         TOPIC_SEQUENCE,
			 STATE_TYPE>::on_subscription_matched (
    DDS::DataReader_ptr,
    const DDS::SubscriptionMatchedStatus &)
  throw (CORBA::SystemException)
{
}

template <typename TOPIC_TYPE, 
	  typename TOPIC_DATA_READER, 
          typename TOPIC_SEQUENCE,
	  typename STATE_TYPE>
void
DDSStateReaderListener_T<TOPIC_TYPE, 
			 TOPIC_DATA_READER, 
                         TOPIC_SEQUENCE,
			 STATE_TYPE>::on_sample_rejected (
    DDS::DataReader_ptr,
    const DDS::SampleRejectedStatus &)
  throw (CORBA::SystemException)
{
  ACE_DEBUG ((LM_TRACE, ACE_TEXT ("DDSStateReaderListener_T::")
	      ACE_TEXT ("on_sample_rejected ()\n")));
}

template <typename TOPIC_TYPE, 
	  typename TOPIC_DATA_READER, 
          typename TOPIC_SEQUENCE,
	  typename STATE_TYPE>
void
DDSStateReaderListener_T<TOPIC_TYPE, 
			 TOPIC_DATA_READER, 
                         TOPIC_SEQUENCE,
			 STATE_TYPE>::on_data_available (
    DDS::DataReader_ptr reader)
  throw (CORBA::SystemException)
{
  typename TOPIC_DATA_READER::_var_type state_dr = 
    TOPIC_DATA_READER::_narrow (reader);
  
  if (CORBA::is_nil (state_dr.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("on_data_available(): ")
                  ACE_TEXT ("DDSStateReaderListener_T::_narrow() failed.\n")));
    }
    
  TOPIC_SEQUENCE state_samples;
  DDS::SampleInfoSeq sis;
  
  DDS::ReturnCode_t status = state_dr->take (state_samples, 
					     sis, 
					     1,
					     DDS::NOT_READ_SAMPLE_STATE,
					     DDS::ANY_VIEW_STATE,
					     DDS::ANY_INSTANCE_STATE);
    
  TOPIC_TYPE state_sample = state_samples[0];
  DDS::SampleInfo si = sis[0];

  if (status == DDS::RETCODE_OK)
    {
      //      ACE_DEBUG ((LM_TRACE, ACE_TEXT ("DDSStateReaderListener_T ")
      //		  ACE_TEXT ("sample-id %s\n"), state_sample.id.in ()));

      // only update the state if it is not sent from within the same process
      if (id_.compare (state_sample.id) != 0)
	{
	  // put state information into an any and send it to the application
	  CORBA::Any_var state (new CORBA::Any);

	  // get state value from topic sample
	  STATE_TYPE value = state_sample.value;

	  // insert state value into the any
	  *state <<= value;

	  try
	    {
	      application_->set_state (state.in ());
	    }
	  catch (CORBA::SystemException & ex)
	    {
	      ACE_ERROR ((LM_ERROR,
			  ACE_TEXT ("on_data_available(): ")
			  ACE_TEXT ("could not send state information ")
			  ACE_TEXT ("to application: %s"), 
			  ex._info ().c_str ()));
	    }
	}
    }
  else if (status == DDS::RETCODE_NO_DATA)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("on_data_available(): ")
                  ACE_TEXT ("reader received DDS::RETCODE_NO_DATA.\n")));
  else
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("on_data_available(): ")
                  ACE_TEXT ("reader error: %d.\n"), status));
}

template <typename TOPIC_TYPE, 
	  typename TOPIC_DATA_READER, 
          typename TOPIC_SEQUENCE,
	  typename STATE_TYPE>
void
DDSStateReaderListener_T<TOPIC_TYPE, 
		         TOPIC_DATA_READER, 
                         TOPIC_SEQUENCE,
		         STATE_TYPE>::on_sample_lost (
    DDS::DataReader_ptr,
    const DDS::SampleLostStatus &)
  throw (CORBA::SystemException)
{
  ACE_DEBUG ((LM_TRACE, ACE_TEXT ("DDSStateReaderListener_T::")
	      ACE_TEXT ("on_sample_lost ()\n")));
}

#endif /* _DDS_STATE_READER_LISTENER_T_CPP_ */
