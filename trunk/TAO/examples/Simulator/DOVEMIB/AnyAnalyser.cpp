// $Id$
// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//    AnyAnalyser.cpp
//
// = AUTHOR
//    Michael Kircher
//
// = DESCRIPTION
//   Accepts an CORBA::Any, traverses it, copies it into a tree structure
//   and prints out the tree.
//
// ============================================================================

#include "AnyAnalyser.h"
#include "tao/Align.h"

ACE_RCSID(DOVEMIB, AnyAnalyser, "$Id$")

AnyAnalyser::AnyAnalyser (const char *file_name)
  : printVisitor_ptr_(new PrintVisitor (file_name)) {
}

AnyAnalyser::~AnyAnalyser () {
  delete this->printVisitor_ptr_;
}

void
AnyAnalyser::close () {
  this->printVisitor_ptr_->close();
}

void
AnyAnalyser::printTimeStamp (ACE_hrtime_t creation,
                             ACE_hrtime_t ec_recv,
                             ACE_hrtime_t ec_send)
{
  this->printVisitor_ptr_->printTimeStamp (creation,
                                          ec_recv,
                                          ec_send);
}


void
AnyAnalyser::printAny (CORBA::TypeCode_ptr any_type, const void *any_value) {

  // Analyse the any and store the results in a tree structure
  RecurseInfo recurseInfo_ = {PARENT_IS_NO_STRUCT,// identifies parent
                              0,                  // parent typecode
                              0,                  // member count
                              0};                 // recursion level

  // have a variable with can be incremented by all
  // recursive analyse calls
  const unsigned char *value_ptr_ = (const unsigned char *)any_value;

  // analyse the any
  Node *node_ptr_ = analyse (any_type,      // typecode information
                             value_ptr_,    // pointer to the memory
                             recurseInfo_); // recurse information

  // print the results
  node_ptr_->Accept ((NodeVisitor *)printVisitor_ptr_);
}

Node *
AnyAnalyser::analyse (CORBA::TypeCode_ptr tc_ptr,
                      const unsigned char *&value_ptr,
          RecurseInfo ri)
{
  CORBA::Long alignment, align_offset;
  CORBA::TypeCode_ptr param;
  const unsigned char *start_addr = value_ptr;

  TAO_TRY {
    Node *node_ptr_ = 0;

    if (tc_ptr != 0) {

      switch (tc_ptr->kind(TAO_TRY_ENV)) {

        case CORBA::tk_struct:
                {
            // to hold a pointer to the start of the struct
            start_addr = value_ptr;

            // create a new Node
                  StructNode *structNode_ptr_ = new StructNode (tc_ptr->name (TAO_TRY_ENV),
                                                                                   ri.recursion_level);

            for (unsigned int i = 0; i < tc_ptr->member_count (TAO_TRY_ENV); i++) {

                    // get the TypeCode pointer to the ith parameter
                    // and analyse it recursively
              RecurseInfo recurseInfo_ = {PARENT_IS_STRUCT,   // identifies parent
                                          tc_ptr,             // parent typecode
                                          i,                  // member count
                                          ri.recursion_level+1};


              // get the type code of the child i
              param = tc_ptr->member_type (i, TAO_TRY_ENV);
              TAO_CHECK_ENV;

              // get the size
              /* size = */ param->size (TAO_TRY_ENV);
              TAO_CHECK_ENV;

              // get the alignment
              alignment = param->alignment (TAO_TRY_ENV);
              TAO_CHECK_ENV;

              // calculate
              align_offset =
                      (ptr_arith_t) ptr_align_binary (value_ptr, alignment)
                      - (ptr_arith_t) value_ptr
                      + (ptr_arith_t) ptr_align_binary (start_addr, alignment)
                      - (ptr_arith_t) start_addr;
              TAO_CHECK_ENV;

              // if both the start_addr and data are not aligned as per
              // the alignment, we do not add the offset
              value_ptr = (unsigned char *) ((ptr_arith_t) value_ptr +
                                           ((align_offset == alignment) ?
                                            0 : align_offset));

                    structNode_ptr_->addChild (analyse (param,
                                                  value_ptr,
                                                                                      recurseInfo_));
            }
                  node_ptr_ = (Node *)structNode_ptr_;
          }
                break;

        case CORBA::tk_double:
          if (ri.kind == PARENT_IS_STRUCT) {
                  node_ptr_ = (Node *) new DoubleNode ((CORBA::Double *)value_ptr,
                                                                               ri.parent_tc_ptr->member_name(ri.member_number,
                                                                               TAO_TRY_ENV),
                                                                               ri.recursion_level);
          }
          else {
                  node_ptr_ = (Node *) new DoubleNode ((CORBA::Double *)value_ptr,
                                                                               tc_ptr->name(TAO_TRY_ENV),
                                                                               ri.recursion_level);
          }
                value_ptr += 8;
                break;

              case CORBA::tk_long:
          if (ri.kind == PARENT_IS_STRUCT) {
                node_ptr_ = (Node *) new LongNode ((CORBA::Long *)value_ptr,
                                                                               ri.parent_tc_ptr->member_name(ri.member_number,
                                                                               TAO_TRY_ENV),
                                                                               ri.recursion_level);
          }
          else {
                  node_ptr_ = (Node *) new LongNode ((CORBA::Long *)value_ptr,
                                                                           tc_ptr->name(TAO_TRY_ENV),
                                                                           ri.recursion_level);
          }
                value_ptr += 4;
                break;

              case CORBA::tk_ulong:
          if (ri.kind == PARENT_IS_STRUCT) {
                node_ptr_ = (Node *) new ULongNode ((CORBA::ULong *)value_ptr,
                                                                               ri.parent_tc_ptr->member_name(ri.member_number,
                                                                               TAO_TRY_ENV),
                                                                               ri.recursion_level);
          }
          else {
                  node_ptr_ = (Node *) new ULongNode ((CORBA::ULong *)value_ptr,
                                                                              tc_ptr->name(TAO_TRY_ENV),
                                                                              ri.recursion_level);
          }
                value_ptr += 4;
                break;

              case CORBA::tk_string:
          if (ri.kind == PARENT_IS_STRUCT) {
                node_ptr_ = (Node *) new StringNode (*(CORBA::String_var *)value_ptr,
                                                                               ri.parent_tc_ptr->member_name(ri.member_number,
                                                                               TAO_TRY_ENV),
                                                                               ri.recursion_level);
          }
          else {
                node_ptr_ = (Node *) new StringNode (*(CORBA::String_var *)value_ptr,
                                                                               tc_ptr->name(TAO_TRY_ENV),
                                                                               ri.recursion_level);
          }
                  value_ptr += 4;
                break;

              default: ACE_ERROR ((LM_ERROR, "AnyAnalyser::analyse: No known kind of type detected!\n"));
                exit (1);
                break;
      }
      TAO_CHECK_ENV;
            return node_ptr_;
    }
    else {
            ACE_DEBUG ((LM_ERROR, "AnyAnalyser::analyse: TypeCode pointer to member was Null!\n"));
    }
  }
  TAO_CATCHANY {
    ACE_ERROR ((LM_ERROR, "(%t)AnyAnalyser::analyse: Error in analysing the any.\n"));
  }
  TAO_ENDTRY;
  return 0;
}


