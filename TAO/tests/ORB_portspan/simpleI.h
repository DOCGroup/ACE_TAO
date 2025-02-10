#ifndef SIMPLEI_H_
#define SIMPLEI_H_

#include "simpleS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//Class simple_i
class  simple_i : public virtual POA_simple
{
public:
  //Constructor
  simple_i ();

  //Destructor
  ~simple_i () override = default;

  void test () override;
};

#endif /* SIMPLEI_H_  */
