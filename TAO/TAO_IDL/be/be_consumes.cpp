#include "be_consumes.h"
#include "be_eventtype.h"
#include "be_visitor.h"

be_consumes::be_consumes (UTL_ScopedName *n,
                          AST_Type *consumes_type)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_consumes,
              n),
    AST_Field (AST_Decl::NT_consumes,
               consumes_type,
               n),
    AST_Consumes (n,
                  consumes_type),
    be_decl (AST_Decl::NT_consumes,
             n),
    be_field (consumes_type,
              n)
{
}

be_consumes::~be_consumes ()
{
}

be_eventtype *
be_consumes::consumes_type () const
{
  return
    dynamic_cast<be_eventtype*> (
      this->AST_Consumes::consumes_type ());
}

int
be_consumes::accept (be_visitor *visitor)
{
  return visitor->visit_consumes (this);
}

void
be_consumes::destroy ()
{
  this->AST_Consumes::destroy ();
  this->be_field::destroy ();
}
