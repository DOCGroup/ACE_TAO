// -*- C++ -*-

//=============================================================================
/**
 * @file Policy.h
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
  explicit Policy (CORBA::ULong val);

  Policy (Policy const &) = delete;
  Policy (Policy &&) = delete;
  Policy &operator= (Policy const &) = delete;
  Policy &operator= (Policy &&) = delete;

  /// Test method defined in test IDL.
  CORBA::ULong value () override;

  CORBA::PolicyType policy_type () override;

  CORBA::Policy_ptr copy () override;

  void destroy () override;

protected:
  /**
   * Protected destructor to enforce proper memory management through
   * the reference counting mechanism.
   */
  ~Policy () override = default;

private:
  /// Pseudo-reference to the ORB.
  const CORBA::ULong value_;
};

#endif  /* POLICY_H */
