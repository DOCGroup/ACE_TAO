#ifndef IMR_RECONNECTSERVER_H
#define IMR_RECONNECTSERVER_H

#include "testS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class  Test_Dummy_i
  : public virtual POA_Test::Dummy
{
public:
  // Constructor
  Test_Dummy_i ();

  // Destructor
  virtual ~Test_Dummy_i ();

  char * getMessage () override;
};

class  Test_Time_i
  : public virtual POA_Test::Time
{
public:
  // Constructor
  Test_Time_i ();

  // Destructor
  virtual ~Test_Time_i ();

  ::CORBA::Long current_time () override;

  void shutdown () override;
};


#endif /* IMR_RECONNECTSERVER_H  */

