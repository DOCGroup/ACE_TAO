/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Containers.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#if !defined (ACE_CONTAINERS_H)
#define ACE_CONTAINERS_H

#include "ace/OS.h"

template <class T> 
class ACE_Double_Linked_List;

template <class T> 
class ACE_Double_Linked_List_Iterator;

class ACE_Export ACE_DLList_Node
{
  // = TITLE
  //     Base implementation of element in a DL list.  Needed for
  //     ACE_Double_Linked_List.

  friend class ACE_Double_Linked_List<ACE_DLList_Node>;
  friend class ACE_Double_Linked_List_Iterator<ACE_DLList_Node>;

public:
  ACE_DLList_Node (void *&i,
                   ACE_DLList_Node *n = 0,
                   ACE_DLList_Node *p = 0);
  ~ACE_DLList_Node (void);

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  void *item_;

  ACE_DLList_Node *next_;
  ACE_DLList_Node *prev_;

protected:
  ACE_DLList_Node (void);
};

#if defined (__ACE_INLINE__)
#include "ace/Containers.i"
#endif /* __ACE_INLINE__ */

#include "ace/Containers_T.h"

#endif /* ACE_CONTAINERS_H */
