// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Policy_Factory.h
//
// = AUTHOR
//   Angelo Corsaro (corsaro@cs.wustl.edu)
//
// ============================================================================

#ifndef RTPOLICY_FACTORY_H_
#define RTPOLICY_FACTORY_H_

#include "ace/pre.h"
#include "tao/corbafwd.h"

#include "tao/PolicyC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


// @@ Angelo, all TAO implementation classes should start with TAO_.
// Please change.
class TAO_Export Policy_Factory
{
  // = TITLE
  //   Policy_Factory
  //
  // = DESCRIPTION
  //   This class allows the creation of RT-Policies based on the
  //   type of the policy.

  // @@ Angelo, does this class creates only RT policies as the
  // comment above indicates?
public:

  static CORBA::Policy * create_policy (CORBA::PolicyType ptype);
  // Creates a Policy of the type specified by <ptype>.  NULL is
  // returned if the policy type is unknown.

protected:
  Policy_Factory (void);
  // @@ Angelo, The only method in this class is a static.  Do we
  // need the constructor?
};


#if defined (__ACE_INLINE__)
# include "tao/Policy_Factory.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif // defined (RTPOLICY_FACTORY_H_)
