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

#ifndef ACE_CONTAINERS_H
#include "ace/pre.h"
#define ACE_CONTAINERS_H

#include "ace/OS.h"
#include "ace/Malloc_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <class T>
class ACE_Double_Linked_List;

template <class T>
class ACE_Double_Linked_List_Iterator_Base;
template <class T>
class ACE_Double_Linked_List_Iterator;
template <class T>
class ACE_Double_Linked_List_Reverse_Iterator;

class ACE_Export ACE_DLList_Node
{
  // = TITLE
  //     Base implementation of element in a DL list.  Needed for
  //     ACE_Double_Linked_List.
public:
  friend class ACE_Double_Linked_List<ACE_DLList_Node>;
  friend class ACE_Double_Linked_List_Iterator_Base<ACE_DLList_Node>;
  friend class ACE_Double_Linked_List_Iterator<ACE_DLList_Node>;
  friend class ACE_Double_Linked_List_Reverse_Iterator<ACE_DLList_Node>;

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

#include "ace/post.h"
#endif /* ACE_CONTAINERS_H */
