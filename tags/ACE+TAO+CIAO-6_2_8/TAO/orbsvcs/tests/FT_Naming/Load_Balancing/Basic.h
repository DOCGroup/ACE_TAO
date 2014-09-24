// -*- C++ -*-
//
// $Id$

#ifndef BASIC_H
#define BASIC_H

#include "TestS.h"
#include "orbsvcs/FT_NamingManagerC.h"
#include "orbsvcs/PortableGroupC.h"

#if defined (_MSC_VER)
# pragma warning(push)
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
         FT_Naming::NamingManager_ptr lm,
         CORBA::ORB_ptr orb,
         const char *loc);

  virtual char * get_string (void);

  virtual void shutdown (void);

  virtual void remove_member (void);

private:

  const char* object_group_name_;

  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;

  /// Load Manager Reference used to delete the servant reference from the
  /// object group.
  FT_Naming::NamingManager_var nm_;

  /// location of the servant
  CORBA::String_var location_;

  /// Object Group reference.
  CORBA::Object_var object_group_;
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* BASIC_H */
