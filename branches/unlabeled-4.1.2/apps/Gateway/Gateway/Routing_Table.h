/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Routing_Table.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_ROUTING_TABLE_H)
#define _ROUTING_TABLE_H

#include "ace/Map_Manager.h"

template <class EXT_ID, class INT_ID, class LOCK>
class Routing_Table
{
  // = TITLE
  //    Define a generic routing table based on the ACE Map_Manager.  
  //
  // = DESCRIPTION
  //    We need to have this table, rather than just using the Map_Manager
  //    directly in order to ignore "inactive" routing entries...
public:
  int bind (EXT_ID ext_id, INT_ID *int_id);
  // Associate EXT_ID with the INT_ID.

  int find (EXT_ID ext_id, INT_ID *&int_id);
  // Break any association of EXID.

  int unbind (EXT_ID ext_id);
  // Locate EXID and pass out parameter via INID.  If found,
  // return 0, else -1.

public:
  ACE_Map_Manager<EXT_ID, INT_ID *, LOCK> map_;
  // Map external IDs to internal IDs.
};

template <class EXT_ID, class INT_ID, class LOCK>
class Routing_Iterator
{
  // = TITLE
  //    Define an iterator for the Routing Table.
public:
  Routing_Iterator (Routing_Table<EXT_ID, INT_ID, LOCK> &mm, 
		    int ignore_inactive = 1);
  int next (INT_ID *&);
  int advance (void);

private:
  ACE_Map_Iterator<EXT_ID, INT_ID *, LOCK> map_iter_;
  int ignore_inactive_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Routing_Table.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */
#endif /* _ROUTING_TABLE_H */
