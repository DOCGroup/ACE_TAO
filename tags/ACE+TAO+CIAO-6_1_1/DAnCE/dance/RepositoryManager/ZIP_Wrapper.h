
/* -*- C++ -*- */

//=======================================================================
/**
 *  @file     ZIP_Wrapper.h
 *
 *  $Id$
 *
 *  Purpose:  to provide a wrapper around minizip for easy handling of
 *            ZIP archives. This wrapper can be used as an auxiliary
 *            class that allows a program to become ZIP-aware
 *
 *  @author   Stoyan Paunov, Vipul Singh
 *
 *
 */
//=======================================================================

#ifndef _ZIP_WRAPPER_H_
#define _ZIP_WRAPPER_H_

#include "ace/Containers_T.h"     //for ACE_Double_Linked_List
#include "ace/Message_Block.h"    //for ACE_Message_Block
#include "ace/SString.h"          //for ACE_CString


#include "ace/OS_NS_fcntl.h"      //for open
#include "ace/OS_NS_sys_stat.h"   //for filesize and mkdir

#include "minizip/unzip.h"


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

  /// Get file and store it into an ACE_Message_Block. The function
  /// averts subdirectory traversal problems.
  /// NOTE: Be sure to release the message block even if the function returns
  /// false becuase the return value might be due to unsuccessful allocation

  ///archive_path is the zip archive with the path
  ///filename is the name of the file to be looked for in the zip archive.
  ///the file is stored in ACE message block.
  static bool get_file (char* archive_path, char* filename,
                        ACE_Message_Block &file);

  /// uncompress the zip file
  /// The zip file will be uncompressed into a directory with the
  ///name of zip archive.
  /// The path is assumed to be an existing directory

  ///zip_archive is the arcive to be uncompressed with full path.
  ///path is used for creating a directory with the name of zip archive.
  static bool uncompress (char* zip_archive, char* path = 0,
                          bool verbose = true);

  /// Get a list of the files in the archive

  ///zip_name is the name of zipfile with fullpath.
  ///list stores information about each entry in zip file.
  static int file_list_info (char* zip_name,
                             ACE_Double_Linked_List<ZIP_File_Info> &list);

  ///Check if an entry of a zip file is a file or directory
  ///We assume a directoryname terminates with a forward slash
  ///Returns 1 for directory while 0 for file.

  ///filename_inzip is an entry in a zipfile
  static int checkdir (char* filename_inzip);

  ///Create directory structure if entry in zipfile is a directory

  ///filename_inzip is an entry in a zipfile
  ///arch_dir stores the name of the directory to be created
  static int makethedir (char* filename_inzip, ACE_CString arch_dir);

  ///If entry in zipfile is a file, then read the file and write
  /// the uncompressed data at the proper filepath.

 ///filename_inzip is an entry in a zipfile
 ///uf refers to the zip archive
 ///file_info is used to get information about current file
 ///verbose decides if the details are to be printed or not
 ///arch_dir is the name of file with full path where it is to be
 ///uncompressed
  static int handlethefile (char* filename_inzip, unzFile uf,
                            unz_file_info file_info,
                            bool verbose, ACE_CString arch_dir);
};

#endif
