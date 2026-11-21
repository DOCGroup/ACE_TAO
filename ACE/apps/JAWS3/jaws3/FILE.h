// -*- c++ -*-

#ifndef JAWS_FILE_H
#define JAWS_FILE_H

#include "ace/FILE_IO.h"
#include "ace/Mem_Map.h"
#include <memory>

#include "jaws3/Export.h"

class JAWS_Export JAWS_FILE : public ACE_FILE_IO
//
// Like ACE_FILE_IO, but support for ACE_Mem_Map;
{
public:
  JAWS_FILE ();

  ~JAWS_FILE ();
  JAWS_FILE (JAWS_FILE const &) = delete;
  JAWS_FILE (JAWS_FILE &&) = delete;
  JAWS_FILE &operator= (JAWS_FILE const &) = delete;
  JAWS_FILE &operator= (JAWS_FILE &&) = delete;

  ACE_Mem_Map *mem_map (int length = -1,
                        int prot = PROT_RDWR,
                        int share = ACE_MAP_PRIVATE,
                        void *addr = nullptr,
                        ACE_OFF_T offset = 0,
                        LPSECURITY_ATTRIBUTES sa = 0);
  ACE_Mem_Map *mem_map (int length = -1,
                        int prot = PROT_RDWR,
                        int share = ACE_MAP_PRIVATE,
                        void *addr = nullptr,
                        ACE_OFF_T offset = 0,
                        LPSECURITY_ATTRIBUTES sa = 0) const;

  ACE_Mem_Map *map () const;

  void can_map (bool);

private:
  ACE_SYNCH_MUTEX lock_;
  std::unique_ptr<ACE_Mem_Map> map_;

  bool can_map_;
};

#endif /* JAWS_FILE_H */
