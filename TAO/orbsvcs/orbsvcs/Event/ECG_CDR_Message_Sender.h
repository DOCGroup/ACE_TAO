/* -*- C++ -*- */
/**
 *  @file ECG_CDR_Message_Sender.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *  @author Marina Spivak (marina@atdesk.com)
 */

#ifndef TAO_ECG_CDR_MESSAGE_SENDER_H
#define TAO_ECG_CDR_MESSAGE_SENDER_H
#include /**/ "ace/pre.h"

#include "ECG_UDP_Out_Endpoint.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "event_export.h"
#include "tao/corbafwd.h"
#include "tao/Exception.h"
#include "tao/Environment.h"
#include "ace/INET_Addr.h"

/**
 * @class TAO_ECG_CDR_Message_Sender
 *
 * @brief Sends CDR messages using UDP.
 *        NOT THREAD-SAFE.
 *
 * This class breaks up a CDR message into fragments and sends each
 * fragment with a header (described below) using UDP.
 * The UDP address can be a normal IP address or it can be a multicast
 * group. The UDP address is obtained from a RtecUDPAdmin::AddrServer
 * class.
 *
 * This class is used by various Gateway (Senders/Receivers) classes
 * responsible for federating Event Channels with UDP/Mcast.
 *
 * <H2>MESSAGE FORMAT</H2>
 * Message header are encapsulated using CDR, with the
 * following format:
 * struct Header {
 * octet byte_order_flags;
 * // bit 0 represents the byte order as in GIOP 1.1
 * // bit 1 is set if this is the last fragment
 * unsigned long request_id;
 * // The request ID, senders must not send two requests with
 * // the same ID, senders can be distinguished using recvfrom..
 * unsigned long request_size;
 * // The size of this request, this can be used to pre-allocate
 * // the request buffer.
 * unsgined long fragment_size;
 * // The size of this fragment, excluding the header...
 * unsigned long fragment_offset;
 * // Where does this fragment fit in the complete message...
 * unsigned long fragment_id;
 * // The ID of this fragment...
 * unsigned long fragment_count;
 * // The total number of fragments to expect in this request
 *
 * // @todo This could be eliminated if efficient reassembly
 * // could be implemented without it.
 * octet padding[4];
 *
 * // Ensures the header ends at an 8-byte boundary.
 * }; // size (in CDR stream) = 32
 */
class TAO_RTEvent_Export TAO_ECG_CDR_Message_Sender
{
public:

  enum {
    ECG_HEADER_SIZE = 32,
    ECG_MIN_MTU = 32 + 8,
    ECG_MAX_MTU = 65536, // Really optimistic...
    ECG_DEFAULT_MTU = 1024
  };

  /// Initialization and termination methods.
  //@{
  TAO_ECG_CDR_Message_Sender (CORBA::Boolean crc = 0);

  /// Set the endpoint for sending messages.
  /**
   * If init () is successful, shutdown () must be called when the
   * sender is no longer needed.  If shutdown () is not called by the
   * user, cleanup activities will be performed by the destructor.
   */
  void init (TAO_ECG_Refcounted_Endpoint endpoint_rptr
             ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));

  // Shutdown this component.  Frees up the endpoint.
  void shutdown (ACE_ENV_SINGLE_ARG_DECL);
  //@}

  /// Setters/getters.
  //@{
  /// Get the local endpoint used to send the events.
  int get_local_addr (ACE_INET_Addr& addr);

  /**
   * The sender may need to fragment the message, otherwise the
   * network may drop the packets.
   * Setting the MTU can fail if the value is too small (at least the
   * header + 8 bytes must fit).
   */
  int mtu (CORBA::ULong mtu);
  CORBA::ULong mtu (void) const;
  //@}

  /// The main method - send a CDR message.
  /**
   * @todo Under some platforms, notably Linux, the fragmentation code
   * in this method is woefully naive.  The fragments are sent it a
   * big burst, unfortunately, that can fill up the local kernel
   * buffer before all the data is sent.  In those circumstances some
   * of the fragments are silently (gulp!) dropped by the kernel,
   * check the documentation for sendto(2) specially the ENOBUFS
   * error condition.
   * There is no easy solution that I know off, except "pacing" the
   * fragments, i.e. never sending more than a prescribed number of
   * bytes per-second, sleeping before sending more or queueing them
   * to send later via the reactor.
   */
  void send_message (const TAO_OutputCDR &cdr,
                     const ACE_INET_Addr &addr
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Return the datagram...
  ACE_SOCK_Dgram& dgram (void);

  /**
   * Send one fragment, the first entry in the iovec is used to send
   * the header, the rest of the iovec array should contain pointers
   * to the actual data.
   */
  void send_fragment (const ACE_INET_Addr &addr,
                      CORBA::ULong request_id,
                      CORBA::ULong request_size,
                      CORBA::ULong fragment_size,
                      CORBA::ULong fragment_offset,
                      CORBA::ULong fragment_id,
                      CORBA::ULong fragment_count,
                      iovec iov[],
                      int iovcnt
                      ACE_ENV_ARG_DECL);

  /**
   * Count the number of fragments that will be required to send the
   * message blocks in the range [begin,end)
   * The maximum fragment payload (i.e. the size without the header is
   * also required); <total_length> returns the total message size.
   */
  CORBA::ULong compute_fragment_count (const ACE_Message_Block* begin,
                                       const ACE_Message_Block* end,
                                       int iov_size,
                                       CORBA::ULong max_fragment_payload,
                                       CORBA::ULong& total_length);

private:
  /// The datagram used for sendto ().
  TAO_ECG_Refcounted_Endpoint endpoint_rptr_;

  /// The MTU for this sender...
  CORBA::ULong mtu_;

  /// Should crc checksum be caluclated and sent?
  CORBA::Boolean checksum_;
};

#if defined(__ACE_INLINE__)
#include "ECG_CDR_Message_Sender.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_ECG_CDR_MESSAGE_SENDER_H */
