// $Id$

#ifndef ACE_RMCAST_FRAGMENT_H
#define ACE_RMCAST_FRAGMENT_H
#include /**/ "ace/pre.h"

#include "RMCast_Module.h"
#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//! Default fragment size
#ifndef ACE_RMCAST_DEFAULT_FRAGMENT_SIZE
# define ACE_RMCAST_DEFAULT_FRAGMENT_SIZE 1024
#endif /* ACE_RMCAST_DEFAULT_FRAGMENT_SIZE */

//! Fragmentation module
/*!
 * Some transports cannot send very big messages, for example UDP
 * imposes a limit of 64K, and in practice the limit is even more
 * strict than that.
 * This class decomposes a message into multiple fragments, using an
 * application defined maximum size.
 */
class ACE_RMCast_Export ACE_RMCast_Fragment : public ACE_RMCast_Module
{
public:
  //! Constructor
  ACE_RMCast_Fragment (void);

  //! Destructor
  virtual ~ACE_RMCast_Fragment (void);

  //! Accessor for the max_fragment size.
  /*! There is no modifier, the maximum fragment size is obtained
   *  using feedback from the lower layers (transport?)
   *  @@TODO We have not implemented the feedback mechanisms yet!
   */
  size_t max_fragment_size (void) const;

  /*!
   * Only data messages need fragmentation, the control messages are
   * all small enough for all the transports that I know about.
   * Well, actually for CAN-Bus (Controller Area Network), they may be
   * too big, because the max payload there is 8 bytes, but we don't
   * play with those in ACE.
   */
  virtual int data (ACE_RMCast::Data &data);

private:
  //! Current fragment size limit
  size_t max_fragment_size_;
};

#if defined (__ACE_INLINE__)
#include "RMCast_Fragment.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_RMCAST_FRAGMENT_H */
