// -*- C++ -*-

//=============================================================================
/**
 * @file      Host_Table.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_HOST_TABLE_H_
#define _CUTS_HOST_TABLE_H_

#include "cuts/config.h"
#include "cuts/CUTS_export.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/SStringfwd.h"

// Forward decl.
class ACE_RW_Thread_Mutex;

//=============================================================================
/**
 * @class CUTS_Host_Table
 *
 * @brief Mapping of IP-addresses to names of hosts.
 */
//=============================================================================

class CUTS_Export CUTS_Host_Table
{
public:
  /// Default constructor.
  CUTS_Host_Table (void);

  /// Destructor.
  ~CUTS_Host_Table (void);

  /**
   * Bind the IP-address to the specified hostname.
   *
   * @param[in]     ipaddr            IP-address.
   * @param[in]     hostname          Name of the host.
   * @retval        >0                Succeeded
   * @retval        -1                Failed
   */
  int bind (const ACE_CString & ipaddr,
            const ACE_CString & hostname);

  /**
   * Remove an entry by using its hostname.
   *
   * @param[in]     hostname    Target hostname.
   */
  void unbind_by_addr (const ACE_CString & hostname);

  /**
   * Remove an entry by using its IP-address.
   *
   * @param[in]     ipaddr      Target IP-address.
   */
  void unbind_by_name (const ACE_CString & ipaddr);

  /**
   * Find the hostname given an IP-address.
   *
   * @retval      -1      The hostname was not found.
   */
  int find_by_addr (const ACE_CString & ipaddr,
                    ACE_CString & hostname);

  /**
   * Find the IP-address given a hostname.
   *
   * @retval      -1      The hostname was not found.
   */
  int find_by_name (const ACE_CString & hostname,
                    ACE_CString & ipaddr);

private:
  /// Type definition of an IP-address map.
  typedef ACE_Hash_Map_Manager <ACE_CString,
                                void *,
                                ACE_RW_Thread_Mutex> IP_Address_Map;

  /// Type defintion of a hostname map.
  typedef ACE_Hash_Map_Manager <ACE_CString,
                                void *,
                                ACE_RW_Thread_Mutex> Hostname_Map;

  /**
   * Locates the entry for the IP-address.
   *
   * @param[in]       ipaddr      Target IP-address.
   * @param[out]      entry       Pointer to the hostname map entry.
   * @return          0           Successfully found entry.
   * @return          -1          Failed to find entry.
   */
  int find_by_addr_i (const ACE_CString & ipaddr,
                      Hostname_Map::ENTRY * & entry);

  /**
   * Locates the entry for the hostname.
   *
   * @param[in]       hostname    Target hostname.
   * @param[out]      entry       Pointer to the IP-address map entry.
   * @return          0           Successfully found entry.
   * @return          -1          Failed to find entry.
   */
  int find_by_name_i (const ACE_CString & hostname,
                      IP_Address_Map::ENTRY * & entry);

  /// Mapping of IP-address to hostname.
  IP_Address_Map ipaddr_map_;

  /// Mapping of hostname to IP-address.
  Hostname_Map host_map_;
};

#endif  // !defined _CUTS_HOST_TABLE_H_
