/* -*- C++ -*- */
// $Id$

/* This file handles all the operations upon host machines names 
   and addresses. */ 

#ifndef _HOST_MANAGER_H
#define _HOST_MANAGER_H

#include "ace/OS.h"

struct Host_Elem 
{
  char *host_name;
  in_addr host_addr;
  int checked_off;
  Host_Elem *next;

  Host_Elem (char *h_name, Host_Elem *n);
};

class Multicast_Manager 
{
public:
  static void 	add_host (char *host_name);
  static void 	checkoff_host (in_addr host_addr);
  static int	get_next_host_addr (in_addr &host_addr);
  static int	outstanding_hosts_remain (void);
  static int 	get_next_non_responding_host (char *&host_name);
  static int    insert_hosts_from_file (char *filename);
  static void   insert_default_hosts (void);

private:
  static hostent *get_host_entry (char *host);

  static int		received_host_count;
  static char 		*host_names[];
  static Host_Elem 	*drwho_list;
  static Host_Elem	*current_ptr;
};

#ifdef __OPTIMIZE__
inline 
Multicast_Manager::Host_Elem::Host_Elem (char *h_name, Multicast_Manager::Host_Elem *n)
     : host_name (h_name), checked_off (0), next (n)
{
}

inline int 
Multicast_Manager::outstanding_hosts_remain (void)
{
  return Multicast_Manager::received_host_count > 0;
}
#endif /* __OPTIMIZE__ */
#endif /* _HOST_MANAGER_H */
