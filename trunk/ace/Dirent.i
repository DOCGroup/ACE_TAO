/* -*- C++ -*- */
// $Id$

ACE_INLINE int
ACE_Dirent::opendir (char *dirname) 
{
  this->dirp_ = ACE_OS::opendir (dirname);

  if (this->dirp == 0)
    return -1;
  else
    return 0;
}

ACE_INLINE 
ACE_Dirent::Dirent (char *dirname) 
{
  if (this->open (dirname) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n", 
                "Dirent::Dirent"))
}

ACE_INLINE 
Dirent::~Dirent (void)
{
  ACE_OS::closedir (this->dirp_);
}

ACE_INLINE struct direct *
Dirent::readdir (void)
{
  return ACE_OS::readdir (this->dirp_);
}

ACE_INLINE int
Dirent::readdir_r (struct dirent *entry,
                   struct dirent **result)
{
  return ACE_OS::readdir_r (this->dirp_,
                            entry,
                            result);
}

ACE_INLINE void
Dirent::closedir (void)
{
  ACE_OS::closedir (this->dirp_);
}

ACE_INLINE void
Dirent::rewinddir (void)
{
  ACE_OS::rewinddir (this->dirp_);
}

ACE_INLINE void
Dirent::seekdir (long loc)
{
  ACE_OS::seekdir (this->dirp_, loc);
}

ACE_INLINE long
Dirent::telldir (void)
{
  return ACE_OS::telldir (this->dirp_);
}

