/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    examples/NT_Service
//
// = FILENAME
//    ntsvc.h
//
// = DESCRIPTION
//      This is the definition of the sample NT Service class.  This example
//      only runs on Win32 platforms.
//
// = AUTHOR
//    Gonzalo Diethelm and Steve Huston
//
// ============================================================================

#ifndef NTSVC_H_
#define NTSVC_H_

#include "ace/Event_Handler.h"
#include "ace/NT_Service.h"
#include "ace/Singleton.h"
#include "ace/Synch.h"

class Service : public ACE_NT_Service
{
public:
  Service (void);

  virtual void handle_control (DWORD control_code);
  // We override <handle_control> because it handles stop requests
  // privately.

  virtual int  handle_exception (ACE_HANDLE h);
  // We override <handle_exception> so a 'stop' control code can pop
  // the reactor off of its wait.

  virtual int svc (void);
  // This is a virtual method inherited from ACE_NT_Service.

  virtual int handle_timeout (const ACE_Time_Value& tv,
                              const void *arg = 0);
  // Where the real work is done:

private:
  typedef ACE_NT_Service inherited;

private:
  int stop_;
};

// Define a singleton class as a way to insure that there's only one
// Service instance in the program, and to protect against access from
// multiple threads.  The first reference to it at runtime creates it,
// and the ACE_Object_Manager deletes it at run-down.

typedef ACE_Singleton<Service, ACE_Mutex> SERVICE;

#endif /* #ifndef NTSVC_H_ */
