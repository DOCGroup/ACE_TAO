/* -*- C++ -*- */
// $Id$

ACE_INLINE int
ACE_Dirent::open (const char *dirname) 
{
  // If the directory stream is already open, close it to prevent
  // possible resource leaks.

  if (this->dirp_ != 0) 
    {
      ACE_OS::closedir (this->dirp_);
      this->dirp_ = 0;
    }

  this->dirp_ = ACE_OS::opendir (dirname);

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
ACE_Dirent::ACE_Dirent (const char *dirname) 
  : dirp_ (0)
{
  if (this->open (dirname) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n", 
                "Dirent::Dirent"));
}

ACE_INLINE 
ACE_Dirent::~ACE_Dirent (void)
{
  if (this->dirp_ != 0)
    ACE_OS::closedir (this->dirp_);
}

ACE_INLINE dirent *
ACE_Dirent::read (void)
{
  return ACE_OS::readdir (this->dirp_);
}

ACE_INLINE int
ACE_Dirent::read (struct dirent *entry,
                  struct dirent **result)
{
  return ACE_OS::readdir_r (this->dirp_,
                            entry,
                            result);
}

ACE_INLINE void
ACE_Dirent::close (void)
{
  if (this->dirp_ != 0) 
    {
      ACE_OS::closedir (this->dirp_);

      // Prevent double closure
      this->dirp_ = 0;
    }
}

ACE_INLINE void
ACE_Dirent::rewind (void)
{
  ACE_OS::rewinddir (this->dirp_);
}

ACE_INLINE void
ACE_Dirent::seek (long loc)
{
  ACE_OS::seekdir (this->dirp_, loc);
}

ACE_INLINE long
ACE_Dirent::tell (void)
{
  return ACE_OS::telldir (this->dirp_);
}

