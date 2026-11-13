// -*- C++ -*-

//=============================================================================
/**
 *  @file    Element_Tree.h
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
#include <memory>

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
  ACEXML_Element_Tree_Node () = default;

  virtual ~ACEXML_Element_Tree_Node ();
  ACEXML_Element_Tree_Node (ACEXML_Element_Tree_Node const &) = delete;
  ACEXML_Element_Tree_Node (ACEXML_Element_Tree_Node &&) = delete;
  ACEXML_Element_Tree_Node &operator= (ACEXML_Element_Tree_Node const &) = delete;
  ACEXML_Element_Tree_Node &operator= (ACEXML_Element_Tree_Node &&) = delete;

  /// Get the next element in the chain.
  ACEXML_Element_Tree_Node *next () const;
  /// Set the next element, taking ownership of n.
  void next (ACEXML_Element_Tree_Node *n);

  /// Display the content.
  virtual void dump () = 0;

  ACE_ALLOC_HOOK_DECLARE;

protected:
  std::unique_ptr<ACEXML_Element_Tree_Node> next_ {};
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
  explicit ACEXML_Element_Tree_Name_Node (ACEXML_Char const *name,
                                          bool release = true);
  ACEXML_Element_Tree_Name_Node (ACEXML_Element_Tree_Name_Node const &) = delete;
  ACEXML_Element_Tree_Name_Node (ACEXML_Element_Tree_Name_Node &&) = delete;
  ACEXML_Element_Tree_Name_Node &operator= (ACEXML_Element_Tree_Name_Node const &) = delete;
  ACEXML_Element_Tree_Name_Node &operator= (ACEXML_Element_Tree_Name_Node &&) = delete;

  /// Change the name of this node.
  void set (ACEXML_Char const *name,
            bool release = true);

  void dump () override;

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

  enum class LIST_TYPE {
    SEQUENCE,
    CHOICE
  };

  ACEXML_Element_Tree_List_Node ();

  ~ACEXML_Element_Tree_List_Node () override;
  ACEXML_Element_Tree_List_Node (ACEXML_Element_Tree_List_Node const &) = delete;
  ACEXML_Element_Tree_List_Node (ACEXML_Element_Tree_List_Node &&) = delete;
  ACEXML_Element_Tree_List_Node &operator= (ACEXML_Element_Tree_List_Node const &) = delete;
  ACEXML_Element_Tree_List_Node &operator= (ACEXML_Element_Tree_List_Node &&) = delete;

  /// Insert a new ACEXML_Element_Tree_Node into the list.
  /// Takes ownership of node.
  int insert (ACEXML_Element_Tree_Node *node);

  /// Get/set the type of list.
  LIST_TYPE get () const;
  int set (LIST_TYPE type);

  void dump () override;

  ACE_ALLOC_HOOK_DECLARE;
protected:
  LIST_TYPE type_ {LIST_TYPE::SEQUENCE};

  std::unique_ptr<ACEXML_Element_Tree_Node> head_ {};

  ACEXML_Element_Tree_Node *tail_ {}; // Non-owning alias into the child node chain.

  /// Non-owning stack link.
  ACEXML_Element_Tree_List_Node *pop_next_ {};
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
  ACEXML_Element_Tree_List_Stack () = default;
  ACEXML_Element_Tree_List_Stack (ACEXML_Element_Tree_List_Stack const &) = delete;
  ACEXML_Element_Tree_List_Stack (ACEXML_Element_Tree_List_Stack &&) = delete;
  ACEXML_Element_Tree_List_Stack &operator= (ACEXML_Element_Tree_List_Stack const &) = delete;
  ACEXML_Element_Tree_List_Stack &operator= (ACEXML_Element_Tree_List_Stack &&) = delete;

  void push (ACEXML_Element_Tree_List_Node *n);

  ACEXML_Element_Tree_List_Node *pop ();

  ACEXML_Element_Tree_List_Node *top () const;

  bool empty () const;

  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// Non-owning pointer to the top node; the tree owns the list nodes.
  ACEXML_Element_Tree_List_Node *top_ {};
};

#if defined (__ACEXML_INLINE__)
# include "ACEXML/parser/debug_validator/Element_Tree.inl"
#endif /* __ACEXML_INLINE__ */

#include /**/ "ace/post.h"

#endif /* _ACEXML_ELEMENT_TREE_H_ */
