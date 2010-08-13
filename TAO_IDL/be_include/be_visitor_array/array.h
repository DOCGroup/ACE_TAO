
//=============================================================================
/**
 *  @file    array.h
 *
 *  $Id$
 *
 *  Visitors for generation of code for Arrays.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ARRAY_ARRAY_H_
#define _BE_VISITOR_ARRAY_ARRAY_H_

// ************************************************************
// class be_visitor_array
// ************************************************************

/**
 * @class be_visitor_array
 *
 * @brief be_visitor_array
 *
 * This is a generic visitor for arrays
 */
class be_visitor_array : public be_visitor_decl
{
public:
  /// Constructor
  be_visitor_array (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_array (void);

  /// visit the array node
  virtual int visit_array (be_array *);

  // = Visitor methods on array types

  /// visit an enum
  virtual int visit_enum (be_enum *node);

  /// visit an interface
  virtual int visit_interface (be_interface *node);

  /// visit an interface forward node
  virtual int visit_interface_fwd (be_interface_fwd *node);

  /// visit a valuebox
  virtual int visit_valuebox (be_valuebox *node);

  /// visit a valuetype
  virtual int visit_valuetype (be_valuetype *node);

  /// visit a valuetype forward node
  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);

  /// visit a predefined type node
  virtual int visit_predefined_type (be_predefined_type *node);

  /// visit sequence
  virtual int visit_sequence (be_sequence *node);

  /// visit string
  virtual int visit_string (be_string *node);

  /// visit structure
  virtual int visit_structure (be_structure *node);

  /// visit typedef
  virtual int visit_typedef (be_typedef *node);

  /// visit union
  virtual int visit_union (be_union *node);

protected:

  /// helper that does the common job
  int visit_node (be_type *);

  /// helper that does the common job
  int emit_common (be_type *node);

};

#endif /* _BE_VISITOR_ARRAY_ARRAY_H_*/
