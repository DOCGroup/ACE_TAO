// $Id$

#ifndef MMAP_H
#define MMAP_H

// The expected filename for ACE_Shared_Memory_MM.h
#include "ace/Shared_Memory_MM.h"

// Just enough for the alphabet...
#define SHMSZ 27

/*
  Here we use a real filename instead of an arbitrary number.  This
  actually will exist in the filesystem.  You can 'cat' it and
  everything!
*/
#define SHM_KEY "mmapfile"

/*
  The SV Shared Memory SharedData object returns.  It is identical to
  the one we used in that tutorial.  I didn't even change the name.
 */
class SharedData
{
public:
  SharedData (int initialize = 1);
    
  void set (void);
  void show (void);
  int available (void);
  void available (int not_in_use);
        
protected:
  char buf_[128];
  int available_;
};

#endif /* MMAP_H */
