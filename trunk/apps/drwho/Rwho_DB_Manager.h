/* -*- C++ -*- */
// $Id$

/* This class returns the user/machine pairs one at a time from the rwho database. */

#ifndef _RWHO_DB_MANAGER_H
#define _RWHO_DB_MANAGER_H

#include "rwhod.h"
#include "Dirent.h"
#include "Protocol_Record.h"

class RWho_DB_Manager 
{
private:
  Dirent	rwho_dir;
  whod 		host_data;
  int		number_of_users;
  int		current_user;
  const int 	WHOD_HEADER_SIZE;
  char	        original_pathname[MAXPATHLEN + 1];
  char     	*rwho_dir_name;

  int		get_next_host (void);

public:
	        RWho_DB_Manager (void);
		~RWho_DB_Manager (void);
   int		get_next_user (Protocol_Record &protocol_record);
};
#endif /* _RWHO_DB_MANAGER_H */
