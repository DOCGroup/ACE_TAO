// -*- C++ -*-
// $Id$

#ifndef BE_PROVIDES_H
#define BE_PROVIDES_H

#include "ast_provides.h"

#include "be_field.h"
#include "be_interface.h"

class be_interface;
class be_visitor;

class be_provides : public virtual AST_Provides,
                    public virtual be_field
{
public:
  be_provides (UTL_ScopedName *n,
               AST_Type *provides_type);

  virtual ~be_provides (void);

  // Non-virtual override of frontend method.
  be_type *provides_type (void) const;

  // Common code called by visitors.
  int gen_facet_svnt_tmpl_decl (TAO_OutStream &os);
  int gen_facet_svnt_tmpl_defn (TAO_OutStream &os);

  // Narrowing.
  DEF_NARROW_FROM_DECL(be_provides);

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Cleanup.
  virtual void destroy (void);
};

// ====================================================

class be_facet_op_attr_defn_helper
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  be_facet_op_attr_defn_helper (be_interface *op_scope);

  virtual int emit (be_interface *derived_interface,
                    TAO_OutStream *os,
                    be_interface *base_interface);

private:
  be_interface *op_scope_;
};

#endif // BE_PROVIDES_H

