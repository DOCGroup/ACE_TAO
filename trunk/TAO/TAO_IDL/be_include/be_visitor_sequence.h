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
  //   This is a concrete visitor to generate the client header for
  //   sequences
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
  be_visitor_sequence_base_ch (TAO_OutStream *stream,
			       be_decl *sequence_scope,
			       be_type *base_type);
  // Constructor, set the stream to write to, the scope where the
  // sequence is defined and the base type for the sequence.

  ~be_visitor_sequence_base_ch (void);
  // destructor

  // = Visitor methods.
  virtual int visit_predefined_type (be_predefined_type *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_interface_fwd (be_interface_fwd *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_union (be_union *node);
  virtual int visit_array (be_array *node);
  virtual int visit_string (be_string *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_typedef (be_typedef *node);

protected:
  be_decl *seq_scope (void);
  // scope node of the saved sequence node

  int visit_node (be_type *);
  // helper that does the common job

private:
  be_type* current_type_;
  // The scoped name for the current argument type name.
  // We cannot use just node->name() because we could be using an
  // aliased name (through typedefs) in which case the real name must
  // be used.
};

class be_visitor_sequence_elemtype : public be_visitor
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
  be_visitor_sequence_elemtype (TAO_OutStream *stream,
				be_decl *sequence_scope,
				be_type *base_type);
  // Constructor, set the stream to write to, the scope where the
  // sequence is defined and the base type for the sequence.

  ~be_visitor_sequence_elemtype (void);
  // destructor

  // = Visitors
  virtual int visit_predefined_type (be_predefined_type *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_interface_fwd (be_interface_fwd *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_union (be_union *node);
  virtual int visit_array (be_array *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_string (be_string *node);

protected:
  be_decl *seq_scope (void);
  // scope node of the saved sequence node

  int visit_node (be_type *);
  // helper that does the common job

  be_type* current_type_;
  // The scoped name for the current argument type name.
  // We cannot use just node->name() because we could be using an
  // aliased name (through typedefs) in which case the real name must
  // be used.
};

#endif /* TAO_BE_VISITOR_SEQUENCE_H */
