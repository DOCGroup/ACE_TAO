// $Id$

/* -*- c++ -*- */

#ifndef JAWS_FILE_H
#define JAWS_FILE_H

#include "ace/FILE_IO.h"
#include "ace/Mem_Map.h"

class JAWS_Export JAWS_FILE : public ACE_FILE_IO
//
// Like ACE_FILE_IO, but support for ACE_Mem_Map;
{
public:

  JAWS_FILE (void);
  JAWS_FILE (ACE_HANDLE map_handle);

  ACE_Mem_Map *mem_map (void);
  ACE_Mem_Map *mem_map (void) const;

  ACE_Mem_Map *map (void) const;

private:

  ACE_HANDLE map_handle_;

  ACE_SYNCH_MUTEX lock_;
  ACE_Mem_Map *map_;

};

#endif /* JAWS_FILE_H */
