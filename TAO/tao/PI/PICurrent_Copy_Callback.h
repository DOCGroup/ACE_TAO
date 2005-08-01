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

#include "pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbconf.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "ace/Functor.h"

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
    : public ACE_Command_Base
  {
  public:

    /// Constructor.
    PICurrent_Copy_Callback (void);

    /// Destructor.
    ~PICurrent_Copy_Callback (void);

    /// Perform the copy.
    virtual int execute (void * arg = 0);

    /// Set the source and destination PICurrent_Impl objects.
    void src_and_dst (PICurrent_Impl * src, PICurrent_Impl * dst);

  private:

    /// Source PICurrent from which copies will be made.
    PICurrent_Impl * src_;

    /// Destination PICurrent to which copies will be made.
    PICurrent_Impl * dst_;

  };
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_PI_CURRENT_CALLBACK_H */
