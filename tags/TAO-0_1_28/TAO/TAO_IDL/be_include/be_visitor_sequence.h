/* -*- c++ -*- */
//
// $Id$
//

#if !defined (TAO_BE_VISITOR_SEQUENCE_H)
#define TAO_BE_VISITOR_SEQUENCE_H

#include "be_visitor_decl.h"

class be_visitor_sequence_ch : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_sequence_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for
  //   sequences
  //
public:
  be_visitor_sequence_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_sequence_ch (void);
  // destructor

  virtual int visit_sequence (be_sequence *node);
  // visit sequence node

  virtual int gen_base_sequence_class (be_sequence *node);
  // generate the base sequence class

  virtual int gen_var_defn (be_sequence *node);
  // generate the var defn

  virtual int gen_out_defn (be_sequence *node);
  // generate the out defn
};

class be_visitor_sequence_ci : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_sequence_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client inline for
  //   sequences. The only purpose is to generate the implementation for the
  //   _var and _out classes
  //
public:
  be_visitor_sequence_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_sequence_ci (void);
  // destructor

  virtual int visit_sequence (be_sequence *node);
  // visit sequence node

  virtual int gen_var_impl (be_sequence *node);
  // generate the var impl

  virtual int gen_out_impl (be_sequence *node);
  // generate the out impl
};

class be_visitor_sequence_cs : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_sequence_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for
  //   sequences
  //
public:
  be_visitor_sequence_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_sequence_cs (void);
  // destructor

  virtual int visit_sequence (be_sequence *node);
  // visit sequence node

  virtual int gen_base_sequence_class (be_sequence *node);
  // generate the base sequence class

};

// =helper visitors to generate the base type and element type

class be_visitor_sequence_base : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_sequence_base
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the template parameters
  //   for a TAO_*_Sequence instantiation.
  //
  // = BUGS
  //   The class name is misleading, eventually this class could be
  //   used to generate code in other files, not only the client
  //   header.
  //   The visitor factory should provide a factory method that builds
  //   this class on the fly.
  //
public:
  be_visitor_sequence_base (be_visitor_context *ctx);
  // Constructor

  ~be_visitor_sequence_base (void);
  // destructor

  // = Visitor methods.
  virtual int visit_predefined_type (be_predefined_type *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_interface_fwd (be_interface_fwd *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_exception (be_exception *node);
  virtual int visit_union (be_union *node);
  virtual int visit_array (be_array *node);
  virtual int visit_string (be_string *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_typedef (be_typedef *node);

protected:
  int visit_node (be_type *);
  // helper that does the common job

};

class be_visitor_sequence_buffer_type : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_sequence_buffer_type
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the buffer type
  //   for a TAO_*_Sequence instantiation.
  //
public:
  be_visitor_sequence_buffer_type (be_visitor_context *ctx);
  // Constructor

  ~be_visitor_sequence_buffer_type (void);
  // destructor

  // = Visitor methods.
  virtual int visit_predefined_type (be_predefined_type *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_interface_fwd (be_interface_fwd *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_exception (be_exception *node);
  virtual int visit_union (be_union *node);
  virtual int visit_array (be_array *node);
  virtual int visit_string (be_string *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_typedef (be_typedef *node);

protected:
  int visit_node (be_type *);
  // helper that does the common job

};

class be_visitor_sequence_elemtype : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_sequence_elemtype
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the return type for the
  //   [] operators
  //
  // = NOTES
  //    This class may eventually go away when the _var and _out classes are
  //    generated using templates
public:
  be_visitor_sequence_elemtype (be_visitor_context *ctx);
  // Constructor

  ~be_visitor_sequence_elemtype (void);
  // destructor

  // = Visitors
  virtual int visit_predefined_type (be_predefined_type *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_interface_fwd (be_interface_fwd *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_exception (be_exception *node);
  virtual int visit_union (be_union *node);
  virtual int visit_array (be_array *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_string (be_string *node);

protected:
  int visit_node (be_type *);
  // helper that does the common job

};

class be_visitor_sequence_any_op_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_sequence_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for sequence that generates the Any operator
  //   declarations
  //

public:
  be_visitor_sequence_any_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_sequence_any_op_ch (void);
  // destructor

  virtual int visit_sequence (be_sequence *node);
  // visit sequence
};

class be_visitor_sequence_any_op_cs : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_sequence_any_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for sequence that generates the Any operator
  //   implementations
  //

public:
  be_visitor_sequence_any_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_sequence_any_op_cs (void);
  // destructor

  virtual int visit_sequence (be_sequence *node);
  // visit sequence
};

#endif /* TAO_BE_VISITOR_SEQUENCE_H */
