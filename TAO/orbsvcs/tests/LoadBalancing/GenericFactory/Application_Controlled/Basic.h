// -*- C++ -*-
//
// $Id$

#ifndef BASIC_H
#define BASIC_H

#include "TestS.h"
#include "orbsvcs/orbsvcs/CosLoadBalancingC.h"
#include "orbsvcs/orbsvcs/PortableGroupC.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

class LB_Basic_Test;

/// Implement the Test::Basic interface
class Basic
  : public virtual POA_Test::Basic
{
public:
  /// Constructor
  Basic (CORBA::Object_ptr object_group,
         CosLoadBalancing::LoadManager_ptr lm,
         CORBA::ORB_ptr orb,
         const char *loc);

  virtual char * get_string (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void remove_member (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;

  /// Load Manager Reference used to delete the servant reference from the
  /// object group.
  CosLoadBalancing::LoadManager_var lm_;

  /// location of the servant
  const char *location_ ;

  /// Object Group reference.
  CORBA::Object_var object_group_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* BASIC_H */
