//
// $Id$
//

#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

class One_Impl
  : public virtual POA_Test::One
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  One_Impl (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual char * get_string (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

class Two_Impl
  : public virtual POA_Test::Two
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Two_Impl (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual char * get_string (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

class Three_Impl
  : public virtual POA_Test::Three
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Three_Impl (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual char * get_string (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* HELLO_H */
