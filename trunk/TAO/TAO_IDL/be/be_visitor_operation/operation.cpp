//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, operation, "$Id$")


// ************************************************************
// Generic Operation visitor
// ************************************************************

be_visitor_operation::be_visitor_operation (be_visitor_context *ctx)
: be_visitor_scope (ctx)
{
}

be_visitor_operation::~be_visitor_operation (void)
{
}


int
be_visitor_operation::void_return_type (be_type *bt)
{
// is the operation return type void?

if (bt->node_type () == AST_Decl::NT_pre_defined
&& (be_predefined_type::narrow_from_decl (bt)->pt ()
== AST_PredefinedType::PT_void))
return 1;
else
return 0;
}

int
be_visitor_operation::has_param_type (be_operation *node,
AST_Argument::Direction dir)
{
// proceed if the number of members in our scope is greater than 0
if (node->nmembers () > 0)
{
// initialize an iterator to iterate thru our scope
UTL_ScopeActiveIterator *si;
ACE_NEW_RETURN (si,
UTL_ScopeActiveIterator (node,
UTL_Scope::IK_decls),
0);
// continue until each element is visited
while (!si->is_done ())
{
be_argument *bd = be_argument::narrow_from_decl (si->item ());
if (bd && (bd->direction () == dir))
return 1;

si->next ();
} // end of while loop
delete si;
} // end of if

// not of the type we are looking for
return 0;
}

size_t
be_visitor_operation::count_non_out_parameters (be_operation *node)
{
size_t count = 0;

// proceed if the number of members in our scope is greater than 0
if (node->nmembers () > 0)
{
// initialize an iterator to iterate thru our scope
UTL_ScopeActiveIterator *si;
ACE_NEW_RETURN (si,
UTL_ScopeActiveIterator (node,
UTL_Scope::IK_decls),
0);

// Continue until each element is visited
while (!si->is_done ())
{
be_argument *bd =
be_argument::narrow_from_decl (si->item ());

// We do not generate insertion operators for valuetypes
// yet.  Do not include them in the count.
be_valuetype *vt =
be_valuetype::narrow_from_decl (bd->field_type ());

if (bd && (bd->direction () != AST_Argument::dir_OUT) && !vt)
count++;

si->next ();
}

delete si;
}

return count;
}


//Method to generate the throw specs for exceptions that are thrown by the
//operation
int
be_visitor_operation::gen_throw_spec (be_operation *node)
{
TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

if (be_global->use_raw_throw ())
*os << be_idt_nl << "throw (";
else
*os << be_idt_nl << "ACE_THROW_SPEC ((";

*os << be_idt_nl << "CORBA::SystemException";
if (node->exceptions ())
{

// initialize an iterator to iterate thru the exception list
UTL_ExceptlistActiveIterator *ei;
ACE_NEW_RETURN (ei,
UTL_ExceptlistActiveIterator (node->exceptions ()),
-1);
// continue until each element is visited
while (!ei->is_done ())
{
be_exception *excp = be_exception::narrow_from_decl (ei->item ());

if (excp == 0)
{
delete ei;
ACE_ERROR_RETURN ((LM_ERROR,
"(%N:%l) be_visitor_operation"
"gen_throw_spec - "
"bad exception node\n"), -1);

}

*os << "," << be_nl;
// allocator method
*os << excp->name ();
ei->next ();
} // end of while loop

delete ei;
} // end of if

if (be_global->use_raw_throw ())
{
*os << be_uidt_nl << ")" << be_uidt;
}
else
{
*os << be_uidt_nl << "))" << be_uidt;
}

return 0;
}

//Method that returns the appropriate CORBA::Environment variable
const char *
be_visitor_operation::gen_environment_var ()
{
static const char *ace_try_env_decl = "ACE_DECLARE_NEW_CORBA_ENV;";
static const char *null_env_decl = "";

// check if we are generating stubs/skeletons for true C++ exception support
if (be_global->exception_support ())
{
return ace_try_env_decl;
}
else
{
return null_env_decl;
}
}
