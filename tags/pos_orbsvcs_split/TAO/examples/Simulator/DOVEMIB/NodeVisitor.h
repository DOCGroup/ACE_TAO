// $Id$
// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//    NodeVisitor.h
//
// = AUTHOR
//    Michael Kircher
//
// = DESCRIPTION
//    This file descibes the Visitor used by the any evaluator.
//
// ============================================================================

#ifndef NODE_VISITOR_H
#define NODE_VISITOR_H

class StructNode;
class DoubleNode;
class LongNode;
class ULongNode;
class StringNode;

// base class for all the specialized visitors

class NodeVisitor {
public:
  virtual void visitStructNode (StructNode *structNode) = 0;
  virtual void visitDoubleNode (DoubleNode *doubleNode) = 0;
  virtual void visitLongNode (LongNode *longNode) = 0;
  virtual void visitULongNode (ULongNode *uLongNode) = 0;
  virtual void visitStringNode (StringNode *stringNode) = 0;
protected:
  NodeVisitor () {}
};

#endif // NODE_VISITOR_H
