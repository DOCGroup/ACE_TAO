// -*- C++ -*-

//=============================================================================
/**
 * @file Service_Config_DLL.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef SERVICE_CONFIG_DLL_H
#define SERVICE_CONFIG_DLL_H

#include "ace/pre.h"

#include "Service_Config_DLL_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"

/**
 * @class Service_Config_DLL
 *
 * @brief The Service_Config_DLL that is instantiated when the
 *        client-side test module/library is dynamically loaded.
 *
 * This class is the implementation used for all service instances
 * (i.e. those declared using the ACE_FACTORY_* macros).
 */
class Service_Config_DLL_Export Service_Config_DLL : public ACE_Task_Base
{
public:

  /// Constructor.
  Service_Config_DLL (void);

  /// Initializes object when dynamic linking occurs.
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Terminates object when dynamic unlinking occurs.
  virtual int fini (void);

  /// Run by a daemon thread.
  /**
   * Each thread will invoke the Service Configurator via this
   * method unless the object is the "FINAL" object.
   */
  virtual int svc (void);

private:

  /// Directives to be passed to be processed by the Service
  /// Configurator in seperate threads.
  ACE_TCHAR directive_[2][BUFSIZ];

};


ACE_FACTORY_DECLARE (Service_Config_DLL, Service_Config_DLL)

#include "ace/post.h"

#endif  /* SERVICE_CONFIG_DLL_H */
