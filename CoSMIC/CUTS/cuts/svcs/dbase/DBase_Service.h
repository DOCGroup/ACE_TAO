// -*- C++ -*-

//=============================================================================
/**
 * @file      DBase_Service.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_DBASE_SERVICE_H_
#define _CUTS_DBASE_SERVICE_H_

#include "cuts/config.h"
#include "cuts/CUTS_export.h"
#include "ace/RW_Thread_Mutex.h"
#include "ace/Auto_Ptr.h"
#include <map>

// Forward decl.
class CUTS_System_Metric;

// Forward decl.
class ODBC_Connection;

//=============================================================================
/**
 * @class CUTS_Database_Service
 *
 * @brief   Defines the mapping from registration id to component id.
 */
//=============================================================================

typedef std::map <long, long> CUTS_DBase_Svc_Component_Map;

//=============================================================================
/**
 * @class CUTS_Database_Service
 *
 */
//=============================================================================

class CUTS_Export CUTS_Database_Service
{
public:
  /// Constructor.
  CUTS_Database_Service (void);

  /// Destructor.
  ~CUTS_Database_Service (void);

  /**
   * Connect to the database.
   *
   * @param[in]     username      Username for login
   * @param[in]     password      Password for login
   * @param[in]     server        Server hosting the database.
   * @param[in]     port          Port to use for connecting.
   * @retval        true          Connection succeeded.
   * @retval        false         Connection failed.
   */
  bool connect (const char * username,
                const char * password,
                const char * server,
                long port);

  /// Disconnect from the database service.
  void disconnect (void);

  /**
   * Create a new test.
   *
   * @retval    true      Successfully create a new test.
   * @retval    false     Failed to create a new test.
   */
  bool create_new_test (void);

  /**
   * Stop the current test.
   *
   * @retval    true      Successfully stoped the current test.
   * @retval    false     Failed to stop the current test.
   */
  bool stop_current_test (void);

  /**
   * Register a new component.
   *
   * @param[in]      regid      Registration ID of the component.
   * @param[in]      uuid       UUID of the component.
   * @retval         true       Successfully registered component.
   * @retval         false      Failed to register a component.
   */
  bool register_component (long regid,  const char * uuid);

  /**
   * Register an IP-address and hostname w/ the database. If
   * either the IP-address or hostname already exist, then
   * nothing happens.
   *
   * @param[in]       ipaddr        IP-address
   * @param[in]       hostname      Name of the host.
   * @param[out]      hostid        Buffer to receive host id.
   * @retval          true          Registration succeeded.
   * @retval          false         Registration failed.
   */
  bool register_host (const char * ipaddr,
                      const char * hostname,
                      int * hostid = 0);

  /**
   * Get the unique id of a host given it's IP-address. The
   * IP-address can be IPv4 or IPv6.
   *
   * @param[in]     ipaddr      IP-adddress
   * @param[in]     hostname    Name of the host.
   * @param[out]    hostid      ID of the host.
   * @retval        true        The operation succeeded.
   * @retval        false       The operation failed.
   */
  bool get_host_id (const char * ipaddr,
                    const char * hostname,
                    long &hostid);

  /**
   * Archive system metrics.
   *
   * @param[in]     metrics     System metrics.
   * @retval        true        Successfully archived metrics.
   * @retval        false       Failed to archive metrics.
   */
  bool archive_system_metrics (CUTS_System_Metric & metrics);

  /**
   * Get the current test number.
   *
   * @return The current test number.
   */
  long current_test (void) const;

  /**
   * Get the id of a path given its name.
   *
   * @param[in]     pathname      Name of the path.
   * @retval        other         Id of the path.
   * @retval        -1            Path was not found.
   */
  long path_id (const char * pathname);

  bool set_component_uptime (long cid, long hostid);

  bool set_component_downtime (long cid);

  bool get_instance_id (const char * instance, long & id);

private:
  /**
   * Stop the current test. This version of the method does
   * not acquire the internal lock.
   *
   * @retval    true      Successfully stoped the current test.
   * @retval    false     Failed to stop the current test.
   */
  bool stop_current_test_i (void);

  /// Disconnect form the database without acquiring lock.
  void disconnect_no_lock (void);

  /// Database connection.
  ACE_Auto_Ptr <ODBC_Connection> conn_;

  /// Locking mechanism.
  ACE_RW_Thread_Mutex lock_;

  /// Component registration mapping.
  CUTS_DBase_Svc_Component_Map component_mapping_;

  /// The current test number.
  long test_number_;
};

#if defined (__CUTS_INLINE__)
#include "DBase_Service.inl"
#endif

#endif  // !defined _CUTS_DBASE_SERVICE_H_
