// $Id$
// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//    Node.h
//
// = AUTHOR
//    Michael Kircher
//
// = DESCRIPTION
//    This file descibes the various node types for analysing
//    an CORBA::Any. It is no memory allocated, that means
//    The values are not really contained by the nodes, but the
//    nodes refer to the values via pointers.
//
// ============================================================================

#include "tao/corba.h"
#include "ace/Containers.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "NodeVisitor.h"

#ifndef NODE_H
#define NODE_H


// base class for nodes
class Node {
public:
  virtual ~Node () {}
  virtual void Accept (NodeVisitor *NodeVisitor) = 0;
  virtual unsigned int getRecursionLevel () = 0;

protected:
  Node () {}
};



// Node to store information about a Struct
class StructNode : public Node {
public:
  StructNode (const char *Name_ptr,
              unsigned int recursion_level);
  ~StructNode ();

  // accpet a visitor
  void Accept (NodeVisitor *nodeVisitor);

  // add a new child
  int addChild (Node *node);

  // retrieve the nth Child starting at 0
  Node *getChild (unsigned int n);

  // get the number of children
  unsigned int getChildNumber ();

  // get the name of the struct
  const char *getName ();

  // get recursion level
  unsigned int getRecursionLevel ();


private:
  ACE_Unbounded_Queue<Node *> *queue_ptr_;
  ACE_Unbounded_Queue_Iterator<Node *> *queue_iterator_ptr_;
  unsigned int queue_position_;
  const char *Name_ptr_;
  unsigned int recursion_level_;
};


// Node to store information about a Double
class DoubleNode : public Node {
public:
  DoubleNode (CORBA::Double *Double_ptr,
              const char *Name_ptr,
              unsigned int recursion_level);
  ~DoubleNode () {}

  // accpet a visitor
  void Accept (NodeVisitor *nodeVisitor);

  // get the name of the double node
  const char *getName ();

  // get the value
  CORBA::Double getValue ();

  // get recursion level
  unsigned int getRecursionLevel ();

private:
  CORBA::Double *Double_ptr_;
  const char *Name_ptr_;
  unsigned int recursion_level_;
};


// Node to store information about a Long
class LongNode : public Node {
public:
  LongNode (CORBA::Long *Long_ptr,
            const char *Name_ptr,
            unsigned int recursion_level);
  ~LongNode () {}

  // accpet a visitor
  void Accept (NodeVisitor *nodeVisitor);

  //get the name of the long node
  const char *getName ();

  // get the value
  CORBA::Long getValue ();

  // get recursion level
  unsigned int getRecursionLevel ();

private:
  CORBA::Long *Long_ptr_;
  const char *Name_ptr_;
  unsigned int recursion_level_;
};


// Node to store information about a ULong
class ULongNode : public Node {
public:
  ULongNode (CORBA::ULong *Long_ptr,
             const char *Name_ptr,
             unsigned int recursion_level);
  ~ULongNode () {}

  // accpet a visitor
  void Accept (NodeVisitor *nodeVisitor);

  // get the name of the ulong node
  const char *getName ();

  // get the value
  CORBA::ULong getValue ();

  // get recursion level
  unsigned int getRecursionLevel ();

private:
  CORBA::ULong *ULong_ptr_;
  const char *Name_ptr_;
  unsigned int recursion_level_;
};


// Node to store information about a String
class StringNode : public Node {
public:
  StringNode (CORBA::String_var String_var,
              const char *Name_ptr,
              unsigned int recursion_level);
  ~StringNode () {}

  // accpet a visitor
  void Accept (NodeVisitor *nodeVisitor);

  // get the name
  const char *getName ();

  // get the value
  CORBA::String_var getValue ();

  // get recursion level
  unsigned int getRecursionLevel ();

private:
  CORBA::String_var String_var_;
  const char *Name_ptr_;
  unsigned int recursion_level_;
};

#endif // NODE_H


