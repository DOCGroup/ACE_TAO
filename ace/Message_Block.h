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

#if !defined (ACE_MESSAGE_BLOCK_H)
#define ACE_MESSAGE_BLOCK_H

#include "ace/Malloc.h"

// Forward declaration.
class ACE_Data_Block;

class ACE_Export ACE_Message_Block
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
{
  friend class ACE_Data_Block;
public:  
  enum ACE_Message_Type
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
    MB_NORMAL	= 0x00, // Normal priority messages 
    MB_PRIORITY = 0x80, // High priority control messages 
    MB_USER	= 0x200 // User-defined control messages
  };

  typedef u_long Message_Flags;

  enum
  {
    DONT_DELETE = 01, // Don't delete the data on exit since we don't own it.
    USER_FLAGS = 0x1000	// user defined flags start here
  };

  // = Initialization and termination.
  ACE_Message_Block (void);
  // Create an empty message.

  ACE_Message_Block (ACE_Data_Block *);
  // Create an <ACE_Message_Block> that owns the <ACE_Data_Block> *.

  ACE_Message_Block (const char *data,
		     size_t size = 0);
  // Create a Message Block that assumes ownership of <data> without
  // copying it (i.e., we don't delete it since we don't malloc it!).

  ACE_Message_Block (size_t size, 
		     ACE_Message_Type type = MB_DATA, 
		     ACE_Message_Block *cont = 0, 
		     const char *data = 0,
		     ACE_Allocator *allocator_strategy_ = 0,
		     ACE_Lock *locking_strategy = 0,
		     u_long priority = 0);
  // Create an initialized message of type <type> containing <size>
  // bytes.  The <cont> argument initializes the continuation field in
  // the <Message_Block>.  If <data> == 0 then we create and own the
  // <data>, using <allocator> to get the data if it's non-0.  If
  // <data> != 0 we assume ownership of the <data> (and don't delete
  // it).  If <locking_strategy> is non-0 then this is used to protect
  // regions of code that access shared state (e.g., reference
  // counting) from race conditions.

  int init (const char *data,
	    size_t size = 0);
  // Create a Message Block that assumes ownership of <data> (i.e.,
  // doesn't delete it since it didn't malloc it!).

  int init (size_t size, 
	    ACE_Message_Type type = MB_DATA, 
	    ACE_Message_Block *cont = 0, 
	    const char *data = 0,
	    ACE_Allocator *allocator = 0,
	    ACE_Lock *locking_strategy = 0,
	    u_long priority = 0);
  // Create an initialized message of type <type> containing <size>
  // bytes.  The <cont> argument initializes the continuation field in
  // the <Message_Block>.  If <data> == 0 then we create and own the
  // <data>, using <allocator> to get the data if it's non-0.  If
  // <data> != 0 we assume ownership of the <data> (and don't delete
  // it).  If <locking_strategy> is non-0 then this is used to protect
  // regions of code that access shared state (e.g., reference
  // counting) from race conditions.

  virtual ~ACE_Message_Block (void);
  // Delete all the resources held in the message.

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

  // = Deep copy and shallow copy methods.

  virtual ACE_Message_Block *clone (Message_Flags mask = 0) const;
  // Return an exact "deep copy" of the message, i.e., create fresh
  // new copies of all the Data_Blocks and continuations.

  ACE_Message_Block *duplicate (void) const;
  // Return a "shallow" copy that increments our reference count by 1.

  static ACE_Message_Block *duplicate (ACE_Message_Block *mb);
  // Return a "shallow" copy that increments our reference count by 1.
  // This is similar to CORBA's <_duplicate> method, which is useful
  // if you want to eliminate lots of checks for NULL <mb> pointers
  // before calling <_duplicate> on them.

  ACE_Message_Block *release (void);
  // Decrease the shared reference count by 1.  If the reference count
  // is > 0 then return this; else if reference count == 0 then delete
  // <this> and return 0.  Behavior is undefined if reference count <
  // 0.

  static ACE_Message_Block *release (ACE_Message_Block *mb);
  // This behaves like the non-static method <release>, except that it
  // checks if <mb> is 0.  This is similar to <CORBA::release>, which
  // is useful if you want to eliminate lots of checks for NULL
  // pointers before calling <release> on them.  Returns <mb>.

  // = Operations on Message data 

  int copy (const char *buf, size_t n);
  // Copies <n> bytes from <buf> into the Message_Block starting at
  // the wr_ptr() offset.  Return 0 if succeeds and -1 if the size of
  // the message is too small...

  int copy (const char *buf);
  // Copies <buf> into the Message_Block starting at the wr_ptr()
  // offset.  This call assumees that <buf> is NUL-terminated.  Return
  // 0 if succeeds and -1 if the size of the message is too small...

  char *base (void) const;
  // Get message data.

#if 0
  void base (char *data,
	     size_t size, 
	     Message_Flags = DONT_DELETE);
  // Set message data (doesn't reallocate).
#endif

  char *end (void) const;
  // Return a pointer to 1 past the end of the data in a message.

  char *rd_ptr (void);
  // Get the read pointer.
  void rd_ptr (char *ptr);
  // Set the read pointer to <ptr>.
  void rd_ptr (size_t n);
  // Set the read pointer ahead <n> bytes.

  char *wr_ptr (void);
  // Get the write pointer.
  void wr_ptr (char *ptr);
  // Set the write pointer to <ptr>.
  void wr_ptr (size_t n);
  // Set the write pointer ahead <n> bytes.

  // = Message length is wr_ptr() - rd_ptr ().
  size_t length (void) const;
  // Get the length of the message 
  void length (size_t n);
  // Set the length of the message 

  // = Message size is the total amount of space alloted.
  size_t size (void) const;
  // Get the total amount of space in the message.
  int size (size_t length);
  // Set the total amount of space in the message, reallocating space
  // if necessary.  Returns 0 if successful, else -1.

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

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  // = Internal initialization methods.
  ACE_Message_Block (size_t size, 
		     ACE_Message_Type type,
		     ACE_Message_Block *cont,
		     const char *data,
		     ACE_Allocator *allocator,
		     ACE_Lock *locking_strategy,
		     Message_Flags flags,
		     u_long priority,
		     ACE_Data_Block *db);
  // Perform the actual initialization.

  ACE_Message_Block *release_i (ACE_Lock *lock);
  // Internal release implementation

  int init_i (size_t size, 
	      ACE_Message_Type type,
	      ACE_Message_Block *cont,
	      const char *data,
	      ACE_Allocator *allocator,
	      ACE_Lock *locking_strategy,
	      Message_Flags flags,
	      u_long priority, 
	      ACE_Data_Block *db);
  // Perform the actual initialization.

  char *rd_ptr_;	
  // Pointer to beginning of next read.

  char *wr_ptr_;	
  // Pointer to beginning of next write.

  u_long priority_;	
  // Priority of message.

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

  // = Disallow these operations for now (use <clone> instead).
  ACE_Message_Block &operator= (const ACE_Message_Block &);
  ACE_Message_Block (const ACE_Message_Block &);
};

class ACE_Export ACE_Data_Block
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
{
public:
  // = Initialization and termination methods.
  ACE_Data_Block (void);
  // Default "do-nothing" constructor.
  
  ACE_Data_Block (size_t size,
		  ACE_Message_Block::ACE_Message_Type msg_type,
		  const char *msg_data, 
		  ACE_Allocator *allocator_strategy,
		  ACE_Lock *locking_strategy,
		  ACE_Message_Block::Message_Flags flags);
  // Initialize.

  virtual ~ACE_Data_Block (void);
  // Delete all the resources held in the message.

  ACE_Message_Block::ACE_Message_Type msg_type (void) const;
  // Get type of the message.

  void msg_type (ACE_Message_Block::ACE_Message_Type type);
  // Set type of the message.

  char *base (void) const;
  // Get message data pointer

#if 0
  void base (char *data,
	     size_t size, 
	     Message_Flags = DONT_DELETE);
  // Set message data pointer (doesn't reallocate).
#endif

  char *end (void) const;
  // Return a pointer to 1 past the end of the data in a message.

  // = Message length is wr_ptr() - rd_ptr ().
  size_t length (void) const;
  // Get the length of the message 
  void length (size_t n);
  // Set the length of the message 

  // = Message size is the total amount of space alloted.
  size_t size (void) const;
  // Get the total amount of space in the message.
  int size (size_t length);
  // Set the total amount of space in the message.  Returns 0 if
  // successful, else -1.

  ACE_Data_Block *clone (ACE_Message_Block::Message_Flags mask = 0) const;
  // Return an exact "deep copy" of the message, i.e., create fresh
  // new copies of all the Data_Blocks and continuations.

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

  // = The locking strategy prevents race conditions.
  ACE_Lock *locking_strategy (void);
  // Get the locking strategy.
  ACE_Lock *locking_strategy (ACE_Lock *);
  // Set a new locking strategy and return the hold one.

  void dump (void) const;
  // Dump the state of an object.

private:
  ACE_Data_Block *release_i (void);
  // Internal release implementation
  
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
};

#if defined (__ACE_INLINE__)
#include "ace/Message_Block.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_MESSAGE_BLOCK_H */
