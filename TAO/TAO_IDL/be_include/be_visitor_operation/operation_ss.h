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

  virtual int gen_pre_skel_info (be_operation *, be_type *) = 0;
  // generate any pre skeleton code info

  virtual int gen_demarshal_params (be_operation *, be_type *) = 0;
  // generate code for demarshaling incoming parameters

  virtual int gen_marshal_params (be_operation *, be_type *) = 0;
  // generate code for marshaling outgoing parameters

  // = helper
  virtual int post_process (be_decl *);
  // stuff to output after every member of the scope is handled

  virtual int gen_raise_exception (be_type *,
                                   const char * excep,
                                   const char * status,
                                   const char * env);
  // helper that generates code for raising an exception

  virtual int gen_check_exception (be_type *, const char *env);
  // helper that generates code for checking for an exception

};

// concrete visitors

class be_interpretive_visitor_operation_ss : public be_visitor_operation_ss
{
  //
  // = TITLE
  //   be_interpretive_visitor_operation_ss
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server skeletons for
  //   operation using interpretive marshaling
  //
  //
public:
  be_interpretive_visitor_operation_ss (be_visitor_context *ctx);
  // constructor

  ~be_interpretive_visitor_operation_ss (void);
  // destructor

  // template methods

  virtual int gen_pre_skel_info (be_operation *, be_type *);
  // generate any pre skeleton code info

  virtual int gen_demarshal_params (be_operation *, be_type *);
  // generate code for demarshaling incoming parameters

  virtual int gen_marshal_params (be_operation *, be_type *);
  // generate code for marshaling outgoing parameters

};

class be_compiled_visitor_operation_ss : public be_visitor_operation_ss
{
  //
  // = TITLE
  //   be_compiled_visitor_operation_ss
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server skeletons for
  //   operation using compiled marshaling
  //
  //
public:
  be_compiled_visitor_operation_ss (be_visitor_context *ctx);
  // constructor

  ~be_compiled_visitor_operation_ss (void);
  // destructor

  // template methods

  virtual int gen_pre_skel_info (be_operation *, be_type *);
  // generate any pre skeleton code info

  virtual int gen_demarshal_params (be_operation *, be_type *);
  // generate code for demarshaling incoming parameters

  virtual int gen_marshal_params (be_operation *, be_type *);
  // generate code for marshaling outgoing parameters

};

#endif /* _BE_VISITOR_OPERATION_OPERATION_SS_H_ */
