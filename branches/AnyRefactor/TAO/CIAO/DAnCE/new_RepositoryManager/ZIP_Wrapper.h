// $Id$

/**
 * @file ZIP_Wrapper.h
 * @author Stoyan Paunov <spaunov@isis.vanderbilt.edu>
 * 
 * Purpose: to provide a wrapper around ZZIPlib for easy handling of
 * ZIP archives. This wrapper can me used as an auxiliary  class that
 * allows a program to become ZIP-aware
 */

#ifndef _ZIP_WRAPPER_H_
#define _ZIP_WRAPPER_H_

#include "ace/Containers_T.h"           //for ACE_Double_Linked_List
#include "ace/Message_Block.h"          //for ACE_Message_Block

#include "ace/OS_NS_fcntl.h"      //for open
#include "ace/OS_NS_sys_stat.h"   //for filesize and mkdir

#include <string>
                        
/**
 * @class ZIP_File_Info
 * This class is used as a carrier of information about entities
 * residing inside a ZIP archive
 */
class ZIP_File_Info
{
public:
  std::string name_;
  size_t size_;
  ZIP_File_Info* next_;
  ZIP_File_Info* prev_;

  ZIP_File_Info (char* name, size_t size);
  ZIP_File_Info ();
};

                        
/**
 * @class ZIP_Wrapper
 * 
 * This class is the actual workhorse that provides all of the
 * necessary functionality
 */
class ZIP_Wrapper
{

public:

  /// get a list of the files in the archive
  size_t file_list_info (char* zip_name, ACE_Double_Linked_List<ZIP_File_Info> &list);

  /// get file and store it into an ACE_Message_Block
  /// need to provide the correct accessor string. It formed by the ZIP_Options
  /// singleton on argument parsing and stored in ZIP_Options::instance()->read_file_
  /// ACE_Message_Block is null-terminated, but this is not reflected in the size!
  bool get_file (char* accessor, ACE_Message_Block &file);

  /// additional get_file function to avert subdirectory traversal problems with 
  /// zziplib accessors
  bool get_file (char* archive_path, char* filename, ACE_Message_Block &file);

  /// uncompress
  /// the uncompress format will be
  /// mkdir(name of zip archive)
  /// store all files in that directory.
  /// the path is assumed to be an existing directory
  bool uncompress (char* zip_archive, char* path = "");
};

#endif
