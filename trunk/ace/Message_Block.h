/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Message_Block.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#ifndef ACE_MESSAGE_BLOCK_H
#define ACE_MESSAGE_BLOCK_H
#include "ace/pre.h"

#include "ace/Malloc.h"

// Forward declaration.
class ACE_Data_Block;
class ACE_Lock;
class ACE_Time_Value;

/**
 * @class ACE_Message_Block
 *
 * @brief Stores messages for use throughout ACE (particularly
 * <ACE_Message_Queue>).
 *
 * An <ACE_Message_Block> is modeled after the message data
 * structures used in System V STREAMS.  Its purpose is to
 * enable efficient manipulation of arbitrarily-large messages
 * without incurring much memory copying overhead.  Here are the
 * main characteristics of an <ACE_Message_Block>:
 * 1. Contains a pointer to a reference-counted
 *    <ACE_Data_Block>, which in turn points to the actual data
 *    buffer.  This allows very flexible and efficient sharing of
 *    data by multiple <ACE_Message_Block>s.
 * 2. One or more <ACE_Message_Blocks> can be linked to form a
 *    ``fragment chain.''
 * 3. <ACE_Message_Blocks> can be linked together by <prev_> and
 *    <next_> pointers to form a queue of messages (e.g., this is how
 *    <ACE_Message_Queue> works).
 */
class ACE_Export ACE_Message_Block
{
public:
  friend class ACE_Data_Block;

  enum
  {
    // = Data and proto
    /// regular datacol messages (regular and priority)
    MB_DATA     = 0x01,
    /// protocol control
    MB_PROTO    = 0x02,

    // = Control messag
    /// line breakes (regular and priority)
    MB_BREAK    = 0x03,
    /// pass file pointer
    MB_PASSFP   = 0x04,
    /// post an event to an event queue
    MB_EVENT    = 0x05,
    /// generate process signal
    MB_SIG      = 0x06,
    /// ioctl; set/get params
    MB_IOCTL    = 0x07,
    /// set various stream head options
    MB_SETOPTS  = 0x08,

    // = Control messag
    /// acknowledge ioctles (high priority; go to head of queue)
    MB_IOCACK   = 0x81,
    /// negative ioctl acknowledge
    MB_IOCNAK   = 0x82,
    /// priority proto message
    MB_PCPROTO  = 0x83,
    /// generate process signal
    MB_PCSIG    = 0x84,
    /// generate read notification
    MB_READ     = 0x85,
    /// flush your queues
    MB_FLUSH    = 0x86,
    /// stop transmission immediately
    MB_STOP     = 0x87,
    /// restart transmission after stop
    MB_START    = 0x88,
    /// line disconnect
    MB_HANGUP   = 0x89,
    /// fatal error used to set u.u_error
    MB_ERROR    = 0x8a,
    /// post an event to an event queue
    MB_PCEVENT  = 0x8b,

    // Message class ma
    /// Normal priority messagessks
    MB_NORMAL   = 0x00,
    /// High priority control messages
    MB_PRIORITY = 0x80,
    /// User-defined control messages
    MB_USER     = 0x200
  };

  typedef int ACE_Message_Type;
  typedef u_long Message_Flags;

  enum
  {
    /// Don't delete the data on exit since we don't own it.
    DONT_DELETE = 01,
    /// user defined flags start here
    USER_FLAGS = 0x1000
  };

  // = Initialization and termination.
  /// Create an empty message.
  ACE_Message_Block (ACE_Allocator *message_block_allocator = 0);

  /// Create an <ACE_Message_Block> that owns the <ACE_Data_Block> *
  /// without copying it. If the <flags> is set to DONT_DELETE we
  /// own't delete the ACE_Data_Block. It is left to the client's
  /// responsibility to take care of the memory allocated for the
  /// data_block
  ACE_Message_Block (ACE_Data_Block *,
                     Message_Flags flags = 0,
                     ACE_Allocator *message_block_allocator = 0);

  /**
   * Create a Message Block that assumes ownership of <data> without
   * copying it (i.e., we don't delete it since we don't malloc it!).
   * Note that the <size> of the <Message_Block> will be <size>, but
   * the <length> will be 0 until <wr_ptr> is set.
   */
  ACE_Message_Block (const char *data,
                     size_t size = 0,
                     u_long priority = ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY);

  /**
   * Create an initialized message of type <type> containing <size>
   * bytes.  The <cont> argument initializes the continuation field in
   * the <Message_Block>.  If <data> == 0 then we create and own the
   * <data>, using <allocator> to get the data if it's non-0.  If
   * <data> != 0 we assume ownership of the <data> (and don't delete
   * it).  If <locking_strategy> is non-0 then this is used to protect
   * regions of code that access shared state (e.g., reference
   * counting) from race conditions.  Note that the <size> of the
   * <Message_Block> will be <size>, but the <length> will be 0 until
   * <wr_ptr> is set.
   * The <data_block_allocator> is use to allocate the data blocks
   * while the <allocator_strategy> is used to allocate the buffers
   * contained by those.
   * The <message_block_allocator> is used to allocate new
   * <Message_Block> objects when a duplicate method is called.  If
   * a <message_block_allocator> is given, this <Message_Block> and
   * future <Message_Block> objects created by duplicate will be free'ed
   * into this allocator when they are released.  Note: if you use this
   * allocator, the <Message_Block> you created should have been created
   * using this allocator because it will be released to the same allocator.
   */
  ACE_Message_Block (size_t size,
                     ACE_Message_Type type = MB_DATA,
                     ACE_Message_Block *cont = 0,
                     const char *data = 0,
                     ACE_Allocator *allocator_strategy = 0,
                     ACE_Lock *locking_strategy = 0,
                     u_long priority = ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY,
                     const ACE_Time_Value &execution_time = ACE_Time_Value::zero,
                     const ACE_Time_Value &deadline_time = ACE_Time_Value::max_time,
                     ACE_Allocator *data_block_allocator = 0,
                     ACE_Allocator *message_block_allocator = 0);

  /**
   * A copy constructor. This constructor is a bit different. If the
   * incoming Message Block has a data block from the stack this
   * constructor does a deep copy ie. allocates a new data block on
   * the heap and does a copy of the data from the incoming message
   * block. As a final note, the alignment information is used to
   * align the data block if it is created afresh. If the incoming
   * <mb> has a data block has a data block allocated from the heap,
   * then this constructor just duplicates (ie. a shallow copy) the
   * data block of the incoming <mb>.
   */
  ACE_Message_Block (const ACE_Message_Block &mb,
                     size_t align);

  /**
   * Create a Message Block that assumes ownership of <data> (i.e.,
   * doesn't delete it since it didn't malloc it!).  Note that the
   * <size> of the <Message_Block> will be <size>, but the <length>
   * will be 0 until <wr_ptr> is set.
   */
  int init (const char *data,
            size_t size = 0);

  /**
   * Create an initialized message of type <type> containing <size>
   * bytes.  The <cont> argument initializes the continuation field in
   * the <Message_Block>.  If <data> == 0 then we create and own the
   * <data>, using <allocator> to get the data if it's non-0.  If
   * <data> != 0 we assume ownership of the <data> (and don't delete
   * it).  If <locking_strategy> is non-0 then this is used to protect
   * regions of code that access shared state (e.g., reference
   * counting) from race conditions.  Note that the <size> of the
   * <Message_Block> will be <size>, but the <length> will be 0 until
   * <wr_ptr> is set.
   * The <data_block_allocator> is use to allocate the data blocks
   * while the <allocator_strategy> is used to allocate the buffers
   * contained by those.
   */
  int init (size_t size,
            ACE_Message_Type type = MB_DATA,
            ACE_Message_Block *cont = 0,
            const char *data = 0,
            ACE_Allocator *allocator_strategy = 0,
            ACE_Lock *locking_strategy = 0,
            u_long priority = ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY,
            const ACE_Time_Value &execution_time = ACE_Time_Value::zero,
            const ACE_Time_Value &deadline_time = ACE_Time_Value::max_time,
            ACE_Allocator *data_block_allocator = 0,
            ACE_Allocator *message_block_allocator = 0);

  /**
   * Delete all the resources held in the message.
   *
   * Note that <release> is designed to release the continuation
   * chain; the destructor is not. See <release> for details.
   */
  virtual ~ACE_Message_Block (void);

  // = Message Type accessors and mutators.

  /// Get type of the message.
  ACE_Message_Type msg_type (void) const;

  /// Set type of the message.
  void msg_type (ACE_Message_Type type);

  /// Find out what type of message this is.
  int is_data_msg (void) const;

  /// Find out what class of message this is (there are two classes,
  /// <normal> messages and <high-priority> messages).
  ACE_Message_Type msg_class (void) const;

  // = Message flag accessors and mutators.
  /// Bitwise-or the <more_flags> into the existing message flags and
  /// return the new value.
  Message_Flags set_flags (Message_Flags more_flags);

  /// Clear the message flag bits specified in <less_flags> and return
  /// the new value.
  Message_Flags clr_flags (Message_Flags less_flags);

  /// Get the current message flags.
  Message_Flags flags (void) const;

  // = Data Block flag accessors and mutators.
  /// Bitwise-or the <more_flags> into the existing message flags and
  /// return the new value.
  /* @todo: I think the following set of methods could not be used at
   *  all. May be they are useless. Let us have it so that we dont
   *  mess up memory management of the Message_Block. Somebody correct
   *  me if I am totally totally wrong..
   */
  Message_Flags set_self_flags (ACE_Message_Block::Message_Flags more_flags);

  /// Clear the message flag bits specified in <less_flags> and return
  /// the new value.
  Message_Flags clr_self_flags (ACE_Message_Block::Message_Flags less_flags);

  /// Get the current message flags.
  Message_Flags self_flags (void) const;

  /// Get priority of the message.
  u_long msg_priority (void) const;

  /// Set priority of the message.
  void msg_priority (u_long priority);

  /// Get execution time associated with the message.
  const ACE_Time_Value &msg_execution_time (void) const;

  /// Set execution time associated with the message.
  void msg_execution_time (const ACE_Time_Value &et);

  /// Get absolute time of deadline associated with the message.
  const ACE_Time_Value &msg_deadline_time (void) const;

  /// Set absolute time of deadline associated with the message.
  void msg_deadline_time (const ACE_Time_Value &dt);

  // = Deep copy and shallow copy methods.

  /// Return an exact "deep copy" of the message, i.e., create fresh
  /// new copies of all the Data_Blocks and continuations.
  virtual ACE_Message_Block *clone (Message_Flags mask = 0) const;

  /// Return a "shallow" copy that increments our reference count by 1.
  ACE_Message_Block *duplicate (void) const;


  /**
   * Return a "shallow" copy that increments our reference count by 1.
   * This is similar to CORBA's <_duplicate> method, which is useful
   * if you want to eliminate lots of checks for NULL <mb> pointers
   * before calling <_duplicate> on them.
   */
  static ACE_Message_Block *duplicate (const ACE_Message_Block *mb);


  /**
   * Decrease the shared ACE_Data_Block's reference count by 1.  If the
   * ACE_Data_Block's reference count goes to 0, it is deleted.
   * In all cases, this ACE_Message_Block is deleted - it must have come
   * from the heap, or there will be trouble.
   *
   * <release> is designed to release the continuation chain; the
   * destructor is not.  If we make the destructor release the
   * continuation chain by calling <release> or delete on the message
   * blocks in the continuation chain, the following code will not
   * work since the message block in the continuation chain is not off
   * the heap:
   *
   *  ACE_Message_Block mb1 (1024);
   *  ACE_Message_Block mb2 (1024);
   *
   *  mb1.cont (&mb2);
   *
   * And hence, call <release> on a dynamically allocated message
   * block. This will release all the message blocks in the
   * continuation chain.  If you call delete or let the message block
   * fall off the stack, cleanup of the message blocks in the
   * continuation chain becomes the responsibility of the user.
   */
  ACE_Message_Block *release (void);

  /**
   * This behaves like the non-static method <release>, except that it
   * checks if <mb> is 0.  This is similar to <CORBA::release>, which
   * is useful if you want to eliminate lots of checks for NULL
   * pointers before calling <release> on them.  Returns <mb>.
   */
  static ACE_Message_Block *release (ACE_Message_Block *mb);


  // = Operations on Message data

  /**
   * Copies <n> bytes from <buf> into the Message_Block starting at
   * the <wr_ptr> offset.  Return 0 and increment <wr_ptr> by <n> if
   * the method succeeds.  Returns -1 if the size of the message is
   * too small, i.e., for this to work correct, <end> must be >=
   * <wr_ptr>.
   */
  int copy (const char *buf, size_t n);

  /**
   * Copies <buf> into the Message_Block starting at the <wr_ptr>
   * offset.  This call assumes that <buf> is NUL-terminated.  Return
   * 0 and increment <wr_ptr> by <ACE_OS::strlen (buf) + 1> if the
   * method succeeds.  Returns -1 if the size of the message is too
   * small, i.e., for this to work correct, <end> must be >= <wr_ptr>.
   */
  int copy (const char *buf);

  /// Normalizes data in the top-level <Message_Block> to align with the base,
  /// i.e., it "shifts" the data pointed to by <rd_ptr> down to the <base> and
  /// then readjusts <rt_ptr> to point to <base> and <wr_ptr> to point
  /// to <base> + the length of the moved data.
  void crunch (void);

  /// Resets the Message Block data to contain nothing, i.e., sets the
  /// read and write pointers to align with the base.
  void reset (void);

  /// Get message data.
  char *base (void) const;

  /// Set message data (doesn't reallocate).
  void base (char *data,
             size_t size,
             Message_Flags = DONT_DELETE);

  /// Return a pointer to 1 past the end of the allocated data in a message.
  char *end (void) const;

  /**
   * Return a pointer to 1 past the end of the allotted data in a message.
   * Allotted data may be less than allocated data  if a value smaller than
   * capacity() to is passed to size().
   */
  char *mark (void) const;

  /**
   * Get the read pointer.
   * Set the read pointer to <ptr>.
   * Set the read pointer ahead <n> bytes.
   */
  char *rd_ptr (void) const;
  void rd_ptr (char *ptr);
  void rd_ptr (size_t n);

  /**
   * Get the write pointer.
   * Set the write pointer to <ptr>.
   * Set the write pointer ahead <n> bytes.  This is used to compute
   * the <length> of a message.
   */
  char *wr_ptr (void) const;
  void wr_ptr (char *ptr);
  void wr_ptr (size_t n);

  /** @name Message length and size operations
   *
   * Message length is (wr_ptr - rd_ptr).
   *
   * Message size is capacity of the message, including data outside
   * the [rd_ptr,wr_ptr] range.
   */
  //@{
  /// Get the length of the message
  size_t length (void) const;

  /// Set the length of the message
  void length (size_t n);

  /// Get the length of the <Message_Block>s, including chained
  /// <Message_Block>s.
  size_t total_length (void) const;

  /// Get the total number of bytes in all <Message_Block>s, including
  /// chained <Message_Block>s.
  size_t total_size (void) const;

  /// Get the number of bytes in the top-level <Message_Block> (i.e.,
  /// does not consider the bytes in chained <Message_Block>s).
  size_t size (void) const;

  /**
   * Set the number of bytes in the top-level <Message_Block>,
   * reallocating space if necessary.  However, the <rd_ptr_> and
   * <wr_ptr_> remain at the original offsets into the buffer, even if
   * it is reallocated.  Returns 0 if successful, else -1.
   */
  int size (size_t length);

  /// Get the number of allocated bytes in all <Message_Block>, including
  /// chained <Message_Block>s.
  size_t total_capacity (void) const;

  /// Get the number of allocated bytes in the top-level <Message_Block>.
  size_t capacity (void) const;

  /// Get the number of bytes available after the <wr_ptr_> in the
  /// top-level <Message_Block>.
  size_t space (void) const;
  //@}

  // = <ACE_Data_Block> methods.

  /**
   * Get a pointer to the data block. Note that the <ACE_Message_Block>
   * still references the block; this call does not change the reference
   * count.
   */
  ACE_Data_Block *data_block (void) const;

  /**
   * Set a new data block pointer. The original <ACE_Data_Block> is released
   * as a result of this call. If you need to keep the original block, call
   * <replace_data_block> instead. Upon return, this <ACE_Message_Block>
   * holds a pointer to the new <ACE_Data_Block>, taking over the reference
   * you held on it prior to the call.
   */
  void data_block (ACE_Data_Block *);

  /// Set a new data block pointer. A pointer to the original <ACE_Data_Block>
  /// is returned, and not released (as it is with <data_block>).
  ACE_Data_Block *replace_data_block (ACE_Data_Block*);

  // = The continuation field chains together composite messages.
  /// Get the continuation field.
  /// Set the continuation field.
  ACE_Message_Block *cont (void) const;
  void cont (ACE_Message_Block *);

  // = Pointer to the <Message_Block> directly ahead in the <ACE_Message_Queue>.
  /// Get link to next message.
  /// Set link to next message.
  ACE_Message_Block *next (void) const;
  void next (ACE_Message_Block *);

  // = Pointer to the <Message_Block> directly behind in the <ACE_Message_Queue>.
  /// Get link to prev message.
  /// Set link to prev message.
  ACE_Message_Block *prev (void) const;
  void prev (ACE_Message_Block *);

  // = The locking strategy prevents race conditions.
  /// Get the locking strategy.
  /// Set a new locking strategy and return the hold one.
  ACE_Lock *locking_strategy (void);
  ACE_Lock *locking_strategy (ACE_Lock *);

  /// Get the current reference count.
  int reference_count (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  // = Internal initialization methods.
  /// Perform the actual initialization.
  ACE_Message_Block (size_t size,
                     ACE_Message_Type type,
                     ACE_Message_Block *cont,
                     const char *data,
                     ACE_Allocator *allocator_strategy,
                     ACE_Lock *locking_strategy,
                     Message_Flags flags,
                     u_long priority,
                     const ACE_Time_Value &execution_time,
                     const ACE_Time_Value &deadline_time,
                     ACE_Data_Block *db,
                     ACE_Allocator *data_block_allocator,
                     ACE_Allocator *message_block_allocator);

  /// Internal release implementation
  /// Returns 1 if the data block has to be destroyed.
  int release_i (ACE_Lock *lock);

  /// Perform the actual initialization.
  int init_i (size_t size,
              ACE_Message_Type type,
              ACE_Message_Block *cont,
              const char *data,
              ACE_Allocator *allocator_strategy,
              ACE_Lock *locking_strategy,
              Message_Flags flags,
              u_long priority,
              const ACE_Time_Value &execution_time,
              const ACE_Time_Value &deadline_time,
              ACE_Data_Block *db,
              ACE_Allocator *data_block_allocator,
              ACE_Allocator *message_block_allocator);

  /// Pointer to beginning of next read.
  size_t rd_ptr_;

  /// Pointer to beginning of next write.
  size_t wr_ptr_;

  /// Priority of message.
  u_long priority_;

#if defined (ACE_HAS_TIMED_MESSAGE_BLOCKS)
  /// execution time associated with the message
  ACE_Time_Value execution_time_;

  /// absolute deadline time for message
  ACE_Time_Value deadline_time_;
#endif /* ACE_HAS_TIMED_MESSAGE_BLOCKS */

  // = Links to other ACE_Message_Block *s.
  /// Pointer to next message block in the chain.
  ACE_Message_Block *cont_;

  /// Pointer to next message in the list.
  ACE_Message_Block *next_;

  /// Pointer to previous message in the list.
  ACE_Message_Block *prev_;

  /// Misc flags (e.g., DONT_DELETE and USER_FLAGS).
  ACE_Message_Block::Message_Flags flags_;

  /// Pointer to the reference counted data structure that contains the
  /// actual memory buffer.
  ACE_Data_Block *data_block_;

  /// The allocator used to destroy ourselves when release is called
  /// and create new message blocks on duplicate.
  ACE_Allocator *message_block_allocator_;

private:
  // = Disallow these operations for now (use <clone> instead).
  ACE_Message_Block &operator= (const ACE_Message_Block &);
  ACE_Message_Block (const ACE_Message_Block &);
};

/**
 * @class ACE_Data_Block
 *
 * @brief Stores the data payload that is accessed via one or more
 * <ACE_Message_Block>s.
 *
 * This data structure is reference counted to maximize
 * sharing.  It also contains the <locking_strategy_> (which
 * protects the reference count from race conditions in
 * concurrent programs) and the <allocation_strategy_> (which
 * determines what memory pool is used to allocate the memory).
 */
class ACE_Export ACE_Data_Block
{
public:
  // = Initialization and termination methods.
  /// Default "do-nothing" constructor.
  ACE_Data_Block (void);

  /// Initialize.
  ACE_Data_Block (size_t size,
                  ACE_Message_Block::ACE_Message_Type msg_type,
                  const char *msg_data,
                  ACE_Allocator *allocator_strategy,
                  ACE_Lock *locking_strategy,
                  ACE_Message_Block::Message_Flags flags,
                  ACE_Allocator *data_block_allocator);

  /// Delete all the resources held in the message.
  virtual ~ACE_Data_Block (void);

  /// Get type of the message.
  ACE_Message_Block::ACE_Message_Type msg_type (void) const;

  /// Set type of the message.
  void msg_type (ACE_Message_Block::ACE_Message_Type type);

  /// Get message data pointer
  char *base (void) const;

  /// Set message data pointer (doesn't reallocate).
  void base (char *data,
             size_t size,
             ACE_Message_Block::Message_Flags mflags = ACE_Message_Block::DONT_DELETE);

  /// Return a pointer to 1 past the end of the allocated data in a message.
  char *end (void) const;

  /**
   * Return a pointer to 1 past the end of the allotted data in a message.
   * The allotted data may be less than allocated data if <size()> is passed
   * an argument less than <capacity()>.
   */
  char *mark (void) const;

  // = Message size is the total amount of space alloted.

  /// Get the total amount of allotted space in the message.  The amount of
  /// allotted space may be less than allocated space.
  size_t size (void) const;

  /// Set the total amount of space in the message.  Returns 0 if
  /// successful, else -1.
  int size (size_t length);

  /// Get the total amount of allocated space.
  size_t capacity (void) const;

  /**
   * Return an exact "deep copy" of the message, i.e., create fresh
   * new copies of all the Data_Blocks and continuations.
   * Notice that Data_Blocks can act as "Prototypes", i.e. derived
   * classes can override this method and create instances of
   * themselves.
   */
  virtual ACE_Data_Block *clone (ACE_Message_Block::Message_Flags mask = 0) const;

  /**
   * As clone above, but it does not copy the contents of the buffer,
   * i.e., create a new Data_Block of the same dynamic type, with the
   * same allocator, locking_strategy, and with the same amount of
   * storage available but the buffer is unitialized.
   */
  virtual ACE_Data_Block *clone_nocopy (ACE_Message_Block::Message_Flags mask = 0) const;

  /// Return a "shallow" copy that increments our reference count by 1.
  ACE_Data_Block *duplicate (void);

  /**
   * Decrease the shared reference count by 1.  If the reference count
   * is > 0 then return this; else if reference count == 0 then delete
   * <this> and <mb> and return 0.  Behavior is undefined if reference
   * count < 0.
   */
  ACE_Data_Block *release (ACE_Lock *lock = 0);

  // = Message flag accessors and mutators.
  /// Bitwise-or the <more_flags> into the existing message flags and
  /// return the new value.
  ACE_Message_Block::Message_Flags set_flags (ACE_Message_Block::Message_Flags more_flags);

  /// Clear the message flag bits specified in <less_flags> and return
  /// the new value.
  ACE_Message_Block::Message_Flags clr_flags (ACE_Message_Block::Message_Flags less_flags);

  /// Get the current message flags.
  ACE_Message_Block::Message_Flags flags (void) const;

  /// Obtain the allocator strategy.
  ACE_Allocator *allocator_strategy (void) const;

  // = The locking strategy prevents race conditions.
  /// Get the locking strategy.
  /// Set a new locking strategy and return the hold one.
  ACE_Lock *locking_strategy (void);
  ACE_Lock *locking_strategy (ACE_Lock *);

  /// Dump the state of an object.
  void dump (void) const;

  /// Get the current reference count.
  int reference_count (void) const;

  /// Get the allocator used to create this object
  ACE_Allocator *data_block_allocator (void) const;

protected:
  /// Internal release implementation
  ACE_Data_Block *release_i (void);

  /**
   * Decrease the reference count, but don't delete the object.
   * Returns 0 if the object should be removed.
   * If <lock> is equal to the locking strategy then we assume that
   * the lock is beign held by the current thread; this is used to
   * release all the data blocks in a chain while holding a single
   * lock.
   */
  friend class ACE_Message_Block;
  ACE_Data_Block *release_no_delete (ACE_Lock *lock);

  /// Type of message.
  ACE_Message_Block::ACE_Message_Type type_;

  /// Current size of message block.
  size_t cur_size_;

  /// Total size of buffer.
  size_t max_size_;

  /// Misc flags (e.g., DONT_DELETE and USER_FLAGS).
  ACE_Message_Block::Message_Flags flags_;

  /// Pointer To beginning of message payload.
  char *base_;

  // = Strategies.
  /**
   * Pointer to the allocator defined for this <ACE_Data_Block>.  Note
   * that this pointer is shared by all owners of this
   * <ACE_Data_Block>.
   */
  ACE_Allocator *allocator_strategy_;

  /**
   * Pointer to the locking strategy defined for this
   * <ACE_Data_Block>.  This is used to protect regions of code that
   * access shared <ACE_Data_Block> state.  Note that this lock is
   * shared by all owners of the <ACE_Data_Block>'s data.
   */
  ACE_Lock *locking_strategy_;

  /**
   * Reference count for this <ACE_Data_Block>, which is used to avoid
   * deep copies (i.e., <clone>).  Note that this pointer value is
   * shared by all owners of the <Data_Block>'s data, i.e., all the
   * <ACE_Message_Block>s.
   */
  int reference_count_;

  /// The allocator use to destroy ourselves.
  ACE_Allocator *data_block_allocator_;

private:
  // = Disallow these operations.
  ACE_Data_Block &operator= (const ACE_Data_Block &);
  ACE_Data_Block (const ACE_Data_Block &);
};

/**
 * @class ACE_Dynamic_Message_Strategy
 *
 * @brief An abstract base class which provides dynamic priority
 * evaluation methods for use by the <ACE_Dynamic_Message_Queue>
 * class or any other class which needs to manage the priorities
 * of a collection of <ACE_Message_Block>s dynamically.
 *
 * Methods for deadline and laxity based priority evaluation are
 * provided.  These methods assume a specific partitioning of
 * the message priority number into a higher order dynamic bit
 * field and a lower order static priority bit field.  The
 * default partitioning assumes an unsigned dynamic message
 * priority field of 22 bits and an unsigned static message
 * priority field of 10 bits.  This corresponds to the initial
 * values of the static class members.  To provide a different
 * partitioning, assign a different set of values to the static
 * class memebers before using the static member functions.
 */
class ACE_Export ACE_Dynamic_Message_Strategy
{
public:

  // = Message priority status

  // Values are defined as bit flags so that status combinations may
  // be specified easily.

  enum Priority_Status
  {
    /// message can still make its deadline
    PENDING     = 0x01,
    /// message cannot make its deadline
    LATE        = 0x02,
    /// message is so late its priority is undefined
    BEYOND_LATE = 0x04,
    /// mask to match any priority status
    ANY_STATUS  = 0x07
  };

  /// ctor
  ACE_Dynamic_Message_Strategy (u_long static_bit_field_mask,
                                u_long static_bit_field_shift,
                                u_long dynamic_priority_max,
                                u_long dynamic_priority_offset);

  /// virtual dtor
  virtual ~ACE_Dynamic_Message_Strategy (void);

  /// Updates the message's priority and returns its priority status.
  Priority_Status priority_status (ACE_Message_Block &mb,
                                   const ACE_Time_Value &tv);

  /// Get static bit field mask.
  u_long static_bit_field_mask (void) const;

  /// Set static bit field mask.
  void static_bit_field_mask (u_long);

  /// Get left shift value to make room for static bit field.
  u_long static_bit_field_shift (void) const;

  /// Set left shift value to make room for static bit field.
  void static_bit_field_shift (u_long);

  /// Get maximum supported priority value.
  u_long dynamic_priority_max (void) const;

  /// Set maximum supported priority value.
  void dynamic_priority_max (u_long);

  /// Get offset to boundary between signed range and unsigned range.
  u_long dynamic_priority_offset (void) const;

  /// Set offset to boundary between signed range and unsigned range.
  void dynamic_priority_offset (u_long);

  /// Dump the state of the strategy.
  virtual void dump (void) const;

protected:
  /// Hook method for dynamic priority conversion.
  virtual void convert_priority (ACE_Time_Value &priority,
                                 const ACE_Message_Block &mb) = 0;

  /// This is a bit mask with all ones in the static bit field.
  u_long static_bit_field_mask_;

  /**
   * This is a left shift value to make room for static bit field:
   * this value should be the logarithm base 2 of
   * (static_bit_field_mask_ + 1).
   */
  u_long static_bit_field_shift_;

  /// Maximum supported priority value.
  u_long dynamic_priority_max_;

  /// Offset to boundary between signed range and unsigned range.
  u_long dynamic_priority_offset_;

  /// Maximum late time value that can be represented.
  ACE_Time_Value max_late_;

  /// Minimum pending time value that can be represented.
  ACE_Time_Value min_pending_;

  /// Time value by which to shift pending priority.
  ACE_Time_Value pending_shift_;
};

/**
 * @class ACE_Deadline_Message_Strategy
 *
 * @brief Deadline based message priority strategy.
 *
 * Assigns dynamic message priority according to time to deadline.  The
 * message priority is divided into high and low order bit fields.  The
 * high order bit field is used for dynamic message priority, which is
 * updated whenever the convert_priority (...) method is called.  The
 * low order bit field is used for static message priority and is left
 * unchanged.  The partitioning of the priority value into high and low
 * order bit fields is done according to the arguments passed to the
 * strategy object's constructor.
 */
class ACE_Export ACE_Deadline_Message_Strategy : public ACE_Dynamic_Message_Strategy
{
public:
  /// Ctor, with all arguments defaulted.
  ACE_Deadline_Message_Strategy (u_long static_bit_field_mask = 0x3FFUL,       // 2^(10) - 1
                                 u_long static_bit_field_shift = 10,           // 10 low order bits
                                 u_long dynamic_priority_max = 0x3FFFFFUL,     // 2^(22)-1
                                 u_long dynamic_priority_offset = 0x200000UL); // 2^(22-1)

  /// Virtual dtor.
  virtual ~ACE_Deadline_Message_Strategy (void);

  /// Dynamic priority conversion function based on time to deadline.
  virtual void convert_priority (ACE_Time_Value &priority,
                                 const ACE_Message_Block &mb);

  /// Dump the state of the strategy.
  virtual void dump (void) const;
};

/**
 * @class ACE_Laxity_Message_Strategy
 *
 * @brief Laxity based message priority strategy.
 *
 * Assigns dynamic message priority according to laxity (time to
 * deadline minus worst case execution time).  The message priority is
 * divided into high and low order bit fields.  The high order
 * bit field is used for dynamic message priority, which is
 * updated whenever the convert_priority (...) method is called.  The
 * low order bit field is used for static message priority and is left
 * unchanged.  The partitioning of the priority value into high and low
 * order bit fields is done according to the arguments passed to the
 * strategy object's constructor.
 */
class ACE_Export ACE_Laxity_Message_Strategy : public ACE_Dynamic_Message_Strategy
{
public:
  /// Ctor, with all arguments defaulted.
  ACE_Laxity_Message_Strategy (u_long static_bit_field_mask = 0x3FFUL,       // 2^(10) - 1
                               u_long static_bit_field_shift = 10,           // 10 low order bits
                               u_long dynamic_priority_max = 0x3FFFFFUL,     // 2^(22)-1
                               u_long dynamic_priority_offset = 0x200000UL); // 2^(22-1)

  /// virtual dtor.
  virtual ~ACE_Laxity_Message_Strategy (void);

  /// Dynamic priority conversion function based on laxity.
  virtual void convert_priority (ACE_Time_Value &priority,
                                 const ACE_Message_Block &mb);

  /// Dump the state of the strategy.
  virtual void dump (void) const;
};

#if defined (__ACE_INLINE__)
#include "ace/Message_Block.i"
#endif /* __ACE_INLINE__ */

#include "ace/Message_Block_T.h"
#include "ace/post.h"
#endif /* ACE_MESSAGE_BLOCK_H */
