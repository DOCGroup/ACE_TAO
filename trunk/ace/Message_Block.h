/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Message_Block.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#ifndef ACE_MESSAGE_BLOCK_H
#define ACE_MESSAGE_BLOCK_H

#include "ace/Malloc.h"

// Forward declaration.
class ACE_Data_Block;
class ACE_Lock;
class ACE_Time_Value;

class ACE_Export ACE_Message_Block
{
  // = TITLE
  //     Stores messages for use throughout ACE (particularly
  //     <ACE_Message_Queue>).
  //
  // = DESCRIPTION
  //     An <ACE_Message_Block> is modeled after the message data
  //     structures used in System V STREAMS.  Its purpose is to
  //     enable efficient manipulation of arbitrarily-large messages
  //     without much incurring memory copying overhead.  Here are the
  //     main characteristics of an <ACE_Message_Block>:
  //
  //     1. Contains a pointer to a reference-counted
  //     <ACE_Data_Block>, which in turn points to the actual data
  //     buffer.  This allows very flexible and efficient sharing of
  //     data by multiple <ACE_Message_Block>s.
  //
  //     2. One or more <ACE_Message_Blocks> can be linked to form a
  //     ``fragment chain.''
  //
  //     3. <ACE_Message_Blocks> can be linked together by <prev_> and
  //     <next_> pointers to form a queue of messages (e.g., this is how
  //     <ACE_Message_Queue> works).
public:
  friend class ACE_Data_Block;

  enum
  {
    // = Data and protocol messages (regular and priority)
    MB_DATA     = 0x01, // regular data
    MB_PROTO    = 0x02, // protocol control

    // = Control messages (regular and priority)
    MB_BREAK    = 0x03, // line break
    MB_PASSFP   = 0x04, // pass file pointer
    MB_EVENT    = 0x05, // post an event to an event queue
    MB_SIG      = 0x06, // generate process signal
    MB_IOCTL    = 0x07, // ioctl; set/get params
    MB_SETOPTS  = 0x08, // set various stream head options

    // = Control messages (high priority; go to head of queue)
    MB_IOCACK   = 0x81, // acknowledge ioctl
    MB_IOCNAK   = 0x82, // negative ioctl acknowledge
    MB_PCPROTO  = 0x83, // priority proto message
    MB_PCSIG    = 0x84, // generate process signal
    MB_READ     = 0x85, // generate read notification
    MB_FLUSH    = 0x86, // flush your queues
    MB_STOP     = 0x87, // stop transmission immediately
    MB_START    = 0x88, // restart transmission after stop
    MB_HANGUP   = 0x89, // line disconnect
    MB_ERROR    = 0x8a, // fatal error used to set u.u_error
    MB_PCEVENT  = 0x8b, // post an event to an event queue

    // Message class masks
    MB_NORMAL   = 0x00, // Normal priority messages
    MB_PRIORITY = 0x80, // High priority control messages
    MB_USER     = 0x200 // User-defined control messages
  };

  typedef int ACE_Message_Type;
  typedef u_long Message_Flags;

  enum
  {
    DONT_DELETE = 01, // Don't delete the data on exit since we don't own it.
    USER_FLAGS = 0x1000 // user defined flags start here
  };

  // = Initialization and termination.
  ACE_Message_Block (ACE_Allocator *message_block_allocator = 0);
  // Create an empty message.

  ACE_Message_Block (ACE_Data_Block *,
                     ACE_Allocator *message_block_allocator = 0);
  // Create an <ACE_Message_Block> that owns the <ACE_Data_Block> *.

  ACE_Message_Block (const char *data,
                     size_t size = 0,
                     u_long priority = 0);
  // Create a Message Block that assumes ownership of <data> without
  // copying it (i.e., we don't delete it since we don't malloc it!).
  // Note that the <size> of the <Message_Block> will be <size>, but
  // the <length> will be 0 until <wr_ptr> is set.

  ACE_Message_Block (size_t size,
                     ACE_Message_Type type = MB_DATA,
                     ACE_Message_Block *cont = 0,
                     const char *data = 0,
                     ACE_Allocator *allocator_strategy = 0,
                     ACE_Lock *locking_strategy = 0,
                     u_long priority = 0,
                     const ACE_Time_Value & execution_time = ACE_Time_Value::zero,
                     const ACE_Time_Value & deadline_time = ACE_Time_Value::max_time,
                     ACE_Allocator *data_block_allocator = 0,
                     ACE_Allocator *message_block_allocator = 0);
  // Create an initialized message of type <type> containing <size>
  // bytes.  The <cont> argument initializes the continuation field in
  // the <Message_Block>.  If <data> == 0 then we create and own the
  // <data>, using <allocator> to get the data if it's non-0.  If
  // <data> != 0 we assume ownership of the <data> (and don't delete
  // it).  If <locking_strategy> is non-0 then this is used to protect
  // regions of code that access shared state (e.g., reference
  // counting) from race conditions.  Note that the <size> of the
  // <Message_Block> will be <size>, but the <length> will be 0 until
  // <wr_ptr> is set.
  // The <data_block_allocator> is use to allocate the data blocks
  // while the <allocator_strategy> is used to allocate the buffers
  // contained by those.
  // The <message_block_allocator> is used to allocate new
  // <Message_Block> objects when a duplicate method is called.  If
  // a <message_block_allocator> is given, this <Message_Block> and
  // future <Message_Block> objects created by duplicate will be free'ed
  // into this allocator when they are released.  Note: if you use this
  // allocator, the <Message_Block> you created should have been created
  // using this allocator because it will be released to the same allocator.

  int init (const char *data,
            size_t size = 0);
  // Create a Message Block that assumes ownership of <data> (i.e.,
  // doesn't delete it since it didn't malloc it!).  Note that the
  // <size> of the <Message_Block> will be <size>, but the <length>
  // will be 0 until <wr_ptr> is set.

  int init (size_t size,
            ACE_Message_Type type = MB_DATA,
            ACE_Message_Block *cont = 0,
            const char *data = 0,
            ACE_Allocator *allocator_strategy = 0,
            ACE_Lock *locking_strategy = 0,
            u_long priority = 0,
            const ACE_Time_Value & execution_time = ACE_Time_Value::zero,
            const ACE_Time_Value & deadline_time = ACE_Time_Value::max_time,
            ACE_Allocator *data_block_allocator = 0,
            ACE_Allocator *message_block_allocator = 0);
  // Create an initialized message of type <type> containing <size>
  // bytes.  The <cont> argument initializes the continuation field in
  // the <Message_Block>.  If <data> == 0 then we create and own the
  // <data>, using <allocator> to get the data if it's non-0.  If
  // <data> != 0 we assume ownership of the <data> (and don't delete
  // it).  If <locking_strategy> is non-0 then this is used to protect
  // regions of code that access shared state (e.g., reference
  // counting) from race conditions.  Note that the <size> of the
  // <Message_Block> will be <size>, but the <length> will be 0 until
  // <wr_ptr> is set.
  // The <data_block_allocator> is use to allocate the data blocks
  // while the <allocator_strategy> is used to allocate the buffers
  // contained by those.

  virtual ~ACE_Message_Block (void);
  // Delete all the resources held in the message.
  //
  // Note that release() is designed to release the continuation
  // chain; the destructor is not. See release() for details.

  // = Message Type accessors and mutators.

  ACE_Message_Type msg_type (void) const;
  // Get type of the message.

  void msg_type (ACE_Message_Type type);
  // Set type of the message.

  int is_data_msg (void) const;
  // Find out what type of message this is.

  ACE_Message_Type msg_class (void) const;
  // Find out what class of message this is (there are two classes,
  // <normal> messages and <high-priority> messages).

  // = Message flag accessors and mutators.
  Message_Flags set_flags (Message_Flags more_flags);
  // Bitwise-or the <more_flags> into the existing message flags and
  // return the new value.

  Message_Flags clr_flags (Message_Flags less_flags);
  // Clear the message flag bits specified in <less_flags> and return
  // the new value.

  Message_Flags flags (void) const;
  // Get the current message flags.

  u_long msg_priority (void) const;
  // Get priority of the message.

  void msg_priority (u_long priority);
  // Set priority of the message.

  const ACE_Time_Value & msg_execution_time (void) const;
  // Get execution time associated with the message.

  void msg_execution_time (const ACE_Time_Value & et);
  // Set execution time associated with the message.

  const ACE_Time_Value & msg_deadline_time (void) const;
  // Get absolute time of deadline associated with the message.

  void msg_deadline_time (const ACE_Time_Value & dt);
  // Set absolute time of deadline associated with the message.

  // = Deep copy and shallow copy methods.

  virtual ACE_Message_Block *clone (Message_Flags mask = 0) const;
  // Return an exact "deep copy" of the message, i.e., create fresh
  // new copies of all the Data_Blocks and continuations.

  ACE_Message_Block *duplicate (void) const;
  // Return a "shallow" copy that increments our reference count by 1.

  static ACE_Message_Block *duplicate (const ACE_Message_Block *mb);
  // Return a "shallow" copy that increments our reference count by 1.
  // This is similar to CORBA's <_duplicate> method, which is useful
  // if you want to eliminate lots of checks for NULL <mb> pointers
  // before calling <_duplicate> on them.

  ACE_Message_Block *release (void);
  // Decrease the shared ACE_Data_Block's reference count by 1.  If the
  // ACE_Data_Block's reference count goes to 0, it is deleted.
  // In all cases, this ACE_Message_Block is deleted - it must have come
  // from the heap, or there will be trouble.
  //
  // release() is designed to release the continuation chain; the
  // destructor is not.  If we make the destructor release the
  // continuation chain by calling release() or delete on the message
  // blocks in the continuation chain, the following code will not
  // work since the message block in the continuation chain is not off
  // the heap:
  //
  //  ACE_Message_Block mb1 (1024);
  //  ACE_Message_Block mb2 (1024);
  //
  //  mb1.cont (&mb2);
  //
  // And hence, call release() on a dynamically allocated message
  // block. This will release all the message blocks in the
  // continuation chain.  If you call delete or let the message block
  // fall off the stack, cleanup of the message blocks in the
  // continuation chain becomes the responsibility of the user.

  static ACE_Message_Block *release (ACE_Message_Block *mb);
  // This behaves like the non-static method <release>, except that it
  // checks if <mb> is 0.  This is similar to <CORBA::release>, which
  // is useful if you want to eliminate lots of checks for NULL
  // pointers before calling <release> on them.  Returns <mb>.

  // = Operations on Message data

  int copy (const char *buf, size_t n);
  // Copies <n> bytes from <buf> into the Message_Block starting at
  // the <wr_ptr> offset.  Return 0 and increment <wr_ptr> by <n> if
  // the method succeeds.  Returns -1 if the size of the message is
  // too small, i.e., for this to work correct, <end> must be >=
  // <wr_ptr>.

  int copy (const char *buf);
  // Copies <buf> into the Message_Block starting at the wr_ptr()
  // offset.  This call assumees that <buf> is NUL-terminated.  Return
  // 0 and increment <wr_ptr> by <ACE_OS::strlen (buf) + 1> if the
  // method succeeds.  Returns -1 if the size of the message is too
  // small, i.e., for this to work correct, <end> must be >= <wr_ptr>.

  char *base (void) const;
  // Get message data.

  void base (char *data,
             size_t size,
             Message_Flags = DONT_DELETE);
  // Set message data (doesn't reallocate).

  char *end (void) const;
  // Return a pointer to 1 past the end of the data in a message.

  char *rd_ptr (void) const;
  // Get the read pointer.
  void rd_ptr (char *ptr);
  // Set the read pointer to <ptr>.
  void rd_ptr (size_t n);
  // Set the read pointer ahead <n> bytes.

  char *wr_ptr (void) const;
  // Get the write pointer.
  void wr_ptr (char *ptr);
  // Set the write pointer to <ptr>.
  void wr_ptr (size_t n);
  // Set the write pointer ahead <n> bytes.  This is used to compute
  // the <length> of a message.

  // = Message length is wr_ptr() - rd_ptr ().
  size_t length (void) const;
  // Get the length of the message
  void length (size_t n);
  // Set the length of the message

  // = Set/get <Message_Block> size info.
  size_t total_size (void) const;
  // Get the total number of bytes in all <Message_Block>s, including
  // chained <Message_Block>s.

  size_t size (void) const;
  // Get the number of bytes in the top-level <Message_Block> (i.e.,
  // does not consider the bytes in chained <Message_Block>s).

  int size (size_t length);
  // Set the number of bytes in the top-level <Message_Block>,
  // reallocating space if necessary.  However, the <rd_ptr_> and
  // <wr_ptr_> remain at the original offsets into the buffer, even if
  // it is reallocated.  Returns 0 if successful, else -1.

  size_t space (void) const;
  // Get the number of bytes in the top-level <Message_Block>.

  void crunch (void);
  // Normalizes data in message block to align with the base.

  // = <ACE_Data_Block> methods.
  ACE_Data_Block *data_block (void) const;
  // Get the data block.
  void data_block (ACE_Data_Block *);
  // Set the data block (releasing the original one).

  // = The continuation field chains together composite messages.
  ACE_Message_Block *cont (void) const;
  // Get the continuation field.
  void cont (ACE_Message_Block *);
  // Set the continuation field.

  // = Pointer to the <Message_Block> directly ahead in the <ACE_Message_Queue>.
  ACE_Message_Block *next (void) const;
  // Get link to next message.
  void next (ACE_Message_Block *);
  // Set link to next message.

  // = Pointer to the <Message_Block> directly behind in the <ACE_Message_Queue>.
  ACE_Message_Block *prev (void) const;
  // Get link to prev message.
  void prev (ACE_Message_Block *);
  // Set link to prev message.

  // = The locking strategy prevents race conditions.
  ACE_Lock *locking_strategy (void);
  // Get the locking strategy.
  ACE_Lock *locking_strategy (ACE_Lock *);
  // Set a new locking strategy and return the hold one.

  int reference_count (void) const;
  // Get the current reference count.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = Internal initialization methods.
  ACE_Message_Block (size_t size,
                     ACE_Message_Type type,
                     ACE_Message_Block *cont,
                     const char *data,
                     ACE_Allocator *allocator_strategy,
                     ACE_Lock *locking_strategy,
                     Message_Flags flags,
                     u_long priority,
                     const ACE_Time_Value & execution_time,
                     const ACE_Time_Value & deadline_time,
                     ACE_Data_Block *db,
                     ACE_Allocator *data_block_allocator,
                     ACE_Allocator *message_block_allocator);
  // Perform the actual initialization.

  int release_i (ACE_Lock *lock);
  // Internal release implementation
  // Returns 1 if the data block has to be destroyed.

  int init_i (size_t size,
              ACE_Message_Type type,
              ACE_Message_Block *cont,
              const char *data,
              ACE_Allocator *allocator_strategy,
              ACE_Lock *locking_strategy,
              Message_Flags flags,
              u_long priority,
              const ACE_Time_Value & execution_time,
              const ACE_Time_Value & deadline_time,
              ACE_Data_Block *db,
              ACE_Allocator *data_block_allocator,
              ACE_Allocator *message_block_allocator);
  // Perform the actual initialization.

  size_t rd_ptr_;
  // Pointer to beginning of next read.

  size_t wr_ptr_;
  // Pointer to beginning of next write.

  u_long priority_;
  // Priority of message.

  ACE_Time_Value execution_time_;
  // execution time associated with the message

  ACE_Time_Value deadline_time_;
  // absolute deadline time for message

  // = Links to other ACE_Message_Block *s.
  ACE_Message_Block *cont_;
  // Pointer to next message block in the chain.

  ACE_Message_Block *next_;
  // Pointer to next message in the list.

  ACE_Message_Block *prev_;
  // Pointer to previous message in the list.

  ACE_Data_Block *data_block_;
  // Pointer to the reference counted data structure that contains the
  // actual memory buffer.

  ACE_Allocator *message_block_allocator_;
  // The allocator used to destroy ourselves when release is called
  // and create new message blocks on duplicate.

private:
  // = Disallow these operations for now (use <clone> instead).
  ACE_Message_Block &operator= (const ACE_Message_Block &);
  ACE_Message_Block (const ACE_Message_Block &);
};

class ACE_Export ACE_Data_Block
{
  // = TITLE
  //     Stores the data payload that is accessed via one or more
  //     <ACE_Message_Block>s.
  //
  // = DESCRIPTION
  //     This data structure is reference counted to maximize
  //     sharing.  It also contains the <locking_strategy_> (which
  //     protects the reference count from race conditions in
  //     concurrent programs) and the <allocation_strategy_> (which
  //     determines what memory pool is used to allocate the memory).
public:
  // = Initialization and termination methods.
  ACE_Data_Block (void);
  // Default "do-nothing" constructor.

  ACE_Data_Block (size_t size,
                  ACE_Message_Block::ACE_Message_Type msg_type,
                  const char *msg_data,
                  ACE_Allocator *allocator_strategy,
                  ACE_Lock *locking_strategy,
                  ACE_Message_Block::Message_Flags flags,
                  ACE_Allocator *data_block_allocator);
  // Initialize.

  virtual ~ACE_Data_Block (void);
  // Delete all the resources held in the message.

  ACE_Message_Block::ACE_Message_Type msg_type (void) const;
  // Get type of the message.

  void msg_type (ACE_Message_Block::ACE_Message_Type type);
  // Set type of the message.

  char *base (void) const;
  // Get message data pointer

  void base (char *data,
             size_t size,
             ACE_Message_Block::Message_Flags mflags = ACE_Message_Block::DONT_DELETE);
  // Set message data pointer (doesn't reallocate).

  char *end (void) const;
  // Return a pointer to 1 past the end of the data in a message.

  // = Message size is the total amount of space alloted.
  size_t size (void) const;
  // Get the total amount of space in the message.
  int size (size_t length);
  // Set the total amount of space in the message.  Returns 0 if
  // successful, else -1.

  virtual ACE_Data_Block *clone (ACE_Message_Block::Message_Flags mask = 0) const;
  // Return an exact "deep copy" of the message, i.e., create fresh
  // new copies of all the Data_Blocks and continuations.
  // Notice that Data_Blocks can act as "Prototypes", i.e. derived
  // classes can override this method and create instances of
  // themselves.

  ACE_Data_Block *duplicate (void);
  // Return a "shallow" copy that increments our reference count by 1.

  ACE_Data_Block *release (ACE_Lock *lock = 0);
  // Decrease the shared reference count by 1.  If the reference count
  // is > 0 then return this; else if reference count == 0 then delete
  // <this> and <mb> and return 0.  Behavior is undefined if reference
  // count < 0.

  // = Message flag accessors and mutators.
  ACE_Message_Block::Message_Flags set_flags (ACE_Message_Block::Message_Flags more_flags);
  // Bitwise-or the <more_flags> into the existing message flags and
  // return the new value.

  ACE_Message_Block::Message_Flags clr_flags (ACE_Message_Block::Message_Flags less_flags);
  // Clear the message flag bits specified in <less_flags> and return
  // the new value.

  ACE_Message_Block::Message_Flags flags (void) const;
  // Get the current message flags.

  ACE_Allocator *allocator_strategy (void) const;
  // Obtain the allocator strategy.

  // = The locking strategy prevents race conditions.
  ACE_Lock *locking_strategy (void);
  // Get the locking strategy.
  ACE_Lock *locking_strategy (ACE_Lock *);
  // Set a new locking strategy and return the hold one.

  void dump (void) const;
  // Dump the state of an object.

  int reference_count (void) const;
  // Get the current reference count.

  ACE_Allocator *data_block_allocator (void) const;
  // Get the allocator used to create this object

protected:
  ACE_Data_Block *release_i (void);
  // Internal release implementation

  friend class ACE_Message_Block;
  ACE_Data_Block *release_no_delete (ACE_Lock *lock);
  // Decrease the reference count, but don't delete the object.
  // Returns 0 if the object should be removed.
  // If <lock> is equal to the locking strategy then we assume that
  // the lock is beign held by the current thread; this is used to
  // release all the data blocks in a chain while holding a single
  // lock.

  ACE_Message_Block::ACE_Message_Type type_;
  // Type of message.

  size_t cur_size_;
  // Current size of message block.

  size_t max_size_;
  // Total size of buffer.

  ACE_Message_Block::Message_Flags flags_;
  // Misc flags (e.g., DONT_DELETE and USER_FLAGS).

  char *base_;
  // Pointer to beginning of message payload.

  // = Strategies.
  ACE_Allocator *allocator_strategy_;
  // Pointer to the allocator defined for this <ACE_Data_Block>.  Note
  // that this pointer is shared by all owners of this
  // <ACE_Data_Block>.

  ACE_Lock *locking_strategy_;
  // Pointer to the locking strategy defined for this
  // <ACE_Data_Block>.  This is used to protect regions of code that
  // access shared <ACE_Data_Block> state.  Note that this lock is
  // shared by all owners of the <ACE_Data_Block>'s data.

  int reference_count_;
  // Reference count for this <ACE_Data_Block>, which is used to avoid
  // deep copies (i.e., <clone>).  Note that this pointer value is
  // shared by all owners of the <Data_Block>'s data, i.e., all the
  // <ACE_Message_Block>s.

  ACE_Allocator *data_block_allocator_;
  // The allocator use to destroy ourselves.

private:
  // = Disallow these operations.
  ACE_Data_Block &operator= (const ACE_Data_Block &);
  ACE_Data_Block (const ACE_Data_Block &);
};

class ACE_Export ACE_Dynamic_Message_Strategy
{
  // = TITLE
  //     An abstract base class which provides dynamic priority
  //     evaluation methods for use by the <ACE_Dynamic_Message_Queue>
  //     class or any other class which needs to manage the priorities
  //     of a collection of <ACE_Message_Block>s dynamically.
  //
  // = DESCRIPTION
  //     Methods for deadline and laxity based priority evaluation are
  //     provided.  These methods assume a specific partitioning of
  //     the message priority number into a higher order dynamic bit
  //     field and a lower order static priority bit field.  The
  //     default partitioning assumes an unsigned dynamic message
  //     priority field of 22 bits and an unsigned static message
  //     priority field of 10 bits.  This corresponds to the initial
  //     values of the static class members.  To provide a different
  //     partitioning, assign a different set of values to the static
  //     class memebers before using the static member functions.
public:

  // = Message priority status

  // Values are defined as bit flags so that status combinations may
  // be specified easily.

  enum Priority_Status
  {
    PENDING     = 0x01, // message can still make its deadline
    LATE        = 0x02, // message cannot make its deadline
    BEYOND_LATE = 0x04, // message is so late its priority is undefined
    ANY_STATUS  = 0x07  // mask to match any priority status
  };

  ACE_Dynamic_Message_Strategy (u_long static_bit_field_mask,
                                u_long static_bit_field_shift,
                                u_long dynamic_priority_max,
                                u_long dynamic_priority_offset);
  // ctor

  virtual ~ACE_Dynamic_Message_Strategy ();
  // virtual dtor

  Priority_Status priority_status (ACE_Message_Block & mb,
                                   const ACE_Time_Value & tv);
  // Updates the message's priority and returns its priority status.

  u_long static_bit_field_mask (void);
  // Get static bit field mask.

  void static_bit_field_mask (u_long);
  // Set static bit field mask.

  u_long static_bit_field_shift (void);
  // Get left shift value to make room for static bit field.

  void static_bit_field_shift (u_long);
  // Set left shift value to make room for static bit field.

  u_long dynamic_priority_max (void);
  // Get maximum supported priority value.

  void dynamic_priority_max (u_long);
  // Set maximum supported priority value.

  u_long dynamic_priority_offset (void);
  // Get offset to boundary between signed range and unsigned range.

  void dynamic_priority_offset (u_long);
  // Set offset to boundary between signed range and unsigned range.

  virtual void dump (void) const;
  // Dump the state of the strategy.

protected:
  virtual void convert_priority (ACE_Time_Value & priority,
                                 const ACE_Message_Block & mb) = 0;
  // Hook method for dynamic priority conversion.

  u_long static_bit_field_mask_;
  // This is a bit mask with all ones in the static bit field.

  u_long static_bit_field_shift_;
  // This is a left shift value to make room for static bit field:
  // this value should be the logarithm base 2 of
  // (static_bit_field_mask_ + 1).

  u_long dynamic_priority_max_;
  // Maximum supported priority value.

  u_long dynamic_priority_offset_;
  // Offset to boundary between signed range and unsigned range.

  ACE_Time_Value max_late_;
  // Maximum late time value that can be represented.

  ACE_Time_Value min_pending_;
  // Minimum pending time value that can be represented.

  ACE_Time_Value pending_shift_;
  // Time value by which to shift pending priority.
};

class ACE_Export ACE_Deadline_Message_Strategy : public ACE_Dynamic_Message_Strategy
{
  // = TITLE
  //     Deadline based message priority strategy.
  //
  // = DESCRIPTION
  //     Assigns dynamic message priority according to time to deadline.  The
  //     message priority is divided into high and low order bit fields.  The
  //     high order bit field is used for dynamic message priority, which is
  //     updated whenever the convert_priority (...) method is called.  The
  //     low order bit field is used for static message priority and is left
  //     unchanged.  The partitioning of the priority value into high and low
  //     order bit fields is done according to the arguments passed to the
  //     strategy object's constructor.
  //
public:
  ACE_Deadline_Message_Strategy (u_long static_bit_field_mask = 0x3FFUL,       // 2^(10) - 1
                                 u_long static_bit_field_shift = 10,           // 10 low order bits
                                 u_long dynamic_priority_max = 0x3FFFFFUL,     // 2^(22)-1
                                 u_long dynamic_priority_offset = 0x200000UL); // 2^(22-1)
  // Ctor, with all arguments defaulted.

  virtual ~ACE_Deadline_Message_Strategy (void);
  // Virtual dtor.

  virtual void convert_priority (ACE_Time_Value & priority,
                                 const ACE_Message_Block & mb);
  // Dynamic priority conversion function based on time to deadline.

  virtual void dump (void) const;
  // Dump the state of the strategy.
};

class ACE_Export ACE_Laxity_Message_Strategy : public ACE_Dynamic_Message_Strategy
{
  // = TITLE
  //     Laxity based message priority strategy.
  //
  // = DESCRIPTION
  //     Assigns dynamic message priority according to laxity (time to
  //     deadline minus worst case execution time).  The message priority is
  //     divided into high and low order bit fields.  The high order
  //     bit field is used for dynamic message priority, which is
  //     updated whenever the convert_priority (...) method is called.  The
  //     low order bit field is used for static message priority and is left
  //     unchanged.  The partitioning of the priority value into high and low
  //     order bit fields is done according to the arguments passed to the
  //     strategy object's constructor.
  //
public:
  ACE_Laxity_Message_Strategy (u_long static_bit_field_mask = 0x3FFUL,       // 2^(10) - 1
                               u_long static_bit_field_shift = 10,           // 10 low order bits
                               u_long dynamic_priority_max = 0x3FFFFFUL,     // 2^(22)-1
                               u_long dynamic_priority_offset = 0x200000UL); // 2^(22-1)
  // Ctor, with all arguments defaulted.

  virtual ~ACE_Laxity_Message_Strategy (void);
  // virtual dtor.

  virtual void convert_priority (ACE_Time_Value & priority,
                                 const ACE_Message_Block & mb);
  // Dynamic priority conversion function based on laxity.

  virtual void dump (void) const;
  // Dump the state of the strategy.
};

#if defined (__ACE_INLINE__)
#include "ace/Message_Block.i"
#endif /* __ACE_INLINE__ */

#include "ace/Message_Block_T.h"
#endif /* ACE_MESSAGE_BLOCK_H */
