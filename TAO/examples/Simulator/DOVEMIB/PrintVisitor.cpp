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
#include <stdio.h>

ACE_RCSID(DOVEMIB, PrintVisitor, "$Id$")

PrintVisitor::PrintVisitor (const char *file_name) {
  
  TAO_TRY {
    if (file_name != 0) {
      output_ =  ACE_OS::fopen(file_name, "w");
      TAO_CHECK_ENV;

       // print it on the screen if file could be opened
      if (output_ == 0) {

        output_ = stdout;

        ACE_DEBUG ((LM_DEBUG,"PrintVisitor: Use stdout for output.\n"));
      }

      // everything is ok
      ACE_DEBUG ((LM_DEBUG,"PrintVisitor: Opened file <%s> successfully.\n",
                  file_name));
    }
    else {
       output_ = stdout;
       ACE_DEBUG ((LM_DEBUG,"PrintVisitor: Use stdout for output.\n"));

    }

  }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "Failing when trying to open the output file.\n"));
    }
  TAO_ENDTRY;  
}


PrintVisitor::~PrintVisitor () {
  this->close ();
}

void 
PrintVisitor::close () {
  if (output_ != stdout && output_ != 0) {
    ACE_OS::fclose (output_);
  }
}


void 
PrintVisitor::printSecNanoSec (ACE_hrtime_t total_nanoseconds)
{
  // Separate to seconds and nanoseconds.
  u_long total_secs  = (u_long) (total_nanoseconds / (ACE_UINT32) ACE_ONE_SECOND_IN_NSECS);
  ACE_UINT32 extra_nsecs = (ACE_UINT32) (total_nanoseconds % (ACE_UINT32) ACE_ONE_SECOND_IN_NSECS);
  
  ACE_OS::fprintf (output_,
                   "%3lu.%06lu secs\n",
                   total_secs, 
                   (extra_nsecs +500u) / 1000u);
}

void 
PrintVisitor::printTimeStamp (ACE_hrtime_t creation,
                              ACE_hrtime_t ec_recv,
                              ACE_hrtime_t ec_send)
{
  ACE_OS::fprintf (output_,
                   "\nTimeStamp\ncreation time: ");
  this->printSecNanoSec (creation);
  ACE_OS::fprintf (output_,
                   "event channel received time: ");
  this->printSecNanoSec (ec_recv);
  ACE_OS::fprintf (output_,
                   "event_channel sent time: ");
  this->printSecNanoSec (ec_send);
}


// Visit a struct node
void
PrintVisitor::visitStructNode (StructNode *structNode) {
  
  // print the padding in front of the line
  printPadding (structNode->getRecursionLevel());
  ACE_OS::fprintf (output_, "struct %s {\n", structNode->getName ());
  
  for (unsigned int i = 0; i < structNode->getChildNumber (); i++) {
    printPadding (structNode->getChild (i)->getRecursionLevel());
    structNode->getChild (i)->Accept((NodeVisitor *)this);
    ACE_OS::fprintf (output_, "\n");  
  }
  
  printPadding (structNode->getRecursionLevel());
  ACE_OS::fprintf (output_, "}\n");  
}

void 
PrintVisitor::visitDoubleNode (DoubleNode *doubleNode) {
  printPadding (doubleNode->getRecursionLevel());  
  ACE_OS::fprintf (output_, "CORBA::double %s = %f;", doubleNode->getName(), doubleNode->getValue());
}

void
PrintVisitor::visitLongNode (LongNode *longNode) {
  printPadding (longNode->getRecursionLevel());  
  ACE_OS::fprintf (output_, "CORBA::Long %s = %d;", longNode->getName(), longNode->getValue());
}

void
PrintVisitor::visitULongNode (ULongNode *uLongNode) { 
  printPadding (uLongNode->getRecursionLevel());  
  ACE_OS::fprintf (output_, "CORBA::ULong %s = %d;", uLongNode->getName(), uLongNode->getValue());
}

void
PrintVisitor::visitStringNode (StringNode *stringNode) { 
  printPadding (stringNode->getRecursionLevel());  
  ACE_OS::fprintf (output_, "CORBA::String %s = \"%s\";", stringNode->getName(), (char *)stringNode->getValue());
}

void
PrintVisitor::printPadding (unsigned int recursion_level) {
  switch (recursion_level) {
  case 0: break;
  case 1: ACE_OS::fprintf (output_, "   ");
    break;
  case 2: ACE_OS::fprintf (output_, "      ");
    break;
  case 3: ACE_OS::fprintf (output_, "         ");
    break;
  case 4: ACE_OS::fprintf (output_, "            ");
    break;
  default: for (unsigned int i = 0; i < recursion_level; i++) 
    {
      ACE_OS::fprintf (output_, "   ");
    }
  break;
  }
}
  
