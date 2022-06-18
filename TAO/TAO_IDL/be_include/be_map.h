// -*- C++ -*-


//=============================================================================
/**
 *  @file    be_map.h
 *
 *  Extension of class AST_Map that provides additional means for C++
 *  mapping.
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#ifndef BE_MAP_H
#define BE_MAP_H

#include "be_scope.h"
#include "be_type.h"
#include "ast_map.h"

class AST_Expression;
class AST_Type;
class be_visitor;
class be_typedef;
class be_field;

// A map in OMG IDL does not define a scoping construct just as a struct
// or union or an interface do. However, in the C++ mapping, a map becomes
// a class. If the base type of a map is another anonymous map, then
// the base type is defined in the scope of this map. Hence we define
// be_map to possess the additional characteristics of a scope.
class be_map : public virtual AST_Map,
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
    MNG_VALUE,
    MNG_PSEUDO
  };

  be_map (AST_Expression *v,
          AST_Type *kt,
          AST_Type *vt,
          UTL_ScopedName *n,
          bool local,
          bool abstract);

  /// Non-virtual override of frontend method.
  be_type *key_type () const;
  be_type *value_type () const;

  /**
   * Returns the fully dealiased key type if it's a typedef. If it's not a
   * typedef, the it returns the same value as as key_type().
   */
  be_type *primitive_key_type () const;

  /**
   * Returns the fully dealiased key type if it's a typedef. If it's not a
   * typedef, the it returns the same value as as value_type().
   */
  be_type *primitive_value_type () const;

  /// Create a name for ourselves. If we are typedefed, then we get the name of
  /// the typedef node, else we generate a name for ourselves.
  virtual int create_name (be_typedef *node);

  /// Return the managed type.
  virtual MANAGED_TYPE managed_type ();

  // Scope management functions.
  virtual AST_Map *fe_add_map (AST_Map *);

  /// Overridden method on the be_scope class.
  virtual be_decl *decl ();

  /// Overridden from class be_type.
  virtual void gen_ostream_operator (TAO_OutStream *os,
                                     bool use_underscore);

  /// Cleanup method.
  virtual void destroy ();

  // Visiting.
  virtual int accept (be_visitor *visitor);

  /// Report the instance name for instantiation.
  const char *instance_name ();
  
  /// Accessors for the member.
  be_field *field_node () const;
  void field_node (be_field *node);

  /// Helper to create_name, also used by the traits visitor.
  virtual char *gen_name ();

protected:

  /// Computes the fully scoped typecode name.
  virtual void compute_tc_name ();

private:
  const char *smart_fwd_helper_name (AST_Decl *elem_scope,
                                     be_type *elem);

private:
  /// Our managed type.
  MANAGED_TYPE mt_;

  /// Used if we are an anonymous member, to help generate a unique name.
  be_field *field_node_;
};

#endif
