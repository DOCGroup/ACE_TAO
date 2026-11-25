#ifndef _AST_STRUCTURE_FWD_AST_STRUCTURE_FWD_HH
#define _AST_STRUCTURE_FWD_AST_STRUCTURE_FWD_HH

#include "ast_type.h"
#include <memory>

class AST_Structure;

/// Represents a forward structure declaration.
class TAO_IDL_FE_Export AST_StructureFwd : public virtual AST_Type
{
public:
  AST_StructureFwd (AST_StructureFwd const &) = delete;
  AST_StructureFwd (AST_StructureFwd &&) = delete;
  AST_StructureFwd &operator= (AST_StructureFwd const &) = delete;
  AST_StructureFwd &operator= (AST_StructureFwd &&) = delete;
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

  /// Whether this declaration is a forward-declared type.
  bool is_fwd () const override;

  /// Return the full definition member rather than the forward declaration,
  /// whether the full definition is complete yet or not.
  AST_Decl *adjust_found (bool ignore_fwd, bool full_def_only) override;

  static AST_Decl::NodeType const NT;

private:
  /// The structure this is a forward declaration of.
  AST_Structure *pd_full_definition;

  /// The dummy structure while this forward declaration owns it.
  std::unique_ptr<AST_Structure> owned_full_definition_;

  /// The full-definition pointer alone does not indicate completeness.
  bool is_defined_ {};
};

#endif           // _AST_STRUCTURE_FWD_AST_STRUCTURE_FWD_HH
