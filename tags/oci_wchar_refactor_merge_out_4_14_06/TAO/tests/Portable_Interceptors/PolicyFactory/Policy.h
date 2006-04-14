// -*- C++ -*-

//=============================================================================
/**
 * @file Policy.h
 *
 * $Id$
 *
 * Implementation header for the test Policy.
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef POLICY_H
#define POLICY_H

#include "testC.h"
#include "tao/LocalObject.h"

/**
 * @class Policy
 *
 * @brief Implementation of the Test::Policy interface.
 *
 * This policy merely returns the value that was passed to it when it
 * was constructed.
 */
class Policy
  : public virtual Test::Policy,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  Policy (CORBA::ULong val);

  /// Test method defined in test IDL.
  virtual CORBA::ULong value (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PolicyType policy_type (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  /// Destructor.
  /**
   * Protected destructor to enforce proper memory management through
   * the reference counting mechanism.
   */
  ~Policy (void);

private:

  /// Pseudo-reference to the ORB.
  const CORBA::ULong value_;

};

#endif  /* POLICY_H */
