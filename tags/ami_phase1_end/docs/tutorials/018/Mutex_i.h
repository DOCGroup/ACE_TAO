// $Id$

#ifndef MUTEX_I_H
#define MUTEX_I_H

#include "Test_T.h"

/* Create a very simple derivative of our Test template.  All we have
   to do is provide our mutex choice and a name.
 */
class Mutex : public Test_T<ACE_Mutex>
{
public:
  Mutex (void) : Test_T<ACE_Mutex> ("Mutex") {}
};

#endif /* MUTEX_I_H */
