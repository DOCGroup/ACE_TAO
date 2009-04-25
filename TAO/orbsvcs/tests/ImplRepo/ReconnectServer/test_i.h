#ifndef IMR_SWITCHSERVER_H
#define IMR_SWITCHSERVER_H

#include "testS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class  Test_Dummy_i
  : public virtual POA_Test::Dummy
{
public:
  // Constructor 
  Test_Dummy_i (void);
  
  // Destructor 
  virtual ~Test_Dummy_i (void);
  
  virtual  char * getMessage (void)
    throw (
      ::CORBA::SystemException
    );
};

class  Test_Time_i
  : public virtual POA_Test::Time
{
public:
  // Constructor 
  Test_Time_i (void);
  
  // Destructor 
  virtual ~Test_Time_i (void);
  
  virtual
  ::CORBA::Long current_time (
      void
    )
    throw (
      ::CORBA::SystemException
    );
  
  virtual
  void shutdown (
      void
    )
    throw (
      ::CORBA::SystemException
    );
};


#endif /* IMR_SWITCHSERVER_H  */

