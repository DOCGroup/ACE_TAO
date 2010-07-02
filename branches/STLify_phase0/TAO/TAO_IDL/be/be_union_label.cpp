//
// $Id$
//

#include "be_union_label.h"
#include "be_visitor.h"

be_union_label::be_union_label (AST_UnionLabel::UnionLabel ul,
                                AST_Expression *v)
  : AST_UnionLabel (ul, v)
{
}

int
be_union_label::accept (be_visitor *visitor)
{
  return visitor->visit_union_label (this);
}

void
be_union_label::destroy (void)
{
  this->AST_UnionLabel::destroy ();
}

