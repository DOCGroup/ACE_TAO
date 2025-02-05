/* -*- c++ -*- */

#ifndef JAWS_FILE_H
#define JAWS_FILE_H

#include "ace/FILE_IO.h"
#include "ace/Mem_Map.h"

#include "JAWS/Export.h"
#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"

class JAWS_Export JAWS_FILE : public ACE_FILE_IO
//
// Like ACE_FILE_IO, but support for ACE_Mem_Map;
{
public:
  JAWS_FILE ();

  ~JAWS_FILE ();

  ACE_Mem_Map *mem_map (int length = -1,
                        int prot = PROT_RDWR,
                        int share = ACE_MAP_PRIVATE,
                        void *addr = 0,
                        ACE_OFF_T offset = 0,
                        LPSECURITY_ATTRIBUTES sa = 0);
  ACE_Mem_Map *mem_map (int length = -1,
                        int prot = PROT_RDWR,
                        int share = ACE_MAP_PRIVATE,
                        void *addr = 0,
                        ACE_OFF_T offset = 0,
                        LPSECURITY_ATTRIBUTES sa = 0) const;

  ACE_Mem_Map *map () const;

private:
  ACE_SYNCH_MUTEX lock_;
  ACE_Mem_Map *map_;
};

#endif /* JAWS_FILE_H */
