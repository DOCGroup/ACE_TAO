/* -*- C++ -*- */
// $Id$

#ifdef ACE_SUBSET_0
#include "ace/Log_Msg.h"
#endif

ACE_INLINE int
ACE_Dirent::open (const ACE_TCHAR *dirname) 
{
  // If the directory stream is already open, close it to prevent
  // possible resource leaks.

  if (this->dirp_ != 0) 
    {
      ACE_OS_Dirent::closedir (this->dirp_);
      this->dirp_ = 0;
    }

  this->dirp_ = ACE_OS_Dirent::opendir (dirname);

  if (this->dirp_ == 0)
    return -1;
  else
    return 0;
}

ACE_INLINE 
ACE_Dirent::ACE_Dirent (void)
  : dirp_ (0)
{
}

ACE_INLINE 
ACE_Dirent::ACE_Dirent (const ACE_TCHAR *dirname) 
  : dirp_ (0)
{
#ifdef ACE_SUBSET_0
  if (this->open (dirname) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"), 
                ACE_LIB_TEXT ("Dirent::Dirent")));
#else
  this->open (dirname);
#endif /* ACE_SUBSET_0 */
}

ACE_INLINE 
ACE_Dirent::~ACE_Dirent (void)
{
  if (this->dirp_ != 0)
    ACE_OS_Dirent::closedir (this->dirp_);
}

ACE_INLINE dirent *
ACE_Dirent::read (void)
{
  return this->dirp_ ? ACE_OS_Dirent::readdir (this->dirp_) : 0;
}

ACE_INLINE int
ACE_Dirent::read (struct dirent *entry,
                  struct dirent **result)
{
  return this->dirp_
         ? ACE_OS_Dirent::readdir_r (this->dirp_, entry, result)
         : 0;
}

ACE_INLINE void
ACE_Dirent::close (void)
{
  if (this->dirp_ != 0) 
    {
      ACE_OS_Dirent::closedir (this->dirp_);

      // Prevent double closure
      this->dirp_ = 0;
    }
}

ACE_INLINE void
ACE_Dirent::rewind (void)
{
  if (this->dirp_)
    ACE_OS_Dirent::rewinddir (this->dirp_);
}

ACE_INLINE void
ACE_Dirent::seek (long loc)
{
  if (this->dirp_)
    ACE_OS_Dirent::seekdir (this->dirp_, loc);
}

ACE_INLINE long
ACE_Dirent::tell (void)
{
  return this->dirp_ ? ACE_OS_Dirent::telldir (this->dirp_) : 0;
}

