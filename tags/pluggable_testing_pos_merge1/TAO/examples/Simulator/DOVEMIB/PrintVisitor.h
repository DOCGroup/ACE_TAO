// $Id$
// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//    PrintVisitor.h
//
// = AUTHOR
//    Michael Kircher
//
// = DESCRIPTION
//    This file descibes the PrintVisitor used by the any evaluator.
//
// ============================================================================

#include "tao/corba.h"
#include "NodeVisitor.h"
#include "Node.h"

#ifndef PRINT_VISITOR_H
#define PRINT_VISITOR_H


// This is a concrete Visitor using the Visitor design pattern
class PrintVisitor : public NodeVisitor {
public:

  PrintVisitor (const char *file_name);
  virtual ~PrintVisitor ();

  void printTimeStamp (ACE_hrtime_t creation,
                       ACE_hrtime_t ec_recv,
                       ACE_hrtime_t ec_send);
  // Print the time stamp

  void close ();

  void visitStructNode (StructNode *structNode);
  void visitDoubleNode (DoubleNode *doubleNode);
  void visitLongNode (LongNode *longNode);
  void visitULongNode (ULongNode *uLongNode);
  void visitStringNode (StringNode *stringNode);
private:
  void printPadding (unsigned int recursion_level);
  void printSecNanoSec (ACE_hrtime_t total_nanoseconds);

  FILE *output_;
};

#endif // PRINT_VISITOR_H
