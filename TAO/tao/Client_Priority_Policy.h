/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Client_Priority_Policy.h
 *
 *  $Id$
 *
 *  @author Marina Spivak (marina@cs.wustl.edu)
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *  @author Ossama Othman (othman@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_CLIENT_PRIORITY_POLICY_H
#define TAO_CLIENT_PRIORITY_POLICY_H
#include "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

#include "tao/TAOC.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_Client_Priority_Policy
 */
class TAO_Export TAO_Client_Priority_Policy
  : public TAO::ClientPriorityPolicy,
    public TAO_Local_RefCounted_Object
{
public:
  /// Constructor.
  TAO_Client_Priority_Policy (const TAO::PrioritySpecification &priority_spec);

  /// Copy constructor.
  TAO_Client_Priority_Policy (const TAO_Client_Priority_Policy &rhs);

  /// Helper method for the implementation of CORBA::ORB::create_policy
  static CORBA::Policy_ptr create (
      const CORBA::Any& val,
      CORBA::Environment &ACE_TRY_ENV =
      TAO_default_environment ()
    );

  /// Returns a copy of <this>.
  virtual TAO_Client_Priority_Policy *clone (void) const;

  // = The TAO::Client_Priority_Policy methods
  virtual TAO::PrioritySpecification priority_specification (
      CORBA::Environment &ACE_TRY_ENV =
        CORBA::Environment::default_environment ()
    )
    ACE_THROW_SPEC (());
  virtual CORBA::PolicyType policy_type (
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual CORBA::Policy_ptr copy (
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void destroy (
      CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ()
       )
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// The attribute
  TAO::PrioritySpecification priority_spec_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

#if defined (__ACE_INLINE__)
#include "tao/Client_Priority_Policy.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CLIENT_PRIORITY_POLICY_H */
