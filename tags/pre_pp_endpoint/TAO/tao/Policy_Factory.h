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
  //   This class creates a Policy  based on the type of the type provided.
  //   This class is needed to support the policies serialization.

public:

  // @@ Angelo: I don't think this is a good design, in the future we
  // may want to have the ORB create the policies, and maybe pluggin
  // new policies on the fly (such as protocol specific policies,
  // security policies, etc. etc.).
  //    IMnsHO the right way to do this is to have it implemented in
  //    the ORB, right now I guess we will have to hard code it, but
  //    eventually we want to dynamically load the policies, much like
  //    we can dynamically load protocols.  The ORB would use the
  //    <ptype> to find the right protocol factory from a dynamically
  //    constructed list.
  //
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

#endif // RTPOLICY_FACTORY_H_
