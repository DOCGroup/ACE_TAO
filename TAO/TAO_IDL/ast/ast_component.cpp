// $Id$

#include "ast_component.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "utl_indenter.h"
#include "global_extern.h"

ACE_RCSID (ast, 
           ast_component, 
           "$Id$")

AST_Component::AST_Component (void)
  : pd_base_component (0)
{
}

AST_Component::AST_Component (UTL_ScopedName *n,
                              AST_Component *base_component,
                              AST_Interface **supports,
                              long n_supports,
                              AST_Interface **supports_flat,
                              long n_supports_flat)
  : AST_Interface (n,
                   supports,
                   n_supports,
                   supports_flat,
                   n_supports_flat,
                   I_TRUE,
                   I_FALSE),
    AST_Type (AST_Decl::NT_component,
              n),
    AST_Decl (AST_Decl::NT_component,
              n),
    UTL_Scope (AST_Decl::NT_component),
    COMMON_Base (I_TRUE,
                 I_FALSE),
    pd_base_component (base_component)
{
}

AST_Component::~AST_Component (void)
{
}

AST_Component *
AST_Component::base_component (void) const
{
  return this->pd_base_component;
}

AST_Interface **
AST_Component::supports (void) const
{
  return this->inherits ();
}

long 
AST_Component::n_supports (void) const
{
  return this->n_inherits ();
}

ACE_Unbounded_Queue<AST_Interface *> &
AST_Component::provides (void)
{
  return this->pd_provides;
}

void 
AST_Component::provides (AST_Interface *d)
{
  this->pd_provides.enqueue_tail (d);
}

ACE_Unbounded_Queue<AST_Component::uses_description *> &
AST_Component::uses (void)
{
  return this->pd_uses;
}

void 
AST_Component::uses (AST_Component::uses_description *d)
{
  this->pd_uses.enqueue_tail (d);
}

ACE_Unbounded_Queue<AST_ValueType *> &
AST_Component::emits (void)
{
  return this->pd_emits;
}

void 
AST_Component::emits (AST_ValueType *d)
{
  this->pd_emits.enqueue_tail (d);
}

ACE_Unbounded_Queue<AST_ValueType *> &
AST_Component::publishes (void)
{
  return this->pd_publishes;
}

void 
AST_Component::publishes (AST_ValueType *d)
{
  this->pd_publishes.enqueue_tail (d);
}

ACE_Unbounded_Queue<AST_ValueType *> &
AST_Component::consumes (void)
{
  return this->pd_consumes;
}

void 
AST_Component::consumes (AST_ValueType *d)
{
  this->pd_consumes.enqueue_tail (d);
}

void
AST_Component::destroy (void)
{
  this->AST_Interface::destroy ();
}

void
AST_Component::dump (ACE_OSTREAM_TYPE &o)
{
  o << "component ";

  this->local_name ()->dump (o);

  o << " ";

  if (this->pd_base_component != 0)
    {
      o << ": ";
      this->pd_base_component->local_name ()->dump (o);
    }

  if (this->pd_n_inherits > 0)
    {
      o << "supports ";

      for (long i = 0; i < this->pd_n_inherits; ++i)
        {
          this->pd_inherits[i]->local_name ()->dump (o);

          if (i < this->pd_n_inherits - 1)
            {
              o << ", ";
            }
        }
    }

  o << " {\n";

  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);

  o << "}";
}

int
AST_Component::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_component (this);
}

// Narrowing methods.
IMPL_NARROW_METHODS1(AST_Component, AST_Interface)
IMPL_NARROW_FROM_DECL(AST_Component)
IMPL_NARROW_FROM_SCOPE(AST_Component)
