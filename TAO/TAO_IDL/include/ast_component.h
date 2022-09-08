#ifndef _AST_COMPONENT_AST_COMPONENT_HH
#define _AST_COMPONENT_AST_COMPONENT_HH

#include "ast_interface.h"
#include "ace/Unbounded_Queue.h"

class AST_Provides;
class AST_Uses;
class AST_Publishes;
class AST_Emits;
class AST_Consumes;
class AST_Extended_Port;
class AST_Mirror_Port;

class TAO_IDL_FE_Export AST_Component
  : public virtual AST_Interface
{
public:
  AST_Component (UTL_ScopedName *n,
                 AST_Component *base_component,
                 AST_Type **supports,
                 long n_supports,
                 AST_Interface **supports_flat,
                 long n_supports_flat);

  ~AST_Component () override;

  // This also calls the base class version.
  void redefine (AST_Interface *from) override;

  // Extend lookup to the base component.
  AST_Decl *look_in_inherited (UTL_ScopedName *e,
                              bool full_def_only) override;

  // Extend lookup to the supported interfaces.
  AST_Decl *look_in_supported (UTL_ScopedName *e,
                              bool full_def_only) override;

  // Accessors.

  AST_Component *base_component () const;

  AST_Type **supports () const;

  long n_supports () const;

  // Override for component of UTL_Scope method.
  AST_Decl *special_lookup (UTL_ScopedName *e,
                           bool full_def_only,
                           AST_Decl *&final_parent_decl) override;

  // Cleanup function.
  void destroy () override;

  // Allows adding an uses to a later point
  // The port i is inserted after port ix, if
  // ix is not null.
  int be_add_uses (AST_Uses *i,
                   AST_Uses *ix = 0);

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  static AST_Decl::NodeType const NT;
  typedef AST_ComponentFwd FWD_TYPE;

protected:
  AST_Provides *fe_add_provides (AST_Provides *p) override;

  AST_Uses *fe_add_uses (AST_Uses *p) override;

  AST_Publishes *fe_add_publishes (AST_Publishes *p) override;

  AST_Emits *fe_add_emits (AST_Emits *p) override;

  AST_Consumes *fe_add_consumes (AST_Consumes *p) override;

  AST_Extended_Port *fe_add_extended_port (AST_Extended_Port *p) override;

  AST_Mirror_Port *fe_add_mirror_port (AST_Mirror_Port *p) override;

protected:
  AST_Component *pd_base_component;
};

#endif // _AST_COMPONENT_AST_COMPONENT_HH
