// $Id$

#ifndef SHMEM_H
#define SHMEM_H

// This is where you'll find the ACE_Shared_Memory_SV object
#include "ace/Shared_Memory_SV.h"

// SHMSZ is just enough for the alphabet and a null terminator
#define SHMSZ 27

// Play with this, pick a value you like that isn't used by something else.
#define SHM_KEY 4200

/*
  This is what we stuff into shared memory via placement new in the
  second client/server pair.  Notice that it is a very basic object
  with no virtual methods and only concrete data.
 */
class SharedData
{
public:
  // Construct the object and optionally initialize buf_.
  SharedData (int initialized = 1);

  // Put some data into buf_
  void set (void);

  // Show the data in buf_
  void show (void);

  // What is the value of available_
  int available (void);

  // Set the value of available_
  void available (int not_in_use);
        
protected:
  // Big enough for a simple message
  char buf_[128];
  // A cheap mutex
  int available_;
};

#endif /* SHMEM_H */
