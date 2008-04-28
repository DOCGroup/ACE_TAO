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
    public virtual ::CORBA::LocalObject
{
public:

  /// Constructor.
  Policy (CORBA::ULong val);

  /// Test method defined in test IDL.
  virtual CORBA::ULong value (void);

  virtual CORBA::PolicyType policy_type (void);

  virtual CORBA::Policy_ptr copy (void);

  virtual void destroy (void);

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
