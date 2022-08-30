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

  ~AST_StructureFwd () override;

  AST_Structure *full_definition ();
  void set_full_definition (AST_Structure *nfd);

  bool is_defined () override;
  void set_as_defined ();

  // Cleanup function.
  void destroy () override;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  // Is this decl a forward declared type (Yes)
  bool is_fwd () override;

  // We don't actually want the forward declaration,
  // but want to return the full definition member,
  // whether defined yet or not.
  AST_Decl *adjust_found (bool ignore_fwd, bool full_def_only) override;

  static AST_Decl::NodeType const NT;

private:
  AST_Structure *pd_full_definition;
  // The structure this is a forward declaration of.

  bool is_defined_;
  // Checking the member above isn't good enough.
};

#endif           // _AST_STRUCTURE_FWD_AST_STRUCTURE_FWD_HH
