// $Id$

#include "PSDL_Extern.h"
#include "PSDL_Node.h"

extern int TAO_PSDL_yyparse (void *);
extern FILE *TAO_PSDL_yyin;
extern TAO_PSDL_Node *yyval;

TAO_PSDL_Extern::TAO_PSDL_Extern (void)
{
  // Constructor
}

int
TAO_PSDL_Extern::TAO_PSDL_Extern_yyparse (void *result)
{
  return TAO_PSDL_yyparse (result);
}

void
TAO_PSDL_Extern::TAO_PSDL_Extern_yyin (FILE *fp)
{
  TAO_PSDL_yyin = fp;
}

TAO_PSDL_Node *
TAO_PSDL_Extern::TAO_PSDL_Extern_yyval (void)
{
  return yyval;
}
