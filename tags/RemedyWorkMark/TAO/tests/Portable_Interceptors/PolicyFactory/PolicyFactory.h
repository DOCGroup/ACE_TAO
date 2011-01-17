// -*- C++ -*-

//=============================================================================
/**
 * @file PolicyFactory.h
 *
 * $Id$
 *
 * Implementation header for the test policy factory.
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef POLICY_FACTORY_H
#define POLICY_FACTORY_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class PolicyFactory
 *
 * @brief Simple concrete PolicyFactory.
 *
 * This is a test PolicyFactory that constructs a Test::Policy
 * object.
 */
class PolicyFactory
  : public virtual PortableInterceptor::PolicyFactory,
    public virtual ::CORBA::LocalObject
{
public:

  /**
   * @name Methods Required by the PolicyFactory Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all PolicyFactory sub-classes.
   */
  //@{

  /// Construct a Test::Policy object as a test.
  virtual CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
                                           const CORBA::Any & value);

  //@}

};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* POLICY_FACTORY_H */
