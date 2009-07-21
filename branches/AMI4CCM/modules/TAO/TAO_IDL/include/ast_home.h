// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef _AST_HOME_AST_HOME_HH
#define _AST_HOME_AST_HOME_HH

#include "ast_interface.h"
#include "ace/Unbounded_Queue.h"

class AST_Home;
class AST_Component;
class AST_ValueType;

class TAO_IDL_FE_Export AST_Home : public virtual AST_Interface
{
public:
  AST_Home (void);

  AST_Home (UTL_ScopedName *n,
            AST_Home *base_home,
            AST_Component *managed_component,
            AST_ValueType *primary_key,
            AST_Interface **supports,
            long n_supports,
            AST_Interface **supports_flat,
            long n_supports_flat);

  virtual ~AST_Home (void);

  // Extend lookup to the base home.
  virtual AST_Decl *look_in_inherited (UTL_ScopedName *e,
                                       bool treat_as_ref);

  // Extend lookup to the supported interfaces.
  virtual AST_Decl *look_in_supported (UTL_ScopedName *e,
                                       bool treat_as_ref);

  // Accessors.

  AST_Home *base_home (void) const;

  AST_Interface **supports (void) const;

  long n_supports (void) const;

  AST_Component *managed_component (void) const;

  AST_ValueType *primary_key (void) const;
  
  typedef ACE_Unbounded_Queue<AST_Operation *> INIT_LIST;

  INIT_LIST &factories (void);

  INIT_LIST &finders (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL(AST_Home);
  DEF_NARROW_FROM_SCOPE(AST_Home);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

private:
  AST_Home *pd_base_home;
  AST_Component *pd_managed_component;
  AST_ValueType *pd_primary_key;
  INIT_LIST pd_factories;
  INIT_LIST pd_finders;
};

#endif // _AST_HOME_AST_HOME_HH
