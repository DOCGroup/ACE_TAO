// $Id$

#include "ttcp_i.h"

ACE_RCSID(Orbix, ttcp_i, "$Id$")

/* callback routine that will change the size of the socket queue */
static void
SD_Ready (int io_descriptor)
{
  if (sockbufsize)
    {
      if (setsockopt (io_descriptor, SOL_SOCKET, SO_RCVBUF,
                (char *) &sockbufsize, sizeof sockbufsize) < 0)
        perror ("setsockopt: rcvbuf"), exit (1);
  }
}

/* the ttcp_i class implementation */
ttcp_sequence_i::ttcp_sequence_i()
{
  this->nbytes_ = 0;
  numCalls = 0;
  // register a callback so we can manipulate the descriptor
  // being used by orbix.

  if (CORBA::Orbix.registerIOCallback (OrbixIOCallback(SD_Ready),
        FD_OPEN_CALLBACK) != 0)
    perror ("can't register callback"), exit (1);
}


void
ttcp_sequence_i::start_timer (CORBA::Environment &IT_env)
{
    this->nbytes_ = 0;
    ::prep_timer ();
}

void
ttcp_sequence_i::stop_timer (CORBA::Environment &IT_env)
{
  (void) ::read_timer (stats, sizeof (stats));
        ::nbytes = this->nbytes_;
        ::PrintStats();
        // reset
        this->nbytes_ = 0;
        numCalls = 0;
#if defined (ACE_HAS_QUANTIFY)
    quantify_stop_recording_data();
    exit(0);
#endif
}

void
ttcp_sequence_i::sendShortSeq(const ttcp_sequence::ShortSeq& ttcp_seq, CORBA::Environment &IT_env)
{
    numCalls++;
    this->nbytes_ += ttcp_seq.length()*sizeof(CORBA::Short);
}

void
ttcp_sequence_i::sendLongSeq(const ttcp_sequence::LongSeq& ttcp_seq, CORBA::Environment &IT_env)
{
    numCalls++;
    this->nbytes_ += ttcp_seq.length()*sizeof(CORBA::Long) ;
}

void
ttcp_sequence_i::sendOctetSeq(const ttcp_sequence::OctetSeq& ttcp_seq, CORBA::Environment &IT_env)
{
    numCalls++;
    this->nbytes_ += ttcp_seq.length()*sizeof(CORBA::Octet) ;
}

void
ttcp_sequence_i::sendDoubleSeq(const ttcp_sequence::DoubleSeq& ttcp_seq, CORBA::Environment &IT_env)
{
    numCalls++;
    this->nbytes_ += ttcp_seq.length()*sizeof(CORBA::Double) ;
}

void
ttcp_sequence_i::sendCharSeq(const ttcp_sequence::CharSeq& ttcp_seq, CORBA::Environment &IT_env)
{
    numCalls++;
    this->nbytes_ += ttcp_seq.length()*sizeof(CORBA::Char) ;
}

void
ttcp_sequence_i::sendStructSeq(const ttcp_sequence::StructSeq& ttcp_seq, CORBA::Environment &IT_env)
{
    numCalls++;
    this->nbytes_ += ttcp_seq.length()*sizeof(PerfStruct) ;
}

void
ttcp_sequence_i::sendCompositeSeq(const ttcp_sequence::CompositeSeq& ttcp_seq, CORBA::Environment &IT_env)
{
    numCalls++;
    this->nbytes_ += ttcp_seq.length()*sizeof(CompositeStruct) ;
}
