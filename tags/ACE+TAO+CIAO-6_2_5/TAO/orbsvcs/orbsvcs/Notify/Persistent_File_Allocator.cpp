// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Persistent_File_Allocator.h"

#include "tao/debug.h"
#include "ace/OS_NS_string.h"

//#define DEBUG_LEVEL 9
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{

Persistent_Callback::~Persistent_Callback()
{
}

Persistent_Storage_Block::Persistent_Storage_Block(const size_t block_number,
  const size_t block_size)
  : block_number_(block_number)
  , no_write_(false)
  , sync_(false)
  , block_size_(block_size)
  , callback_(0)
  , allocator_owns_(true)
{
  ACE_NEW(this->data_, unsigned char[this->block_size_]);
  ACE_OS::memset(this->data_, 0, this->block_size_);

}

Persistent_Storage_Block::Persistent_Storage_Block(
  const Persistent_Storage_Block& psb)
  : block_number_(psb.block_number_)
  , no_write_(psb.no_write_)
  , sync_(psb.sync_)
  , block_size_(psb.block_size_)
  , callback_(psb.callback_)
  , allocator_owns_(psb.allocator_owns_)
{
  ACE_NEW(this->data_, unsigned char[this->block_size_]);
  ACE_OS::memcpy(this->data_, psb.data(), this->block_size_);
}

Persistent_Storage_Block::~Persistent_Storage_Block()
{
  delete [] this->data_;
  this->data_ = 0;
}

void
Persistent_Storage_Block::set_no_write()
{
  this->no_write_ = true;
  this->reassign_data(0, true);
}

bool
Persistent_Storage_Block::get_no_write()
{
  return this->no_write_;
}

void
Persistent_Storage_Block::set_sync()
{
  this->sync_ = true;
}

bool
Persistent_Storage_Block::get_sync() const
{
  return this->sync_;
}

size_t
Persistent_Storage_Block::block_number() const
{
  return this->block_number_;
}

unsigned char*
Persistent_Storage_Block::data() const
{
  return this->data_;
}

void
Persistent_Storage_Block::reassign_data(unsigned char* newptr,
  bool delete_old)
{
  if (delete_old)
  {
    delete [] this->data_;
  }
  this->data_ = newptr;
}

void
Persistent_Storage_Block::set_callback(Persistent_Callback* callback)
{
  this->callback_ = callback;
}

Persistent_Callback*
Persistent_Storage_Block::get_callback() const
{
  return this->callback_;
}

void
Persistent_Storage_Block::set_allocator_owns(bool allocator_owns)
{
  this->allocator_owns_ = allocator_owns;
}

bool
Persistent_Storage_Block::get_allocator_owns() const
{
  return this->allocator_owns_;
}

Persistent_File_Allocator::Persistent_File_Allocator()
  : pstore_()
  , terminate_thread_(false)
  , thread_active_(false)
  , wake_up_thread_(queue_lock_)
{
}

Persistent_File_Allocator::~Persistent_File_Allocator()
{
  this->shutdown_thread();
}

bool
Persistent_File_Allocator::open (const ACE_TCHAR* filename,
  const size_t block_size)
{
  bool file_opened = this->pstore_.open(filename, block_size);
  if (file_opened)
  {
    this->thread_active_ = true;
    this->thread_manager_.spawn(this->thr_func, this);
  }
  return file_opened;
}

void
Persistent_File_Allocator::shutdown()
{
  this->shutdown_thread();
}

Persistent_Storage_Block*
Persistent_File_Allocator::allocate()
{
  Persistent_Storage_Block* result = 0;
  size_t block_number = 0;
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);
  if (!this->allocate_block(block_number))
  {
    //@@todo: this should never happen
    // why not.  What if the disk is full?  Oh, I see we
    // allocate non-existent blocks.  FIX this
  }
  if (DEBUG_LEVEL > 0) ORBSVCS_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P|%t) Persistent_File_Allocator::allocate: %B\n"),
    block_number
    ));
  result = this->allocate_at(block_number);
  return result;
}

Persistent_Storage_Block*
Persistent_File_Allocator::allocate_at(size_t block_number)
{
  Persistent_Storage_Block* result = 0;
  this->used(block_number);
  if (DEBUG_LEVEL > 0) ORBSVCS_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P|%t) Persistent_File_Allocator::allocate at : %B\n"),
    block_number
    ));
  ACE_NEW_RETURN(result, Persistent_Storage_Block(
    block_number,
    this->block_size()),
    0);
  return result;
}

Persistent_Storage_Block*
Persistent_File_Allocator::allocate_nowrite()
{
  Persistent_Storage_Block* result = 0;
  ACE_NEW_RETURN (result,
                  Persistent_Storage_Block (static_cast<size_t> (~0), 0),
                  0);
  result->set_no_write();

  return result;
}

void
Persistent_File_Allocator::used(size_t block_number)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->free_blocks_lock_);
  if (DEBUG_LEVEL > 0) ORBSVCS_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P|%t) Persistent_File_Allocator::used: %B\n"),
    block_number
    ));
  ACE_ASSERT (!this->free_blocks_.is_set (block_number));
  this->free_blocks_.set_bit(block_number, true);
}

void
Persistent_File_Allocator::free(size_t block_number)
{
  if (DEBUG_LEVEL > 0) ORBSVCS_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P|%t) Persistent_File_Allocator::free: %B\n"),
    block_number
    ));
  ACE_ASSERT (this->free_blocks_.is_set (block_number));
  this->free_block(block_number);
}

size_t
Persistent_File_Allocator::block_size() const
{
  return pstore_.block_size();
}

bool
Persistent_File_Allocator::read(Persistent_Storage_Block* psb)
{
  bool result = this->thread_active_;
  bool cached = false;
  if (result)
  {
    Persistent_Storage_Block** psbtemp = 0;
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->queue_lock_, false);
      size_t queue_size = this->block_queue_.size();
      for (size_t idx = 0; !cached && (idx < queue_size); ++idx)
      {
        // We want to start at the end of the queue and work backwards...
        size_t actual_block = (queue_size - idx) - 1;
        if ((0 == this->block_queue_.get(psbtemp, actual_block))
            && (psbtemp != 0))
        {
          cached = ((*psbtemp)->block_number() == psb->block_number());
        }
      }
      // this needs to be done in the guarded section
      if (cached && (0 != psbtemp))
      {
        ACE_OS::memcpy(psb->data(), (*psbtemp)->data(), this->block_size());
      }
    }
    if (!cached)
    {
      result = pstore_.read(psb->block_number(), psb->data());
    }
  }
  return result;
}

bool
Persistent_File_Allocator::write(Persistent_Storage_Block* psb)
{
  bool result = this->thread_active_;
  if (result)
  {
    Persistent_Storage_Block* ourpsb = psb;
    if (!psb->get_allocator_owns())
    {
      if (DEBUG_LEVEL) ORBSVCS_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Copy PSB %B\n")
        , psb->block_number ()
        ));
      ACE_NEW_RETURN(ourpsb, Persistent_Storage_Block(*psb), false);
      ourpsb->set_allocator_owns(true);
    }
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->queue_lock_, false);
    if (DEBUG_LEVEL) ORBSVCS_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Queueing PSB to write block %B\n")
      , psb->block_number ()
      ));
    result = (0 == this->block_queue_.enqueue_tail(ourpsb));
    this->wake_up_thread_.signal();
  }
  return result;
}

void
Persistent_File_Allocator::free_block(const size_t block_number)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->free_blocks_lock_);
  ACE_ASSERT (this->free_blocks_.is_set (block_number));
  this->free_blocks_.set_bit(block_number, false);
}

bool
Persistent_File_Allocator::allocate_block(size_t& block_number)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->free_blocks_lock_, 0);
  block_number = this->free_blocks_.find_first_bit(false);
  return true;
}

ACE_THR_FUNC_RETURN
Persistent_File_Allocator::thr_func(void * arg)
{
  Persistent_File_Allocator* pfa = static_cast<Persistent_File_Allocator*> (arg);
  pfa->run();
  return 0;
}

ACE_OFF_T
Persistent_File_Allocator::file_size () const
{
  return this->pstore_.size ();
}

void
Persistent_File_Allocator::shutdown_thread()
{
  if (this->thread_active_)
  {
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->queue_lock_);
      this->terminate_thread_ = true;
      this->wake_up_thread_.signal();
    }
    this->thread_manager_.close();
    ACE_ASSERT (!this->terminate_thread_);
    ACE_ASSERT (!this->thread_active_);
  }
}

void
Persistent_File_Allocator::run()
{
  // We need this because we could be working on writing data
  // when a call to terminate comes in!
  bool do_more_work = true;
  while (do_more_work)
  {
    do_more_work = false;
    Persistent_Storage_Block * blk = 0;
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->queue_lock_);
      while (this->block_queue_.is_empty() && !terminate_thread_)
      {
        this->wake_up_thread_.wait();
      }
      // Awkward interface to peek at head of unbounded queue
      Persistent_Storage_Block ** pblk = 0;
      if (0 == this->block_queue_.get(pblk))
      {
        do_more_work = true;
        blk = *pblk;
      }
    }
    if (0 != blk)
    {
      Persistent_Callback *callback = blk->get_callback();
      if (!blk->get_no_write())
      {
        pstore_.write(blk->block_number(), blk->data(), blk->get_sync());
      }
      {
        Persistent_Storage_Block * blk2 = 0;
        ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->queue_lock_);
        this->block_queue_.dequeue_head (blk2);
        // if this triggers, someone pushed onto the head of the queue
        // or removed the head from the queue without telling ME.
        ACE_ASSERT (blk2 == blk);
      }
      // If we own the block, then delete it.
      if (blk->get_allocator_owns())
      {
        delete blk;
        blk = 0;
      }
      if (0 != callback)
      {
        callback->persist_complete();
      }
    }
  }
  this->terminate_thread_ = false;
  this->thread_active_ = false;
}

} /* namespace TAO_Notify */

TAO_END_VERSIONED_NAMESPACE_DECL
