// -*- C++ -*-

//=============================================================================
/**
 * @file ORB_DLL.h
 *
 * @author Iliyan Jeliazkov <iliyan@ociweb.com>
 */
//=============================================================================

#ifndef SERVICE_CONFIG_ORB_DLL_H
#define SERVICE_CONFIG_ORB_DLL_H

#include /**/ "ace/pre.h"

#include "ORB_DLL_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Auto_Ptr.h"
#include "ace/Task.h"
#include "ace/ARGV.h"
#include "ace/String_Base.h"
#include "tao/ORB.h"

///
class Abstract_Worker
{
public:
  Abstract_Worker (const ACE_TCHAR *ior);
  virtual ~Abstract_Worker (void);
  virtual int test_main (int argc,
                    ACE_TCHAR *argv[]) = 0;
  virtual const ACE_TCHAR * kind (void) const = 0;
protected:
  ACE_TString ior_file_;
};

/**
 * A server kind of test
 */
class Server_Worker : public Abstract_Worker
{
public:
  Server_Worker ();
  ~Server_Worker (void);
  int test_main (int argc, ACE_TCHAR *argv[]);
  const ACE_TCHAR * kind (void) const;

private:
  int parse_args (int argc, ACE_TCHAR *argv[]);
};

/**
 * A client kind of test
 */
class Client_Worker : public Abstract_Worker
{
public:
  Client_Worker ();
  ~Client_Worker (void);
  int test_main (int argc, ACE_TCHAR *argv[]);
  const ACE_TCHAR * kind (void) const;

private:
  int parse_args (int argc, ACE_TCHAR *argv[]);
};

/**
 * @class Service_Config_ORB_DLL
 *
 * @brief The Service_Config_ORB_DLL that is instantiated when the
 *        client-side test module/library is dynamically loaded.
 */
class Service_Config_ORB_DLL_Export Service_Config_ORB_DLL
  : public ACE_Task_Base
{
public:

  /// Constructor.
  Service_Config_ORB_DLL (void);
  ~Service_Config_ORB_DLL (void);

  /// Initializes object when dynamic linking occurs.
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Run by a daemon thread.
  /**
   * Each thread will invoke the Service Configurator via this
   * method unless the object is the "FINAL" object.
   */
  virtual int svc (void);

private:
  signed char is_server_;
  ACE_Auto_Ptr<Abstract_Worker> worker_;
  ACE_Auto_Ptr<ACE_ARGV> argv_;
};

ACE_FACTORY_DECLARE (Service_Config_ORB_DLL, Service_Config_ORB_DLL)

#include /**/ "ace/post.h"

#endif  /* SERVICE_CONFIG_ORB_DLL_H */
