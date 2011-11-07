// $Id$

#include "orbsvcs/Notify/Routing_Slip_Persistence_Manager.h"
#include "orbsvcs/Notify/Standard_Event_Persistence.h"
#include "orbsvcs/Notify/Persistent_File_Allocator.h"
#include "ace/Truncate.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{

Routing_Slip_Persistence_Manager::Routing_Slip_Persistence_Manager(
  Standard_Event_Persistence_Factory* factory)
  : removed_(false)
  , serial_number_(0)
  , allocator_(factory->allocator())
  , factory_(factory)
  , first_event_block_(0)
  , first_routing_slip_block_(0)
  , callback_(0)
  , event_mb_ (0)
  , routing_slip_mb_(0)
{
  this->prev_manager_ = this;
  this->next_manager_ = this;
}

Routing_Slip_Persistence_Manager::~Routing_Slip_Persistence_Manager()
{
  ACE_ASSERT(this->prev_manager_ == this);
  ACE_ASSERT(this->next_manager_ == this);
  delete this->first_event_block_;
  this->first_event_block_ = 0;
  delete this->first_routing_slip_block_;
  this->first_routing_slip_block_ = 0;
  delete this->event_mb_;
  this->event_mb_ = 0;
  delete this->routing_slip_mb_;
  this->routing_slip_mb_ = 0;
}

void
Routing_Slip_Persistence_Manager::set_callback(Persistent_Callback* callback)
{
  ACE_GUARD(TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->callback_ = callback;
}

bool
Routing_Slip_Persistence_Manager::store_root()
{
  bool result = false;

  this->factory_->get_preallocated_pointer (
    this->routing_slip_header_.next_serial_number,
    this->routing_slip_header_.next_routing_slip_block);

  // we should already have a psb, but JIC
  ACE_ASSERT(this->first_routing_slip_block_ != 0);
  ACE_ASSERT(this->first_routing_slip_block_->block_number() ==
    ROUTING_SLIP_ROOT_BLOCK_NUMBER);

  // Don't take any chances.  Use hard-wired root serial number.
  this->routing_slip_header_.serial_number = ROUTING_SLIP_ROOT_SERIAL_NUMBER;

  // This will eventually break after something like 58000 years.
  // At such time we should change this to !=.
  ACE_ASSERT(this->routing_slip_header_.next_serial_number >
    ROUTING_SLIP_ROOT_SERIAL_NUMBER);

  ACE_Message_Block versioninfo(2);
  versioninfo.wr_ptr()[0] = 1; // Major version number
  versioninfo.wr_ptr()[1] = 0; // Minor version number
  versioninfo.wr_ptr(2);
  ACE_GUARD_RETURN(TAO_SYNCH_MUTEX, ace_mon, this->lock_, result);
  result = this->build_chain(this->first_routing_slip_block_,
    this->routing_slip_header_, this->allocated_routing_slip_blocks_,
    versioninfo);
  if (result)
  {
   this->routing_slip_header_.put_header(*this->first_routing_slip_block_);
   this->allocator_->write(this->first_routing_slip_block_);
  }
  return result;
}

bool
Routing_Slip_Persistence_Manager::reload(
  ACE_Message_Block*& event,
  ACE_Message_Block*& routing_slip)
{
  bool result = false;
  if (this->event_mb_ != 0 && this->routing_slip_mb_ != 0)
  {
    event = this->event_mb_;
    this->event_mb_ = 0;
    routing_slip = this->routing_slip_mb_;
    this->routing_slip_mb_ = 0;
    result = true;
  }
  else
  {
    event = 0;
    routing_slip = 0;
  }
  return result;
}

bool
Routing_Slip_Persistence_Manager::load(
  Block_Number block_number,
  Block_Serial_Number expected_serial_number)
{
  /**
   * NOTE: There is no need to worry about guarding anything.  We assume
   *       that there will be one and only one thread doing the entire
   *       reload process.
   */
  bool result = false;
  size_t block_size = this->allocator_->block_size();
  this->first_routing_slip_block_ =
    this->allocator_->allocate_at(block_number);
  this->first_routing_slip_block_->set_allocator_owns(false);
  this->first_routing_slip_block_->set_sync();

  this->serial_number_ = expected_serial_number;

  ACE_NEW_NORETURN(this->routing_slip_mb_, ACE_Message_Block(block_size));
  ACE_NEW_NORETURN(this->event_mb_, ACE_Message_Block(block_size));
  if (this->event_mb_ != 0 && this->routing_slip_mb_ != 0)
  {
    if (this->reload_chain(
          this->first_routing_slip_block_,
          this->routing_slip_header_,
          this->allocated_routing_slip_blocks_,
          this->routing_slip_mb_,
          expected_serial_number))
    {
      if (this->routing_slip_header_.event_block != 0)
      {
        this->first_event_block_ = this->allocator_->allocate_at(
          this->routing_slip_header_.event_block);
        result = this->reload_chain(
          this->first_event_block_,
          this->event_header_,
          this->allocated_event_blocks_,
          this->event_mb_,
          0);
      }
      else if (block_number == ROUTING_SLIP_ROOT_BLOCK_NUMBER)
      {
        // only the root can lack event
        result = true;
      }
      else
      {
        ACE_ERROR((LM_ERROR,
          ACE_TEXT(
            "(%P|%t) Reloaded Persistent Event is missing event.\n")
          ));
      }
    }
  }
  if (! result)
  {
    delete this->routing_slip_mb_;
    this->routing_slip_mb_ = 0;
    delete this->event_mb_;
    this->event_mb_ = 0;
  }
  return result;
}

Routing_Slip_Persistence_Manager *
Routing_Slip_Persistence_Manager::load_next ()
{
  Routing_Slip_Persistence_Manager * result;
  ACE_NEW_RETURN(result, Routing_Slip_Persistence_Manager (this->factory_), 0);

  if (result->load(this->routing_slip_header_.next_routing_slip_block,
    this->routing_slip_header_.next_serial_number))
  {
    result->dllist_push_back();
  }
  else
  {
    // steal the psb for use as the next psb.
    // delete the rspm.  We'll create another one later.
    Persistent_Storage_Block * next_psb = result->first_routing_slip_block_;
    result->first_routing_slip_block_ = 0;
//    next_psb->set_allocator_owns(true);
    this->factory_->done_reloading (
      next_psb,
      result->serial_number_);
    delete result;
    result = 0;
  }
  return result;
}

bool
Routing_Slip_Persistence_Manager::store(const ACE_Message_Block& event,
  const ACE_Message_Block& routing_slip)
{
  bool result = false;
  ACE_GUARD_RETURN(TAO_SYNCH_MUTEX, ace_mon, this->lock_, result);
  if (!this->removed_)
  {
    result = store_i(event, routing_slip);
  }
  return result;
}

bool
Routing_Slip_Persistence_Manager::update(const ACE_Message_Block& routing_slip)
{
  bool result = false;
  ACE_GUARD_RETURN(TAO_SYNCH_MUTEX, ace_mon, this->lock_, result);
  // If we have not gotten the event yet or we have no allocator, fail
  if (!this->removed_)
  {
    if (this->persisted())
    {
      result = update_i(routing_slip);
    }
  }
  return result;
}

bool
Routing_Slip_Persistence_Manager::remove()
{
  bool result = false;
  ACE_GUARD_RETURN(TAO_SYNCH_MUTEX, ace_mon, this->lock_, result);
  // Assert that this is in the dllist
  ACE_ASSERT(this->prev_manager_ != this);
  ACE_ASSERT(this->persisted());
  Routing_Slip_Persistence_Manager* prev = this->prev_manager_;
  // Once our previous manager removes us, we can deallocate in any order
  this->factory_->lock.acquire();
  this->remove_from_dllist();
  result = prev->update_next_manager(this);
  this->factory_->lock.release();
  size_t block_number = 0;
  if (this->first_routing_slip_block_ != 0)
  {
    this->allocator_->free(this->first_routing_slip_block_->block_number());
    delete this->first_routing_slip_block_;
    this->first_routing_slip_block_ = 0;
  }
  if (this->first_event_block_ != 0)
  {
    this->allocator_->free(this->first_event_block_->block_number());
    delete this->first_event_block_;
    this->first_event_block_ = 0;
  }
  while (this->allocated_routing_slip_blocks_.pop(block_number) == 0)
  {
    this->allocator_->free(block_number);
  }
  while (this->allocated_event_blocks_.pop(block_number) == 0)
  {
    this->allocator_->free(block_number);
  }
  this->removed_ = true;
  Persistent_Storage_Block* callbackblock =
    this->allocator_->allocate_nowrite();
  callbackblock->set_callback(this->callback_);
  result &= this->allocator_->write(callbackblock);
  return result;
}

Routing_Slip_Persistence_Manager::Block_Header::Block_Header(Header_Type type)
  : serial_number (0)
  , next_overflow(0)
  , header_type (static_cast<Block_Type> (type))
  , data_size(0)
{
}
Routing_Slip_Persistence_Manager::Block_Header::~Block_Header (void)
{
}

size_t
Routing_Slip_Persistence_Manager::Block_Header::extract_header(
  Persistent_Storage_Block& psb, size_t offset)
{
  size_t pos = offset;
  unsigned char* data = psb.data();

  serial_number = data[pos++];
  serial_number = (serial_number << 8) + data[pos++];
  serial_number = (serial_number << 8) + data[pos++];
  serial_number = (serial_number << 8) + data[pos++];
  serial_number = (serial_number << 8) + data[pos++];
  serial_number = (serial_number << 8) + data[pos++];
  serial_number = (serial_number << 8) + data[pos++];
  serial_number = (serial_number << 8) + data[pos++];

  next_overflow = data[pos++];
  next_overflow = (next_overflow << 8) + data[pos++];
  next_overflow = (next_overflow << 8) + data[pos++];
  next_overflow = (next_overflow << 8) + data[pos++];

  header_type = data[pos++];
  header_type = (data_size << 8) + data[pos++];

  data_size = data[pos++];
  data_size = (data_size << 8) + data[pos++];
  return pos;
}

size_t
Routing_Slip_Persistence_Manager::Block_Header::put_header(
  Persistent_Storage_Block& psb, size_t offset)
{
  // Assume that our psb can hold our small amount of data...
  size_t pos = offset;
  unsigned char* data = psb.data();
  // Store serial_number
  data[pos++] = static_cast<unsigned char> ((serial_number >> 56) & 0xff);
  data[pos++] = static_cast<unsigned char> ((serial_number >> 48) & 0xff);
  data[pos++] = static_cast<unsigned char> ((serial_number >> 40) & 0xff);
  data[pos++] = static_cast<unsigned char> ((serial_number >> 32) & 0xff);
  data[pos++] = static_cast<unsigned char> ((serial_number >> 24) & 0xff);
  data[pos++] = static_cast<unsigned char> ((serial_number >> 16) & 0xff);
  data[pos++] = static_cast<unsigned char> ((serial_number >> 8) & 0xff);
  data[pos++] = static_cast<unsigned char> ((serial_number >> 0) & 0xff);
  // Store next_overflow
  data[pos++] = static_cast<unsigned char> (next_overflow >> 24);
  data[pos++] = static_cast<unsigned char> ((next_overflow >> 16) & 0xff);
  data[pos++] = static_cast<unsigned char> ((next_overflow >> 8) & 0xff);
  data[pos++] = static_cast<unsigned char> (next_overflow & 0xff);
  // Store header_type
  data[pos++] = static_cast<unsigned char> ((header_type >> 8) & 0xff);
  data[pos++] = static_cast<unsigned char> (header_type & 0xff);
  // Store data_size
  data[pos++] = static_cast<unsigned char> ((data_size >> 8) & 0xff);
  data[pos++] = static_cast<unsigned char> (data_size & 0xff);

  return pos;
}

Routing_Slip_Persistence_Manager::Routing_Slip_Header::Routing_Slip_Header()
  : Block_Header (BT_Event)
  , next_routing_slip_block(0)
  , next_serial_number(0)
  , event_block(0)
{
}

size_t
Routing_Slip_Persistence_Manager::Routing_Slip_Header::extract_header(
  Persistent_Storage_Block& psb, size_t offset)
{
  size_t pos = offset;
  pos = this->Block_Header::extract_header(psb, pos);
  unsigned char* data = psb.data();
  next_routing_slip_block = data[pos++];
  next_routing_slip_block = (next_routing_slip_block << 8) + data[pos++];
  next_routing_slip_block = (next_routing_slip_block << 8) + data[pos++];
  next_routing_slip_block = (next_routing_slip_block << 8) + data[pos++];
  next_serial_number = data[pos++];
  next_serial_number = (next_serial_number << 8) + data[pos++];
  next_serial_number = (next_serial_number << 8) + data[pos++];
  next_serial_number = (next_serial_number << 8) + data[pos++];
  next_serial_number = (next_serial_number << 8) + data[pos++];
  next_serial_number = (next_serial_number << 8) + data[pos++];
  next_serial_number = (next_serial_number << 8) + data[pos++];
  next_serial_number = (next_serial_number << 8) + data[pos++];
  event_block = data[pos++];
  event_block = (event_block << 8) + data[pos++];
  event_block = (event_block << 8) + data[pos++];
  event_block = (event_block << 8) + data[pos++];
  return pos;
}

size_t
Routing_Slip_Persistence_Manager::Routing_Slip_Header::put_header(
  Persistent_Storage_Block& psb, size_t offset)
{
  // Assume that our psb can hold our small amount of data...
  size_t pos = offset;
  // Store serial number, next_overflow and data_size
  pos = this->Block_Header::put_header(psb, pos);

  unsigned char* data = psb.data();
  // Store next_routing_slip_block
  data[pos++] = static_cast<unsigned char> (next_routing_slip_block >> 24);
  data[pos++] = static_cast<unsigned char> ((next_routing_slip_block >> 16) & 0xff);
  data[pos++] = static_cast<unsigned char> ((next_routing_slip_block >> 8) & 0xff);
  data[pos++] = static_cast<unsigned char> (next_routing_slip_block & 0xff);
  // Store serial_number
  data[pos++] = static_cast<unsigned char> ((next_serial_number >> 56) & 0xff);
  data[pos++] = static_cast<unsigned char> ((next_serial_number >> 48) & 0xff);
  data[pos++] = static_cast<unsigned char> ((next_serial_number >> 40) & 0xff);
  data[pos++] = static_cast<unsigned char> ((next_serial_number >> 32) & 0xff);
  data[pos++] = static_cast<unsigned char> ((next_serial_number >> 24) & 0xff);
  data[pos++] = static_cast<unsigned char> ((next_serial_number >> 16) & 0xff);
  data[pos++] = static_cast<unsigned char> ((next_serial_number >> 8) & 0xff);
  data[pos++] = static_cast<unsigned char> ((next_serial_number >> 0) & 0xff);
  // Store event_block
  data[pos++] = static_cast<unsigned char> (event_block >> 24);
  data[pos++] = static_cast<unsigned char> ((event_block >> 16) & 0xff);
  data[pos++] = static_cast<unsigned char> ((event_block >> 8) & 0xff);
  data[pos++] = static_cast<unsigned char> (event_block & 0xff);
  return pos;
}

Routing_Slip_Persistence_Manager::Overflow_Header::Overflow_Header ()
  : Block_Header (BT_Overflow)
{
}

Routing_Slip_Persistence_Manager::Event_Header::Event_Header ()
  : Block_Header (BT_Routing_Slip)
{
}

bool
Routing_Slip_Persistence_Manager::store_i(const ACE_Message_Block& event,
  const ACE_Message_Block& routing_slip)
{
  bool result = false;

  bool initially_persisted = this->persisted();
  if (!initially_persisted)
  {
    this->factory_->lock.acquire();
    this->factory_->preallocate_next_record(this->serial_number_,
      this->first_routing_slip_block_,
      this->routing_slip_header_.next_serial_number,
      this->routing_slip_header_.next_routing_slip_block);
    this->routing_slip_header_.serial_number = this->serial_number_;
  }

  result = this->build_chain(this->first_routing_slip_block_,
    this->routing_slip_header_, this->allocated_routing_slip_blocks_,
    routing_slip);

  if (result)
  {
    // No need for a callback here since we do our own below
    result &= this->store_event(event);
    // If we have an event block allocated, update our header
    if (this->first_event_block_ != 0)
    {
      this->routing_slip_header_.event_block =
        ACE_Utils::truncate_cast<Block_Number> (this->first_event_block_->block_number());
    }
    else
    {
      ACE_ERROR((LM_ERROR,
        ACE_TEXT(
          "(%P|%t) No Event is being stored with this routing slip.\n")
        ));
    }
    // Always write our first block out.
    this->dllist_push_back();
    result &= (this->write_first_routing_slip_block() != 0);
    // because the first rs blocks everywhere have been given sync, we are
    // guaranteed that they will be totally written by the time we get to this
    // empty callback-only block.
    Persistent_Storage_Block* callbackblock =
      this->allocator_->allocate_nowrite();
    callbackblock->set_callback(this->callback_);
    result &= this->allocator_->write(callbackblock);
  }
  if (!initially_persisted)
  {
    this->factory_->lock.release();
  }
  return result;
}

bool
Routing_Slip_Persistence_Manager::update_i(
  const ACE_Message_Block& routing_slip)
{
  bool result = true;
  size_t routing_slip_size = routing_slip.total_length();
  if (routing_slip_size != 0)
  {
    result = this->build_chain(this->first_routing_slip_block_,
      this->routing_slip_header_, this->allocated_routing_slip_blocks_,
      routing_slip);

    result &= this->allocator_->write(this->first_routing_slip_block_);
  }
  Persistent_Storage_Block* callbackblock =
    this->allocator_->allocate_nowrite();
  callbackblock->set_callback(this->callback_);
  result &= this->allocator_->write(callbackblock);
  return result;
}

bool
Routing_Slip_Persistence_Manager::store_event(
  const ACE_Message_Block& event)
{
  bool result = true;
  size_t event_size = event.total_length();
  if (event_size != 0)
  {
    if (this->first_event_block_ == 0)
    {
      this->first_event_block_ = this->allocator_->allocate();
      this->first_event_block_->set_allocator_owns(false);
    }

    result = this->build_chain(this->first_event_block_,
      this->event_header_, this->allocated_event_blocks_,
      event);

    result &= this->allocator_->write(this->first_event_block_);
  }
  return result;
}

size_t
Routing_Slip_Persistence_Manager::fill_block(Persistent_Storage_Block& psb,
  size_t offset_into_block, const ACE_Message_Block* data,
  size_t offset_into_msg)
{
  unsigned char* ptr = (unsigned char*)data->rd_ptr();
  return this->fill_block(psb, offset_into_block, ptr + offset_into_msg,
  data->length() - offset_into_msg);
}

size_t
Routing_Slip_Persistence_Manager::fill_block(Persistent_Storage_Block& psb,
  size_t offset_into_block, unsigned char* data, size_t data_size)
{
  size_t result = 0;
  if (data_size > 0)
  {
    const size_t max_size = this->allocator_->block_size() - offset_into_block;
    size_t size_to_copy = data_size;
    if (size_to_copy > max_size)
    {
      size_to_copy = max_size;
      result = data_size - size_to_copy;
    }
    else
    {
      result = 0;
    }
    ACE_OS::memcpy(psb.data() + offset_into_block, data, size_to_copy);
  }
  return result;
}

bool
Routing_Slip_Persistence_Manager::build_chain(
    Persistent_Storage_Block* first_block, Block_Header& first_header,
    ACE_Unbounded_Stack<size_t>& allocated_blocks,
    const ACE_Message_Block& data)
{
  size_t data_size = data.total_length();
  size_t remainder = data_size;
  bool result = true;
  // Save the number of items currently on the allocation list for
  ACE_Unbounded_Stack<size_t> blocks_to_free;
  size_t block_number = 0;

  // reverse the order so when we pop, we free up things closer to block 0
  // first
  while (allocated_blocks.pop(block_number) == 0)
  {
    blocks_to_free.push(block_number);
  }
  size_t pos = first_header.put_header(
    *first_block);
  const ACE_Message_Block* mblk = &data;
  remainder = this->fill_block(*first_block, pos, mblk, 0);
  while ((remainder == 0) && (mblk->cont() != 0))
  {
    pos += mblk->length();
    mblk = mblk->cont();
    remainder = this->fill_block(*first_block, pos, mblk, 0);
  }
  first_header.data_size =
    static_cast<TAO_Notify::Routing_Slip_Persistence_Manager::Block_Size> (data_size - remainder);
  first_header.next_overflow = 0;

  Block_Header* prevhdr = &first_header;
  Persistent_Storage_Block* prevblk = first_block;

  while (remainder > 0)
  {
    Overflow_Header* hdr = 0;
    ACE_NEW_RETURN(hdr, Overflow_Header, result);

    Persistent_Storage_Block* curblk = this->allocator_->allocate();
    allocated_blocks.push(curblk->block_number());
    // Set the previous block's overflow "pointer" to us.
    prevhdr->next_overflow = ACE_Utils::truncate_cast<Block_Number> (curblk->block_number());
    prevhdr->put_header(*prevblk);
    pos = hdr->put_header(*curblk);
    hdr->data_size =
      static_cast<TAO_Notify::Routing_Slip_Persistence_Manager::Block_Size> (remainder);

    size_t offset_into_msg = mblk->length() - remainder;
    remainder = this->fill_block(*curblk, pos, mblk, offset_into_msg);
    while ((remainder == 0) && (mblk->cont() != 0))
    {
      pos += mblk->length();
      mblk = mblk->cont();
      remainder = this->fill_block(*curblk, pos, mblk, 0);
    }

    hdr->data_size = hdr->data_size -
      static_cast<TAO_Notify::Routing_Slip_Persistence_Manager::Block_Size> (remainder);
    if (prevblk != first_block)
    {
      // allocator obtains ownership, so write out and delete the header
      // only.
      result &= this->allocator_->write(prevblk);

      if (prevhdr != &first_header)
        delete prevhdr;
    }
    prevblk = curblk;
    prevhdr = hdr;
  }
  if (prevblk != first_block)
  {
    prevhdr->put_header(*prevblk);
    result &= this->allocator_->write(prevblk);

    if (prevhdr != &first_header)
      delete prevhdr;
  }
  pos = first_header.put_header(
    *first_block);
  // Free all but the first routing_slip_block
  while (blocks_to_free.pop(block_number) == 0)
  {
    this->allocator_->free(block_number);
  }

  return result;
}

bool
Routing_Slip_Persistence_Manager::reload_chain(
  Persistent_Storage_Block* first_block, Block_Header& first_header,
  ACE_Unbounded_Stack<size_t>& allocated_blocks,
  ACE_Message_Block* amb,
  ACE_UINT64 expected_serial_number
  )
{
  bool result = false;
  size_t block_size = this->allocator_->block_size();
  if (this->allocator_->read(first_block))
  {
    size_t pos = 0;
    size_t nextptr = 0;
    ACE_Message_Block* mbptr = amb;
    ACE_Message_Block* mbnew = 0;

    pos = first_header.extract_header(*first_block);
    if (first_header.serial_number == expected_serial_number)
    {
      // We have to copy the first block because we cache it.
      ACE_OS::memcpy(mbptr->wr_ptr(), first_block->data(),
        block_size);
      mbptr->rd_ptr(pos);
      mbptr->wr_ptr(pos + first_header.data_size);
      nextptr = first_header.next_overflow;
      while (nextptr != 0)
      {
        Overflow_Header overflow_header;
        ACE_NEW_RETURN(mbnew, ACE_Message_Block(block_size), result);
        mbptr->cont(mbnew);
        Persistent_Storage_Block* psb = this->allocator_->allocate_at(nextptr);
        mbptr = mbnew;
        // Deallocate the PSB's data and reallocate it to our wr_ptr()...
        psb->reassign_data(static_cast<unsigned char*> (static_cast<void*> (mbptr->wr_ptr())), true);
        // ...read into the PSB (whose data is inside of the AMB)...
        this->allocator_->read(psb);
        allocated_blocks.push(psb->block_number());
        // ...extract all headers so we know the data's size...
        pos = overflow_header.extract_header(*psb);
        // ...set up the region that somebody else can look at...
        mbptr->rd_ptr(pos);
        mbptr->wr_ptr(pos + overflow_header.data_size);
        // ...then make sure we don't delete data since we don't own it.
        psb->reassign_data(0);
        delete psb;
        nextptr = overflow_header.next_overflow;
      }
      result = true;
    }
  }
  return result;
}

bool
Routing_Slip_Persistence_Manager::update_next_manager(
  Routing_Slip_Persistence_Manager* next)
{
  bool result = false;
  ACE_GUARD_RETURN(TAO_SYNCH_MUTEX, ace_mon, this->lock_, result);
  ACE_ASSERT(this->persisted());
  if (!this->removed_)
  {
    bool updated = false;
    if (this->next_manager_ != 0)
    {
      if (this->routing_slip_header_.next_serial_number !=
        next->routing_slip_header_.next_serial_number)
      {
        this->routing_slip_header_.next_serial_number =
          next->routing_slip_header_.next_serial_number;
        updated = true;
      }
      if (this->routing_slip_header_.next_routing_slip_block !=
        next->routing_slip_header_.next_routing_slip_block)
      {
        this->routing_slip_header_.next_routing_slip_block =
          next->routing_slip_header_.next_routing_slip_block;
        updated = true;
      }
    }
    if (updated)
    {
      this->write_first_routing_slip_block();
    }
  }
  return result;
}

bool
Routing_Slip_Persistence_Manager::persisted()
{
  return (0 != this->first_routing_slip_block_);
}

bool
Routing_Slip_Persistence_Manager::is_root () const
{
  return this->serial_number_ == ROUTING_SLIP_ROOT_SERIAL_NUMBER;
}

void
Routing_Slip_Persistence_Manager::release_all ()
{
  ACE_ASSERT(is_root());
  while (this->next_manager_ != this)
  {
    Routing_Slip_Persistence_Manager * next = this->next_manager_;
    next->remove_from_dllist();
    ACE_ASSERT(next != this->next_manager_);
    delete next;
  }
}

size_t
Routing_Slip_Persistence_Manager::write_first_routing_slip_block(
  bool prepare_only)
{
  size_t pos = this->routing_slip_header_.put_header(
    *this->first_routing_slip_block_);
  if (!prepare_only)
  {
    this->allocator_->write(this->first_routing_slip_block_);
  }
  return pos;
}

void
Routing_Slip_Persistence_Manager::dllist_push_back()
{
  insert_before (&this->factory_->root());
}

void
Routing_Slip_Persistence_Manager::insert_before (Routing_Slip_Persistence_Manager * node)
{
  // Since this is a private function, the caller should have done locking
  // on the factory before calling here.  The same is true for removals.
  ACE_ASSERT(this->prev_manager_ == this);
  ACE_ASSERT(this->next_manager_ == this);
  ACE_ASSERT(node != this);
  this->prev_manager_ = node->prev_manager_;
  node->prev_manager_ = this;
  this->next_manager_ = node;
  this->prev_manager_->next_manager_ = this;
}

void
Routing_Slip_Persistence_Manager::remove_from_dllist()
{
  // Since this is a private function, the caller should have done locking
  // on the factory before calling here.  The same is true for insertions.
  ACE_ASSERT(this->persisted());
  ACE_ASSERT(this->prev_manager_ != this);
  ACE_ASSERT(this->next_manager_ != this);
  this->prev_manager_->next_manager_ = this->next_manager_;
  this->next_manager_->prev_manager_ = this->prev_manager_;
  this->prev_manager_ = this;
  this->next_manager_ = this;
}

} /* namespace TAO_Notify */

TAO_END_VERSIONED_NAMESPACE_DECL
