/* -*- c++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    typecode_defn.h
//
// = DESCRIPTION
//    Concrete visitor for the generation of TypeCodes.
//    This one generates the definitions.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_TYPECODE_TYPECODE_DEFN_H_
#define _BE_VISITOR_TYPECODE_TYPECODE_DEFN_H_

const int TAO_BE_VISITOR_TYPECODE_DEFN_MAX_STACK_SIZE = 1024;

class be_visitor_typecode_defn : public be_visitor_scope
{
  //
  // = TITLE
  //    be_visitor_typecode_defn
  //
  // = DESCRIPTION
  //    This is a concrete visitor for generating TypeCode definitions
  //

public:
  be_visitor_typecode_defn (be_visitor_context *ctx);
  // constructor

  ~be_visitor_typecode_defn (void);
  // destructor

  virtual int visit_type (be_type *node);
  // visit a generic be_type node

  // =visit methods on all types. They call the generic visit_type

  virtual int visit_array (be_array *node);
  // visit a array

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_exception (be_exception *node);
  // visit an enum

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined types

  virtual int visit_sequence (be_sequence *node);
  // visit a sequence

  virtual int visit_string (be_string *node);
  // visit a structure

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_typedef (be_typedef *node);
  // visit a typedef

  virtual int visit_union (be_union *node);
  // visit a union

  // = visit methods for the scope elements

  virtual int visit_enum_val (be_enum_val *node);
  // visit the enumeration values

  virtual int visit_field (be_field *node);
  // visit the field (struct and exception member)

  virtual int visit_union_branch (be_union_branch *node);
  // visit the union member

  // = special methods

  virtual int gen_nested_namespace_begin (be_module *node);
  // generate the nested namespace begins

  virtual int gen_nested_namespace_end (be_module *node);
  // generate the nested namespace ends

  // = methods for generating typecodes

  int gen_typecode (be_array *node);

  int gen_typecode (be_enum *node);

  int gen_typecode (be_exception *node);

  int gen_typecode (be_interface *node);

  int gen_typecode (be_interface_fwd *node);

  int gen_typecode (be_predefined_type *node);

  int gen_typecode (be_sequence *node);

  int gen_typecode (be_string *node);

  int gen_typecode (be_structure *node);

  int gen_typecode (be_typedef *node);

  int gen_typecode (be_union *node);

  // = methods to generate encapsulations for typecodes

  int gen_encapsulation (be_array *node);

  int gen_encapsulation (be_enum *node);

  int gen_encapsulation (be_enum_val *node);

  int gen_encapsulation (be_exception *node);

  int gen_encapsulation (be_field *node);

  int gen_encapsulation (be_interface *node);

  int gen_encapsulation (be_interface_fwd *node);

  int gen_encapsulation (be_predefined_type *node);

  int gen_encapsulation (be_sequence *node);

  int gen_encapsulation (be_string *node);

  int gen_encapsulation (be_structure *node);

  int gen_encapsulation (be_typedef *node);

  int gen_encapsulation (be_union *node);

  int gen_encapsulation (be_union_branch *node);

  // = methods for computing typecode sizes

  ACE_CDR::Long compute_tc_size (be_array *node);

  ACE_CDR::Long compute_tc_size (be_enum *node);

  ACE_CDR::Long compute_tc_size (be_exception *node);

  ACE_CDR::Long compute_tc_size (be_interface *node);

  ACE_CDR::Long compute_tc_size (be_interface_fwd *node);

  ACE_CDR::Long compute_tc_size (be_predefined_type *node);

  ACE_CDR::Long compute_tc_size (be_sequence *node);

  ACE_CDR::Long compute_tc_size (be_string *node);

  ACE_CDR::Long compute_tc_size (be_structure *node);

  ACE_CDR::Long compute_tc_size (be_typedef *node);

  ACE_CDR::Long compute_tc_size (be_union *node);

  // methods for computing the encapsulation length

  ACE_CDR::Long compute_encap_length (be_array *node);

  ACE_CDR::Long compute_encap_length (be_enum *node);

  ACE_CDR::Long compute_encap_length (be_enum_val *node);

  ACE_CDR::Long compute_encap_length (be_exception *node);

  ACE_CDR::Long compute_encap_length (be_field *node);

  ACE_CDR::Long compute_encap_length (be_interface *node);

  ACE_CDR::Long compute_encap_length (be_interface_fwd *node);

  ACE_CDR::Long compute_encap_length (be_predefined_type *node);

  ACE_CDR::Long compute_encap_length (be_sequence *node);

  ACE_CDR::Long compute_encap_length (be_string *node);

  ACE_CDR::Long compute_encap_length (be_structure *node);

  ACE_CDR::Long compute_encap_length (be_typedef *node);

  ACE_CDR::Long compute_encap_length (be_union *node);

  ACE_CDR::Long compute_encap_length (be_union_branch *node);

  // helpers to generate the repoID and name

  void gen_repoID (be_decl *node);

  void gen_name (be_decl *node);

  // helpers to generate encap len of the repoID and name

  ACE_CDR::ULong repoID_encap_len (be_decl *node);

  ACE_CDR::ULong name_encap_len (be_decl *node);

  // helper to get an array of long from a name

  int tc_name2long (const char *name, 
                    ACE_CDR::ULong *&larr, 
                    ACE_CDR::ULong &arrlen);

  // processing for scopes

  virtual int post_process (be_decl *);
  // do any processing after every element except the last one of the scope is
  // processed

  // data structure for handling recursive and repeated typecodes

  struct QNode
  {
    be_type *node;
    ACE_CDR::Long offset;
  };

private:
  
  ACE_CDR::Long computed_tc_size_;
  // the tc size of the node under consideration

  ACE_CDR::Long computed_encap_len_;
  // the encap length of the node under consideration

  ACE_CDR::Long computed_scope_encap_len_;
  // the encap length of the scope of the node under consideration

  // the following are used for recursive and repeated typecodes 

  ACE_CDR::Long tc_offset_;
  // current computed length of the typecode

  ACE_Unbounded_Queue <QNode*> tc_queue_;
  // queue to keep nodes

  ACE_Unbounded_Queue <QNode*> compute_queue_;
  // queue to keep nodes

  ACE_CDR::Long scope_stack_ [TAO_BE_VISITOR_TYPECODE_DEFN_MAX_STACK_SIZE];
  // stores scope lens during computation

  // scope related routines

  int index_;

  int push (ACE_CDR::Long);

  int pop (ACE_CDR::Long &);

  // queue related routines

  const QNode *queue_insert (ACE_Unbounded_Queue <QNode*> &, 
                             be_type *node, ACE_CDR::Long offset);

  const QNode *queue_lookup (ACE_Unbounded_Queue <QNode*> &, be_type *node);

  void queue_reset (ACE_Unbounded_Queue <QNode*> &);

};

#endif /* _BE_VISITOR_TYPECODE_TYPECODE_DEFN_H_ */
