// $Id$

#define ACE_BUILD_DLL
#include "ace/Message_Block.h"
#include "ace/Synch_T.h"

// #define ACE_ENABLE_TIMEPROBES
#include "ace/Timeprobe.h"

#if !defined (__ACE_INLINE__)
#include "ace/Message_Block.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Message_Block, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Message_Block)

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *ACE_MB_Timeprobe_Description[] =
{
  "Message_Block::init_i - enter",
  "Message_Block::init_i - leave",
  "Message_Block::init_i - db alloc",
  "Message_Block::init_i - db ctor",
  "Data_Block::ctor[1] - enter",
  "Data_Block::ctor[1] - leave",
  "Data_Block::ctor[2] - enter",
  "Data_Block::ctor[2] - leave"
};

enum
{
  ACE_MESSAGE_BLOCK_INIT_I_ENTER = 3000,
  ACE_MESSAGE_BLOCK_INIT_I_LEAVE,
  ACE_MESSAGE_BLOCK_INIT_I_DB_ALLOC,
  ACE_MESSAGE_BLOCK_INIT_I_DB_CTOR,
  ACE_DATA_BLOCK_CTOR1_ENTER,
  ACE_DATA_BLOCK_CTOR1_LEAVE,
  ACE_DATA_BLOCK_CTOR2_ENTER,
  ACE_DATA_BLOCK_CTOR2_LEAVE
};


// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (ACE_MB_Timeprobe_Description,
                                  ACE_MESSAGE_BLOCK_INIT_I_ENTER);

#endif /* ACE_ENABLE_TIMEPROBES */

void
ACE_Message_Block::data_block (ACE_Data_Block *db)
{
  ACE_TRACE ("ACE_Message_Block::data_block");
  if (this->data_block_ != 0)
    this->data_block_->release ();

  this->data_block_ = db;

  // Set the read and write pointers in the <Message_Block> to point
  // to the buffer in the <ACE_Data_Block>.
  this->rd_ptr (this->data_block ()->base ());
  this->wr_ptr (this->data_block ()->base ());
}

int
ACE_Message_Block::copy (const char *buf, size_t n)
{
  ACE_TRACE ("ACE_Message_Block::copy");

  // Note that for this to work correct, end() *must* be >= wr_ptr().
  size_t len = ACE_static_cast(size_t,
                               this->end () - this->wr_ptr ());
  if (len < n)
    return -1;
  else
    {
      (void) ACE_OS::memcpy (this->wr_ptr (),
                             buf,
                             n);
      this->wr_ptr (n);
      return 0;
    }
}

int
ACE_Message_Block::copy (const char *buf)
{
  ACE_TRACE ("ACE_Message_Block::copy");

  // Note that for this to work correct, end() *must* be >= wr_ptr().
  size_t len = ACE_static_cast(size_t,
                               (this->end () - this->wr_ptr ()));
  size_t buflen = ACE_OS::strlen (buf) + 1;

  if (len < buflen)
    return -1;
  else
    {
      (void) ACE_OS::memcpy (this->wr_ptr (),
                             buf,
                             buflen);
      this->wr_ptr (buflen);
      return 0;
    }
}

void
ACE_Message_Block::crunch (void)
{
  if (this->rd_ptr () > this->base ())
    {
      size_t len = this->length ();
      (void) ACE_OS::memmove (this->base (),
                              this->rd_ptr (),
                              len);
      this->rd_ptr (this->base ());
      this->wr_ptr (this->base () + len);
    }
}

void
ACE_Data_Block::dump (void) const
{
  ACE_TRACE ("ACE_Data_Block::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("-----( Data Block )-----\n")
              ASYS_TEXT ("type_ = %d\n")
              ASYS_TEXT ("cur_size_ = %u\n")
              ASYS_TEXT ("max_size_ = %u\n")
              ASYS_TEXT ("flags_ = %u\n")
              ASYS_TEXT ("base_ = %u\n")
              ASYS_TEXT ("locking_strategy_ = %u\n")
              ASYS_TEXT ("reference_count_ = %u\n")
              ASYS_TEXT ("---------------------------\n"),
              this->type_,
              this->cur_size_,
              this->max_size_,
              this->flags_,
              this->base_,
              this->locking_strategy_,
              this->reference_count_));
  this->allocator_strategy_->dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

void
ACE_Message_Block::dump (void) const
{
  ACE_TRACE ("ACE_Message_Block::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("-----( Message Block )-----\n")
              ASYS_TEXT ("priority_ = %d\n")
              ASYS_TEXT ("next_ = %u\n")
              ASYS_TEXT ("prev_ = %u\n")
              ASYS_TEXT ("cont_ = %u\n")
              ASYS_TEXT ("rd_ptr_ = %u\n")
              ASYS_TEXT ("wr_ptr_ = %u\n")
              ASYS_TEXT ("---------------------------\n"),
              this->priority_,
              this->next_,
              this->prev_,
              this->cont_,
              this->rd_ptr_,
              this->wr_ptr_));
  this->data_block ()->dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

int
ACE_Data_Block::size (size_t length)
{
  ACE_TRACE ("ACE_Data_Block::size");

  if (length <= this->max_size_)
    this->cur_size_ = length;
  else
    {
      // We need to resize!
      char *buf;
      ACE_ALLOCATOR_RETURN (buf,
                            (char *) this->allocator_strategy_->malloc (length),
                            -1);

      ACE_OS::memcpy (buf,
                      this->base_,
                      this->cur_size_);
      if (ACE_BIT_DISABLED (this->flags_,
                            ACE_Message_Block::DONT_DELETE))
        this->allocator_strategy_->free ((void *) this->base_);
      else
        // We now assume ownership.
        ACE_CLR_BITS (this->flags_,
                      ACE_Message_Block::DONT_DELETE);
      this->max_size_ = length;
      this->cur_size_ = length;
      this->base_ = buf;
    }
  return 0;
}

int
ACE_Message_Block::size (size_t length)
{
  ACE_TRACE ("ACE_Message_Block::size");

  // Resize the underlying <ACE_Data_Block>.
  if (this->data_block ()->size (length) == -1)
    return -1;

  return 0;
}

size_t
ACE_Message_Block::total_size (void) const
{
  ACE_TRACE ("ACE_Message_Block::total_size");

  size_t s = 0;

  for (ACE_Message_Block *temp = ACE_const_cast (ACE_Message_Block *, this);
       temp != 0;
       temp = temp->cont ())
    s += temp->size ();

  return s;
}

ACE_Data_Block::ACE_Data_Block (void)
  : type_ (ACE_Message_Block::MB_DATA),
    cur_size_ (0),
    max_size_ (0),
    flags_ (ACE_Message_Block::DONT_DELETE),
    base_ (0),
    allocator_strategy_ (0),
    locking_strategy_ (0),
    reference_count_ (1),
    data_block_allocator_ (0)
{
  ACE_TRACE ("ACE_Data_Block::ACE_Data_Block");
  ACE_FUNCTION_TIMEPROBE (ACE_DATA_BLOCK_CTOR1_ENTER);
}

ACE_Data_Block::ACE_Data_Block (size_t size,
                                ACE_Message_Block::ACE_Message_Type msg_type,
                                const char *msg_data,
                                ACE_Allocator *allocator_strategy,
                                ACE_Lock *locking_strategy,
                                ACE_Message_Block::Message_Flags flags,
                                ACE_Allocator *data_block_allocator)
  : type_ (msg_type),
    cur_size_ (size),
    max_size_ (size),
    flags_ (flags),
    base_ ((char *) msg_data),
    allocator_strategy_ (allocator_strategy),
    locking_strategy_ (locking_strategy),
    reference_count_ (1),
    data_block_allocator_ (data_block_allocator)
{
  ACE_TRACE ("ACE_Data_Block::ACE_Data_Block");
  ACE_FUNCTION_TIMEPROBE (ACE_DATA_BLOCK_CTOR2_ENTER);

  // If the user didn't pass one in, let's use the
  // <ACE_Allocator::instance>.
  if (this->allocator_strategy_ == 0)
    ACE_ALLOCATOR (this->allocator_strategy_,
                   ACE_Allocator::instance ());

  if (this->data_block_allocator_ == 0)
    ACE_ALLOCATOR (this->data_block_allocator_,
                   ACE_Allocator::instance ());

  if (msg_data == 0)
    ACE_ALLOCATOR (this->base_,
                   (char *) this->allocator_strategy_->malloc (size));
  else
    this->base_ = (char *) msg_data;
}

ACE_Message_Block::ACE_Message_Block (const char *data,
                                      size_t size,
                                      u_long priority)
  : data_block_ (0)
{
  ACE_TRACE ("ACE_Message_Block::ACE_Message_Block");

  if (this->init_i (size,    // size
                    MB_DATA, // type
                    0,       // cont
                    data,    // data
                    0,       // allocator
                    0,       // locking strategy
                    ACE_Message_Block::DONT_DELETE, // flags
                    priority, // priority
                    ACE_Time_Value::zero,     // execution time
                    ACE_Time_Value::max_time, // absolute time of deadline
                    0,  // data block
                    0,  // data_block allocator
                    0) == -1) // message_block allocator
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("ACE_Message_Block")));
}

ACE_Message_Block::ACE_Message_Block (ACE_Allocator *message_block_allocator)
  : data_block_ (0)
{
  ACE_TRACE ("ACE_Message_Block::ACE_Message_Block");

  if (this->init_i (0,       // size
                    MB_DATA, // type
                    0,       // cont
                    0,       // data
                    0,       // allocator
                    0,       // locking strategy
                    ACE_Message_Block::DONT_DELETE, // flags
                    0, // priority
                    ACE_Time_Value::zero,     // execution time
                    ACE_Time_Value::max_time, // absolute time of deadline
                    0, // data block
                    0, // data_block allocator
                    message_block_allocator) == -1) // message_block allocator
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("ACE_Message_Block")));
}

ACE_Message_Block::ACE_Message_Block (size_t size,
                                      ACE_Message_Type msg_type,
                                      ACE_Message_Block *msg_cont,
                                      const char *msg_data,
                                      ACE_Allocator *allocator_strategy,
                                      ACE_Lock *locking_strategy,
                                      u_long priority,
                                      const ACE_Time_Value & execution_time,
                                      const ACE_Time_Value & deadline_time,
                                      ACE_Allocator *data_block_allocator,
                                      ACE_Allocator *message_block_allocator)
  : data_block_ (0)
{
  ACE_TRACE ("ACE_Message_Block::ACE_Message_Block");

  if (this->init_i (size,
                    msg_type,
                    msg_cont,
                    msg_data,
                    allocator_strategy,
                    locking_strategy,
                    msg_data ? ACE_Message_Block::DONT_DELETE : 0,
                    priority,
                    execution_time,
                    deadline_time,
                    0, // data block
                    data_block_allocator,
                    message_block_allocator) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("ACE_Message_Block")));
}

int
ACE_Message_Block::init (size_t size,
                         ACE_Message_Type msg_type,
                         ACE_Message_Block *msg_cont,
                         const char *msg_data,
                         ACE_Allocator *allocator_strategy,
                         ACE_Lock *locking_strategy,
                         u_long priority,
                         const ACE_Time_Value & execution_time,
                         const ACE_Time_Value & deadline_time,
                         ACE_Allocator *data_block_allocator,
                         ACE_Allocator *message_block_allocator)
{
  ACE_TRACE ("ACE_Message_Block::init");

  return this->init_i (size,
                       msg_type,
                       msg_cont,
                       msg_data,
                       allocator_strategy,
                       locking_strategy,
                       msg_data ? ACE_Message_Block::DONT_DELETE : 0,
                       priority,
                       execution_time,
                       deadline_time,
                       0,  // data block
                       data_block_allocator,
                       message_block_allocator);
}

int
ACE_Message_Block::init (const char *data,
                         size_t size)
{
  ACE_TRACE ("ACE_Message_Block::init");
  // Should we also initialize all the other fields, as well?

  return this->init_i (size,    // size
                       MB_DATA, // type
                       0,       // cont
                       data,    // data
                       0,       // allocator
                       0,       // locking strategy
                       ACE_Message_Block::DONT_DELETE,  // flags
                       0,  // priority
                       ACE_Time_Value::zero,     // execution time
                       ACE_Time_Value::max_time, // absolute time of deadline
                       0,  // data block
                       0,  // data_block allocator
                       0); // message_block allocator
}

ACE_Message_Block::ACE_Message_Block (size_t size,
                                      ACE_Message_Type msg_type,
                                      ACE_Message_Block *msg_cont,
                                      const char *msg_data,
                                      ACE_Allocator *allocator_strategy,
                                      ACE_Lock *locking_strategy,
                                      Message_Flags flags,
                                      u_long priority,
                                      const ACE_Time_Value & execution_time,
                                      const ACE_Time_Value & deadline_time,
                                      ACE_Data_Block *db,
                                      ACE_Allocator *data_block_allocator,
                                      ACE_Allocator *message_block_allocator)
  : data_block_ (0)
{
  ACE_TRACE ("ACE_Message_Block::ACE_Message_Block");

  if (this->init_i (size,
                    msg_type,
                    msg_cont,
                    msg_data,
                    allocator_strategy,
                    locking_strategy,
                    flags,
                    priority,
                    execution_time,
                    deadline_time,
                    db,
                    data_block_allocator,
                    message_block_allocator) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("ACE_Message_Block")));
}

ACE_Message_Block::ACE_Message_Block (ACE_Data_Block *data_block,
                                      ACE_Allocator *message_block_allocator)
  : data_block_ (0)
{
  ACE_TRACE ("ACE_Message_Block::ACE_Message_Block");

  if (this->init_i (0,         // size
                    MB_NORMAL, // type
                    0,         // cont
                    0,         // data
                    0,         // allocator
                    0,         // locking strategy
                    0,         // flags
                    0,         // priority
                    ACE_Time_Value::zero,     // execution time
                    ACE_Time_Value::max_time, // absolute time of deadline
                    data_block, // data block
                    data_block->data_block_allocator (),
                    message_block_allocator) == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("ACE_Message_Block")));
}

int
ACE_Message_Block::init_i (size_t size,
                           ACE_Message_Type msg_type,
                           ACE_Message_Block *msg_cont,
                           const char *msg_data,
                           ACE_Allocator *allocator_strategy,
                           ACE_Lock *locking_strategy,
                           Message_Flags flags,
                           u_long priority,
                           const ACE_Time_Value & execution_time,
                           const ACE_Time_Value & deadline_time,
                           ACE_Data_Block *db,
                           ACE_Allocator *data_block_allocator,
                           ACE_Allocator *message_block_allocator)
{
  ACE_TRACE ("ACE_Message_Block::init_i");
  ACE_FUNCTION_TIMEPROBE (ACE_MESSAGE_BLOCK_INIT_I_ENTER);

  this->rd_ptr_ = 0;
  this->wr_ptr_ = 0;
  this->priority_ = priority;
  this->execution_time_ = execution_time;
  this->deadline_time_ = deadline_time;
  this->cont_ = msg_cont;
  this->next_ = 0;
  this->prev_ = 0;

  this->message_block_allocator_ = message_block_allocator;

  if (this->data_block_ != 0)
    {
      this->data_block_->release ();
      this->data_block_ = 0;
    }

  if (db == 0)
    {
      if (data_block_allocator == 0)
        ACE_ALLOCATOR_RETURN (data_block_allocator,
                              ACE_Allocator::instance (),
                              -1);

      ACE_TIMEPROBE (ACE_MESSAGE_BLOCK_INIT_I_DB_ALLOC);

      // Allocate the <ACE_Data_Block> portion, which is reference
      // counted.
      ACE_NEW_MALLOC_RETURN (db,
                             ACE_static_cast(ACE_Data_Block*,
                                             data_block_allocator->malloc (sizeof (ACE_Data_Block))),
                             ACE_Data_Block (size,
                                             msg_type,
                                             msg_data,
                                             allocator_strategy,
                                             locking_strategy,
                                             flags,
                                             data_block_allocator),
                             -1);
      ACE_TIMEPROBE (ACE_MESSAGE_BLOCK_INIT_I_DB_CTOR);
    }

  // Reset the data_block_ pointer.
  this->data_block (db);
  return 0;
}

ACE_Data_Block::~ACE_Data_Block (void)
{
  // Sanity check...
  ACE_ASSERT (this->reference_count_ <= 1);

  // Just to be safe...
  this->reference_count_ = 0;

  if (ACE_BIT_DISABLED (this->flags_,
                        ACE_Message_Block::DONT_DELETE))
    {
      this->allocator_strategy_->free ((void *) this->base_);
      this->base_ = 0;
    }
}

ACE_Data_Block *
ACE_Data_Block::release_i (void)
{
  ACE_TRACE ("ACE_Data_Block::release_i");

  ACE_ASSERT (this->reference_count_ > 0);

  ACE_Data_Block *result = 0;

  // decrement reference count
  this->reference_count_--;

  if (this->reference_count_ == 0)
    // this will cause deletion of this
    result = 0;
  else
    result = this;

  return result;
}

ACE_Data_Block *
ACE_Data_Block::release_no_delete (ACE_Lock *lock)
{
  ACE_TRACE ("ACE_Data_Block::release_no_delete");

  ACE_Data_Block *result = 0;
  ACE_Lock *lock_to_be_used = 0;

  // Check if we were passed in a lock
  if (lock != 0)
    {
      // Make sure that the lock passed in and our lock are the same
      if (lock == this->locking_strategy_)
        // In this case no locking is required.
        lock_to_be_used = 0;

      // The lock passed in does not match our lock
      else
        // Lock to be used is our lock
        lock_to_be_used = this->locking_strategy_;
    }
  // This is the case when no lock was passed in
  else
    // Lock to be used is our lock
    lock_to_be_used = this->locking_strategy_;

  // If there's a locking strategy then we need to acquire the lock
  // before decrementing the count.
  if (lock_to_be_used != 0)
    {
      ACE_GUARD_RETURN (ACE_Lock, ace_mon, *lock_to_be_used, 0);

      result = this->release_i ();
    }
  else
    result = this->release_i ();

  return result;
}

ACE_Data_Block *
ACE_Data_Block::release (ACE_Lock *lock)
{
  ACE_TRACE ("ACE_Data_Block::release");

  ACE_Allocator *allocator = this->data_block_allocator_;

  ACE_Data_Block *result = this->release_no_delete (lock);

  // We must delete this outside the scope of the locking_strategy_
  // since otherwise we'd be trying to "release" through a deleted
  // pointer!
  if (result == 0)
    ACE_DES_FREE (this,
                  allocator->free,
                  ACE_Data_Block);
  return result;
}

ACE_Message_Block *
ACE_Message_Block::release (void)
{
  ACE_TRACE ("ACE_Message_Block::release");

  // We want to hold the data block in a temporary variable because we
  // invoked "delete this;" at some point, so using this->data_block_
  // could be a bad idea.
  ACE_Data_Block *tmp = this->data_block ();

  // This flag is set to 1 when we have to destroy the data_block
  int destroy_dblock = 0;

  ACE_Lock *lock = 0;

  // Do we have a valid data block
  if (this->data_block ())
    {
      // Grab the lock that belongs to my data block
      lock = this->data_block ()->locking_strategy ();

      // if we have a lock
      if (lock != 0)
        {
          // One guard for all
          ACE_GUARD_RETURN (ACE_Lock, ace_mon, *lock, 0);

          // Call non-guarded release with <lock>
          destroy_dblock = this->release_i (lock);
        }
      // This is the case when we have a valid data block but no lock
      else
        // Call non-guarded release with no lock
        destroy_dblock = this->release_i (0);
    }
  else
    // This is the case when we don't even have a valid data block
    destroy_dblock = this->release_i (0);

  if (destroy_dblock != 0)
    ACE_DES_FREE (tmp,
                  tmp->data_block_allocator ()->free,
                  ACE_Data_Block);
  return 0;
}

int
ACE_Message_Block::release_i (ACE_Lock *lock)
{
  ACE_TRACE ("ACE_Message_Block::release_i");

  // Free up all the continuation messages.
  if (this->cont_)
    {
      ACE_Message_Block *mb = this->cont_;
      ACE_Message_Block *tmp;

      do
        {
          tmp = mb;
          mb = mb->cont_;
          tmp->cont_ = 0;

          ACE_Data_Block *db = tmp->data_block ();
          if (tmp->release_i (lock) != 0)
            ACE_DES_FREE (db,
                          db->data_block_allocator ()->free,
                          ACE_Data_Block);
        }
      while (mb);

      this->cont_ = 0;
    }

  int result = 0;

  if (this->data_block ())
    {
      if (this->data_block ()->release_no_delete (lock) == 0)
        result = 1;
      this->data_block_ = 0;
    }

  // We will now commit suicide: this object *must* have come from the
  // allocator given.
  if (this->message_block_allocator_ == 0)
    delete this;
  else
    ACE_DES_FREE (this,
                  message_block_allocator_->free,
                  ACE_Message_Block);

  return result;
}

/* static */ ACE_Message_Block *
ACE_Message_Block::release (ACE_Message_Block *mb)
{
  ACE_TRACE ("ACE_Message_Block::release");

  if (mb != 0)
    return mb->release ();
  else
    return 0;
}

ACE_Message_Block::~ACE_Message_Block (void)
{
  ACE_TRACE ("ACE_Message_Block::~ACE_Message_Block");

  if (this->data_block ())
    this->data_block ()->release ();

  this->prev_ = 0;
  this->next_ = 0;
}

ACE_Data_Block *
ACE_Data_Block::duplicate (void)
{
  ACE_TRACE ("ACE_Data_Block::duplicate");

  // Create a new <ACE_Message_Block>, but share the <base_> pointer
  // data (i.e., don't copy that).
  if (this->locking_strategy_)
    {
      // We need to acquire the lock before incrementing the count.
      ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->locking_strategy_, 0);
      this->reference_count_++;
    }
  else
    this->reference_count_++;

  return this;
}

ACE_Message_Block *
ACE_Message_Block::duplicate (void) const
{
  ACE_TRACE ("ACE_Message_Block::duplicate");

  ACE_Message_Block *nb;

  // Create a new <ACE_Message_Block> that contains unique copies of
  // the message block fields, but a reference counted duplicate of
  // the <ACE_Data_Block>.

  // If there is no allocator, use the standard new and delete calls.
  if (this->message_block_allocator_ == 0)
    ACE_NEW_RETURN (nb,
                    ACE_Message_Block (0, // size
                                       ACE_Message_Type (0), // type
                                       0, // cont
                                       0, // data
                                       0, // allocator
                                       0, // locking strategy
                                       0, // flags
                                       this->priority_, // priority
                                       this->execution_time_, // execution time
                                       this->deadline_time_, // absolute time to deadline
                                       // Get a pointer to a
                                       // "duplicated" <ACE_Data_Block>
                                       // (will simply increment the
                                       // reference count).
                                       this->data_block ()->duplicate  (),
                                       this->data_block ()->data_block_allocator (),
                                       this->message_block_allocator_),
                  0);
  else // Otherwise, use the message_block_allocator passed in.
    ACE_NEW_MALLOC_RETURN (nb,
                           ACE_reinterpret_cast(ACE_Message_Block*,
                                                message_block_allocator_->malloc (sizeof (ACE_Message_Block))),
                           ACE_Message_Block (0, // size
                                              ACE_Message_Type (0), // type
                                              0, // cont
                                              0, // data
                                              0, // allocator
                                              0, // locking strategy
                                              0, // flags
                                              this->priority_, // priority
                                              this->execution_time_, // execution time
                                              this->deadline_time_, // absolute time to deadline
                                              // Get a pointer to a
                                              // "duplicated" <ACE_Data_Block>
                                              // (will simply increment the
                                              // reference count).
                                              this->data_block ()->duplicate  (),
                                              this->data_block ()->data_block_allocator (),
                                              this->message_block_allocator_),
                           0);

  // Set the read and write pointers in the new <Message_Block> to the
  // same relative offset as in the existing <Message_Block>.  Note
  // that we are assuming that the data_block()->base() pointer
  // doesn't change when it's duplicated.
  nb->rd_ptr (this->rd_ptr_);
  nb->wr_ptr (this->wr_ptr_);

  // Increment the reference counts of all the continuation messages.
  if (this->cont_)
    {
      nb->cont_ = this->cont_->duplicate ();

      // If things go wrong, release all of our resources and return
      // 0.
      if (nb->cont_ == 0)
        {
          nb->release ();
          nb = 0;
        }
    }

  return nb;
}

ACE_Message_Block *
ACE_Message_Block::duplicate (const ACE_Message_Block *mb)
{
  ACE_TRACE ("ACE_Message_Block::duplicate");
  if (mb == 0)
    return 0;
  else
    return mb->duplicate ();
}

ACE_Data_Block *
ACE_Data_Block::clone (ACE_Message_Block::Message_Flags mask) const
{
  ACE_TRACE ("ACE_Data_Block::clone");

  // You always want to clear this one to prevent memory leaks but you
  // might add some others later.
  const ACE_Message_Block::Message_Flags always_clear =
    ACE_Message_Block::DONT_DELETE;

  ACE_Data_Block *nb;

  ACE_NEW_MALLOC_RETURN (nb,
                         ACE_static_cast(ACE_Data_Block*,
                                         this->data_block_allocator_->malloc (sizeof (ACE_Data_Block))),
                         ACE_Data_Block (this->max_size_, // size
                                         this->type_,     // type
                                         0,               // data
                                         this->allocator_strategy_, // allocator
                                         this->locking_strategy_, // locking strategy
                                         this->flags_,  // flags
                                         this->data_block_allocator_),
                         0);

  // Copy all of the payload memory into the new object.
  ACE_OS::memcpy (nb->base_,
                  this->base_,
                  this->max_size_);

  // Set new flags minus the mask...
  nb->clr_flags (mask | always_clear);
  return nb;
}

ACE_Message_Block *
ACE_Message_Block::clone (Message_Flags mask) const
{
  ACE_TRACE ("ACE_Message_Block::clone");

  // Get a pointer to a "cloned" <ACE_Data_Block> (will copy the
  // values rather than increment the reference count).
  ACE_Data_Block *db = this->data_block ()->clone (mask);

  if (db == 0)
    return 0;

  ACE_Message_Block *nb;

  if(message_block_allocator_ == 0)
    {
      nb = new ACE_Message_Block (0, // size
                                  ACE_Message_Type (0), // type
                                  0, // cont
                                  0, // data
                                  0, // allocator
                                  0, // locking strategy
                                  0, // flags
                                  this->priority_, // priority
                                  this->execution_time_, // execution time
                                  this->deadline_time_, // absolute time to deadline
                                // Get a pointer to a
                                // "duplicated" <ACE_Data_Block>
                                // (will simply increment the
                                // reference count).
                                db,
                                db->data_block_allocator (),
                                this->message_block_allocator_);
    }
  else
    {
      // This is the ACE_NEW_MALLOC macro with the return check removed.
      // We need to do it this way because if it fails we need to release
      // the cloned data block that was created above.  If we used
      // ACE_NEW_MALLOC_RETURN, there would be a memory leak because the
      // above db pointer would be left dangling.
      nb = ACE_static_cast(ACE_Message_Block*,message_block_allocator_->malloc (sizeof (ACE_Message_Block)));
      if(nb != 0)
        new (nb) ACE_Message_Block (0, // size
                                    ACE_Message_Type (0), // type
                                    0, // cont
                                    0, // data
                                    0, // allocator
                                    0, // locking strategy
                                    0, // flags
                                    this->priority_, // priority
                                    this->execution_time_, // execution time
                                    this->deadline_time_, // absolute time to deadline
                                    db,
                                    db->data_block_allocator(),
                                    this->message_block_allocator_);
    }

  if (nb == 0)
    {
      db->release ();
      return 0;
    }

  // Set the read and write pointers in the new <Message_Block> to the
  // same relative offset as in the existing <Message_Block>.
  nb->rd_ptr (this->rd_ptr_);
  nb->wr_ptr (this->wr_ptr_);

  // Clone all the continuation messages if necessary.
  if (this->cont () != 0
      && (nb->cont_ = this->cont ()->clone (mask)) == 0)
    {
      nb->release ();
      return 0;
    }
  return nb;
}

// This is private.
ACE_Message_Block &
ACE_Message_Block::operator= (const ACE_Message_Block &)
{
  ACE_TRACE ("ACE_Message_Block::operator=");
  return *this;
}

void
ACE_Data_Block::base (char *msg_data,
                      size_t msg_length,
                      ACE_Message_Block::Message_Flags msg_flags)
{
  if (ACE_BIT_DISABLED (this->flags_,
                        ACE_Message_Block::DONT_DELETE))
    this->allocator_strategy_->free (this->base_);
  this->max_size_ = msg_length;
  this->cur_size_ = msg_length;
  this->base_ = msg_data;
  this->flags_ = msg_flags;
}

// ctor

ACE_Dynamic_Message_Strategy::ACE_Dynamic_Message_Strategy (u_long static_bit_field_mask,
                                                            u_long static_bit_field_shift,
                                                            u_long dynamic_priority_max,
                                                            u_long dynamic_priority_offset)
  : static_bit_field_mask_ (static_bit_field_mask),
    static_bit_field_shift_ (static_bit_field_shift),
    dynamic_priority_max_ (dynamic_priority_max),
    dynamic_priority_offset_ (dynamic_priority_offset),
    max_late_ (0, dynamic_priority_offset - 1),
    min_pending_ (0, dynamic_priority_offset),
    pending_shift_ (0, dynamic_priority_max)
{
}

// dtor

ACE_Dynamic_Message_Strategy::~ACE_Dynamic_Message_Strategy (void)
{
}

// Dump the state of the strategy.

void
ACE_Dynamic_Message_Strategy::dump (void) const
{
  ACE_TRACE ("ACE_Dynamic_Message_Strategy::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("static_bit_field_mask_ = %lu\n")
              ASYS_TEXT ("static_bit_field_shift_ = %lu\n")
              ASYS_TEXT ("dynamic_priority_max_ = %lu\n")
              ASYS_TEXT ("dynamic_priority_offset_ = %lu\n")
              ASYS_TEXT ("max_late_ = [%ld sec, %ld usec]\n")
              ASYS_TEXT ("min_pending_ = [%ld sec, %ld usec]\n")
              ASYS_TEXT ("pending_shift_ = [%ld sec, %ld usec]\n"),
              this->static_bit_field_mask_,
              this->static_bit_field_shift_,
              this->dynamic_priority_max_,
              this->dynamic_priority_offset_,
              this->max_late_.sec (),
              this->max_late_.usec (),
              this->min_pending_.sec (),
              this->min_pending_.usec (),
              this->pending_shift_.sec (),
              this->pending_shift_.usec ()));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Deadline_Message_Strategy:: ACE_Deadline_Message_Strategy (u_long static_bit_field_mask,
                                                               u_long static_bit_field_shift,
                                                               u_long dynamic_priority_max,
                                                               u_long dynamic_priority_offset)
  : ACE_Dynamic_Message_Strategy (static_bit_field_mask,
                                  static_bit_field_shift,
                                  dynamic_priority_max,
                                  dynamic_priority_offset)
{
}

ACE_Deadline_Message_Strategy::~ACE_Deadline_Message_Strategy (void)
{
}

void
ACE_Deadline_Message_Strategy::dump (void) const
{
  ACE_TRACE ("ACE_Deadline_Message_Strategy::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("ACE_Dynamic_Message_Strategy base class: \n")));
  this->ACE_Dynamic_Message_Strategy::dump ();

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nderived class: ACE_Deadline_Message_Strategy\n")));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_Laxity_Message_Strategy::ACE_Laxity_Message_Strategy (u_long static_bit_field_mask,
                                                          u_long static_bit_field_shift,
                                                          u_long dynamic_priority_max,
                                                          u_long dynamic_priority_offset)
  : ACE_Dynamic_Message_Strategy (static_bit_field_mask,
                                  static_bit_field_shift,
                                  dynamic_priority_max,
                                  dynamic_priority_offset)
{
}

ACE_Laxity_Message_Strategy::~ACE_Laxity_Message_Strategy (void)
{
}

void
ACE_Laxity_Message_Strategy::dump (void) const
{
  ACE_TRACE ("ACE_Laxity_Message_Strategy::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("ACE_Dynamic_Message_Strategy base class: \n")));
  this->ACE_Dynamic_Message_Strategy::dump ();

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nderived class: ACE_Laxity_Message_Strategy\n")));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
  // Dump the state of the strategy.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Guard <ACE_Lock>;
// These specializations aren't needed for the ACE library because
// Service_Config.cpp has them:
//
// template class ACE_Malloc <ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex>;
// template class ACE_Allocator_Adapter <ACE_Malloc <ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex> >;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Guard <ACE_Lock>
// These specializations aren't needed for the ACE library because
// Service_Config.cpp has them:
//
// #pragma instantiate ACE_Malloc <ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex>
// #pragma instantiate ACE_Allocator_Adapter <ACE_Malloc <ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex> >
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
