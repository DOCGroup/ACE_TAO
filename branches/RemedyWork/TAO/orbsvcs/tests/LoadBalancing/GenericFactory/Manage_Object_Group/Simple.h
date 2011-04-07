// -*- C++ -*-
//
// $Id$

#ifndef SIMPLE_H
#define SIMPLE_H

#include "TestS.h"
#include "orbsvcs/CosLoadBalancingC.h"
#include "orbsvcs/PortableGroupC.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

class LB_Basic_Test;

/// Implement the Test::Simple interface
class Simple
  : public virtual POA_Test::Simple
{
public:
  /// Constructor
  Simple (CORBA::Object_ptr object_group,
         CosLoadBalancing::LoadManager_ptr lm,
         CORBA::ORB_ptr orb,
         CORBA::Short num,
         const char *loc);

  virtual char * get_string (void);

  virtual void shutdown (void);

  virtual void remove_member (void);

  virtual CORBA::Short number (void);

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

  /// ID of the servant
  CORBA::Short number_;
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* SIMPLE_H */
