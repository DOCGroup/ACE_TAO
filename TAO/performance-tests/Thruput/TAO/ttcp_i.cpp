// $Id$

#include "ace/ACE.h"
#include "ttcp_i.h"
#include "ttcp_decl.h"

ACE_RCSID(TAO, ttcp_i, "$Id$")

/* the ttcp_i class implementation */
ttcp_sequence_i::ttcp_sequence_i()
{
  this->nbytes_ = 0;
  numCalls = 0;
}

void
ttcp_sequence_i::start_timer (CORBA::Environment &IT_env)
{
  ACE_UNUSED_ARG (IT_env);
  this->nbytes_ = 0;
  ::prep_timer ();
}

void
ttcp_sequence_i::stop_timer (CORBA::Environment &IT_env)
{
  ACE_UNUSED_ARG (IT_env);

  (void) ::read_timer (stats, sizeof (stats));
        ::nbytes = this->nbytes_;
        ::PrintStats();
        // reset
        this->nbytes_ = 0;
        numCalls = 0;
#if defined (ACE_HAS_QUANTIFY)
    quantify_stop_recording_data();
    ACE_Service_Config::end_reactor_event_loop();
    ACE_OS::fprintf (stderr, "*********** just before exiting\n");
#endif /* ACE_HAS_QUANTIFY */
#if defined (ACE_HAS_PURIFY)
    ACE_Service_Config::end_reactor_event_loop();
    ACE_OS::fprintf (stderr, "*********** just before exiting\n");
#endif
}

void
ttcp_sequence_i::sendShortSeq(const ttcp_sequence::ShortSeq& ttcp_seq, CORBA::Environment &IT_env)
{
  ACE_UNUSED_ARG (IT_env);
  numCalls++;
  this->nbytes_ += ttcp_seq.length()*sizeof(CORBA::Short);
}

void
ttcp_sequence_i::sendLongSeq(const ttcp_sequence::LongSeq& ttcp_seq, CORBA::Environment &IT_env)
{
  ACE_UNUSED_ARG (IT_env);
  numCalls++;
  this->nbytes_ += ttcp_seq.length()*sizeof(CORBA::Long) ;
}

void
ttcp_sequence_i::sendOctetSeq(const ttcp_sequence::OctetSeq& ttcp_seq, CORBA::Environment &IT_env)
{
  ACE_UNUSED_ARG (IT_env);
  numCalls++;
  this->nbytes_ += ttcp_seq.length()*sizeof(CORBA::Octet) ;
}

void
ttcp_sequence_i::sendDoubleSeq(const ttcp_sequence::DoubleSeq& ttcp_seq, CORBA::Environment &IT_env)
{
  ACE_UNUSED_ARG (IT_env);
  numCalls++;
  this->nbytes_ += ttcp_seq.length()*sizeof(CORBA::Double) ;
}

void
ttcp_sequence_i::sendCharSeq(const ttcp_sequence::CharSeq& ttcp_seq, CORBA::Environment &IT_env)
{
  ACE_UNUSED_ARG (IT_env);
  numCalls++;
  this->nbytes_ += ttcp_seq.length()*sizeof(CORBA::Char) ;
}

void
ttcp_sequence_i::sendStructSeq(const ttcp_sequence::StructSeq& ttcp_seq, CORBA::Environment &IT_env)
{
  ACE_UNUSED_ARG (IT_env);
  numCalls++;
  this->nbytes_ += ttcp_seq.length()*sizeof(BinStruct) ;
#ifdef DEBUG
  ACE_DEBUG ((LM_DEBUG,
              "Bytes received so far = %d\n", this->nbytes_));
#endif
}
