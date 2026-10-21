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
  ~Test_interfaceOne_i () override;

  void getstruct (::Test::structOne_out tester) override;
};


#endif /* TESTIMPL_H_  */

