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

#include <memory>
#include "ace/Task.h"
#include "ace/ARGV.h"
#include "ace/String_Base.h"
#include "tao/ORB.h"

/// @brief Common interface for server and client test workers.
class Abstract_Worker
{
public:
  explicit Abstract_Worker (ACE_TCHAR const *ior);
  virtual ~Abstract_Worker ();
  virtual int test_main (int argc,
                         ACE_TCHAR *argv[]) = 0;
  virtual ACE_TCHAR const *kind () const = 0;

  Abstract_Worker (Abstract_Worker const &) = delete;
  Abstract_Worker &operator= (Abstract_Worker const &) = delete;
  Abstract_Worker (Abstract_Worker &&) = delete;
  Abstract_Worker &operator= (Abstract_Worker &&) = delete;

protected:
  ACE_TString ior_file_;
};

/// @brief Runs the server side of the test.
class Server_Worker : public Abstract_Worker
{
public:
  Server_Worker ();
  ~Server_Worker () override;
  int test_main (int argc, ACE_TCHAR *argv[]) override;
  ACE_TCHAR const *kind () const override;

private:
  int parse_args (int argc, ACE_TCHAR *argv[]);
};

/// @brief Runs the client side of the test.
class Client_Worker : public Abstract_Worker
{
public:
  Client_Worker ();
  ~Client_Worker () override;
  int test_main (int argc, ACE_TCHAR *argv[]) override;
  ACE_TCHAR const *kind () const override;

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
  Service_Config_ORB_DLL ();
  ~Service_Config_ORB_DLL () override;

  Service_Config_ORB_DLL (Service_Config_ORB_DLL const &) = delete;
  Service_Config_ORB_DLL &operator= (Service_Config_ORB_DLL const &) = delete;
  Service_Config_ORB_DLL (Service_Config_ORB_DLL &&) = delete;
  Service_Config_ORB_DLL &operator= (Service_Config_ORB_DLL &&) = delete;

  /// Initializes object when dynamic linking occurs.
  int init (int argc, ACE_TCHAR *argv[]) override;

  /// Run by a daemon thread.
  /**
   * Each thread will invoke the Service Configurator via this
   * method unless the object is the "FINAL" object.
   */
  int svc () override;

private:
  enum class Worker_Type
  {
    unspecified,
    client,
    server
  };

  Worker_Type worker_type_ {};
  std::unique_ptr<Abstract_Worker> worker_ {};
  std::unique_ptr<ACE_ARGV> argv_ {};
};

ACE_FACTORY_DECLARE (Service_Config_ORB_DLL, Service_Config_ORB_DLL)

#include /**/ "ace/post.h"

#endif  /* SERVICE_CONFIG_ORB_DLL_H */
