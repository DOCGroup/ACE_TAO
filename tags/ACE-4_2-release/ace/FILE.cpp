// FILE.cpp
// $Id$

/* Defines the member functions for the base class of the ACE_IO_SAP
   ACE_FILE abstraction. */ 

#define ACE_BUILD_DLL
#include "ace/FILE.h"

ACE_ALLOC_HOOK_DEFINE(ACE_FILE)

void
ACE_FILE::dump (void) const
{
  ACE_TRACE ("ACE_FILE::dump");
  ACE_IO_SAP::dump ();
}

/* This is the do-nothing constructor. */

ACE_FILE::ACE_FILE (void)
{
  ACE_TRACE ("ACE_FILE::ACE_FILE");
}

// Close the file

int
ACE_FILE::close (void)
{
  ACE_TRACE ("ACE_FILE::close");
  int result = ACE_OS::close (this->get_handle ());
  this->set_handle (ACE_INVALID_HANDLE);
  return result;
}

int
ACE_FILE::get_info (ACE_FILE_Info *finfo)
{
  ACE_TRACE ("ACE_FILE::get_info");
  struct stat filestatus;
  int result = ACE_OS::fstat (this->get_handle (), &filestatus);

  if (result == 0)
    {
      finfo->mode_ = filestatus.st_mode;
      finfo->nlink_ = filestatus.st_nlink;
      finfo->size_ = filestatus.st_size;
    }
  return result;
}

int
ACE_FILE::truncate (off_t length)
{
  ACE_TRACE ("ACE_FILE::truncate");
  return ACE_OS::ftruncate (this->get_handle(), length);
}

off_t
ACE_FILE::position (long offset, int startpos)
{
  ACE_TRACE ("ACE_FILE::position");
  return ACE_OS::lseek (this->get_handle (), offset, startpos);
}

off_t
ACE_FILE::position (void)
{
  ACE_TRACE ("ACE_FILE::position");
  return ACE_OS::lseek (this->get_handle (), 0, SEEK_CUR);
} 
