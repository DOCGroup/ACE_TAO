// $Id$
// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//    AnyAnalyser.h
//
// = AUTHOR
//    Michael Kircher
//
// = DESCRIPTION
//   Accepts an CORBA::Any, traverses it, copies it into a tree structure
//   and prints out the tree.
//
// ============================================================================


#include "PrintVisitor.h"
#include "tao/corba.h"

#ifndef ANYANALYSER_H
#define ANYANALYSER_H

typedef enum {PARENT_IS_STRUCT, PARENT_IS_NO_STRUCT} KIND;

typedef struct {
          KIND kind;
          CORBA::TypeCode_ptr parent_tc_ptr;
          unsigned int member_number;
          unsigned int recursion_level;
} RecurseInfo;


class AnyAnalyser {

public:
  AnyAnalyser (const char *filename);

  ~AnyAnalyser ();

  // cause the PrintViewer to be deleted to close the file
  void close ();

  // main starting point includes analysing and printing
  void printAny (CORBA::TypeCode_ptr any_type, const void *any_value);

  void printTimeStamp (ACE_hrtime_t creation,
                       ACE_hrtime_t ec_recv,
                       ACE_hrtime_t ec_send);
  // Print the time stamp, which means forward this
  // call to the PrintVisitor

private:
  Node *analyse (CORBA::TypeCode_ptr tc_ptr,
                             const unsigned char *&value_ptr,
                 RecurseInfo recurseInfo);

  PrintVisitor *printVisitor_ptr_;
};

#endif

