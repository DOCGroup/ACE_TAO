// -*- C++ -*-

//=============================================================================
/**
 *  @file   Init_ACE.h
 *
 *  $Id$
 *
 * This class consolidates the operations on the Handles.
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================


#ifndef ACE_INIT_ACE_H
#define ACE_INIT_ACE_H

#include /**/ "ace/pre.h"

#include "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_Init_ACE
 *
 * @brief Initialize ACE library services.  Can be called only once
 *   per program invocation.
 *
 */
class ACE_Export ACE_Init_ACE
{
public:
  /**
   * This class implements the functions for the initialization and
   * shutting down ACE.  These functions are called only once per ACE
   * invokation.
   * @return Returns 0 on success, -1 on failure, and 1 if it had already been
   * called.
   */
  static int init (void);

  /**
   * Shut down ACE library services.  Can be called only once per
   * program invocation.
   * @return Returns 0 on success, -1 on failure, and 1 if it had already been
   * called.
   */
  static int fini (void);

private:
  /**
   * Counter to match <init()>/<fini()> calls.  <init()> must increment it;
   * <fini()> must decrement it.  <fini()> then does nothing until it
   * reaches 0.
   */
  static unsigned int init_fini_count_;
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Init_ACE.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include /**/ "ace/post.h"

#endif  /* ACE_INIT_ACE_H */
