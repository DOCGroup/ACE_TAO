// -*- C++ -*-
//
// $Id$

#ifndef BASIC_H
#define BASIC_H

// @@ Jai, you don't need this header for non-library code.
#include "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */


/// Implement the Test::Basic interface
class Basic
  : public virtual POA_Test::Basic
// @@ Jai, do you really want to enable reference counting in this
//    servant?  There is nothing wrong in doing so but the code in
//    server.cpp assumes that the server is not reference counted.
//
//    If you're going to leave reference counting enabled, add a
//    protected destructor to force proper memory management through
//    the reference counting mechanism.
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Basic (CORBA::ORB_ptr orb);

  virtual char * get_string (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

// @@ Jai, you don't need this header for non-library code.
#include "ace/post.h"
#endif /* BASIC_H */
