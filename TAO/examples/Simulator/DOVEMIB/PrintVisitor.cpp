// $Id$
// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//    any_test_i.cpp
//
// = AUTHOR
//    Michael Kircher 
//
// = DESCRIPTION
//   Visitor for the Nodes of the any analyser.
//
// ============================================================================

#include "PrintVisitor.h"


PrintVisitor::PrintVisitor (const char *file_name) {
  
  TAO_TRY {
    output_ =  stdout;//ACE_OS::fopen(file_name, "w");
    TAO_CHECK_ENV;
  }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "Failing when trying to open the output file.\n"));
    }
  TAO_ENDTRY;  
}


PrintVisitor::~PrintVisitor () {
  ACE_OS::fclose (output_);
}


// Visit a struct node
void
PrintVisitor::visitStructNode (StructNode *structNode) {
  
  // print the padding in front of the line
  printPadding (structNode->getRecursionLevel());
  fprintf (output_, "struct %s {\n", structNode->getName ());
  
  for (unsigned int i = 0; i < structNode->getChildNumber (); i++) {
    printPadding (structNode->getChild (i)->getRecursionLevel());
    structNode->getChild (i)->Accept((NodeVisitor *)this);
    fprintf (output_, "\n");  
  }
  
  printPadding (structNode->getRecursionLevel());
  fprintf (output_, "}\n");  
}

void 
PrintVisitor::visitDoubleNode (DoubleNode *doubleNode) {
  printPadding (doubleNode->getRecursionLevel());  
  fprintf (output_, "CORBA::double %s = %f;", doubleNode->getName(), doubleNode->getValue());
}

void
PrintVisitor::visitLongNode (LongNode *longNode) {
  printPadding (longNode->getRecursionLevel());  
  fprintf (output_, "CORBA::Long %s = %d;", longNode->getName(), longNode->getValue());
}

void
PrintVisitor::visitULongNode (ULongNode *uLongNode) { 
  printPadding (uLongNode->getRecursionLevel());  
  fprintf (output_, "CORBA::ULong %s = %d;", uLongNode->getName(), uLongNode->getValue());
}

void
PrintVisitor::visitStringNode (StringNode *stringNode) { 
  printPadding (stringNode->getRecursionLevel());  
  fprintf (output_, "CORBA::String %s = %s;", stringNode->getName(), (char *)stringNode->getValue());
}

void
PrintVisitor::printPadding (unsigned int recursion_level) {
  switch (recursion_level) {
  case 0: break;
  case 1: fprintf (output_, "   ");
    break;
  case 2: fprintf (output_, "      ");
    break;
  case 3: fprintf (output_, "         ");
    break;
  case 4: fprintf (output_, "            ");
    break;
  default: for (unsigned int i = 0; i < recursion_level; i++) 
    {
      fprintf (output_, "   ");
    }
  break;
  }
}
  
