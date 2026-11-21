/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Activator_NT_Service.h
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 *  @author John Tucker <jtucker@infoglide.com>
 *  @author Mike Vitalo <mvitalo@infoglide.com>
 */
//=============================================================================

#ifndef Activator_NT_Service_H
#define Activator_NT_Service_H

#include "ace/config-all.h"

#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_SERVICES)

#include "ace/NT_Service.h"
#include "ace/Singleton.h"
#include "ace/Synch.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

extern const ACE_TCHAR * IMR_ACTIVATOR_SERVICE_NAME;
extern const ACE_TCHAR * IMR_ACTIVATOR_DISPLAY_NAME;
extern const ACE_TCHAR * IMR_ACTIVATOR_DESCRIPTION;

/**
 * @class Activator_NT_Service
 *
 * @brief Allows the Implementation Repository to act as a Windows NT Service.
 */
class Activator_NT_Service : public ACE_NT_Service
{
public:
  using MUTEX = TAO_SYNCH_RECURSIVE_MUTEX;

  /// We override <handle_control> because it handles stop requests
  /// privately.
  void handle_control (DWORD control_code) override;

  /// We override <handle_exception> so a 'stop' control code can wake
  /// the reactor off of its wait.
  int handle_exception (ACE_HANDLE h) override;

  /// This is a virtual method inherited from ACE_NT_Service.
  int svc () override;
private:
  friend class ACE_Singleton<Activator_NT_Service, MUTEX>;
};

using SERVICE = ACE_Singleton<Activator_NT_Service, ACE_Mutex>;

#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_SERVICES */

#endif /* Activator_NT_Service_H */
