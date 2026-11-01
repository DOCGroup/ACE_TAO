/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    QoS_Util.h
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */
//=============================================================================


#ifndef QOS_UTIL_H
#define QOS_UTIL_H

#include "ace/INET_Addr.h"
#include "ace/QoS/QoS_Session.h"

#include <memory>

/**
 * @class QoS_Util
 *
 * @brief This class provides the utility functions like parse_args ()
 * required by a QoS enabled application.
 */
class QoS_Util
{
public:
  // constructor.
  QoS_Util (int argc, ACE_TCHAR *argv[]);
  QoS_Util (QoS_Util const &) = delete;
  QoS_Util (QoS_Util &&) = delete;
  QoS_Util &operator= (QoS_Util const &) = delete;
  QoS_Util &operator= (QoS_Util &&) = delete;

  // destructor.
  ~QoS_Util () = default;

  // Parse command-line arguments.
  int parse_args ();

  // GET methods.
  ACE_INET_Addr *mult_session_addr () const;

  ACE_INET_Addr *dest_addr () const;

  u_short source_port () const;

  ACE_Protocol_ID protocol () const;

  bool multicast_flag () const;

private:
  // Command line arguments.
  int argc_;
  ACE_TCHAR **argv_;

  // Multicast session address.
  std::unique_ptr<ACE_INET_Addr> mult_session_addr_;

  // Unicast destination address of the receiver.
  std::unique_ptr<ACE_INET_Addr> dest_addr_;

  // Source port for the sender.
  u_short source_port_;

  // Protocol.
  ACE_Protocol_ID protocol_;

  // Multicast Flag.
  bool multicast_flag_;
};

#endif /* QOS_UTIL_H */

