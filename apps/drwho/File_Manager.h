/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    drwho
//
// = FILENAME
//    File_Manager.h
//
// = AUTHOR
//    Douglas C. Schmidt
//
// ============================================================================

#if !defined (_FILE_MANAGER_H)
#define _FILE_MANAGER_H

#include "global.h"

class File_Manager
{
  // = TITLE
  //   This class provides a file ADT for our friends info.
public:
  static int open_file (const char *filename);
  static int get_login_and_real_name (char *&login_name,
                                      char *&real_name);

private:
  static int number_of_friends;
  static int max_key_length;

  static char *buffer_ptr;
  static char *current_ptr;
  static int buffer_size;

  static int open_friends_file (const char *filename);
  static int open_passwd_file (void);
};

#endif /* _FILE_MANAGER_H */
