//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    post_marshal_ss.h
//
// = DESCRIPTION
//    Visitors for generation of code for Arguments. This generates the
//    code that does post-marshal processing (cleaning up) for arguments
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (_BE_VISITOR_ARGUMENT_POST_MARSHAL_SS_H_)
#define _BE_VISITOR_ARGUMENT_POST_MARSHAL_SS_H_

// ************************************************************
// class be_visitor_args_post_marshal_ss
// ************************************************************
class be_visitor_args_post_marshal_ss : public be_visitor_args
{
  //
  // = TITLE
  //   be_visitor_args_post_marshal_ss
  //
  // = DESCRIPTION
  //   Visitor for post processing after marshal
  //
public:
  be_visitor_args_post_marshal_ss (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_post_marshal_ss (void);
  // destructor

  virtual int visit_argument (be_argument *node);
  // visit the argument node

  // =visit all the nodes that can be the types for the argument

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit the enum node

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_string (be_string *node);
  // visit string type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_union (be_union *node);
  // visit union type

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef type

};

#endif /* _BE_VISITOR_ARGUMENT_POST_MARSHAL_SS_H */
