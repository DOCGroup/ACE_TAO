/* -*- C++ -*- */
// $Id$

/* This module presents a file ADT for our friends info. */

#ifndef _FILE_MANAGER_H
#define _FILE_MANAGER_H

#include "ace/OS.h"

class File_Manager
{
private:
  static int	number_of_friends;
  static int	max_key_length;

  static char	*buffer_ptr;
  static char	*current_ptr;
  static int	buffer_size;

  static int	open_friends_file (char *filename);
  static int	open_passwd_file (void);

public:
  static int	open_file (char *filename);
  static int	get_login_and_real_name (char *&login_name, char *&real_name);
};

#ifdef __OPTIMIZE__
#endif /* __OPTIMIZE__ */
#endif /* _FILE_MANAGER_H */
