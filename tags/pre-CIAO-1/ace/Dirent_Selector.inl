// -*- C++ -*-
//
// $Id$

ACE_INLINE int
ACE_Dirent_Selector::length (void) const
{
  return n_;
}

ACE_INLINE dirent *
ACE_Dirent_Selector::operator[] (const int n) const
{
  return this->namelist_[n];
}
