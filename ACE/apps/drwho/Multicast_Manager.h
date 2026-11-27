/**
 *  @file    Multicast_Manager.h
 *
 *  @author Douglas C. Schmidt
 */

#ifndef _MULTICAST_MANAGER_H
#define _MULTICAST_MANAGER_H

#include "global.h"
#include "ace/os_include/netinet/os_in.h"
#include "ace/os_include/os_netdb.h"

class Host_Elem
{
public:
  char const *host_name;
  in_addr host_addr;
  bool checked_off {};
  Host_Elem *next;

  Host_Elem (char const *h_name, Host_Elem *n);
};

/**
 * @class Multicast_Manager
 *
 * @brief This file handles all the operations upon host machines names
 * and addresses.
 */
class Multicast_Manager
{
public:
  Multicast_Manager () = delete;

  static void add_host (char const *host_name);
  static void checkoff_host (in_addr host_addr);
  static int get_next_host_addr (in_addr &host_addr);
  static int outstanding_hosts_remain ();
  static int get_next_non_responding_host (char const *&host_name);
  static int insert_hosts_from_file (char const *filename);
  static void insert_default_hosts ();

private:
  static hostent *get_host_entry (char const *host);

  static int received_host_count;
  static char const *host_names[];
  static Host_Elem *drwho_list;
  static Host_Elem *current_ptr;
};

#endif /* _MULTICAST_MANAGER_H */
