// $Id$

#include "pbroker/corba/orb.h"
#include "pbroker/corba/environ.h"
#include <pbroker/corba/xpsboa.h>
#include <pbroker/corba/lifecycl/impldecl.h>
#include "ace/ACE.h"
#include "ttcp_i.h"
#include "ttcp_decl.h"
#ifdef DEBUG
# include "ace/streams.h"
#endif

ACE_RCSID(CORBAplus, ttcp_i, "$Id$")

/* the ttcp_i class implementation */
ttcp_sequence_i::ttcp_sequence_i(const char *obj_name)
  : key_(obj_name)
{
  this->nbytes_ = 0;
  numCalls = 0;
}

const char *
ttcp_sequence_i::_get_name (void)
{
  return key_;
}

void
ttcp_sequence_i::start_timer (void)
{
  this->nbytes_ = 0;
  ::prep_timer ();
}

void
ttcp_sequence_i::stop_timer (void)
{
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
#endif
#if defined (ACE_HAS_PURIFY)
    ACE_Service_Config::end_reactor_event_loop();
    ACE_OS::fprintf (stderr, "*********** just before exiting\n");
#endif
}

void
ttcp_sequence_i::sendShortSeq(const ttcp_sequence::ShortSeq& ttcp_seq)
{
  numCalls++;
  this->nbytes_ += ttcp_seq.length()*sizeof(CORBA::Short);
}

void
ttcp_sequence_i::sendLongSeq(const ttcp_sequence::LongSeq& ttcp_seq)
{
  numCalls++;
  this->nbytes_ += ttcp_seq.length()*sizeof(CORBA::Long) ;
}

void
ttcp_sequence_i::sendOctetSeq(const ttcp_sequence::OctetSeq& ttcp_seq)
{
  numCalls++;
  this->nbytes_ += ttcp_seq.length()*sizeof(CORBA::Octet) ;
}

void
ttcp_sequence_i::sendDoubleSeq(const ttcp_sequence::DoubleSeq& ttcp_seq)
{
  numCalls++;
  this->nbytes_ += ttcp_seq.length()*sizeof(CORBA::Double) ;
}

void
ttcp_sequence_i::sendCharSeq(const ttcp_sequence::CharSeq& ttcp_seq)
{
  numCalls++;
  this->nbytes_ += ttcp_seq.length()*sizeof(CORBA::Char) ;
}

void
ttcp_sequence_i::sendStructSeq(const ttcp_sequence::StructSeq& ttcp_seq)
{
  numCalls++;
  this->nbytes_ += ttcp_seq.length()*sizeof(BinStruct) ;
#ifdef DEBUG
  //    cout << "Bytes received so far = " << this->nbytes_ << endl;
#endif
}
