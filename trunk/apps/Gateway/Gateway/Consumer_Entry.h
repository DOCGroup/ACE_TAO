/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Consumer_Entry.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_ROUTING_ENTRY)
#define _ROUTING_ENTRY

#include "ace/Set.h"

// Forward reference.
class IO_Handler;

class Consumer_Entry
{
  // = TITLE
  //    Defines an entry in the Consumer_Map.
public:
  Consumer_Entry (void);
  ~Consumer_Entry (void);

  typedef ACE_Unbounded_Set<IO_Handler *> ENTRY_SET;
  typedef ACE_Unbounded_Set_Iterator<IO_Handler *> ENTRY_ITERATOR;

  // = Set/get the associated set of destinations.
  ENTRY_SET *destinations (void);
  void destinations (ENTRY_SET *);

protected:
  ENTRY_SET *destinations_;
  // The set of destinations;
};

#endif /* _ROUTING_ENTRY */
