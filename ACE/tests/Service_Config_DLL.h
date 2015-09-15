// -*- C++ -*-

//=============================================================================
/**
 * @file Service_Config_DLL.h
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef SERVICE_CONFIG_DLL_H
#define SERVICE_CONFIG_DLL_H

#include /**/ "ace/pre.h"

#include "Service_Config_DLL_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
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
  /// Configurator in separate threads.
  ACE_TCHAR directive_[2][BUFSIZ];

};

/**
 * @class Refuses_Init
 *
 * @brief The Refuses_Init service simply tests for proper operation
 *        when a service's init() hook returns -1.
 */
class Service_Config_DLL_Export Refuses_Init : public ACE_Service_Object
{
public:
  // Constructor; required for factory function.
  Refuses_Init () {}

  /// Initializes object when dynamic linking occurs.
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Terminates object when dynamic unlinking occurs.
  virtual int fini (void);

  /// Returns information on a service object.
  virtual int info (ACE_TCHAR **info_string, size_t length = 0) const;
};

ACE_FACTORY_DECLARE (Service_Config_DLL, Service_Config_DLL)
ACE_FACTORY_DECLARE (Service_Config_DLL, Refuses_Init)

#include /**/ "ace/post.h"

#endif  /* SERVICE_CONFIG_DLL_H */
