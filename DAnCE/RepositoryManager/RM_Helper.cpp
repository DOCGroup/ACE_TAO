// $Id$

#include "RM_Helper.h"
#include "ace/Auto_Ptr.h"            //for Auto_Ptr
#include "ace/OS_NS_fcntl.h"         //for open
#include "ace/OS_NS_unistd.h"        //for close
#include "ace/OS_NS_sys_stat.h"      //for filesize and mkdir
#include "ace/OS_NS_string.h"
#include "Logger/Log_Macros.h"

void
RM_Helper::pc_to_cdr (const Deployment::PackageConfiguration& pc, TAO_OutputCDR& cdr)
{
  cdr << pc;
}


void
RM_Helper::cdr_to_pc (Deployment::PackageConfiguration& pc, TAO_InputCDR& cdr)
{
  cdr >> pc;
}

bool
RM_Helper::externalize (const Deployment::PackageConfiguration& pc, const char* path)
{
  size_t bufsiz = 0;
  TAO_OutputCDR out (bufsiz);

  RM_Helper::pc_to_cdr (pc, out);

  const ACE_Message_Block* mb = out.begin ();

  return write_pc_to_disk (path, *(const_cast<ACE_Message_Block*> (mb)));
}


bool
RM_Helper::reincarnate (Deployment::PackageConfiguration& pc, const char* path)
{
  size_t length = 0;
  ACE_Auto_Ptr<ACE_Message_Block> mb (read_pc_from_disk (path, length));

  if (!mb.get ())
    return false;

  TAO_InputCDR in (mb.get ());

  RM_Helper::cdr_to_pc (pc, in);

  return true;
}


/// This function attempts to copy the file from a specified location
/// to another specified location on the hard disk.
bool
RM_Helper::copy_from_disk_to_disk (const char* from_path, const char* to_path)
{
  if (ACE_OS::strcmp (from_path, to_path) == 0)
    return true;

  // Open the files
  ACE_HANDLE from_handle = ACE_OS::open (from_path, O_RDONLY);
  if (from_handle == ACE_INVALID_HANDLE)
      DANCE_ERROR_RETURN (1, (LM_ERROR,
                          ACE_TEXT ("%p\n"),
                          ACE_TEXT ("[RM::copy_from_disk_to_disk] file open error")),
                          0);

  ACE_stat file_info;
  ACE_OS::fstat (from_handle, &file_info);
  ACE_UINT64 file_length = file_info.st_size;

  ACE_HANDLE to_handle = ACE_OS::open (to_path, O_CREAT | O_TRUNC | O_WRONLY);
  if (to_handle == ACE_INVALID_HANDLE)
      DANCE_ERROR_RETURN (1, (LM_ERROR,
                          ACE_TEXT ("%p\n"),
                          ACE_TEXT ("[RM::copy_from_disk_to_disk] file creation error")),
                          0);


  // Read the contents of the file into the buffer and write the data to another file
  ACE_Message_Block *mb = 0;
  size_t length;
  size_t number = 0;
  bool last = false;

  while (true)
  {
    if ((file_length - BUFSIZ*number) > BUFSIZ)
      length = BUFSIZ;
    else
    {
      length = static_cast<size_t> (file_length - BUFSIZ*number);
      last = true;
    }

    mb = new ACE_Message_Block (length);

    if (ACE_OS::read_n (from_handle, mb->wr_ptr (), length) == -1)
        DANCE_ERROR_RETURN (1, (LM_ERROR,
                            ACE_TEXT ("%p\n"),
                            ACE_TEXT ("[RM::copy_from_disk_to_disk] file read error")),
                            0);

    ++number;

    mb->length (length);

    for (ACE_Message_Block *curr = mb; curr != 0; curr = curr->cont ())
      if (ACE_OS::write_n (to_handle, curr->rd_ptr (), curr->length ()) == -1)
      {
        mb->release ();
        DANCE_ERROR_RETURN (1, (LM_ERROR,
                            ACE_TEXT ("%p\n"),
                            ACE_TEXT ("[RM::copy_from_disk_to_disk] file write error")),
                            0);
      }

    mb->release ();

    if (last)
      break;
  }

  // Close the files
  ACE_OS::close (from_handle);
  ACE_OS::close (to_handle);

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
      DANCE_ERROR_RETURN (1, (LM_ERROR,
                          ACE_TEXT ("%p\n"),
                          ACE_TEXT ("[RM::write_to_disk] file creation error")),
                          false);

  // Write the data to the file
  for (ACE_Message_Block * curr = &mb; curr != 0; curr = curr->cont ())
    if (ACE_OS::write_n (handle, curr->rd_ptr(), curr->length()) == -1)
        DANCE_ERROR_RETURN (1, (LM_ERROR,
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
  ACE_HANDLE const handle = ACE_OS::open (full_path, O_CREAT | O_TRUNC | O_WRONLY);
  if (handle == ACE_INVALID_HANDLE)
      DANCE_ERROR_RETURN (1, (LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("[RM::write_to_disk] file creation error")),
                         false);

  // write the data to the file
  for (ACE_Message_Block * curr = &mb; curr != 0; curr = curr->cont ())
    if (ACE_OS::write_n (handle, curr->rd_ptr(), curr->length()) == -1)
        DANCE_ERROR_RETURN (1, (LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("write error")),
                          0);

  // Close the file handle
  ACE_OS::close (handle);

  return true;
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

  // Open the file
  ACE_HANDLE const handle = ACE_OS::open (full_path, O_RDONLY);
  if (handle == ACE_INVALID_HANDLE)
      DANCE_ERROR_RETURN (1, (LM_ERROR,
                          ACE_TEXT ("%p\n"),
                          ACE_TEXT ("[RM::read_mb_from_disk] file open error")),
                          0);

  ACE_stat file_info;
  ACE_OS::fstat (handle, &file_info);

  // Get and check the length of the file
  length = static_cast<size_t> (file_info.st_size);

  ACE_INT64 check = length;
  if (check != file_info.st_size)
  {
    length = 0;
    DANCE_ERROR_RETURN (1, (LM_ERROR,
                          ACE_TEXT ("%p\n"),
                          ACE_TEXT ("[RM::read_mb_from_disk] file length error")),
                          0);
  }

  // Read the contents of the file into the buffer
  ACE_Message_Block* mb = 0;
  ACE_NEW_RETURN (mb, ACE_Message_Block (length + 1), 0);

  if (ACE_OS::read_n (handle, mb->wr_ptr (), length) == -1)
        DANCE_ERROR_RETURN (1, (LM_ERROR,
                           ACE_TEXT ("%p\n"),
               ACE_TEXT ("[RM::read_mb_from_disk] file read error")),
                           0);

  mb->length (length);

  // Close the file handle
  ACE_OS::close (handle);

  return mb;
}
