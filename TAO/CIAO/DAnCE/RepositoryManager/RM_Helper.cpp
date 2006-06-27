// $Id$

#include "RM_Helper.h"
#include "ciao/Packaging_DataC.h"    //for the PackageConfiguration declaration
#include "tao/CDR.h"                 //for TAO CDR classes
#include "ace/Message_Block.h"       //for ACE_Message_Block
#include "ace/Auto_Ptr.h"            //for Auto_Ptr
#include "ace/OS_NS_fcntl.h"         //for open
#include "ace/OS_NS_unistd.h"        //for close
#include "ace/OS_NS_sys_stat.h"      //for filesize and mkdir


void RM_Helper::pc_to_cdr (const Deployment::PackageConfiguration& pc, TAO_OutputCDR& cdr)
{
  cdr << pc;
}


void RM_Helper::cdr_to_pc (Deployment::PackageConfiguration& pc, TAO_InputCDR& cdr)
{
  cdr >> pc;
}


bool RM_Helper::externalize (const Deployment::PackageConfiguration& pc, const char* path)
{
  size_t bufsiz = 0;
  TAO_OutputCDR out (bufsiz);

  RM_Helper::pc_to_cdr (pc, out);

  const ACE_Message_Block* mb = out.begin ();

  return write_pc_to_disk (path, *(const_cast<ACE_Message_Block*> (mb)));
}

bool RM_Helper::reincarnate (Deployment::PackageConfiguration& pc, const char* path)
{
  size_t length = 0;
  ACE_Auto_Ptr<ACE_Message_Block> mb (read_pc_from_disk (path, length));

  if (!mb.get ())
    return false;

  TAO_InputCDR in (mb.get ());

  RM_Helper::cdr_to_pc (pc, in);

  return true;
}


//---------------------------------------------------------------------
//These are a bit obsolete but until I am sure I will keep them

//This function attempts to write a sequence of bytes to
//a specified location. A 0 is returned in the case of an error
//and a 1 upon success

bool RM_Helper::write_to_disk (
                 const char* full_path,
                 const CORBA::Octet* buffer,
                 size_t length
                )
{

  // Open a file handle to the local filesystem
  ACE_HANDLE handle = ACE_OS::open (full_path, O_CREAT | O_TRUNC | O_WRONLY);
  if (handle == ACE_INVALID_HANDLE)
      ACE_ERROR_RETURN ((LM_ERROR,
                          ACE_TEXT ("%p\n"),
                          ACE_TEXT ("[RM::write_to_disk] file creation error")),
                          false);

  // Write the data to the file
  if (ACE_OS::write (handle, buffer, length) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("[RM::write_to_disk] file write error")),
                           false);

  // Close the file handle
  ACE_OS::close (handle);

  return true;
}


/// This function attempts to write a sequence of bytes from an
/// ACE_Message_Block to a specified location. A 0 is returned
/// in the case of an error and a 1 upon success
bool RM_Helper::write_to_disk (
                 const char* full_path,
                 ACE_Message_Block& mb,
                 bool replace
                )
{

  ACE_stat stat;

  if (ACE_OS::stat(full_path, &stat) != -1 && !replace)
    return false;

  // Open a file handle to the local filesystem
  ACE_HANDLE handle = ACE_OS::open (full_path, O_CREAT | O_TRUNC | O_WRONLY);
  if (handle == ACE_INVALID_HANDLE)
      ACE_ERROR_RETURN ((LM_ERROR,
                          ACE_TEXT ("%p\n"),
                          ACE_TEXT ("[RM::write_to_disk] file creation error")),
                          false);

  // Write the data to the file
  for (ACE_Message_Block * curr = &mb; curr != 0; curr = curr->cont ())
    if (ACE_OS::write_n (handle, curr->rd_ptr(), curr->length()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("write error")),
                          false);

  // Close the file handle
  ACE_OS::close (handle);

  return true;
}


/// This function attempts to write a sequence of bytes from an
/// ACE_Message_Block to a specified location. A 0 is returned
/// in the case of an error and a 1 upon success
///
/// @note This function write the contents in a way that preserves the
/// structure of the ACE_Message_Block. It is relevant for
/// PackageConfigurations ONLY
bool RM_Helper::write_pc_to_disk (
                  const char* full_path,
                  ACE_Message_Block& mb,
                  bool replace
                 )
{

  ACE_stat stat;

  if (ACE_OS::stat(full_path, &stat) != -1 && !replace)
    return false;

  // Open a file handle to the local filesystem
  ACE_HANDLE handle = ACE_OS::open (full_path, O_CREAT | O_TRUNC | O_WRONLY);
  if (handle == ACE_INVALID_HANDLE)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("[RM::write_to_disk] file creation error")),
                         false);

  //write the data to the file
  for (ACE_Message_Block * curr = &mb; curr != 0; curr = curr->cont ())
    if (ACE_OS::write_n (handle, curr->rd_ptr(), curr->length()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("write error")),
                          0);

  // Close the file handle
  ACE_OS::close (handle);

  return true;
}


/// This function attempts to read a sequence of bytes from a specified
/// location and returns an octet sequence. A 0 is returned
/// in the case of an error and a 1 upon success
CORBA::Octet* RM_Helper::read_from_disk (
                     const char* full_path,
                      size_t &length
                    )
{
  //open the file
  ACE_HANDLE handle = ACE_OS::open (full_path, O_RDONLY);
  if (handle == ACE_INVALID_HANDLE)
      ACE_ERROR_RETURN ((LM_ERROR,
                          ACE_TEXT ("%p\n"),
                          ACE_TEXT ("[RM::read_from_disk] file open error")),
                          0);

  ACE_stat file_info;

  ACE_OS::fstat (handle, &file_info);

  CORBA::Octet* buffer = 0;
  ACE_NEW_RETURN (buffer, CORBA::Octet[file_info.st_size], 0);

  // Read the contents of the file into the buffer
  if (ACE_OS::read_n (handle, buffer, file_info.st_size) == -1)
     ACE_ERROR_RETURN ((LM_ERROR,
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("[RM::read_from_disk] file read error")),
                        0);

  // Close the file handle
  ACE_OS::close (handle);

  length = file_info.st_size;
  return buffer;
}



/// Function to read the contents of a file from disk into an ACE_Message_Block
/// returns a pointer to an ACE_Message_Block and updates the lenght on success
/// 0 on failure
ACE_Message_Block*
RM_Helper::read_pc_from_disk (
                           const char* full_path,
                           size_t &length
                          )
{
  length = 0;

  //open the file
  ACE_HANDLE handle = ACE_OS::open (full_path, O_RDONLY);
  if (handle == ACE_INVALID_HANDLE)
      ACE_ERROR_RETURN ((LM_ERROR,
                          ACE_TEXT ("%p\n"),
                          ACE_TEXT ("[RM::read_mb_from_disk] file open error")),
                          0);

  ACE_stat file_info;

  ACE_OS::fstat (handle, &file_info);

  ACE_Message_Block* mb = 0;
  ACE_NEW_RETURN (mb, ACE_Message_Block (file_info.st_size + 1), 0);

  // Read the contents of the file into the buffer
  if (ACE_OS::read_n (handle, mb->wr_ptr (), file_info.st_size) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
               ACE_TEXT ("[RM::read_mb_from_disk] file read error")),
                           0);

  mb->length (file_info.st_size);

  // Close the file handle
  ACE_OS::close (handle);

  length = file_info.st_size;
  return mb;
}
