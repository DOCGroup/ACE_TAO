// $Id$

// ============================================================================
//
// = DESCRIPTION
//   Helper class used in the reassembly layer of the realiable
//   multicast library.
//
// = AUTHOR
//    Carlos O'Ryan <coryan@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_HEADER_SIZE_H
#define ACE_RMCAST_HEADER_SIZE_H
#include "ace/pre.h"

#include "RMCast_Export.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_Export ACE_RMCast_Header_Size : public ACE_Message_Block
{
  // = TITLE
  //   A control message to compute the total header size in the stack
  //
  // = DESCRIPTION
  //   As layers are pushed into the stack it is often required to add
  //   headers in one or more layers.  The headers could be appended
  //   using the message block chain.
  //   
public:
  ACE_RMCast_Header_Size (void);
  // Constructor

  void add_to_header_size (size_t size);
  // Add a header to the total size

  size_t header_size (void) const;
  // Return the current header size

private:
  size_t header_size_;
  // The header size
};

#if defined (__ACE_INLINE__)
#include "RMCast_Header_Size.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_HEADER_SIZE_H */
