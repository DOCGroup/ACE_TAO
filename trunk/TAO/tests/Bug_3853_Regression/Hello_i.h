//
// $Id$
//

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

  // Destructor
  virtual ~Demo_HelloWorld_i (void);

  virtual void sayHello (const char * msg);

  virtual void shutdown();

private:
  CORBA::ORB_var orb_;
};


#endif /* HELLO_I_H  */

