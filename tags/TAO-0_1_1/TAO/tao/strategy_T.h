// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
// 
// = FILENAME
//     strategy_T.h
//
// = AUTHOR
//     Chris Cleeland
//
// ============================================================================

#if !defined (TAO_STRATEGY_T_H)
#  define TAO_STRATEGY_T_H

#include "ace/Strategies_T.h"

template <class SH>
class TAO_Reactive_Strategy : public ACE_Reactive_Strategy<SH>
{
public:
  TAO_Reactive_Strategy (void);
  ~TAO_Reactive_Strategy (void);

  virtual int activate_svc_handler (SH *sh, void *arg);
};

#if defined (__ACE_INLINE__)
#include "tao/strategy_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/strategy_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("strategy_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_STRATEGY_T_H */
