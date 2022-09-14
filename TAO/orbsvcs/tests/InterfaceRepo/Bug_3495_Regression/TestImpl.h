// -*- C++ -*-

#ifndef TESTIMPL_H_
#define TESTIMPL_H_

#include "TestS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class  Test_interfaceOne_i
  : public virtual POA_Test::interfaceOne
{
public:
  // Constructor
  Test_interfaceOne_i ();

  // Destructor
  virtual ~Test_interfaceOne_i ();

  virtual void getstruct (::Test::structOne_out tester);
};


#endif /* TESTIMPL_H_  */

