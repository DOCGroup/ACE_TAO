#include "ttcp_i.h"
#include <iostream.h>

/* the ttcp_i class implementation */
ttcp_sequence_i::ttcp_sequence_i(const char *obj_name)
  :_skel_ttcp_sequence(obj_name)
{
  this->nbytes_ = 0;
  numCalls = 0;
}


void
ttcp_sequence_i::start_timer (CORBA_Environment &IT_env)
{
    this->nbytes_ = 0;
    ::prep_timer ();
}

void 
ttcp_sequence_i::stop_timer (CORBA_Environment &IT_env)
{
  (void) ::read_timer (stats, sizeof (stats));
	::nbytes = this->nbytes_;
	::PrintStats();
	// reset
	this->nbytes_ = 0;
	numCalls = 0;
#if defined (USE_QUANTIFY)
    quantify_stop_recording_data();
    ACE_Service_Config::end_reactor_event_loop();
    cerr << "*********** just before exiting " << endl;
#endif
}

void
ttcp_sequence_i::sendShortSeq(const ttcp_sequence::ShortSeq& ttcp_seq, CORBA_Environment &IT_env)
{
    numCalls++;
    this->nbytes_ += ttcp_seq.length()*sizeof(CORBA_Short);
}

void
ttcp_sequence_i::sendLongSeq(const ttcp_sequence::LongSeq& ttcp_seq, CORBA_Environment &IT_env)
{
    numCalls++;
    this->nbytes_ += ttcp_seq.length()*sizeof(CORBA_Long) ;
}

void
ttcp_sequence_i::sendOctetSeq(const ttcp_sequence::OctetSeq& ttcp_seq, CORBA_Environment &IT_env)
{
    numCalls++;
    this->nbytes_ += ttcp_seq.length()*sizeof(CORBA_Octet) ;
}

void
ttcp_sequence_i::sendDoubleSeq(const ttcp_sequence::DoubleSeq& ttcp_seq, CORBA_Environment &IT_env)
{
    numCalls++;
    this->nbytes_ += ttcp_seq.length()*sizeof(CORBA_Double) ;
}

void
ttcp_sequence_i::sendCharSeq(const ttcp_sequence::CharSeq& ttcp_seq, CORBA_Environment &IT_env)
{
    numCalls++;
    this->nbytes_ += ttcp_seq.length()*sizeof(CORBA_Char) ;
}

void
ttcp_sequence_i::sendStructSeq(const ttcp_sequence::StructSeq& ttcp_seq, CORBA_Environment &IT_env)
{
    numCalls++;
    this->nbytes_ += ttcp_seq.length()*sizeof(PerfStruct) ;
#ifdef DEBUG
    //    cout << "Bytes received so far = " << this->nbytes_ << endl;
#endif
}









