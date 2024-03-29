// -*- C++ -*-
/**
 *  @file ECG_CDR_Message_Receiver.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *  @author Marina Spivak (marina@atdesk.com)
 */

#ifndef TAO_ECG_CDR_MESSAGE_RECEIVER_H
#define TAO_ECG_CDR_MESSAGE_RECEIVER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Event/ECG_UDP_Out_Endpoint.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CDR.h"

#include "ace/Hash_Map_Manager.h"
#include "ace/INET_Addr.h"
#include "ace/Null_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_ECG_CDR_Processor
 *
 * @brief Interface for callback objects used by
 *        TAO_ECG_CDR_Message_Receiver to propagate received data to
 *        its callers.
 */
class TAO_ECG_CDR_Processor
{
public:
  virtual ~TAO_ECG_CDR_Processor ();

  /// Extracts data from @a cdr.  Returns 0 on success, -1 on error.
  virtual int decode (TAO_InputCDR &cdr) = 0;
};

// ****************************************************************
/**
 * @class TAO_ECG_UDP_Request_Entry
 *
 * @brief Keeps information about an incomplete request.
 *
 * When a request arrives in fragments this object is used to
 * keep track of the incoming data.
 */
class TAO_ECG_UDP_Request_Entry
{
public:
  enum {
    ECG_DEFAULT_FRAGMENT_BUFSIZ = 8
  };

  /// Initialize the fragment, allocating memory, etc.
  TAO_ECG_UDP_Request_Entry (CORBA::Boolean byte_order,
                             CORBA::ULong request_id,
                             CORBA::ULong request_size,
                             CORBA::ULong fragment_count);

  ~TAO_ECG_UDP_Request_Entry ();

  /// Validate a fragment, it should be rejected if it is invalid..
  int validate_fragment (CORBA::Boolean byte_order,
                         CORBA::ULong request_size,
                         CORBA::ULong fragment_size,
                         CORBA::ULong fragment_offset,
                         CORBA::ULong fragment_id,
                         CORBA::ULong fragment_count) const;

  /// Has @a fragment_id been received?
  int test_received (CORBA::ULong fragment_id) const;

  /// Mark @a fragment_id as received, reset timeout counter...
  void mark_received (CORBA::ULong fragment_id);

  /// Is the message complete?
  int complete () const;

  /// Return a buffer for the fragment at offset @a fragment_offset
  char* fragment_buffer (CORBA::ULong fragment_offset);

private:
  TAO_ECG_UDP_Request_Entry (const TAO_ECG_UDP_Request_Entry & rhs);
  TAO_ECG_UDP_Request_Entry& operator= (const TAO_ECG_UDP_Request_Entry & rhs);

private:
  /// This attributes should remain constant in all the fragments, used
  /// for validation....
  CORBA::Boolean byte_order_;
  CORBA::ULong request_id_;
  CORBA::ULong request_size_;
  CORBA::ULong fragment_count_;

  ACE_Message_Block payload_;

  /// This is a bit vector, used to keep track of the received buffers.
  CORBA::ULong* received_fragments_;
  int own_received_fragments_;
  CORBA::ULong received_fragments_size_;
  CORBA::ULong default_received_fragments_[ECG_DEFAULT_FRAGMENT_BUFSIZ];
};

// ****************************************************************

/**
 * @class TAO_ECG_CDR_Message_Receiver
 *
 * @brief Receives UDP and Multicast messages.
 *
 * @todo Update class documentation below.
 *
 *       5)  Make status array size and purge_count configurable.
 *
 * This class receives UDP and Multicast message fragments, assembles
 * them (described in detail below), and passes complete messages
 * in the form of cdr streams to the calling classes.
 *
 * This class is used by various Gateway classes (Senders/Receivers)
 * responsible for federating Event Channels with UDP/Mcast.
 *
 * = REASSEMBLY
 * Fragmentation is described in ECG_CDR_Message_Sender.h
 * Whenever an incomplete fragment is received (one with
 * fragment_count > 1) we allocate an entry for the message in an
 * map indexed by (host,port,request_id).  The entry contains the
 * buffer, a bit vector to keep track of the fragments received
 * so far, and a timeout counter.  This timeout counter is set to
 * 0 on each (new) fragment arrival, and incremented on a regular
 * basis.  If the counter reaches a maximum value the message is
 * dropped.
 * Once all the fragments have been received the message is sent
 * up to the calling classes, and the memory reclaimed.
 */
class TAO_RTEvent_Serv_Export TAO_ECG_CDR_Message_Receiver
{
public:
  /// Initialization and termination methods.
  //@{
  TAO_ECG_CDR_Message_Receiver (CORBA::Boolean check_crc);
  ~TAO_ECG_CDR_Message_Receiver ();

  /**
   * @param ignore_from Endpoint used to remove events generated by
   *        the same process.
   */
  void init (TAO_ECG_Refcounted_Endpoint ignore_from
             /* , ACE_Lock *lock = 0 */);

  // Shutdown the component: close down the request map, etc.
  void shutdown ();
  //@}

  /// Main method: read the data from @a dgram and either pass ready data
  /// to @a cdr_processor or update the <request_map_> if the request
  /// is not yet complete.
  /**
   * Returns 1 if data was read successfully and accepted by
   * <cdr_processor> without errors.
   * Returns 0 if there were no errors, but no data has been passed to
   * <cdr_processor>, either due to request being incomplete (not all
   * fragments received), or it being a duplicate.
   * Returns -1 if there were errors.
   */
  int handle_input (ACE_SOCK_Dgram& dgram,
                    TAO_ECG_CDR_Processor *cdr_processor);

  /// Represents any request that has been fully received and
  /// serviced, to simplify the internal logic.
  static TAO_ECG_UDP_Request_Entry Request_Completed_;

private:
  enum {
    ECG_DEFAULT_MAX_FRAGMENTED_REQUESTS = 1024,
    ECG_DEFAULT_FRAGMENTED_REQUESTS_MIN_PURGE_COUNT = 32
  };

  struct Mcast_Header;
  class Requests;

  typedef ACE_Hash_Map_Manager<ACE_INET_Addr,
                               Requests*,
                               ACE_Null_Mutex> Request_Map;

private:
  /// Returns 1 on success, 0 if <request_id> has already been
  /// received or is below current request range, and -1 on error.
  int mark_received (const ACE_INET_Addr &from,
                     CORBA::ULong request_id);

  /// Returns 1 if complete request is received and <event> is
  /// populated, 0 if request has only partially been received or is a
  /// duplicate, and -1 on error.
  int process_fragment (const ACE_INET_Addr &from,
                        const Mcast_Header &header,
                        char * data_buf,
                        TAO_ECG_CDR_Processor *cdr_processor);


  Request_Map::ENTRY* get_source_entry (const ACE_INET_Addr &from);

private:
  /// Ignore any events coming from this IP address.
  TAO_ECG_Refcounted_Endpoint ignore_from_;

  /// The map containing all the incoming requests which have been
  /// partially received.
  Request_Map request_map_;

  /// Serializes use of <request_map_>.
  //  ACE_Lock* lock_;

  /// Size of a fragmented requests array, i.e., max number of
  /// partially received requests kept at any given time per source.
  size_t max_requests_;

  /// Minimum number of requests purged from a fragmented requests
  /// array when the range of requests represented there needs to be
  /// shifted.
  size_t min_purge_count_;

  /// Flag to indicate whether CRC should be computed and checked.
  CORBA::Boolean check_crc_;
};

// ****************************************************************

/// Helper for decoding, validating and storing mcast header.
struct TAO_ECG_CDR_Message_Receiver::Mcast_Header
{
  int byte_order;
  CORBA::ULong request_id;
  CORBA::ULong request_size;
  CORBA::ULong fragment_size;
  CORBA::ULong fragment_offset;
  CORBA::ULong fragment_id;
  CORBA::ULong fragment_count;
  CORBA::ULong crc;
  int read (char * header,
            size_t bytes_received,
            CORBA::Boolean checkcrc = 0);
};

// ****************************************************************

/// Once init() has been called:
/// Invariant: id_range_high_- id_range_low_ == size_ - 1
class TAO_ECG_CDR_Message_Receiver::Requests
{
public:
  Requests ();
  ~Requests ();

  /// Allocates and initializes <fragmented_requests_>.
  int init (size_t size, size_t min_purge_count);

  /// Returns pointer to a <fragmented_requests_> element
  /// representing <request_id>.
  /**
   * If <request_id> < <id_range_low> return 0.
   * If <request_id> > <id_range_high>, shift the range so it
   * includes <request_id>, purging incomplete requests as needed.
   */
  TAO_ECG_UDP_Request_Entry ** get_request (CORBA::ULong request_id);

private:
  /// Delete any outstanding requests with ids in the range
  /// [<purge_first>, <purge_last>] from <fragmented_requests> and
  /// and reset their slots.
  void purge_requests (CORBA::ULong purge_first,
                       CORBA::ULong purge_last);

  Requests & operator= (const Requests &rhs);
  Requests (const Requests &rhs);

private:
  /// Array, used in a circular fashion, that stores partially received
  /// requests (and info on which requests have been fully received
  /// and processed) for a range of request ids.
  TAO_ECG_UDP_Request_Entry** fragmented_requests_;

  /// Size of <fragmented_requests_> array.
  size_t size_;

  /// The range of request ids, currently represented in
  /// <fragmented_requests>.
  //@{
  CORBA::ULong id_range_low_;
  CORBA::ULong id_range_high_;
  //@}

  /// Minimum range shifting amount.
  size_t min_purge_count_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "orbsvcs/Event/ECG_CDR_Message_Receiver.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_ECG_CDR_MESSAGE_RECEIVER_H */
