// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_state_attribute.cpp
//
// = DESCRIPTION
//    state based code generation for attributes.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

// constructor
be_state_attribute::be_state_attribute (void)
{
}

int
be_state_attribute::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  // Macro to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (bt);
  ACE_UNUSED_ARG (d);
  ACE_UNUSED_ARG (type);

  return 0;
}
