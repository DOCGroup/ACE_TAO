// -*- C++ -*-

//=============================================================================
/**
 *  @file Asynch_Queued_Message.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_ASYNCH_QUEUED_MESSAGE_H
#define TAO_ASYNCH_QUEUED_MESSAGE_H
#include /**/ "ace/pre.h"

#include "Queued_Message.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Asynch_Queued_Message
 *
 * @brief Specialize TAO_Queued_Message for asynch requests,
 *        i.e. oneways sent with SYNC_NONE policy.
 *
 */
class TAO_Export TAO_Asynch_Queued_Message : public TAO_Queued_Message
{
public:
  /// Constructor
  /**
   * @param contents The message block chain that must be sent.
   *
   * @param alloc Allocator used for creating <this> object.
   *
   * @todo I'm almost sure this class will require a callback
   *       interface for AMIs sent with SYNC_NONE policy.  Those guys
   *       need to hear when the connection timeouts or closes, but
   *       cannot block waiting for the message to be delivered.
   */
  TAO_Asynch_Queued_Message (const ACE_Message_Block *contents,
                             ACE_Allocator *alloc = 0,
                             int is_heap_allocated = 0);


  /// Destructor
  virtual ~TAO_Asynch_Queued_Message (void);

  /** Implement the Template Methods from TAO_Queued_Message
   */
  //@{
  virtual size_t message_length (void) const;
  virtual int all_data_sent (void) const;
  virtual void fill_iov (int iovcnt_max, int &iovcnt, iovec iov[]) const;
  virtual void bytes_transferred (size_t &byte_count);
  /// @@NOTE: No reason to belive why this would be called. But have
  /// it here for the sake of uniformity.
  virtual TAO_Queued_Message *clone (ACE_Allocator *alloc);
  virtual void destroy (void);
  //@}

protected:
  /// Constructor
  /**
   * @param buf The buffer that needs to be sent on the wire. The
   *            buffer will be owned by this class. The buffer will be
   *            deleted when the destructor is called and hence the
   *            buffer should always come off the heap!
   *
   * @param size The size of the buffer <buf> that is being handed
   *             over.
   *
   * @param alloc Allocator used for creating <this> object.
   */
  TAO_Asynch_Queued_Message (char *buf,
                             size_t size,
                             ACE_Allocator *alloc = 0);
private:
  /// The number of bytes in the buffer
  size_t size_;

  /// The offset in the buffer
  /**
   * Data up to @c offset has been sent already, only the
   * [offset_,size_) range remains to be sent.
   */
  size_t offset_;

  /// The buffer containing the complete message.
  char *buffer_;
};

#include /**/ "ace/post.h"
#endif  /* TAO_ASYNCH_QUEUED_MESSAGE_H */
