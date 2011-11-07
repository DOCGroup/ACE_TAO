//
// $Id$
//

#ifndef SIMPLE_H
#define SIMPLE_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Simple interface
class Simple
  : public virtual POA_Test::Simple
{
public:
  /// Constructor
  Simple (void);

  // = The skeleton methods

  virtual void destroy (void);
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* SIMPLE_H */
