/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   NT_Service.h
 *
 *  $Id$
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 *  @author John Tucker <jtucker@infoglide.com> 
 *  @author Mike Vitalo <mvitalo@infoglide.com>   
 */
//=============================================================================

#ifndef IMR_NT_SERVICE_H
#define IMR_NT_SERVICE_H

#include "ace/config.h"

// Only on windows
#if defined (ACE_WIN32)

#include "ace/NT_Service.h"
#include "ace/Singleton.h"
#include "ace/Synch.h"
#include "tao/orbconf.h"

static const char *IMR_SERVICE_NAME = "TAOImplRepo";
static const char *IMR_DISPLAY_NAME = "TAO Implementation Repository";

// @todo Not used yet, maybe ACE_NT_Service needs to be changed to accept it.
static const char *IMR_DESCRIPTION = "Activation service for TAO";

/**
 * @class ImR_NT_Service
 *
 * @brief Allows the Implementation Repository to act as a Windows NT Service.
 */
class ImR_NT_Service : public ACE_NT_Service
{
public:
  typedef TAO_SYNCH_RECURSIVE_MUTEX MUTEX;

  /// Constructor (not protected since it is used by ACE_NT_SERVICE_DEFINE).
  ImR_NT_Service (void);

  /// Destructor
  virtual ~ImR_NT_Service (void);

  /// We override <handle_control> because it handles stop requests
  /// privately.
  virtual void handle_control (DWORD control_code);

  /// We override <handle_exception> so a 'stop' control code can wake
  /// the reactor off of its wait.
  virtual int handle_exception (ACE_HANDLE h);

  /// This is a virtual method inherited from ACE_NT_Service.
  virtual int svc (void);
        
private:
  friend class ACE_Singleton<ImR_NT_Service, MUTEX>;
};

typedef ACE_Singleton<ImR_NT_Service, ACE_Mutex> SERVICE;

#endif /* ACE_WIN32 */

#endif /* IMR_NT_SERVICE_H */


