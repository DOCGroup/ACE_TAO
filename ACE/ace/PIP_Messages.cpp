// $Id$

#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_string.h"
#include "ace/PIP_Messages.h"

ACE_PIP_Message::ACE_PIP_Message()
  : block_(0)
  , dirty_(false)
  , next_(0)
{}
 
ACE_PIP_Message::~ACE_PIP_Message()
{
   if (this->next_)
   {
      delete this->next_;
   }
   
   if (block_)
   {
      block_->release();
   }
}

void ACE_PIP_Message::block(ACE_Message_Block* block)
{
   // Remove the other block if it exist.
   if (block_)
   {
      block_->release();
   }
   
   block_ = block;
   
   // Extract the values from the block.
   unpack();
}

ACE_PIP_Data_Message::ACE_PIP_Data_Message()
  : message_priority_(-1)
  , reply_expected_(false)
  , source_handler_id_(-1)
  , source_site_id_(-1)
  , destination_handler_id_(-1)
  , destination_site_id_(-1)
{
}

int ACE_PIP_Data_Message::serialize(ACE_SOCK_Stream& stream)
{
   int total_bytes_sent(0);
   
   // Only serialize if there is a block. If not,
   // there's nothing we can do but fail since we don't
   // have enough information to create a block and unpack it.
   if (block_)
   {
      if (this->dirty_)
      {
	 pack();
      }
      
      ACE_Message_Block* curr_block = block_;
      int bytes_sent(0);
      while(curr_block)
      {
	 bytes_sent = stream.send_n(curr_block->rd_ptr(), curr_block->length());
	 if (bytes_sent > 0)
	 {
	    total_bytes_sent += bytes_sent;
	    curr_block = curr_block->next();
	 }
	 else
	 {
	    total_bytes_sent = -1;
	    break;
	 }
      }
   }
   else
   {
      total_bytes_sent = -1;
   }
   
   return total_bytes_sent;
}

int ACE_PIP_Data_Message::pack()
{
   char* write_ptr = block_->wr_ptr();
   char* read_ptr = block_->rd_ptr();
   
   block_->reset();
   
   // Pack reply expected into buffer.
   ACE_OS::memcpy(block_->wr_ptr(), &this->reply_expected_, sizeof(this->reply_expected_));
   block_->wr_ptr(sizeof(this->reply_expected_));
   
   // Pack the message priority into the buffer.
   ACE_OS::memcpy(block_->wr_ptr(), &this->message_priority_, sizeof(this->message_priority_));
   block_->wr_ptr(sizeof(this->message_priority_));
   
   // Pack the destination handler ID into the buffer
   ACE_OS::memcpy(block_->wr_ptr(), &this->destination_handler_id_, sizeof(this->destination_handler_id_));
   block_->wr_ptr(sizeof(this->destination_handler_id_));

   // Pack the source handler ID into the buffer
   ACE_OS::memcpy(block_->wr_ptr(), &this->source_handler_id_, sizeof(this->source_handler_id_));
   block_->wr_ptr(sizeof(this->source_handler_id_));

   // Pack the destination site ID into the buffer
   ACE_OS::memcpy(block_->wr_ptr(), &this->destination_site_id_, sizeof(this->destination_site_id_));
   block_->wr_ptr(sizeof(this->destination_site_id_));

   // Pack the source site ID into the buffer
   ACE_OS::memcpy(block_->wr_ptr(), &this->source_site_id_, sizeof(this->source_site_id_));
   block_->wr_ptr(sizeof(this->source_site_id_));
   
   // Reset the buffer pointers to where they were so that the message length remains
   // accurate.
   block_->rd_ptr(read_ptr);
   block_->wr_ptr(write_ptr);

   return 0;
}

void ACE_PIP_Data_Message::unpack()
{
   if (block_)
   {
      char* write_ptr = block_->wr_ptr();
      block_->reset();

      // reply_expected_
      ACE_OS::memcpy(&this->reply_expected_, block_->rd_ptr(), sizeof(this->reply_expected_));
      block_->rd_ptr(sizeof(this->reply_expected_));
      
      // message priority
      ACE_OS::memcpy(&this->message_priority_, block_->rd_ptr(), sizeof(this->message_priority_));
      block_->rd_ptr(sizeof(this->message_priority_));

      // destination handler ID
      ACE_OS::memcpy(&this->destination_handler_id_, block_->rd_ptr(), sizeof(this->destination_handler_id_));
      block_->rd_ptr(sizeof(this->destination_handler_id_));

      // source handler ID
      ACE_OS::memcpy(&this->source_handler_id_, block_->rd_ptr(), sizeof(this->source_handler_id_));
      block_->rd_ptr(sizeof(this->source_handler_id_));

      // destination site ID
      ACE_OS::memcpy(&this->destination_site_id_, block_->rd_ptr(), sizeof(this->destination_site_id_));
      block_->rd_ptr(sizeof(this->destination_site_id_));

      // source site ID
      ACE_OS::memcpy(&this->source_site_id_, block_->rd_ptr(), sizeof(this->source_site_id_));
      block_->rd_ptr(sizeof(this->source_site_id_));
      
      block_->reset();
      block_->wr_ptr(write_ptr);
   }
   
   this->dirty_ = false;
}

ACE_PIP_Accel_Message::ACE_PIP_Accel_Message()
   : ACCEL_HEADER_LENGTH_(2*sizeof(ACE_UINT32))
   , destination_address_(0)
   , destination_port_(0)
   , new_priority_(0)
   , old_priority_(0)
{
}

int ACE_PIP_Accel_Message::serialize(ACE_SOCK_Stream& stream)
{   
   pack();
   
   int bytes_sent = stream.send_n(block_->rd_ptr(), block_->length());
   return bytes_sent;
}

int ACE_PIP_Accel_Message::pack()
{
   if (!block_)
   {
     ACE_NEW_RETURN(block_, ACE_Message_Block(ACCEL_HEADER_LENGTH_), -1);
     this->dirty_ = true;
   }
   
   if (this->dirty_)
   {
        
      // Set the buffer pointers to the start of the buffer to
      // ensure we're writing to the correct location
      block_->reset();
      
      // Pack the contents of the struct into the message block
      ACE_OS::memcpy(block_->wr_ptr(), &this->old_priority_, sizeof(this->old_priority_));
      block_->wr_ptr(sizeof(this->old_priority_));
      
      ACE_OS::memcpy(block_->wr_ptr(), &this->new_priority_, sizeof(this->new_priority_));
      block_->wr_ptr(sizeof(this->new_priority_));

      this->dirty_ = false;
   }

   return 0;
}

void ACE_PIP_Accel_Message::unpack()
{	
   if (block_)
   {
      char* write_ptr = block_->wr_ptr();
      block_->reset();
      
      this->old_priority_ = (*block_->rd_ptr());
      block_->rd_ptr(sizeof(this->old_priority_));
      
      this->new_priority_ = (*block_->rd_ptr());
      block_->rd_ptr(sizeof (this->new_priority_));
      
      // Reset the read and write pointers to their original location
      // in the block.
      block_->reset();
      block_->wr_ptr(write_ptr);
   }
   
   this->dirty_ = false;
}

ACE_PIP_Accel_Message* ACE_PIP_Accel_Message::copy()
{
  ACE_PIP_Accel_Message* copy(0);
  ACE_NEW_RETURN(copy, ACE_PIP_Accel_Message, 0);
  
  copy->new_priority_ = this->new_priority_;
  copy->old_priority_ = this->old_priority_;
  copy->pack();

  return copy;
}

ACE_PIP_Protocol_Message::ACE_PIP_Protocol_Message()
   : FIXED_HEADER_LENGTH_(sizeof(Message_Type) +
			  sizeof(message_id_) +
			  sizeof(num_payload_blocks_))
   , message_type_(NONE)
   , num_payload_blocks_(0)
   , message_id_(0)
{
}

int ACE_PIP_Protocol_Message::serialize(ACE_SOCK_Stream& stream)
{
   int total_bytes_sent(0);

   pack();

   ACE_Message_Block* curr_block = block_;
   int bytes_sent(0);
   
   // Write each of the message blocks associated with this
   // header into the stream
   while(curr_block)
   {
      bytes_sent = stream.send_n(curr_block->rd_ptr(), curr_block->length());
      if (bytes_sent > 0)
      {
	 total_bytes_sent += bytes_sent;
	 curr_block = curr_block->next();
      }
      else
      {
	 total_bytes_sent = -1;
	 break;
      }
   }
   if ((total_bytes_sent > 0) && this->next_)
   {
      int next_sent = this->next_->serialize(stream);
      if (next_sent > 0)
      {
	 total_bytes_sent += next_sent;
      }
      else
      {
	 total_bytes_sent = -1;
      }
   }
   else
   {
      total_bytes_sent = -1;
   }
   
   return total_bytes_sent;
}

int ACE_PIP_Protocol_Message::deserialize(ACE_SOCK_Stream& stream)
{
   int total_bytes_received(-1);
      
   ACE_Message_Block* header_block(0);

   ACE_Message_Block* lengths_block(0);
   ACE_Message_Block* curr_payload_block(0);
   ACE_Message_Block* payload_blocks(0);

   ACE_NEW_RETURN(header_block, ACE_Message_Block(FIXED_HEADER_LENGTH_), -1);
   // Read the fixed-length portion of the protocol header.
   int bytes_received = stream.recv_n(header_block->wr_ptr(), FIXED_HEADER_LENGTH_);
   if (bytes_received == FIXED_HEADER_LENGTH_)
   {
      total_bytes_received = bytes_received;
      
      // Determine number of data message blocks in the payload.
      header_block->rd_ptr(FIXED_HEADER_LENGTH_ - sizeof(this->num_payload_blocks_));
      ACE_OS::memcpy(&this->num_payload_blocks_, header_block->rd_ptr(), 
                     sizeof(this->num_payload_blocks_));
      
      header_block->reset();
      header_block->wr_ptr(bytes_received);

      // Extract the length of each payload block.
      if (this->num_payload_blocks_ > 0)
      {
	 // Read the lengths of each block.
	 int bytes_to_read = this->num_payload_blocks_ * sizeof(ACE_UINT32);
	 ACE_NEW_RETURN(lengths_block, ACE_Message_Block(bytes_to_read), -1);
	 bytes_received = stream.recv_n(lengths_block->wr_ptr(), bytes_to_read);
         
	 if (bytes_received == bytes_to_read)
	 {
	    total_bytes_received += bytes_received;
	    lengths_block->wr_ptr(bytes_received);
	    
	    // The lengths of each block have been successfully written, so 
	    // unpack them.
	    header_block->next(lengths_block);
	    block(header_block);
	    
            ACE_NEW_RETURN(curr_payload_block, ACE_Message_Block(payload_block_lengths_[0]), -1);
	    payload_blocks = curr_payload_block;
	    unsigned int i = 0;
	    for (; i < this->num_payload_blocks_ && bytes_received != -1; ++i)
	    {
	       // Read the block.
	       bytes_received = stream.recv_n(curr_payload_block->wr_ptr(), 
                                              payload_block_lengths_[i]);
	       if (bytes_received > 0)
	       {
		  total_bytes_received += bytes_received;
		  curr_payload_block->wr_ptr(bytes_received);
		  if (i < (this->num_payload_blocks_ - 1))
		  {
		     curr_payload_block->next(
                        new ACE_Message_Block(payload_block_lengths_[i + 1]));

		     curr_payload_block = curr_payload_block->next();
		  }
		  else
		  {
		     curr_payload_block->next(0);
		  }
		  
	       }	
	       else
	       {
		  total_bytes_received = -1;
		  break;
	       }
	    }
	 }
	 else
	 {
	    total_bytes_received = -1;
	 }
      }
   }
   else
   {
      total_bytes_received = -1;
   }
   
   if (total_bytes_received > 0)
   {
      if (this->message_type_ == ACCEL)
      {
         ACE_NEW_RETURN(this->next_, ACE_PIP_Accel_Message, -1);
      }
      else
      {
         ACE_NEW_RETURN(this->next_, ACE_PIP_Data_Message, -1);
      }
      
      // Pass the payload blocks to the next message struct
      // so it can unpack it.
      this->next_->block(payload_blocks);
   }
   else if (block_)
   {
      // Something failed during reading, so cleanup any allocated memory.
      block_->release();
   }
   
   return total_bytes_received;
}

void ACE_PIP_Protocol_Message::next(ACE_PIP_Message* next)
{
   // Determine the number and length of payload blocks.
   payload_block_lengths_.clear();
   this->num_payload_blocks_ = 0;
   next->pack();
   ACE_Message_Block* curr_block = next->block();
   while (curr_block)
   {
      ++this->num_payload_blocks_;
      payload_block_lengths_.push_back(curr_block->length());
      curr_block = curr_block->next();
   }

   this->next_ = next;
   this->dirty_ = true;
}

int ACE_PIP_Protocol_Message::process_message_payload(ACE_Message_Block* payload)
{
   payload_block_lengths_.clear();
   this->num_payload_blocks_ = 0;
   
   // Determine the length and number of payload blocks.
   ACE_Message_Block* curr_block = payload;
   while (curr_block)
   {
      ++this->num_payload_blocks_;
      payload_block_lengths_.push_back(curr_block->length());
      curr_block = curr_block->next();
   }
   
   if (!this->next_)
   {
      if (this->message_type_ == ACCEL)
      {
        ACE_NEW_RETURN(this->next_, ACE_PIP_Accel_Message, -1);
      }
      else
      {
        ACE_NEW_RETURN(this->next_, ACE_PIP_Data_Message, -1);
      }
   }
   
   this->next_->block(payload);
   this->dirty_ = true;

   return 0;
}

int ACE_PIP_Protocol_Message::pack()
{
   if (!block_)
   {
      // Create the message buffer for the protocol header.
      block_ = new ACE_Message_Block(FIXED_HEADER_LENGTH_);
      
      // Create the message buffer for the list of payload block lengths.
      block_->next(new ACE_Message_Block(this->num_payload_blocks_ * sizeof(ACE_UINT32)));
      block_->next()->next(0);
      this->dirty_ = true;
   }
   if (this->dirty_)
   {
      // Set the buffer pointers to the start of the buffer
      // so that we write to the appropriate location.
      block_->reset();
      
      // pack the process Id.
      ACE_OS::memcpy(block_->wr_ptr(), &message_id_, sizeof(message_id_));
      block_->wr_ptr(sizeof (message_id_));
      
      // Pack the message type.
      ACE_OS::memcpy(block_->wr_ptr(), &this->message_type_, sizeof(this->message_type_));
      block_->wr_ptr(sizeof(this->message_type_));
      
      // Number of blocks in payload.
      ACE_OS::memcpy(block_->wr_ptr(), &this->num_payload_blocks_, sizeof(this->num_payload_blocks_));
      block_->wr_ptr(sizeof(this->num_payload_blocks_));
      
      ACE_Message_Block* next_block = block_->next();
      if (next_block)
      {
	 next_block->reset();
	 
	 // Write the block lengths into the message block.
	 for (unsigned int i = 0; i < this->num_payload_blocks_; ++i)
	 {
	    ACE_OS::memcpy(next_block->wr_ptr(), 
                           &payload_block_lengths_[i], 
                           sizeof(ACE_UINT32));

	    next_block->wr_ptr(sizeof(ACE_UINT32));
	 }
      }

      this->dirty_ = false;
   }

   return 0;
}

void ACE_PIP_Protocol_Message::unpack()
{
   if (block_)
   {
      char* write_ptr = block_->wr_ptr();
      //      char* read_ptr  = block_->rd_ptr();
      block_->reset();

      // Extract the process ID.
      ACE_OS::memcpy(&message_id_, block_->rd_ptr(), sizeof(message_id_));
      block_->rd_ptr(sizeof (message_id_));
      
      // Extract the message type.
      ACE_OS::memcpy(&this->message_type_, block_->rd_ptr(), sizeof(this->message_type_));
      block_->rd_ptr(sizeof(this->message_type_));
      
      // Number of blocks in payload.
      ACE_OS::memcpy(&this->num_payload_blocks_, block_->rd_ptr(), 
                     sizeof(this->num_payload_blocks_));

      block_->rd_ptr(sizeof(this->num_payload_blocks_));
      
      // Reset buffer pointers to be where they were prior to unpacking.
      block_->reset();
      block_->wr_ptr(write_ptr);
      
      // The next block holds the lengths of each payload block.
      ACE_Message_Block* next_block = block_->next();
      if (next_block)
      {
	 write_ptr = next_block->wr_ptr();
	 next_block->reset();
	 payload_block_lengths_.resize(this->num_payload_blocks_, 0);
	 ACE_UINT32 block_length(0);
	 
	 // Extract the lengths of each payload block, which will
	 // be used to recreate the structure of the original payload.
	 for (ACE_UINT32 i = 0; i < this->num_payload_blocks_; ++i)
	 {
	    ACE_OS::memcpy(&block_length, next_block->rd_ptr(), sizeof(block_length));
	    next_block->rd_ptr(sizeof(block_length));
	    payload_block_lengths_[i] = block_length;
	 }
	 
	 // Reset the buffer pointers to where they were prior to unpacking.
         next_block->reset();
	 next_block->wr_ptr(write_ptr);
      }
   }
  
   this->dirty_ = false;
}

ACE_PIP_Protocol_Message* ACE_PIP_Protocol_Message::copy()
{
  ACE_PIP_Protocol_Message* message_copy(0);
  ACE_NEW_RETURN(message_copy, ACE_PIP_Protocol_Message, 0);
   message_copy->message_type_ = this->message_type_;
   message_copy->num_payload_blocks_ = this->num_payload_blocks_;
   for (ACE_UINT32 block_index = 0; block_index < this->num_payload_blocks_; ++block_index)
   {
      message_copy->payload_block_lengths_[block_index] = payload_block_lengths_[block_index];
   }  

   return message_copy;
}
