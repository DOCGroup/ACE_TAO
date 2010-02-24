// -*- C++ -*-
//
// $Id$

#ifndef SIMPLE_H
#define SIMPLE_H

#include "TestS.h"
#include "Factory.h"

#if defined (_MSC_VER)
# pragma warning(push)
#endif /* _MSC_VER */

/// Implement the Test::Simple interface
class Simple
  : public virtual POA_Test::Simple
{
public:

  Simple (int server_id);
  /// Constructor

  /// Default _non_existent: always returns false.
  virtual CORBA::Boolean _non_existent (void);

  virtual char * get_string (void);

  virtual void exit (void);

private:

  int server_id_;

};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* SIMPLE_H */
