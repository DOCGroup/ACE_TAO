/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Routing_Entry.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_ROUTING_ENTRY)
#define _ROUTING_ENTRY

#include "ace/Set.h"

// Forward reference.
class Channel;

class Routing_Entry
{
  // = TITLE
  //    Defines an entry in the Routing_Table.
public:
  Routing_Entry (int validity_interval = 0);
  ~Routing_Entry (void);

  typedef ACE_Unbounded_Set<Channel *> ENTRY_SET;
  typedef ACE_Unbounded_Set_Iterator<Channel *> ENTRY_ITERATOR;

  // = Set/get the associated set of destinations.
  ENTRY_SET *destinations (void);
  void destinations (ENTRY_SET *);

  // = Set/get current validity interval for this routing entry.
  int validity_interval (void);
  void validity_interval (int);

protected:
  ENTRY_SET *destinations_;
  // The set of destinations;

  int validity_interval_;
  // The current validity interval of this link.
};

#endif /* _ROUTING_ENTRY */
