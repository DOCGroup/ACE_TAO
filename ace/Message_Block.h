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

class ACE_Export ACE_Message_Block
  // = TITLE
  //     Stores messages for use throughout ACE (particularly
  //     <ACE_Message_Queue>).
  //
  // = DESCRIPTION
  //     An <ACE_Message_Block> is modeled after the message data
  //     structures used in System V STREAMS.  An <ACE_Message_Block>
  //     is composed of one or more <ACE_Message_Blocks> that can be
  //     linked to form a ``fragment chain.''  In addition,
  //     <ACE_Message_Blocks> can be linked together by <prev_> and
  //     <next_> pointers to form a queue of messages (this is how
  //     <ACE_Message_Queue> works).  This structure enables efficient
  //     manipulation of arbitrarily-large messages *without*
  //     incurring memory copying overhead since (1)
  //     <ACE_Message_Blocks> can be chained together via pointers and
  //     (2) <ACE_Message_Blocks> keep a reference count.
{
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

  ACE_Message_Block (const char *data,
		     size_t size = 0);
  // Create a Message Block that assumes ownership of <data> without
  // copying it (i.e., we don't delete it since we don't malloc it!).

  ACE_Message_Block (size_t size, 
		     ACE_Message_Type type = MB_DATA, 
		     ACE_Message_Block *cont = 0, 
		     const char *data = 0,
		     ACE_Allocator *allocator_strategy_ = 0,
		     ACE_Lock *locking_strategy = 0);
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
	    ACE_Lock *locking_strategy = 0);
  // Create an initialized message of type <type> containing <size>
  // bytes.  The <cont> argument initializes the continuation field in
  // the <Message_Block>.  If <data> == 0 then we create and own the
  // <data>, using <allocator> to get the data if it's non-0.  If
  // <data> != 0 we assume ownership of the <data> (and don't delete
  // it).  If <locking_strategy> is non-0 then this is used to protect
  // regions of code that access shared state (e.g., reference
  // counting) from race conditions.

  ~ACE_Message_Block (void);
  // Delete all the resources held in the message.

  int is_data_msg (void) const;
  // Find out what type of message this is.

  ACE_Message_Type msg_class (void) const;
  // Find out what class of message this is (there are two classes,
  // <normal> messages and <high-priority> messages).

  ACE_Message_Type msg_type (void) const;
  // Get type of the message.

  // = Set/Unset/Inspect the message flags.
  Message_Flags set_flags (Message_Flags more_flags);
  Message_Flags clr_flags (Message_Flags less_flags);
  Message_Flags flags (void) const;

  void msg_type (ACE_Message_Type type);
  // Set type of the message.

  u_long msg_priority (void) const;
  // Get priority of the message.

  void msg_priority (u_long priority);
  // Set priority of the message.

  ACE_Message_Block *clone (Message_Flags mask = ACE_Message_Block::DONT_DELETE) const;
  // Return an exact "deep copy" of the message.

  // = Reference counting methods.
  ACE_Message_Block *duplicate (void);
  // Increment our reference count by one.

  ACE_Message_Block *release (void);
  // Decrement our reference count by one.  If the reference count is
  // > 0 then return this; else if reference count == 0 then delete
  // <this> and return 0.  Behavior is undefined if reference count <
  // 0.

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

  void base (char *data,
	     size_t size, 
	     Message_Flags = DONT_DELETE);
  // Set message data.

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
  // Set the total amount of space in the message.  Returns 0 if
  // successful, else -1.

  // = The continuation field chains together composite messages.
  ACE_Message_Block *cont (void) const;
  // Get the continuation field.
  void cont (ACE_Message_Block *);
  // Set the continuation field.

  // = The <next_> pointer is a link to the <Message_Block> directly ahead in the Message_Queue.
  ACE_Message_Block *next (void) const;
  // Get link to next message.
  void next (ACE_Message_Block *);
  // Set link to next message.

  // = The <prev_> pointer is a link to the <Message_Block> directly ahead in the Message_Queue.
  ACE_Message_Block *prev (void) const;
  // Get link to prev message.
  void prev (ACE_Message_Block *);
  // Set link to prev message.

  // = The locking strategy prevents race condition.
  ACE_Lock *locking_strategy (void);
  // Get the locking strategy.
  ACE_Lock *locking_strategy (ACE_Lock *);
  // Set a new locking strategy and return the hold one.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  Message_Flags flags_; 	
  // Misc flags (e.g., DONT_DELETE and USER_FLAGS).

  char *base_;	
  // Pointer to beginning of message block.

  size_t cur_size_;	
  // Current size of message block.

  size_t max_size_;	
  // Total size of buffer.

  char *rd_ptr_;	
  // Pointer to beginning of next read.

  char *wr_ptr_;	
  // Pointer to beginning of next write.

  ACE_Message_Type type_;		
  // Type of message.

  u_long priority_;	
  // Priority of message.

  // = Links to other ACE_Message_Block *s.
  ACE_Message_Block *cont_;	
  // Pointer to next message block in the chain.

  ACE_Message_Block *next_;	
  // Pointer to next message in the list.

  ACE_Message_Block *prev_;	
  // Pointer to previous message in the list.

  ACE_Allocator *allocator_strategy_;
  // Pointer to the allocator defined for this message block.

  ACE_Lock *locking_strategy_;
  // Pointer to the locking defined for this message block.  This is
  // used to protect regions of code containing 

  size_t reference_count_;
  // Reference count for this <Message_Block> which is used to avoid
  // deep copies (i.e., <clone>).

  // = Disallow these operations for now (use <clone> instead).
  ACE_Message_Block &operator= (const ACE_Message_Block &);
  ACE_Message_Block (const ACE_Message_Block &);
};

#if defined (__ACE_INLINE__)
#include "ace/Message_Block.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_MESSAGE_BLOCK_H */
