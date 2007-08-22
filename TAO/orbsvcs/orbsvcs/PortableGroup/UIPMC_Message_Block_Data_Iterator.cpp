// $Id$

#include "orbsvcs/PortableGroup/UIPMC_Message_Block_Data_Iterator.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

UIPMC_Message_Block_Data_Iterator::UIPMC_Message_Block_Data_Iterator (iovec *iov, int iovcnt) :
  iov_ (iov),
  iovcnt_ (iovcnt),
  iov_ptr_ (0),
  iov_index_ (0),
  iov_len_left_ (0),
  state_ (INTER_BLOCK)
{
}

size_t
UIPMC_Message_Block_Data_Iterator::next_block (size_t max_length,
                                               iovec &block)
{
  if (this->state_ == INTER_BLOCK)
    {
      // Check that there are some iovec buffers left.
      if (this->iov_index_ >= this->iovcnt_)
        return 0;


      size_t current_iov_len =
                    this->iov_[this->iov_index_].iov_len;

      if (current_iov_len <= max_length)
        {
          // Return the full data portion.
          block.iov_len = static_cast<u_long> (current_iov_len);
          block.iov_base = this->iov_[this->iov_index_].iov_base;

          // Go to the next block.
          this->iov_index_++;

          return current_iov_len;
        }
      else
        {
          // Let the caller use the first part of this
          // message block.
          block.iov_len = static_cast<u_long> (max_length);
          block.iov_base = this->iov_[this->iov_index_].iov_base;

          // Break up the block.
          this->iov_len_left_ = current_iov_len - max_length;
          this->iov_ptr_ =
            reinterpret_cast<char *> (reinterpret_cast<char *> (block.iov_base)
                                  + max_length);
          this->state_ = INTRA_BLOCK;

          return max_length;
        }
    }
  else
    {
      // Currently scanning a split block.
      if (this->iov_len_left_ <= max_length)
        {
          // Return everything that's left in the block.
          block.iov_len = static_cast<u_long> (this->iov_len_left_);
          block.iov_base = this->iov_ptr_;

          // Go to the next block.
          this->iov_index_++;

          // Update the state.
          this->state_ = INTER_BLOCK;

          return this->iov_len_left_;
        }
      else
        {
          // Split a little more off the block.
          block.iov_len = static_cast<u_long> (this->iov_len_left_);
          block.iov_base = this->iov_ptr_;

          this->iov_len_left_ -= max_length;
          this->iov_ptr_ += max_length;
          return max_length;
        }
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
