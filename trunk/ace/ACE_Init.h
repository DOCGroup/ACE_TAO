// $Id$

#ifndef ACE_ACE_INVOKE_H
#define ACE_ACE_INVOKE_H
#include "ace/pre.h"

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_Init
{

  // DESCRIPTION: This class implements the fucntions for the
  //              initialization and shutting down ACE.
  //              These functions are called only once per ACE
  //              invokation.
 public:
    // Initialize ACE library services.  Can be called only once per
  // program invocation.
  static int init (void);
  // Returns 0 on success, -1 on failure, and 1 if it had already been called.

  // Shut down ACE library services.  Can be called only once per
  // program invocation.
  static int fini (void);
  // Returns 0 on success, -1 on failure, and 1 if it had already been called.

 private:

  static u_int init_fini_count_;
  // Counter to match <init>/<fini> calls.  <init> must increment it;
  // <fini> must decrement it.  <fini> then does nothing until it
  // reaches 0.

};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/ACE_Init.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/post.h"
#endif  /* ACE_ACE_INVOKE_H */
