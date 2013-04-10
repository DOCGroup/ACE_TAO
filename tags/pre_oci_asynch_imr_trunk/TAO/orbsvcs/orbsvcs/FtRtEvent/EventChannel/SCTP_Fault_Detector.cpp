// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/SCTP_Fault_Detector.h"

#if (TAO_HAS_SCIOP == 1)

#include "orbsvcs/FtRtEvent/EventChannel/FTEC_Fault_Listener.h"

#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_sys_socket.h"

#ifndef SCTP_HEARTBEAT_ITVL
#define SCTP_HEARTBEAT_ITVL 19
#endif
#ifndef SOL_SCTP
#define SOL_SCTP 132
#endif

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

SCTP_Connector::SCTP_Connector()
: heart_beat_(0)
{
}

int
SCTP_Connector::connect (ACE_SOCK_SEQPACK_Association &new_association,
                             const ACE_Addr &remote_sap,
                             const ACE_Time_Value *timeout,
                             const ACE_Addr &local_sap,
                             int reuse_addr,
                             int /* flags */,
                             int /* perms */,
                             int protocol)
{
  ACE_TRACE ("SCTP_Connector::connect");

  if (this->shared_open (new_association,
                         remote_sap.get_type (),
                         protocol,
                         reuse_addr) == -1)
    return -1;
  else if (this->shared_connect_start (new_association,
                                       timeout,
                                       local_sap) == -1)
    return -1;

  if (new_association.set_option(SOL_SCTP,
                                 SCTP_HEARTBEAT_ITVL,
                                 &heart_beat_,
                                 sizeof(heart_beat_)) == -1)
    return -1;

  int result = ACE_OS::connect (new_association.get_handle (),
                                reinterpret_cast<sockaddr *> (remote_sap.get_addr ()),
                                remote_sap.get_size ());

  return this->shared_connect_finish (new_association,
                                      timeout,
                                      result);
}


inline void SCTP_Connector::set_heart_beat(int hb)
{
  heart_beat_ = hb;
}


SCTP_Fault_Detector::~SCTP_Fault_Detector()
{
}


int SCTP_Fault_Detector::parse_conf(int argc, ACE_TCHAR** argv)
{
  ACE_TRACE ("SCTP_Fault_Detector::parse_conf");

  for (int curarg = 0; curarg < argc; curarg++)
    if (ACE_OS::strcasecmp (argv[curarg],
      ACE_TEXT("-HeartBeat")) == 0)
    {
      curarg++;
      if (curarg < argc)
        connector_.set_heart_beat(ACE_OS::atoi(argv[curarg]));
    }
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_SCTP == 1 */
