/* -*- c++ -*- */
//
// $Id$
//

#if !defined (TAO_BE_VISITOR_SEQUENCE_H)
#define TAO_BE_VISITOR_SEQUENCE_H

#include "be_visitor.h"

class be_visitor_sequence_ch : public be_visitor
{
  //
  // = TITLE
  //    be_visitor_sequence_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for sequences
  //
public:
  be_visitor_sequence_ch (void);
  // constructor

  ~be_visitor_sequence_ch (void);
  // destructor

  virtual int visit_sequence (be_sequence *node);
  // visit sequence node
};

typedef ACE_Singleton <be_visitor_sequence_ch, ACE_SYNCH_RECURSIVE_MUTEX>
TAO_BE_VISITOR_SEQUENCE_CH;

class be_visitor_sequence_base_ch : public be_visitor
{
  //
  // = TITLE
  //    be_visitor_sequence_base_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for sequence
  //   base
  //
public:
  be_visitor_sequence_base_ch (void);
  // constructor

  ~be_visitor_sequence_base_ch (void);
  // destructor

  virtual int visit_predefined_type (be_predefined_type *node);
  // base type is predefined type

  virtual int visit_interface (be_interface *node);
  // base type is object reference

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // base type is object reference

  virtual int visit_structure (be_structure *node);
  // base type is a structure

  virtual int visit_enum (be_enum *node);
  // base type is an enum

  virtual int visit_union (be_union *node);
  // base type is a union

  virtual int visit_array (be_array *node);
  // base type is an array

  virtual int visit_sequence (be_sequence *node);
  // base type is a sequence

  virtual int visit_typedef (be_typedef *node);
  // base type is a typedef to something

protected:
  be_decl *seq_scope (void);
  // scope node of the saved sequence node

  int visit_node (be_type *);
  // helper that does the common job
};

typedef ACE_Singleton <be_visitor_sequence_base_ch, ACE_SYNCH_RECURSIVE_MUTEX>
TAO_BE_VISITOR_SEQUENCE_BASE_CH;

class be_visitor_sequence_elemtype : public be_visitor
{
  //
  // = TITLE
  //    be_visitor_sequence_elemtype
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the return type for the [] operators
  //
  // = NOTES
  //    This class may eventually go away when the _var and _out classes are
  //    generated using templates
public:
  be_visitor_sequence_elemtype (void);
  // constructor

  ~be_visitor_sequence_elemtype (void);
  // destructor

  virtual int visit_predefined_type (be_predefined_type *node);
  // base type is predefined type

  virtual int visit_interface (be_interface *node);
  // base type is object reference

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // base type is object reference

  virtual int visit_structure (be_structure *node);
  // base type is a structure

  virtual int visit_enum (be_enum *node);
  // base type is an enum

  virtual int visit_union (be_union *node);
  // base type is a union

  virtual int visit_array (be_array *node);
  // base type is an array

  virtual int visit_sequence (be_sequence *node);
  // base type is a sequence

  virtual int visit_typedef (be_typedef *node);
  // base type is a typedef to something

  virtual int visit_string (be_string *node);
  // base type is a string

protected:
  be_decl *seq_scope (void);
  // scope node of the saved sequence node

  int visit_node (be_type *);
  // helper that does the common job
};

typedef ACE_Singleton <be_visitor_sequence_elemtype, ACE_SYNCH_RECURSIVE_MUTEX>
TAO_BE_VISITOR_SEQUENCE_ELEMTYPE;


#endif /* TAO_BE_VISITOR_SEQUENCE_H */
