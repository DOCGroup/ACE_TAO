
//=============================================================================
/**
 *  @file    cdr_op_cs.h
 *
 *  The array's own CDR operators are generated in the .i file, but if the
 *  array contains an anonymous sequence, those CDR operators will not
 *  get generated on their own, and, since the sequence may contain a
 *  recursive type, they must also be generated in the .cpp file. That is
 *  the sole purpose of this visitor type.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef _BE_VISITOR_ARRAY_CDR_OP_CS_H_
#define _BE_VISITOR_ARRAY_CDR_OP_CS_H_

/**
 * @class be_visitor_array_cdr_op_cs
 *
 * @brief be_visitor_array_cdr_op_cs
 *
 * This is a concrete visitor for array that generates the CDR operator
 * implementations for the array's element type, if that type is
 * an anonymous sequence.
 */
class be_visitor_array_cdr_op_cs : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_array_cdr_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_array_cdr_op_cs ();

  /// visit array
  virtual int visit_array (be_array *node);

  // = Visitor methods on array types
  virtual int visit_enum (be_enum *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_interface_fwd (be_interface_fwd *node);
  virtual int visit_valuebox (be_valuebox *node);
  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  virtual int visit_predefined_type (be_predefined_type *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_map (be_map *node);
  virtual int visit_string (be_string *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_union (be_union *node);

protected:
  /// helper that does the common job
  int visit_node (be_type *);
};

#endif /* _BE_VISITOR_ARRAY_CDR_OP_CS_H_ */
