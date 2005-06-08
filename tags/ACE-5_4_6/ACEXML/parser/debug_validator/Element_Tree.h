// -*- C++ -*-

//=============================================================================
/**
 *  @file    Element_Tree.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
#ifndef _ACEXML_ELEMENT_TREE_H_
#define _ACEXML_ELEMENT_TREE_H_

#include /**/ "ace/pre.h"
#include "ACEXML/parser/debug_validator/Debug_DTD_Manager_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"

/**
 * @class ACEXML_Element_Tree_Node Element_Tree.h "parser/debug_validator/Element_Tree.h"
 *
 * @brief An abstract base class for describing DTD child element definition.
 *
 * @sa ACEXML_Element_Tree_Name_Node, ACEXML_Element_Tree_List_Node
 */
class ACEXML_DEBUG_DTD_MANAGER_Export ACEXML_Element_Tree_Node
{
public:

  /// Default constructor.
  ACEXML_Element_Tree_Node ();

  /// Destructor
  virtual ~ACEXML_Element_Tree_Node ();

  /// Accessor for next element in chain
  ACEXML_Element_Tree_Node *next ();
  void next (ACEXML_Element_Tree_Node *n);

  /// Displaying the content.
  virtual void dump () = 0;

  ACE_ALLOC_HOOK_DECLARE;

protected:
  ACEXML_Element_Tree_Node *next_;
};

/**
 * @class ACEXML_Element_Tree_Name_Node Element_Tree.h "parser/debug_validator/Element_Tree.h"
 *
 * @brief An abstract base class for describing a name node in a DTD child
 * element definition.
 *
 * @sa ACEXML_Element_Tree_Node, ACEXML_Element_Tree_List_Node
 */
class ACEXML_DEBUG_DTD_MANAGER_Export ACEXML_Element_Tree_Name_Node
  : public ACEXML_Element_Tree_Node
{
public:
  /// Constructor.
  ACEXML_Element_Tree_Name_Node (const ACEXML_Char *name,
                                 int release = 1);

  /// Change the name of this node.
  void set (const ACEXML_Char *name,
            int release = 1);

  virtual void dump ();

  ACE_ALLOC_HOOK_DECLARE;
protected:
  ACEXML_String name_;
};

class ACEXML_Element_Tree_List_Stack;

/**
 * @class ACEXML_Element_Tree_List_Node Element_Tree.h "parser/debug_validator/Element_Tree.h"
 *
 * @brief An abstract base class for describing a node list in a DTD child
 * element definition.
 *
 * @sa ACEXML_Element_Tree_Node, ACEXML_Element_Tree_Name_Node
 */
class ACEXML_DEBUG_DTD_MANAGER_Export ACEXML_Element_Tree_List_Node
  : public ACEXML_Element_Tree_Node
{
public:
  friend class ACEXML_Element_Tree_List_Stack;

  typedef enum {
    SEQUENCE,
    CHOICE
  } LIST_TYPE;

  /// Default constructor.
  ACEXML_Element_Tree_List_Node (void);

  /// Destructor.
  virtual ~ACEXML_Element_Tree_List_Node (void);

  /// Insert a new ACEXML_Element_Tree_Node into the list.
  int insert (ACEXML_Element_Tree_Node *node);

  /// Get/set the type of list.
  LIST_TYPE get (void);
  int set (LIST_TYPE type);

  virtual void dump ();

  ACE_ALLOC_HOOK_DECLARE;
protected:
  LIST_TYPE type_;

  ACEXML_Element_Tree_Node *head_;

  ACEXML_Element_Tree_Node *tail_;

  ACEXML_Element_Tree_List_Node *pop_next_;
};

/**
 * @class ACEXML_Element_Tree_List_Stack Element_Tree.h "parser/debug_validator/Element_Tree.h"
 *
 * @brief A class for managing a stack of ACEXML_Element_Tree_List_Node's.
 *
 * @sa ACEXML_Element_Tree_List_Node
 */
class ACEXML_DEBUG_DTD_MANAGER_Export ACEXML_Element_Tree_List_Stack
{
public:
  ACEXML_Element_Tree_List_Stack ();

  void push (ACEXML_Element_Tree_List_Node *n);

  ACEXML_Element_Tree_List_Node *pop (void);

  ACEXML_Element_Tree_List_Node *top (void);

  int empty (void);

  ACE_ALLOC_HOOK_DECLARE;

protected:
  ACEXML_Element_Tree_List_Node *top_;
};

#if defined (__ACEXML_INLINE__)
# include "ACEXML/parser/debug_validator/Element_Tree.i"
#endif /* __ACEXML_INLINE__ */

#include /**/ "ace/post.h"

#endif /* _ACEXML_ELEMENT_TREE_H_ */
