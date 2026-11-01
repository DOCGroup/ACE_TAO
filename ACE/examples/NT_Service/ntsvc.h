/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ntsvc.h
 *
 *    This is the definition of the sample NT Service class.  This example
 *    only runs on Win32 platforms.
 *
 *  @author Gonzalo Diethelm and Steve Huston
 */
//=============================================================================


#ifndef NTSVC_H_
#define NTSVC_H_

#include "ace/config-lite.h"

#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_SERVICES)

#include "ace/Event_Handler.h"
#include "ace/NT_Service.h"
#include "ace/Singleton.h"
#include "ace/Mutex.h"

class Service : public ACE_NT_Service
{
public:
  Service ();
  Service (Service const &) = delete;
  Service (Service &&) = delete;
  Service &operator= (Service const &) = delete;
  Service &operator= (Service &&) = delete;

  ~Service () override;

  /// We override <handle_control> because it handles stop requests
  /// privately.
  void handle_control (DWORD control_code) override;

  /// We override <handle_exception> so a 'stop' control code can pop
  /// the reactor off of its wait.
  int handle_exception (ACE_HANDLE h) override;

  /// This is a virtual method inherited from ACE_NT_Service.
  int svc () override;

  /// Where the real work is done:
  int handle_timeout (ACE_Time_Value const &tv,
                      void const *arg = nullptr) override;

private:
  using inherited = ACE_NT_Service;

private:
  bool stop_ {};
};

// Define a singleton class as a way to insure that there's only one
// Service instance in the program, and to protect against access from
// multiple threads.  The first reference to it at runtime creates it,
// and the ACE_Object_Manager deletes it at run-down.

using SERVICE = ACE_Singleton<Service, ACE_Mutex>;

#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_SERVICES */

#endif /* #ifndef NTSVC_H_ */
