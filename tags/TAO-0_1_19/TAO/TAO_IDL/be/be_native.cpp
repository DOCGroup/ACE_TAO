// Id
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_native.cpp
//
// = DESCRIPTION
//    The native IDL type
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"


// be_native: New IDL type added by the POA spec
/*
 * Constructor(s)
 */
be_native::be_native (void)
{
}

be_native::be_native (UTL_ScopedName *n,
                      UTL_StrList *p)
  : AST_Native (n, p),
    AST_Decl (AST_Decl::NT_native, n, p)
{
}

int
be_native::gen_client_header (void)
{
  return 0;
}

int
be_native::gen_client_stubs (void)
{
  return 0;
}

int
be_native::gen_server_header (void)
{
  return 0;
}

int
be_native::gen_server_skeletons (void)
{
  return 0;
}

int
be_native::gen_client_inline (void)
{
  return 0;
}

int
be_native::gen_server_inline (void)
{
  return 0;
}

int
be_native::gen_typecode (void)
{
  return 0;
}

long
be_native::tc_size (void)
{
  return 0;
}

int
be_native::write_as_return (TAO_OutStream *stream,
			       be_type *type)
{
  *stream << type->name ();
  return 0;
}

int
be_native::accept (be_visitor *visitor)
{
  return visitor->visit_native (this);
}

// Narrowing
IMPL_NARROW_METHODS2(be_native, AST_Native, be_type)
IMPL_NARROW_FROM_DECL(be_native)
