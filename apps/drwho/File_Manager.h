/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    File_Manager.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _FILE_MANAGER_H
#define _FILE_MANAGER_H

#include "global.h"
#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Mem_Map.h"

/**
 * @class File_Manager
 *
 * @brief This class provides a file ADT for our friends info.
 */
class File_Manager
{
public:
  /// Constructor.
  File_Manager (void);

  int open_file (const char *filename);
  void get_login_and_real_name (const char *&login_name,
                                const char *&real_name);
private:
  int number_of_friends;
  int max_key_length;

  char *buffer_ptr;
  char *current_ptr;
  int buffer_size;

  int open_friends_file (const char *filename);
  int open_passwd_file (void);

  ACE_Mem_Map mmap_;
};

// Make a Singleton.
typedef ACE_Singleton <File_Manager, ACE_Null_Mutex> FILE_MANAGER;

#endif /* _FILE_MANAGER_H */
