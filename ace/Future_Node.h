/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Future_Node.h
//
// = AUTHOR
//    John Tucker <jtucker@infoglide.com>
//
// ============================================================================

#ifndef ACE_FUTURE_NODE_H
#define ACE_FUTURE_NODE_H

#include "ace/Future.h"
#include "ace/Thread.h"
#include "ace/Containers_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_THREADS)

// Forward decl.
template <class T> class ACE_Future_Node;
template <class T> class ACE_DLList_Future_Node;

template <class T>
class ACE_DLList_Future_Node
{
  // = TITLE
  //     Implementation of element in a ACE_Future list.
  //     Needed for ACE_Double_Linked_List.

  friend class ACE_Double_Linked_List<ACE_DLList_Future_Node>;
  friend class ACE_Double_Linked_List_Iterator<ACE_DLList_Future_Node>;

public:
  // = Initialization
  ACE_DLList_Future_Node (const ACE_Future<T> &future,
                          ACE_DLList_Future_Node *n = 0,
                          ACE_DLList_Future_Node *p = 0);
  ~ACE_DLList_Future_Node (void);

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  ACE_Future<T> item_;
  ACE_DLList_Future_Node *next_;
  ACE_DLList_Future_Node *prev_;

protected:
  ACE_DLList_Future_Node (void);
};


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Future_Node.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Future_Node.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_HAS_THREADS */
#endif /* ACE_FUTURE_NODE_H */
