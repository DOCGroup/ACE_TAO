// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_sequence.h
//
// = DESCRIPTION
//    Extension of class AST_Sequence that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef BE_SEQUENCE_H
#define BE_SEQUENCE_H

#include "be_scope.h"
#include "be_type.h"
#include "ast_sequence.h"

class AST_Expression;
class AST_Type;
class be_visitor;
class be_typedef;
class be_field;

// A sequence in OMG IDL does not define a scoping construct just as a struct
// or union or an interface do. However, in the C++ mapping, a sequence becomes
// a class. If the base type of a sequence is another anonymous sequence, then
// the base type is defined in the scope of this sequence. Hence we define
// be_sequence to possess the additional characteristics of a scope.
class be_sequence : public virtual AST_Sequence,
                    public virtual be_scope,
                    public virtual be_type
{
public:
  enum MANAGED_TYPE
  {
    MNG_UNKNOWN,
    MNG_NONE,
    MNG_STRING,
    MNG_WSTRING,
    MNG_OBJREF,
    MNG_ABSTRACT,
    MNG_VALUE,
    MNG_PSEUDO
  };

  be_sequence (void);
  // Default constructor.

  be_sequence (AST_Expression *v,
               AST_Type *bt,
               UTL_ScopedName *n,
               idl_bool local,
               idl_bool abstract);
  // Constructor.

  virtual int create_name (be_typedef *node = 0);
  // Create a name for ourselves. If we are typedefed, then we get the name of
  // the typedef node, else we generate a name for ourselves.

  virtual MANAGED_TYPE managed_type (void);
  // Return the managed type.

  // Scope management functions.
  virtual AST_Sequence *fe_add_sequence (AST_Sequence *);

  virtual be_decl *decl (void);
  // Overridden method on the be_scope class.

  virtual void destroy (void);
  // Cleanup method.

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing.
  DEF_NARROW_METHODS3 (be_sequence, AST_Sequence, be_scope, be_type);
  DEF_NARROW_FROM_DECL (be_sequence);

  const char *instance_name (void);
  // Report the instance name for instantiation.

  int gen_base_class_name (TAO_OutStream *os,
                           AST_Decl *elem_scope);
  // Common code for generating the name and parameters of our
  // template sequence base class.

  be_field *field_node (void) const;
  void field_node (be_field *node);
  // Accessors for the member.

protected:
  virtual char *gen_name (void);
  // Helper to create_name.

private:
  const char *smart_fwd_helper_name (AST_Decl *elem_scope,
                                     be_type *elem);

private:
  MANAGED_TYPE mt_;
  // Our managed type.

  be_field *field_node_;
  // Used if we are an anonymous member, to help generate a unique name.
};

#endif
