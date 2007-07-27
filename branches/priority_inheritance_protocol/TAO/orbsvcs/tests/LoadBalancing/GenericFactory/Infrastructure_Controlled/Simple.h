// -*- C++ -*-
//
// $Id$

#ifndef SIMPLE_H
#define SIMPLE_H

#include "TestS.h"

#if defined (_MSC_VER)
# pragma warning(push)
#endif /* _MSC_VER */

/// Implement the Test::Simple interface
class Simple
  : public virtual POA_Test::Simple
{
public:

  Simple (void);
  /// Constructor

  virtual char * get_string (void);

};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* SIMPLE_H */
