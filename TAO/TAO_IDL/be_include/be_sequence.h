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

/*
 * BE_Sequence
 */

class be_typedef;

// A sequence in OMG IDL does not define a scoping construct just as a struct
// or union or an interface do. However, in the C++ mapping, a sequence becomes
// a class. If the base type of a sequence is another anonymous sequence, then
// the base type is defined in the scope of this sequence. Hence we define
// be_sequence to possess the additional characteristics of a scope
class be_sequence : public virtual AST_Sequence,
                    public virtual be_scope,
                    public virtual be_type
{
public:
  enum MANAGED_TYPE
  {
    MNG_UNKNOWN ,
    MNG_NONE,
    MNG_STRING,
    MNG_OBJREF,
    MNG_PSEUDO
  };

  // =Operations

  be_sequence (void);
  // default constructor

  be_sequence (AST_Expression *v, AST_Type *bt);
  // constructor

  virtual int create_name (be_typedef *node=0);
  // create a name for ourselves. If we are typedefed, then we get the name of
  // the typedef node, else we generate a name for ourselves

  virtual int gen_typecode (void);
  // generate the typecode

  virtual int gen_encapsulation (void);
  // encapsulation for parameters

  virtual long tc_size (void);
  // return typecode size

  virtual long tc_encap_len (void);
  // return length of encapsulation

  virtual MANAGED_TYPE managed_type (void);
  // return the managed type

  virtual idl_bool unbounded (void) const;
  // is this sequence bounded or not

  // =Scope management functions
  virtual AST_Sequence *fe_add_sequence (AST_Sequence *);

  virtual be_decl *decl (void);
  // overridden method on the be_scope class

  // Visiting
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_METHODS3 (be_sequence, AST_Sequence, be_scope, be_type);
  DEF_NARROW_FROM_DECL (be_sequence);

  const char *instance_name ();
  // report the instance name for instantiation

  const char *object_manager_name ();
  // report the object manager name for the sequence of objects

protected:
  virtual char *gen_name (void);
  // helper to create_name

private:
  idl_bool unbounded_;
  // whether we are bounded or unbounded

  MANAGED_TYPE mt_;
  // our managed type
};

#endif
