//
// $Id$
//
#ifndef FOO_I_H
#define FOO_I_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Hello interface
class Foo_i
  : public virtual POA_Test::Foo
  , public virtual PortableServer::RefCountServantBase
{
public:
  Foo_i (CORBA::ORB_ptr p)
    : orb_ (CORBA::ORB::_duplicate (p)) {}

private:
  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /*FOO_I_H*/
