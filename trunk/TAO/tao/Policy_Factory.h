// $Id$
// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   RTPolicy_Factory.h
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
#endif // ACE_LACKS_PRAGMA_ONCE 


class TAO_Export Policy_Factory {

  // = TITLE 
  //   RTPolicy_Factory 
  //
  // = DESCRIPTION
  //   This class allows the creation of RT-Policies based on the
  //   type of the policy. 
  

public:
  
  static CORBA::Policy * create_policy (CORBA::PolicyType ptype);
  // Creates a RTPolicy of the type specified by the parameter CORBA::PolicyType ptype
  // NULL is returned if the policy type is unknown.

protected:
  Policy_Factory(void);
  
};


#if defined (__ACE_INLINE__)
# include "tao/Policy_Factory.i"
#endif // __ACE_INLINE__ 


#endif // defined (RTPOLICY_FACTORY_H_)
