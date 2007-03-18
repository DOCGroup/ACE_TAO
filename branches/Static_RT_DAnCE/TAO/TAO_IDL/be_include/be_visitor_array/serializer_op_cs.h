//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    serializer_op_cs.h
//
// = DESCRIPTION
//    The array's own TAO::DCPS::Serializer operators are generated in 
//    the .i file, but if the
//    array contains an anonymous sequence, those 
//    TAO::DCPS::Serializer operators will not
//    get generated on their own, and, since the sequence may contain a
//    recursive type, they must also be generated in the .cpp file. That is
//    the sole purpose of this visitor type.
//
// = AUTHOR
//    Scott Harris <harris_s@ociweb.com> based on code by 
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef _BE_VISITOR_ARRAY_serializer_op_CS_H_
#define _BE_VISITOR_ARRAY_serializer_op_CS_H_

// ************************************************************
// class be_visitor_array_serializer_op_cs
// ************************************************************

class be_visitor_array_serializer_op_cs : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_array_serializer_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for array that generates the Serializer operator
  //   implementations for the array's element type, if that type is
  //   an anonymous sequence.
  //

public:
  be_visitor_array_serializer_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_array_serializer_op_cs (void);
  // destructor

  virtual int visit_array (be_array *node);
  // visit array

  // = Visitor methods on array types

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_interface (be_interface *node);
  // visit an interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface forward node

  virtual int visit_valuetype (be_valuetype *node);
  // visit a valuetype

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  // visit a valuetype forward node

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type node

  virtual int visit_sequence (be_sequence *node);
  // visit sequence

  virtual int visit_string (be_string *node);
  // visit string

  virtual int visit_structure (be_structure *node);
  // visit structure

  virtual int visit_typedef (be_typedef *node);
  // visit typedef

  virtual int visit_union (be_union *node);
  // visit union

protected:
  int visit_node (be_type *);
  // helper that does the common job
};

#endif /* _BE_VISITOR_ARRAY_serializer_op_CS_H_ */
