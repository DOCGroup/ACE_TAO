/* -*- C++ -*- */
// $Id$

//============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Auto_IncDec_T.h
//
// = AUTHOR
//    Edan Ayal <EdanA@cti2.com>
//
//============================================================================

#ifndef ACE_AUTO_INCDEC_T_H
#define ACE_AUTO_INCDEC_T_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <class ACE_SAFELY_INCREMENTABLE_DECREMENTABLE>
class ACE_Auto_IncDec
{
  // = TITLE
  //     This class automatically increments and decrements a
  //     parameterized counter.
  //
  // = DESCRIPTION
  //     This data structure is meant to be used within a method,
  //     function, or scope.  The actual parameter given for the
  //     <ACE_SAFELY_INCREMENTABLE_DECREMENTABLE> template parameter
  //     must provide at least opertaors ++ and --.
public:
  // = Initialization and termination methods.
  ACE_Auto_IncDec (ACE_SAFELY_INCREMENTABLE_DECREMENTABLE &counter);
  // Implicitly increment the counter.

  ~ACE_Auto_IncDec (void);
  // Implicitly decrement the counter.

  void dump (void) const;
  // Dump the state of an object.

protected:
  ACE_SAFELY_INCREMENTABLE_DECREMENTABLE &counter_;
  // Reference to the <ACE_SAFELY_INCREMENTABLE_DECREMENTABLE> counter
  // we're incrementing/decrementing.

private:
  // = Prevent assignment and initialization.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const
                                          ACE_Auto_IncDec<ACE_SAFELY_INCREMENTABLE_DECREMENTABLE> &))
    ACE_UNIMPLEMENTED_FUNC (ACE_Auto_IncDec (const
                                            ACE_Auto_IncDec<ACE_SAFELY_INCREMENTABLE_DECREMENTABLE> &))
};

#if defined (__ACE_INLINE__)
#include "ace/Auto_IncDec_T.i"
// On non-Win32 platforms, this code will be inlined
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Auto_IncDec_T.cpp"
// On Win32 platforms, this code will be included as template source
// code and will not be inlined. Therefore, we first turn off
// ACE_INLINE, set it to be nothing, include the code, and then turn
// ACE_INLINE back to its original setting. All this nonsense is
// necessary, since the generic template code that needs to be
// specialized cannot be inlined, else the compiler will ignore the
// specialization code. Also, the specialization code *must* be
// inlined or the compiler will ignore the specializations.
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Auto_IncDec_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_AUTO_INCDEC_T_H */
