// -*- C++ -*-

//=============================================================================
/**
 *  @file Queued_Message.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_QUEUED_MESSAGE_H
#define TAO_QUEUED_MESSAGE_H
#include "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Message_Block;
class TAO_Message_Sent_Callback;

/**
 * @class TAO_Queued_Message
 *
 * @brief Represent messages queued in the outgoing data path of the
 *        TAO_Transport class.
 *
 * Please read the documentation in the TAO_Transport class to find
 * out more about the design of the outgoing data path.
 *
 * In some configurations TAO needs to maintain a per-connection queue
 * of outgoing messages.  This queue is drained by the pluggable
 * protocols framework, normally under control of the ACE_Reactor, but
 * other configurations are conceivable.  The elements in the queue
 * may may removed early, for example, because the application can
 * specify timeouts for each message, or because the underlying
 * connection is broken.
 *
 * In many cases the message corresponds to some application request,
 * the application may be blocked waiting for the request to be sent,
 * even more importantlyl, the ORB can be configured to use the
 * Leader/Followers strategy, in which case one of the waiting threads
 * can be required to wake up before its message completes
 * each message may contain a 'Sent_Notifier'
 *
 * <H4>NOTE:</H4> The contents of the ACE_Message_Block may have been
 * allocated from TSS storage, in that case we cannot steal them.
 * However, we do not need to perform a deep copy all the time, for
 * example, in a twoway request the sending thread blocks until the
 * data goes out.  The queued message can borrow the memory as it will
 * be deallocated by the sending thread when it finishes.
 * Oneways and asynchronous calls are another story.
 *
 * @todo: Change the ORB to allocate oneway and AMI buffer from global
 *        memory, to avoid the data copy in this path.  What happens
 *        if the there is no queueing?  Can we check that before
 *        allocating the memory?
 *
 */
class TAO_Export TAO_Queued_Message
{
public:
  /// Constructor
  /**
   * @param callback A callback interface to signal any waiting
   * threads about the status of the message.  It is null if there are
   * no waiting threads.
   */
  TAO_Queued_Message (TAO_Message_Sent_Callback *callback = 0);

  /// Destructor
  virtual ~TAO_Queued_Message (void);

  /// Return 0 if the message has not been completely sent
  int done (void) const;

  /// The underlying connection has been closed, release resources and
  /// signal waiting threads.
  void connection_closed (void);

  /// There was an error while sending the data.
  void send_failure (void);

  /** @name Intrusive list manipulation
   *
   * The messages are put in a doubled linked list (for easy insertion
   * and removal).  To minimize memory allocations the list is
   * intrusive, i.e. each element in the list contains the pointers
   * for the next and previous element.
   *
   * The following methods are used to manipulate this implicit list.
   *
   * @todo: We should implement this as a base template, something
   *        like:<BR>
   * template<class T> Intrusive_Node {<BR>
   * public:<BR><BR>
   *   void next (T *);<BR>
   *   T* next () const;<BR><BR>
   * private:<BR>
   *   T* next_;<BR>
   * };<BR>
   * and use it as follows:<BR>
   * class TAO_Queued_Message : public Intrusive_Node<TAO_Queued_Message><BR>
   * {<BR>
   * };<BR>
   *
   */
  //@{
  /// Set/get the next element in the list
  virtual TAO_Queued_Message *next (void) const;

  /// Set/get the previous element in the list
  virtual TAO_Queued_Message *prev (void) const;

  /// Remove this element from the list
  virtual void remove_from_list (TAO_Queued_Message *&head,
                                 TAO_Queued_Message *&tail);

  /// Insert the current element after position.
  /**
   * If position is null then we assume that we are inserting the
   * current element into an empty list.
   */
  virtual void push_back (TAO_Queued_Message *&head,
                          TAO_Queued_Message *&tail);
  //@}

  /** @name Template Methods
   */
  //@{

  /// Return the length of the message
  /**
   * If the message has been partially sent it returns the number of
   * bytes that are still not sent.
   */
  virtual size_t message_length (void) const = 0;

  /// Return 1 if all the data has been sent
  virtual int all_data_sent (void) const = 0;

  /// Fill up an io vector using the connects of the message
  /**
   * Different versions of this class represent the message using
   * either a single buffer, or a message block.
   * This method allows a derived class to fill up the contents of an
   * io vector, the TAO_Transport class uses this method to group as
   * many messages as possible in an iovector before sending them to
   * the OS I/O subsystem.
   *
   * @param iovcnt_max The number of elements in iov
   * @param iovcnt The number of elements already used by iov, this
   *               method should update this counter
   * @param iov The io vector
   */
  virtual void fill_iov (int iovcnt_max, int &iovcnt, iovec iov[]) const = 0;

  /// Update the internal state, data has been sent.
  /**
   * After the TAO_Transport class completes a successful (or
   * partially successful) I/O operation it must update the state of
   * all the messages queued.  This callback method is used by each
   * message to update its state and determine if all the data has
   * been sent already.
   *
   * @param byte_count The number of bytes succesfully sent.  The
   *                   TAO_Queued_Message should decrement this value
   *                   by the number of bytes that must still be sent.
   * @return Returns 1 if the TAO_Queued_Message has any more data to
   *         send.
   */
  virtual int bytes_transferred (size_t &byte_count) = 0;

  /// Reclaim resources
  /**
   * Reliable messages are allocated from the stack, thus they do not
   * be deallocated.
   * Asynchronous (SYNC_NONE) messages are allocated from the heap (or
   * a pool), they need to be reclaimed explicitly.
   */
  virtual void destroy (void) = 0;
  //@}

protected:
  /// Set to 1 if the connection was closed
  int connection_closed_;

  /// Set to 1 if there was a failure while sending the data
  int send_failure_;

  /// Set to 1 if there was a timeout while sending the data
  int timeout_;

private:
  /// If not null, this is the object that we signal to indicate that
  /// the message was sent.
  /**
   * The signaling mechanism used to wakeup the thread waiting for
   * this message to complete changes
   */
  TAO_Message_Sent_Callback *callback_;

  /// Implement an intrusive double-linked list for the message queue
  TAO_Queued_Message *next_;
  TAO_Queued_Message *prev_;
};

#if defined (__ACE_INLINE__)
# include "Queued_Message.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_QUEUED_MESSAGE_H */
