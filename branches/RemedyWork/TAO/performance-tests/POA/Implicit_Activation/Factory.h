//
// $Id$
//

#ifndef FACTORY_H
#define FACTORY_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Factory interface
class Factory
  : public virtual POA_Test::Factory
{
public:
  /// Constructor
  Factory (CORBA::ORB_ptr orb);

  // = The skeleton methods

  virtual Test::Simple_ptr create_simple_object (void);

  virtual void shutdown (void);

private:
  /// Keep a reference to the ORB
  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* FACTORY_H */
