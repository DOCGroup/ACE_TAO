//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_fwd_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interface_Fwd node in the client inline.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface_fwd.h"

ACE_RCSID(be_visitor_interface_fwd, interface_fwd_ci, "$Id$")


// ********************************************************************
// Visitor implementation for the Interface_Fwd type
// This one for the client inline file
// ********************************************************************

be_visitor_interface_fwd_ci::be_visitor_interface_fwd_ci (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_interface_fwd_ci::~be_visitor_interface_fwd_ci (void)
{
}

// visit the Interface_Fwd_ci node and its scope
int
be_visitor_interface_fwd_ci::visit_interface_fwd (be_interface_fwd *node)
{
  if (!node->cli_inline_gen () && !node->imported ())
    {
#if 0
      // We don't generate any code here.....

      // It is possible to generate the definitions for the _var and
      // _out types, but if we do that then the _duplicate() and
      // _nil() methods cannot be inlined.

      // Since these classes will be generated once the forward
      // declaration is resolved there is really no problem here
#endif /* 0 */

      node->cli_inline_gen (I_TRUE);
    }
  return 0;
}
