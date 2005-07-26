// $Id$

/**
 * @file ZIP_Wrapper.cpp
 * @author Stoyan Paunov <spaunov@isis.vanderbilt.edu
 *
 * Purpose: to provide a wrapper around ZZIPlib for easy handling of
 * ZIP archives. This wrapper can me used as an auxiliary class that
 * allows a program to become ZIP-aware.
 */

#include "ace/Containers_T.h"           //for ACE_Double_Linked_List
#include "ace/Message_Block.h"          //for ACE_Message_Block

#include "ace/OS_NS_fcntl.h"      //for open
#include "ace/OS_NS_unistd.h"     //for close
#include "ace/OS_NS_sys_stat.h"   //for filesize and mkdir

#include <string>
#include <memory>                                 //for auto_ptr


/////////////////////////////////////////////////////////////////////////////
//NOTE: some #defines problems with zzip & ACE - put these 2 lines on top!!!!
/////////////////////////////////////////////////////////////////////////////
#include "zziplib.h"                            //for ZZIP
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


//get a list of the files in the archive
size_t ZIP_Wrapper::file_list_info (char* zip_name, ACE_Double_Linked_List<ZIP_File_Info> &list)
{
  size_t num = 0;                                       //number of files in archive
  ZZIP_DIR * dir;                                       //pointer to a zip archive
  ZZIP_DIRENT * dir_entry;              //pointer to a file within the archive

  //open the zip archive
  dir = zzip_opendir(zip_name);
    
  if (!dir)
    return false;

  //read each dir entry and show one line of info per file
  while (dir_entry = zzip_readdir (dir))
    {
      //retrieve the name of the file
      char* name = dir_entry->d_name;

      //remove the subpath part if any          NOTE: Lunux style assumed, need to check
      //while(char* next = strstr(name, "/"))
      //        name = next + 1;

      list.insert_tail (new ZIP_File_Info (name, dir_entry->st_size));
    }

  zzip_closedir(dir);
  return 0;
}

//get file and store it into an ACE_Message_Block
bool ZIP_Wrapper::get_file (char* accessor, ACE_Message_Block &file)
{
  bool return_code = true;

  ZZIP_FILE* zip_file = zzip_open (accessor, O_RDONLY| O_BINARY);

  if (! zip_file)
    return false;
        
  int num_read;
  file.size(BUFSIZ);
  ACE_Message_Block* head = &file;

  // read chunks of 16 bytes into buf and print them to stdout
  while (0 < (num_read = zzip_read(zip_file, head->wr_ptr(), head->size())))
    {
      head->wr_ptr (num_read);
      head->cont (new ACE_Message_Block (BUFSIZ));
      head = head->cont ();
    }

  if (num_read == -1) 
    return_code = false;

  zzip_file_close (zip_file);
        
  return return_code;
}


bool ZIP_Wrapper::get_file (char* archive_path, char* filename, ACE_Message_Block &file)
{
  bool return_code = true;
  ZZIP_DIR * dir;                                       //pointer to a zip archive

  //open the zip archive
  dir = zzip_opendir(archive_path);

  if (!dir)
    return false;

  //get the handle to the file
  ZZIP_FILE* zip_file = zzip_file_open (dir, filename, O_RDONLY | O_BINARY);

  if (!zip_file)
    return false;
        
  int num_read;
  file.size(BUFSIZ);
  ACE_Message_Block* head = &file;

  //read the file into the ACE_Message_Block
  while (0 < (num_read = zzip_read(zip_file, head->wr_ptr(), head->size())))
    {
      head->wr_ptr (num_read);
      head->cont (new ACE_Message_Block (BUFSIZ));
      head = head->cont ();
    }

  if (num_read == -1) 
    return_code = false;

  zzip_file_close (zip_file);
  zzip_closedir(dir);
        
  return return_code;
}


//uncompress
//the uncompress format will be
//mkdir(name of zip archive)
//store all files in that directory.
//the path is assumed to be an existing directory
bool ZIP_Wrapper::uncompress (char* zip_archive, char* path)
{
  ZZIP_DIR * dir;                                       //pointer to a zip archive
  ZZIP_DIRENT * dir_entry;              //pointer to a file within the archive
  ZZIP_FILE* file;                              //pointer to a zip file within an archive

  //open the zip archive
  dir = zzip_opendir(zip_archive);
    
  if (!dir)
    return false;

  //??????
  //check if a directory with the name if the archive exists
  //If not, create it. Else, existing files will be truncated upon open.
  //??????

  //get the name of the archive
  std::string arch_dir (path);
  arch_dir += "/";
  arch_dir += zip_archive;
  arch_dir[arch_dir.length () - 4] = '\0';              //NOTE: Assumes .zip extension

  //create directory
  ACE_OS::mkdir(arch_dir.c_str());                              //if dir exists -1 is returned and ignored

  //read each dir entry and show one line of info per file
  while (dir_entry = zzip_readdir (dir))
    {
      //retrieve the name of the file
      char* name = dir_entry->d_name;

      //remove the subpath part if any          NOTE: Lunux style assumed, need to check
      while(char* next = strstr(name, "/"))
        name = next + 1;

      //open a zip handle
      file = zzip_file_open(dir, dir_entry->d_name, O_RDONLY | O_BINARY);
      if (!file)
        return false;

      //allocate buffer

      //TODO: change to ACE_NEW_RETURN
      std::auto_ptr<char> buffer;
      buffer.reset ( new char [dir_entry->st_size + 1]);
                
      //read in the data
      zzip_read(file, &(*buffer), dir_entry->st_size);

      //close the zip handle
      zzip_file_close (file);

      //create file name + path to open
      std::string file_path (arch_dir.c_str ());                //NOTE: need the c-style char to stop at '\0'
      file_path += "/";
      file_path += name;

      //print out the file to be uncompressed
      ACE_OS::write(ACE_STDOUT, file_path.c_str (), file_path.length () );
      ACE_OS::write(ACE_STDOUT, "\n", 1);

      // Open a file handle to the local filesystem
      ACE_HANDLE handle = ACE_OS::open (file_path.c_str (), O_CREAT | O_TRUNC | O_WRONLY);
      if (handle == ACE_INVALID_HANDLE)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("[uncompress] file creation error")),
                          0);

      //write the uncompressed data to the file
      if (ACE_OS::write (handle, &(*buffer), dir_entry->st_size) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("[uncompress] file write error")),
                          0);

      // Close the file handle
      ACE_OS::close (handle);

      //free buffer
      //TODO: check if auto_ptr has a fxn to release the memory before scope is exited
      //delete [] buffer;

    }

  zzip_closedir(dir);
  return true;
}
