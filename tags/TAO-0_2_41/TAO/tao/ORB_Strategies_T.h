// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     ORB_Strategies_T.h
//
// = AUTHOR
//     Chris Cleeland
//
// ============================================================================

#ifndef TAO_ORB_STRATEGIES_T_H
#  define TAO_ORB_STRATEGIES_T_H

#include "ace/Strategies_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <class SH>
class TAO_Reactive_Strategy : public ACE_Reactive_Strategy<SH>
{
  // = TITLE
  //   TAO_Reactive_Strategy
  //
  // = DESCRIPTION
  //   The ORB uses this reactive strategy for demultiplexing the endpoints.
public:
  TAO_Reactive_Strategy (void);
  // constructor

  ~TAO_Reactive_Strategy (void);
  // destructor

  virtual int activate_svc_handler (SH *sh, void *arg);
  // activate the service handler using this reactive strategy
};

#if defined (__ACE_INLINE__)
#include "tao/ORB_Strategies_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/ORB_Strategies_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ORB_Strategies_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ORB_STRATEGIES_T_H */
