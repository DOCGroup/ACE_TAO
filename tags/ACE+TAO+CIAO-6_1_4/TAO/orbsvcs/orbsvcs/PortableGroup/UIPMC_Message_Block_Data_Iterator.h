// -*- C++ -*-

//=============================================================================
/**
 *  @file     UIPMC_Message_Block_Data_Iterator.h
 *
 *  $Id$
 *
 *  @author Frank Hunleth <frank@hunleth.com>
 */
//=============================================================================

#ifndef TAO_UIPMC_MESSAGE_BLOCK_DATA_ITERATOR_H
#define TAO_UIPMC_MESSAGE_BLOCK_DATA_ITERATOR_H
#include /**/ "ace/pre.h"

#include "ace/os_include/sys/os_uio.h"
#include "ace/os_include/os_stddef.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class UIPMC_Message_Block_Data_Iterator
 *
 * @brief This is a help class for iterating through ACE_Message_Blocks and to
 *        segment them into MIOP packets.
 */
class UIPMC_Message_Block_Data_Iterator
{
public:
  /// Constructor
  UIPMC_Message_Block_Data_Iterator (iovec *iov, int iovcnt);

  /// Get the next data block that has a size less than or equal
  /// to max_length. Return true if there is anything left.
  bool next_block (size_t max_length, iovec &block);

private:
  iovec *iov_;
  int iovcnt_;
  int iov_index_;

  // Point internal to a message block, if we have to split one up.
  char *iov_ptr_;

  // Length used in a split message block.
  size_t iov_len_left_;

  // Current message iterator state.
  enum State
  {
    INTER_BLOCK,
    INTRA_BLOCK
  } state_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif  /* TAO_UIPMC_MESSAGE_BLOCK_DATA_ITERATOR_H */
