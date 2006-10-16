
/* -*- C++ -*- */

//=======================================================================
/**
 *  @file     ZIP_Wrapper.h
 *
 *  $Id$
 *
 *  Purpose:  to provide a wrapper around ZZIPlib for easy handling of
 *            ZIP archives. This wrapper can me used as an auxiliary
 *            class that allows a program to become ZIP-aware
 *
 *  @author   Stoyan Paunov
 */
//=======================================================================

#ifndef _ZIP_WRAPPER_H_
#define _ZIP_WRAPPER_H_

#include "ace/Containers_T.h"     //for ACE_Double_Linked_List
#include "ace/Message_Block.h"    //for ACE_Message_Block
#include "ace/SString.h"          //for ACE_CString

#include "ace/OS_NS_fcntl.h"      //for open
#include "ace/OS_NS_sys_stat.h"   //for filesize and mkdir

#include <string>

/**
 * @class ZIP_File_Info
 *
 * This class is used as a carrier of information
 * about entities residing inside a ZIP archive
 */
class ZIP_File_Info
{
public:
  ACE_CString name_;
  size_t size_;
  ZIP_File_Info* next_;
  ZIP_File_Info* prev_;

  ZIP_File_Info (char* name, size_t size);
  ZIP_File_Info ();
};

/**
 * @class ZIP_Wrappers
 *
 * This class is the actual workhorse that provides all of
 * the necessary functionality
 */
class ZIP_Wrapper
{
public:
  /// Get a list of the files in the archive
  /// returns -1 on failure
  static int file_list_info (char* zip_name,
                             ACE_Double_Linked_List<ZIP_File_Info> &list);

  /**
   * Get file and store it into an ACE_Message_Block need to provide the
   * correct accessor string. It formed by the ZIP_Options singleton on
   * argument parsing and stored in ZIP_Options::instance()->read_file_
   * ACE_Message_Block is null-terminated, but this is not reflected in the
   * size!
   *
   * @note Be sure to release the message block even if the function returns
   * false because the return value might be due to unsuccessful allocation
   */
  static bool get_file (char* accessor, ACE_Message_Block &file);

  /**
   * Additional get_file function to avert subdirectory traversal problems with
   * zziplib accessors
   *
   * @note Be sure to release the message block even if the function returns
   * false because the return value might be due to unsuccessful allocation
   */
  static bool get_file (char* archive_path, char* filename, ACE_Message_Block &file);

  /// uncompress
  /// the uncompress format will be
  /// mkdir(name of zip archive)
  /// store all files in that directory.
  /// the path is assumed to be an existing directory
  static bool uncompress (char* zip_archive, char* path = "", bool verbose = true);
};

#endif
