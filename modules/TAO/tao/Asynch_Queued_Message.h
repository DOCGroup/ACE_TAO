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

#include "tao/Queued_Message.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Time_Value.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Message_Block;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
   * @param alloc Allocator used for creating @c this object.
   *
   * @param timeout The relative timeout after which this
   * message should be expired.
   *
   * @todo I'm almost sure this class will require a callback
   *       interface for AMIs sent with SYNC_NONE policy.  Those guys
   *       need to hear when the connection timeouts or closes, but
   *       cannot block waiting for the message to be delivered.
   */
  TAO_Asynch_Queued_Message (const ACE_Message_Block *contents,
                             TAO_ORB_Core *oc,
                             ACE_Time_Value *timeout,
                             ACE_Allocator *alloc,
                             bool is_heap_allocated);


  /// Destructor
  virtual ~TAO_Asynch_Queued_Message (void);

  /**
   * @name Implement the Template Methods from TAO_Queued_Message
   */
  //@{
  virtual size_t message_length (void) const;
  virtual int all_data_sent (void) const;
  virtual void fill_iov (int iovcnt_max, int &iovcnt, iovec iov[]) const;
  virtual void bytes_transferred (size_t &byte_count);
  /// @note No reason to believe why this would be called. But have
  /// it here for the sake of uniformity.
  virtual TAO_Queued_Message *clone (ACE_Allocator *alloc);
  virtual void destroy (void);
  virtual bool is_expired (const ACE_Time_Value &now) const;
  virtual void copy_if_necessary (const ACE_Message_Block* chain);
  //@}

protected:
  /// Constructor
  /**
   * @param buf The buffer that needs to be sent on the wire. The
   *            buffer will be owned by this class. The buffer will be
   *            deleted when the destructor is called and hence the
   *            buffer should always come off the heap!
   *
   * @param oc The ORB Core
   *
   * @param size The size of the buffer <buf> that is being handed
   *             over.
   *
   * @param abs_timeout The time after which this  message should be expired.
   *
   * @param alloc Allocator used for creating <this> object.
   */
  TAO_Asynch_Queued_Message (char *buf,
                             TAO_ORB_Core *oc,
                             size_t size,
                             const ACE_Time_Value &abs_timeout,
                             ACE_Allocator *alloc,
                             bool is_heap_allocated);

private:
  void operator= (const TAO_Asynch_Queued_Message &);
  TAO_Asynch_Queued_Message (const TAO_Asynch_Queued_Message &);

private:
  /// The number of bytes in the buffer
  size_t const size_;

  /// The offset in the buffer
  /**
   * Data up to @c offset has been sent already, only the
   * [offset_,size_) range remains to be sent.
   */
  size_t offset_;

  /// The buffer containing the complete message.
  char *buffer_;

  // Expiration time
  ACE_Time_Value abs_timeout_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif  /* TAO_ASYNCH_QUEUED_MESSAGE_H */
