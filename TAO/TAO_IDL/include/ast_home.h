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
            AST_ValueType *primary_key);

  virtual ~AST_Home (void);

  // Accessors.

  AST_Home *base_home (void) const;

  AST_Component *managed_component (void) const;

  AST_ValueType *primary_key (void) const;

  ACE_Unbounded_Queue<AST_Operation *> &factories (void);
  void factories (AST_Operation *d);

  ACE_Unbounded_Queue<AST_Operation *> &finders (void);
  void finders (AST_Operation *d);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.
  DEF_NARROW_METHODS1(AST_Home, AST_Interface);
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
  ACE_Unbounded_Queue<AST_Operation *> pd_factories;
  ACE_Unbounded_Queue<AST_Operation *> pd_finders;
};

#endif // _AST_HOME_AST_HOME_HH
