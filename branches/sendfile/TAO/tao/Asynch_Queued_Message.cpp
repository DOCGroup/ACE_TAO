#include "tao/Asynch_Queued_Message.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"

#include "ace/OS_Memory.h"
#include "ace/OS_NS_string.h"
#include "ace/os_include/sys/os_uio.h"
#include "ace/Log_Msg.h"
#include "ace/Message_Block.h"
#include "ace/Malloc_Base.h"


ACE_RCSID (tao,
           Asynch_Queued_Message,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Asynch_Queued_Message::TAO_Asynch_Queued_Message (
  const ACE_Message_Block *contents,
  TAO_ORB_Core *oc,
  ACE_Allocator *alloc,
  int is_heap_allocated)
  : TAO_Queued_Message (oc, alloc, is_heap_allocated)
  , size_ (contents->total_length ())
  , offset_ (0)
{
  // @@ Use a pool for these guys!!
  ACE_NEW (this->buffer_, char[this->size_]);

  size_t copy_offset = 0;
  for (const ACE_Message_Block *i = contents;
       i != 0;
       i = i->cont ())
    {
      ACE_OS::memcpy (this->buffer_ + copy_offset,
                      i->rd_ptr (),
                      i->length ());
      copy_offset += i->length ();
    }
}

TAO_Asynch_Queued_Message::TAO_Asynch_Queued_Message (char *buf,
                                                      TAO_ORB_Core *oc,
                                                      size_t size,
                                                      ACE_Allocator *alloc)
  : TAO_Queued_Message (oc, alloc)
  , size_ (size)
  , offset_ (0)
  , buffer_ (buf)
{
}

TAO_Asynch_Queued_Message::~TAO_Asynch_Queued_Message (void)
{
  // @@ Use a pool for these guys!
  delete [] this->buffer_;
}

size_t
TAO_Asynch_Queued_Message::message_length (void) const
{
  return this->size_ - this->offset_;
}

int
TAO_Asynch_Queued_Message::all_data_sent (void) const
{
  return this->size_ == this->offset_;
}

void
TAO_Asynch_Queued_Message::fill_iov (int iovcnt_max,
                                     int &iovcnt,
                                     iovec iov[]) const
{
  ACE_ASSERT (iovcnt_max > iovcnt);
  ACE_UNUSED_ARG (iovcnt_max); // not used if ACE_ASSERT() is empty

  iov[iovcnt].iov_base = this->buffer_ + this->offset_;
  iov[iovcnt].iov_len  = static_cast<u_long> (this->size_ - this->offset_);
  ++iovcnt;
}

void
TAO_Asynch_Queued_Message::bytes_transferred (size_t &byte_count)
{
  this->state_changed_i (TAO_LF_Event::LFS_ACTIVE);

  size_t const remaining_bytes = this->size_ - this->offset_;
  if (byte_count > remaining_bytes)
    {
      this->offset_ = this->size_;
      byte_count -= remaining_bytes;
      return;
    }
  this->offset_ += byte_count;
  byte_count = 0;

  if (this->all_data_sent ())
    this->state_changed (TAO_LF_Event::LFS_SUCCESS,
                         this->orb_core_->leader_follower ());
}


TAO_Queued_Message *
TAO_Asynch_Queued_Message::clone (ACE_Allocator *alloc)
{
  char *buf = 0;

  // @@todo: Need to use a memory pool. But certain things need to
  // change a bit in this class for that. Till then.

  // Just allocate and copy data that needs to be sent, no point
  // copying the whole buffer.
  size_t const sz = this->size_ - this->offset_;

  ACE_NEW_RETURN (buf,
                  char[sz],
                  0);

  ACE_OS::memcpy (buf,
                  this->buffer_ + this->offset_,
                  sz);

  TAO_Asynch_Queued_Message *qm = 0;

  if (alloc)
    {
      ACE_NEW_MALLOC_RETURN (qm,
                             static_cast<TAO_Asynch_Queued_Message *> (
                                 alloc->malloc (sizeof (TAO_Asynch_Queued_Message))),
                             TAO_Asynch_Queued_Message (buf,
                                                        this->orb_core_,
                                                        sz,
                                                        alloc),
                             0);
    }
  else
    {
      // No allocator, so use the common heap!
      if (TAO_debug_level == 4)
        {
          // This debug is for testing purposes!
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - Asynch_Queued_Message::clone\n"
                      "Using global pool for allocation \n"));
        }

      ACE_NEW_RETURN (qm,
                      TAO_Asynch_Queued_Message (buf,
                                                 this->orb_core_,
                                                 sz),
                      0);
    }

  // Set the flag to indicate that <qm> is created on the heap.
  if (qm)
    qm->is_heap_created_ = 1;

  return qm;
}

void
TAO_Asynch_Queued_Message::destroy (void)
{
  if (this->is_heap_created_)
    {
      // If we have an allocator release the memory to the allocator
      // pool.
      if (this->allocator_)
        {
          ACE_DES_FREE (this,
                        this->allocator_->free,
                        TAO_Asynch_Queued_Message);

        }
      else // global release..
        {
          delete this;
        }
    }

}

TAO_END_VERSIONED_NAMESPACE_DECL
