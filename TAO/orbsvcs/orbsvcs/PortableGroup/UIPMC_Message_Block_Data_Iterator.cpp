// $Id$

#include "orbsvcs/PortableGroup/UIPMC_Message_Block_Data_Iterator.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

UIPMC_Message_Block_Data_Iterator::UIPMC_Message_Block_Data_Iterator (iovec *iov, int iovcnt) :
  iov_ (iov),
  iovcnt_ (iovcnt),
  iov_index_ (0),
  iov_ptr_ (0),
  iov_len_left_ (0),
  state_ (INTER_BLOCK)
{
}

bool
UIPMC_Message_Block_Data_Iterator::next_block (size_t max_length,
                                               iovec &block)
{
  if (this->state_ == INTER_BLOCK)
    {
      // Check that there are some iovec buffers left.
      if (this->iov_index_ >= this->iovcnt_)
        return false;


      if (this->iov_[this->iov_index_].iov_len <= max_length)
        {
          // Return the full data portion.
          block.iov_len  = this->iov_[this->iov_index_].iov_len;
          block.iov_base = this->iov_[this->iov_index_].iov_base;

          // Go to the next block.
          ++this->iov_index_;

          return true;
        }
      else
        {
          // Let the caller use the first part of this
          // message block.
          block.iov_len  = max_length;
          block.iov_base = this->iov_[this->iov_index_].iov_base;

          // Break up the block.
          this->iov_len_left_ =
            this->iov_[this->iov_index_].iov_len - max_length;
          this->iov_ptr_ =
            reinterpret_cast<char *> (block.iov_base) + max_length;
          this->state_ = INTRA_BLOCK;

          return true;
        }
    }
  else
    {
      // Currently scanning a split block.
      if (this->iov_len_left_ <= max_length)
        {
          // Return everything that's left in the block.
          block.iov_len  = this->iov_len_left_;
          block.iov_base = this->iov_ptr_;

          // Go to the next block.
          ++this->iov_index_;

          // Update the state.
          this->state_ = INTER_BLOCK;

          return true;
        }
      else
        {
          // Split a little more off the block.
          block.iov_len  = max_length;
          block.iov_base = this->iov_ptr_;

          this->iov_len_left_ -= max_length;
          this->iov_ptr_ += max_length;

          return true;
        }
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
