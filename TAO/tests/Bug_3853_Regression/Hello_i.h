
#ifndef HELLO_I_H
#define HELLO_I_H

#include "HelloS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class  Demo_HelloWorld_i
  : public virtual POA_Demo::HelloWorld
{
public:
  // Constructor
  Demo_HelloWorld_i (CORBA::ORB_ptr orb);
  Demo_HelloWorld_i (Demo_HelloWorld_i const &) = delete;
  Demo_HelloWorld_i (Demo_HelloWorld_i &&) = delete;
  Demo_HelloWorld_i &operator= (Demo_HelloWorld_i const &) = delete;
  Demo_HelloWorld_i &operator= (Demo_HelloWorld_i &&) = delete;

  // Destructor
  ~Demo_HelloWorld_i () override = default;

  void sayHello (char const *msg) override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;
};


#endif /* HELLO_I_H  */

