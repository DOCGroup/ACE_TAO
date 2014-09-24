
//===============================================================================
/**
 *  @file     ZIP_Wrapper.cpp
 *
 *  $Id$
 *
 *  Purpose: implementing the ZIP_Wrapper class
 *
 *  @author   Stoyan Paunov, Vipul Singh
 *
 */
//===============================================================================

#include "ace/Containers_T.h"      //for ACE_Double_Linked_List
#include "ace/Message_Block.h"     //for ACE_Message_Block
#include "ace/OS_NS_fcntl.h"       //for open
#include "ace/OS_NS_unistd.h"      //for close
#include "ace/OS_NS_string.h"      //for strncpy
#include "ace/SString.h"           //for ACE_CString
#include "ace/OS_NS_sys_stat.h"    //for stat
#include "ace/OS_NS_sys_stat.h"    //for filesize and mkdir
#include "ace/OS_Memory.h"         //for ACE_NEW* macros
#include "dance/Logger/Log_Macros.h"

#include <string>
#include "minizip/unzip.h"
#define MAXFILENAME (256)
#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (8192)

#include "ZIP_Wrapper.h"

//ZIP_File_Info constructor
ZIP_File_Info::ZIP_File_Info (char* name, size_t size)
  : name_ (name),
    size_ (size),
    next_ (0),
    prev_ (0)
{
}

//ZIP_File_Info default constructor
ZIP_File_Info::ZIP_File_Info ()
  : name_ (""),
    size_ (0),
    next_ (0),
    prev_ (0)
{
}


/// Gets a list of the files in the archive.
int ZIP_Wrapper::file_list_info (char* zip_name,
                                 ACE_Double_Linked_List<ZIP_File_Info> &list)
{
  unzFile uf=0;
  char filename_try[MAXFILENAME+16] = "";
  if (zip_name!=0)
    {
      ACE_OS::strncpy(filename_try, zip_name, MAXFILENAME-1);
      /* strncpy doesnt append the trailing NULL, if the string is too long. */
      filename_try[ MAXFILENAME ] = '\0';
      /* open the zip file */
      uf = unzOpen(zip_name);
      /* if zipfile could not be opened, try appending .zip to name */
      if (uf==0)
        {
          ACE_OS::strcat(filename_try, ".zip");
          uf = unzOpen(filename_try);
        }
    }
  /* If zipfile could not be opened still, return */
  if (uf==0)
    {
      DANCE_ERROR (DANCE_LOG_ERROR,
                   (LM_DEBUG, ACE_TEXT("There is some problem in opening")
                    ACE_TEXT(" %s or %s.zip using unzOpen\n"), zip_name, zip_name));
      return 1;
    }
  unz_global_info gi;
  /* get information about all the files in zip file*/
  int err = unzGetGlobalInfo(uf, &gi);
  if (err!=UNZ_OK)
    DANCE_ERROR (DANCE_LOG_ERROR,
                 (LM_DEBUG, ACE_TEXT("unzGetGlobalInfo failed while trying")
                  ACE_TEXT(" to get global information about zipfile\n"), err));
  /* gi.number_entry corresponds to the number of directory entries
     in the zip file */
  for (uLong i=0;i<gi.number_entry;i++)
    {
      char filename_inzip[256];
      unz_file_info file_info;
      /* get information about the current file in zip file */
      err = unzGetCurrentFileInfo(uf, &file_info, filename_inzip,
                                  sizeof(filename_inzip), 0, 0, 0, 0);
      if (err!=UNZ_OK)
        {
          DANCE_ERROR (DANCE_LOG_ERROR,
                       (LM_DEBUG, ACE_TEXT("unzGetCurrentFileInfo failed")
                        ACE_TEXT(" while trying to get information")
                        ACE_TEXT(" about current file\n"), err));
          break;
        }
      ZIP_File_Info* next = 0;
      ACE_NEW_RETURN (next, ZIP_File_Info (filename_inzip,
                      sizeof(filename_inzip)), -1);
      /* add information about current file to the list */
      list.insert_tail (next);
      if ((i+1)<gi.number_entry)
        {
          err = unzGoToNextFile(uf);
          if (err!=UNZ_OK)
          {
            DANCE_ERROR (DANCE_LOG_ERROR,
                         (LM_DEBUG,
                          ACE_TEXT(" unzGoToNextFile failed")
                          ACE_TEXT(" while trying to go to next file\n"),
                          err));
            break;
          }
        }
    }
  unzCloseCurrentFile(uf);
  return gi.number_entry;
}


//get file and store it into ACE message block.
bool ZIP_Wrapper::get_file (char* archive_path, char* filename,
                            ACE_Message_Block &file)
{
  bool return_code = true;
  unzFile uf=0;
  uf = unzOpen(archive_path);
  /* locate the desired file in the zip file and set it as current file*/
  int j=unzLocateFile(uf, filename, 0);
  if (j==UNZ_END_OF_LIST_OF_FILE)
    {
      DANCE_ERROR (DANCE_LOG_ERROR,
                   (LM_DEBUG, ACE_TEXT("File not found in zip archive")));
      return false;
    }
  else if (j==UNZ_OK)
    {
      int k=unzOpenCurrentFile(uf);
      if (k!=UNZ_OK)
        {
          DANCE_ERROR (DANCE_LOG_ERROR,
                       (LM_DEBUG, ACE_TEXT("Error in opening the current")
                        ACE_TEXT(" file using unzOpenCurrentFile")));
          return false;
        }
      else
        {
          int num_read = 0;
          ACE_Message_Block* head = &file;

          //read the file into the ACE_Message_Block
          do
            {
              if (head->space () == 0)
                {
                  ACE_Message_Block* next = 0;
                  ACE_NEW_RETURN (next, ACE_Message_Block (BUFSIZ), false);
                  head->cont ();
                  head = head->cont ();
                }
              num_read = unzReadCurrentFile(archive_path, head->wr_ptr(),
                                                  head->space());
              if (num_read > 0)
                head->wr_ptr (num_read);
            } while (num_read > 0);
          if (num_read < 0)
            return_code = false;
          unzCloseCurrentFile(uf);
          unzClose(uf);
          return return_code;
        }
    }
  return return_code;
}


/// uncompress the zip file
/// The zip file will be uncompressed into a directory
/// with the name of zip archive.
/// the path is assumed to be an existing directory

bool ZIP_Wrapper::uncompress (char* zip_archive, char* path, bool verbose)
{
  //open the zip archive
  unzFile uf=0;
  uf = unzOpen(zip_archive);
  if (uf==0)
    {
      DANCE_ERROR (DANCE_LOG_ERROR,
                   (LM_DEBUG,ACE_TEXT("unzOpen failed to open the")
                    ACE_TEXT(" zipfile\n")));
      return false;
    }
  //get the name of the archive
  ACE_CString arch_dir (path);
  arch_dir += "/";
  //get only the name of the archive; remove path info
  char* n = ACE_OS::strstr (zip_archive, "/");
  char* zip_name = 0;
  while (n != 0)
    {
      zip_name = ++n;
      n = ACE_OS::strstr (n, "/");
    }
  arch_dir += zip_name;
  //NOTE: Assumes .zip or cpk extension
  arch_dir = arch_dir.substring (0, arch_dir.length () - 4);
  //create directory with the name of zip archive
  ACE_OS::mkdir(arch_dir.c_str());
  //if dir exists -1 is returned and ignored
  unz_global_info gi;
  int err = unzGetGlobalInfo(uf, &gi);
  if (err!=UNZ_OK)
    {
      DANCE_ERROR (DANCE_LOG_ERROR, (LM_DEBUG, ACE_TEXT("unzGetGlobalInfo failed to get global")
                           ACE_TEXT(" information about zipfile\n"), err));
      return false;
    }
  err =unzGoToFirstFile(uf);
  if (err!=UNZ_OK)
    {
      DANCE_ERROR (DANCE_LOG_ERROR, (LM_DEBUG,ACE_TEXT("error %d with zipfile in"
                 ACE_TEXT(" unzGoToFirstFile\n")), err));
      return false;
    }
  /* read each entry of zip file, create directory structure if it is
     a non existing directory whereas if it is a file, write the file
     at the proper path in the directory structure */
  for (uLong i=0;i<gi.number_entry;i++)
    {
      char filename_inzip[256];
      unz_file_info file_info;
      err = unzGetCurrentFileInfo(uf, &file_info, filename_inzip,
                                  sizeof(filename_inzip), 0, 0, 0, 0);
      if (err!=UNZ_OK)
        {
          DANCE_ERROR (DANCE_LOG_ERROR,
                       (LM_DEBUG, ACE_TEXT("unzGetCurrentFileInfo failed")
                        ACE_TEXT(" while trying to get information")
                        ACE_TEXT(" about currentfile\n"), err));
          break;
        }
      int direc = checkdir(filename_inzip);
      /* If it is a directory, we create directory structure */
      if (direc==1)
        {
          makethedir(filename_inzip, arch_dir);
        }
      /* If it is a file, we read its data and write the uncompressed
         data to the file with proper path.*/
      else if (direc==0)
        {
          handlethefile(filename_inzip, uf, file_info, verbose, arch_dir);
        }
      if ((i+1)<gi.number_entry)
        {
          err = unzGoToNextFile(uf);
          if (err!=UNZ_OK)
            {
              DANCE_ERROR (DANCE_LOG_ERROR,
                           (LM_ERROR,ACE_TEXT("unzGoToNextFile failed")
                            ACE_TEXT(" while trying to go to")
                            ACE_TEXT(" nextfile\n"), err));
              break;
            }
        }
    }
  unzClose(uf);
  return true;
}



///try to find if it is a directory OR file
int ZIP_Wrapper::checkdir (char* filename_inzip)
{
  int direc = 0;
  char* dircheck = ACE_OS::strstr (filename_inzip, "/");
  /* We assume that a directory will have its entry terminating in a /
     We also assume that the directory entries in the zip file use
     forward slash for both unix and windows */
  while (dircheck != 0)
    {
      int i = ACE_OS::strcmp(dircheck, "/");
      if (i == 0)
        {
          direc = 1;
          break;
        }
      ++dircheck;
      dircheck = ACE_OS::strstr (dircheck, "/");
    }
  return direc;
}

/// Create directory structure if entry in zipfile is a directory
int ZIP_Wrapper::makethedir (char* filename_inzip, ACE_CString arch_dir)
{
  // let's try to create the directory structure for the package
  char dir_name [2048];
  char* next = ACE_OS::strstr (filename_inzip, "/");
  while (next != 0)
    {
      ACE_CString location (arch_dir);
      ACE_OS::strncpy (dir_name, filename_inzip, next - filename_inzip
                       + 1);

      dir_name[next - filename_inzip + 1] = '\0';
      location += "/";
      location += dir_name;
      ACE_stat stat;
      if (ACE_OS::stat (location.c_str (), &stat) == -1)
        ACE_OS::mkdir (location.c_str ());
      next++;
      next = ACE_OS::strstr (next, "/");
    }
  return 0;
}

/// If entry in zipfile is a file, then read the file and write the
/// uncompressed data at the proper filepath.
int ZIP_Wrapper::handlethefile (char* filename_inzip, unzFile uf,
                                unz_file_info file_info, bool verbose,
                                ACE_CString arch_dir)
{
  int k = unzOpenCurrentFile(uf);
  if (k!=UNZ_OK)
    {
      DANCE_ERROR (DANCE_LOG_ERROR,
                   (LM_ERROR,ACE_TEXT("unzOpenCurrentFile failed in"
                                      " opening the current file")));
      return false;
    }
  else
    {
      size_t const file_size = file_info.uncompressed_size;
      char* temp = 0;
      ACE_NEW_RETURN (temp, char [file_size], false);
      ACE_Auto_Basic_Array_Ptr<char> buffer (temp);
      //read in the data
      unzReadCurrentFile(uf, &(*buffer), file_size);
      //close the zip handle
      unzCloseCurrentFile(uf);
      //create file name + path to open
      std::string file_path (arch_dir.c_str ());
     //NOTE: need the c-style char to stop at '\0'
      file_path += "/";
      file_path += filename_inzip;
      //print out the file to be uncompressed
      if (verbose)
        {
          ACE_OS::write(ACE_STDOUT, file_path.c_str (),
                              file_path.length () );
          ACE_OS::write(ACE_STDOUT, "\n", 1);
        }
      // Open a file handle to the local filesystem
      ACE_HANDLE handle = ACE_OS::open (file_path.c_str (),
                                        O_CREAT | O_TRUNC | O_WRONLY);
      if (handle == ACE_INVALID_HANDLE)
        {
          unzClose(uf);
          DANCE_ERROR_RETURN (DANCE_LOG_ERROR, (LM_ERROR,
                             ACE_TEXT ("%p\n"),
                             ACE_TEXT ("[uncompress] file creation error")),
                             0);
        }
      //write the uncompressed data to the file
      if (ACE_OS::write (handle, &(*buffer), file_size) == -1)
        {
          unzClose(uf);
          DANCE_ERROR_RETURN (DANCE_LOG_ERROR,
                              (LM_ERROR,
                               ACE_TEXT ("%p\n"),
                               ACE_TEXT ("[uncompress] file write error")),
                              0);
        }
      // Close the file handle
      ACE_OS::close (handle);
    }
  return 0;
}
