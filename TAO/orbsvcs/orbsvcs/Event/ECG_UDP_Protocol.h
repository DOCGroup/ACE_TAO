/* -*- C++ -*- */
/**
 * @file ECG_UDP_Protocol.h
 *
 * @brief Document the protocol to federate multiple Real-time Event
 * Channels via UDP.
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 *
 */
#ifndef TAO_ECG_UDP_PROTOCOL_H
#define TAO_ECG_UDP_PROTOCOL_H
#include "ace/pre.h"

#include "ace/config-all.h"

/**
 * @class TAO_ECG_UDP_Protocol
 *
 * @brief Define the protocol parameters to federate TAO's Real-time
 * Event Service via UDP.
 *
 * <H2>MESSAGE FORMAT</H2>
 * The messages header are encapsulated using CDR, with the
 * following format:
 *
 * <CODE>
 * struct Header {
 * octet byte_order_flags;<BR>
 * // bit 0 represents the byte order as in GIOP 1.1<BR>
 * // bit 1 is set if this is the last fragment<BR>
 * unsigned long request_id;<BR>
 * // The request ID, protocols must not send two requests with<BR>
 * // the same ID, protocols can be distinguished using recvfrom..<BR>
 * unsigned long request_size;<BR>
 * // The size of this request, this can be used to pre-allocate<BR>
 * // the request buffer.<BR>
 * unsgined long fragment_size;<BR>
 * // The size of this fragment, excluding the header...<BR>
 * unsigned long fragment_offset;<BR>
 * // Where does this fragment fit in the complete message...<BR>
 * unsigned long fragment_id;<BR>
 * // The ID of this fragment...<BR>
 * unsigned long fragment_count;<BR>
 * // The total number of fragments to expect in this request<BR>
 *<BR>
 * // @todo This could be eliminated if efficient reassembly<BR>
 * // could be implemented without it.<BR>
 * octet padding[4];<BR>
 *<BR>
 * // Ensures the header ends at an 8-byte boundary.<BR>
 * }; // size (in CDR stream) = 32<BR>
 *
 */
class TAO_RTEvent_Export TAO_ECG_UDP_Protocol
{
public:
  enum {
    ECG_HEADER_SIZE = 32,
    ECG_MIN_MTU = 32 + 8,
    ECG_MAX_MTU = 65536, // Really optimistic...
    ECG_DEFAULT_MTU = 1024
  };
};

#if defined(__ACE_INLINE__)
#include "ECG_UDP_Protocol.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_ECG_UDP_PROTOCOL_H */
