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

class TAO_Export TAO_Policy_Factory
{
  // = TITLE
  //   TAO_Policy_Factory
  //
  // = DESCRIPTION
  //   This class creates a Policy  based on the
  //   type of the type provided.
  // @@ Angelo, you could also mention why this class is needed, e.g.,
  // why can't we just use ORB::create_policy for this purpose or why
  // can't we creatte whatever policy we need directly...

public:

  static CORBA::Policy * create_policy (CORBA::PolicyType ptype);
  // Creates a Policy of the type specified by <ptype>.  NULL is
  // returned if the policy type is unknown.

protected:
  TAO_Policy_Factory (void);
};


#if defined (__ACE_INLINE__)
# include "tao/Policy_Factory.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif // defined (RTPOLICY_FACTORY_H_)

// @@ Angelo, I already left you a similar comment last time in another file,
// to use /* RTPOLICY_FACTORY_H_ */ instead of what you have above
