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
  simple_i () = default;

  //Destructor
  ~simple_i () override = default;

  void test () override;

  simple_i (simple_i const&) = delete;
  simple_i (simple_i&&) = delete;
  simple_i& operator= (simple_i const&) = delete;
  simple_i& operator= (simple_i&&) = delete;
};

#endif /* SIMPLEI_H_  */
