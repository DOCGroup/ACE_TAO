//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_ss.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations in server skeletons
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_OPERATION_SS_H_
#define _BE_VISITOR_OPERATION_OPERATION_SS_H_

// ************************************************************
// Operation visitor for server skeletons
// ************************************************************

class be_visitor_operation_ss : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operation_ss
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server skeletons for operation
  //
  //
public:
  be_visitor_operation_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_ss (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation. We provide code for this method in the derived class

  virtual int visit_argument (be_argument *node);
  // visit argument to generate ParamData entries

  // template methods

  virtual int gen_pre_skel_info (be_operation *node);
  // generate any pre skeleton code info

  virtual int gen_demarshal_params (be_operation *node,
                                    be_type *bt);
  // generate code for demarshaling incoming parameters

  virtual int gen_marshal_params (be_operation *node,
                                  be_type *bt);
  // generate code for marshaling outgoing parameters

  // = helper
  virtual int post_process (be_decl *);
  // stuff to output after every member of the scope is handled

  virtual const char *compute_operation_name (be_operation *node);
  // Compute the servant operation name to invoke.  Notice that
  // this method _does_ include the double-quotes.

private:
  char *operation_name_;
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_SS_H_ */
