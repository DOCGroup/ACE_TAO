// $Id$

// ============================================================================
//
// = DESCRIPTION
//   The fragmentation task for the reliable multicast library
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_FRAGMENT_H
#define ACE_RMCAST_FRAGMENT_H
#include "ace/pre.h"

#include "RMCast_Module.h"
#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#ifndef ACE_RMCAST_DEFAULT_FRAGMENT_SIZE
# define ACE_RMCAST_DEFAULT_FRAGMENT_SIZE 1024
#endif /* ACE_RMCAST_DEFAULT_FRAGMENT_SIZE */

class ACE_RMCast_Export ACE_RMCast_Fragment : public ACE_RMCast_Module
{
public:
  ACE_RMCast_Fragment (void);
  // Constructor

  virtual ~ACE_RMCast_Fragment (void);
  // Destructor

  size_t max_fragment_size (void) const;
  // Accessor for the max_fragment size.
  // There is no modifier, the maximum fragment size is obtained using
  // feedback from the lower layer (transport?)

  // = The ACE_RMCast_Module methods
  virtual int data (ACE_RMCast::Data &data);

private:
  size_t max_fragment_size_;
};

#if defined (__ACE_INLINE__)
#include "RMCast_Fragment.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_FRAGMENT_H */
