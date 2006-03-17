// -*- C++ -*-

// ===================================================================
/**
 *  @file   PICurrent_Copy_Callback.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
// ===================================================================

#ifndef TAO_PI_CURRENT_CALLBACK_H
#define TAO_PI_CURRENT_CALLBACK_H

#include /**/ "ace/pre.h"

#include "tao/PI/pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if TAO_HAS_INTERCEPTORS == 1


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class PICurrent_Impl;

  /**
   * @class PICurrent_Copy_Callback
   *
   * @brief PICurrent copy callback class.
   *
   * This class performs deep copies from the source PICurrent_Impl
   * object's slot table to the destination PICurrent_Impl object's
   * slot table.
   */
  class TAO_PI_Export PICurrent_Copy_Callback
  {
  public:

    /// Constructor.
    PICurrent_Copy_Callback (void);

    /// Destructor.
    ~PICurrent_Copy_Callback (void);

    /// Perform the copy.
    void execute (void);

    /// Set the source and destination PICurrent_Impl objects.
    void src_and_dst (PICurrent_Impl * src, PICurrent_Impl * dst);

  private:
    /// Disallow copying and assignment.
    PICurrent_Copy_Callback (const PICurrent_Copy_Callback &);
    PICurrent_Copy_Callback &operator= (const PICurrent_Copy_Callback &);

    /// Source PICurrent from which copies will be made.
    PICurrent_Impl * src_;

    /// Destination PICurrent to which copies will be made.
    PICurrent_Impl * dst_;

  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_PI_CURRENT_CALLBACK_H */
