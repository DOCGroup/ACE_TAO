// -*- C++ -*-

//=============================================================================
/**
 *  @file   Init_ACE.h
 *
 *  $Id$
 *
 *  Initialize ACE library services.  Can be called only once per
 *  program invocation.
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
 * @brief
 *
 */
namespace ACE
{
  /**
   * This class implements the functions for the initialization and
   * shutting down ACE.  These functions are called only once per ACE
   * invokation.
   * @return Returns 0 on success, -1 on failure, and 1 if it had already been
   * called.
   */
  extern ACE_Export int init (void);

  /**
   * Shut down ACE library services.  Can be called only once per
   * program invocation.
   * @return Returns 0 on success, -1 on failure, and 1 if it had already been
   * called.
   */
  extern ACE_Export int fini (void);

  // private:
  //  Used internally, so not exported.

  /**
   * Counter to match <init()>/<fini()> calls.  <init()> must increment it;
   * <fini()> must decrement it.  <fini()> then does nothing until it
   * reaches 0.
   */
  extern unsigned int init_fini_count_;
}

#include /**/ "ace/post.h"

#endif  /* ACE_INIT_ACE_H */
