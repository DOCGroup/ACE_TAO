// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef _AST_STRUCTURE_FWD_AST_STRUCTURE_FWD_HH
#define _AST_STRUCTURE_FWD_AST_STRUCTURE_FWD_HH

#include "ast_type.h"
class AST_Structure;

// Representation of a forward structure declaration.

class TAO_IDL_FE_Export AST_StructureFwd : public virtual AST_Type
{
public:
  AST_StructureFwd (AST_Structure *full_defn,
                    UTL_ScopedName *n);

  virtual ~AST_StructureFwd (void);

  AST_Structure *full_definition (void);
  void set_full_definition (AST_Structure *nfd);

  virtual bool is_defined (void);
  void set_as_defined (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL(AST_StructureFwd);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  // Is this decl a forward declared type (Yes)
  virtual bool is_fwd (void);

  // We don't actually want the forward declaration,
  // but want to return the full definition member,
  // whether defined yet or not.
  virtual AST_Decl *adjust_found (bool ignore_fwd, bool full_def_only);

  static AST_Decl::NodeType const NT;

private:
  AST_Structure *pd_full_definition;
  // The structure this is a forward declaration of.

  bool is_defined_;
  // Checking the member above isn't good enough.
};

#endif           // _AST_STRUCTURE_FWD_AST_STRUCTURE_FWD_HH
