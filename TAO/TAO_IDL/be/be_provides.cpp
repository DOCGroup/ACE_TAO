#include "be_provides.h"
#include "be_interface.h"
#include "be_operation.h"
#include "be_attribute.h"
#include "be_visitor.h"
#include "be_helper.h"
#include "be_extern.h"

#include "be_visitor_operation.h"
#include "be_visitor_attribute.h"
#include "be_visitor_context.h"

#include "utl_identifier.h"

#include "nr_extern.h"

be_provides::be_provides (UTL_ScopedName *n,
                          AST_Type *provides_type)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_provides,
              n),
    AST_Field (AST_Decl::NT_provides,
               provides_type,
               n),
    AST_Provides (n,
                  provides_type),
    be_decl (AST_Decl::NT_provides,
             n),
    be_field (provides_type,
              n)
{
}

be_provides::~be_provides ()
{
}

be_type *
be_provides::provides_type () const
{
  return
    dynamic_cast<be_type*> (
      this->AST_Provides::provides_type ());
}

int
be_provides::accept (be_visitor *visitor)
{
  return visitor->visit_provides (this);
}

void
be_provides::destroy ()
{
  this->AST_Provides::destroy ();
  this->be_field::destroy ();
}

