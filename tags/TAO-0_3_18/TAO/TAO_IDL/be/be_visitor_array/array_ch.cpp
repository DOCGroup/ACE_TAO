//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    array_ch.cpp
//
// = DESCRIPTION
//    Visitor for Array code generation in client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_array.h"

ACE_RCSID(be_visitor_array, array_ch, "$Id$")


// ************************************************************************
//  visitor for array declaration in client header
// ************************************************************************

be_visitor_array_ch::be_visitor_array_ch (be_visitor_context *ctx)
  : be_visitor_array (ctx)
{
}

be_visitor_array_ch::~be_visitor_array_ch (void)
{
}

int be_visitor_array_ch::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_type *bt;  // base type
  be_decl *scope = this->ctx_->scope (); // scope in which it is used

  // nothing to do if we are imported or code is already generated
  if (node->imported () || (node->cli_hdr_gen ()))
    return 0;

  this->ctx_->node (node); // save the array node

  // retrieve the type
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_array - "
                         "Bad base type\n"),
                        -1);
    }

  // generate the ifdefined macro
  os->gen_ifdef_macro (node->flatname ());

  os->indent ();
  *os << "typedef ";
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_array - "
                         "base type decl failed\n"),
                        -1);
    }
  *os << " ";
  if (!this->ctx_->tdef ())
    {
      // we are dealing with an anonymous array case. Generate a typedef with
      // an _ prepended to the name
      *os << "_";
    }
  *os << node->local_name ();

  if (node->gen_dimensions (os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_array - "
                         "gen dimensions failed\n"),
                        -1);
    }
  *os << ";" << be_nl;

  // now define the slice type and other required operations
  *os << "typedef ";
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_array - "
                         "base type decl failed\n"),
                        -1);
    }
  *os << " ";
  if (!this->ctx_->tdef ())
    {
      // we are dealing with an anonymous array case. Generate a typedef with
      // an _ prepended to the name
      *os << "_";
    }
  *os << node->local_name () << "_slice";
  if (node->gen_dimensions (os, 1) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_array - "
                         "gen slice dimensions failed\n"),
                        -1);
    }
  *os << ";\n";

  // typedef the _var, _out, and _forany types
  if (this->gen_var_defn (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_argument - "
                         "var_defn failed\n"),
                        -1);
    }
  // a class is generated for an out defn only for a variable length struct
  if (node->size_type () == be_decl::VARIABLE)
    {
      if (this->gen_out_defn (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_array_ch::"
                             "visit_argument - "
                             "out_defn failed\n"),
                            -1);
        }
    }
  else
    {
      // fixed size
      os->indent ();
      // if we are a typedefed array, we can use the TYPE name to define an
      // _out type. However, for anonymous arrays that do not give rise to a
      // new type, we use the base type for defining an out type
      if (this->ctx_->tdef ())
        {
          *os << "typedef " << node->local_name () << " "
              << node->local_name () << "_out;\n";
        }
      else
        {
          *os << "typedef ";
          if (bt->accept (this) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_array_ch::"
                                 "visit_array - "
                                 "base type decl failed\n"),
                                -1);
            }
          *os << " _" << node->local_name () << "_out";
          if (node->gen_dimensions (os) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_array_ch::"
                                 "visit_array - "
                                 "gen dimensions failed\n"),
                                -1);
            }
          *os << ";\n";
        }
    }

  if (this->gen_forany_defn (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ch::"
                         "visit_argument - "
                         "forany_defn failed\n"),
                        -1);
    }
  // the _alloc, _dup, copy, and free methods. If the node is nested, the
  // methods become static
  os->indent ();
  char *storage_class = 0;

  if (node->is_nested ())
    {
      if (scope->node_type () != AST_Decl::NT_module)
        storage_class = "static ";
      else
        storage_class = "TAO_NAMESPACE_STORAGE_CLASS ";
    }
  else
    storage_class = "";

  if (this->ctx_->tdef ())
    {
      // typedefed array
      if (!node->is_nested ())
        *os << "ACE_INLINE ";
      *os << storage_class << node->nested_type_name (scope, "_slice") << " *";
      *os << node->nested_type_name (scope, "_alloc") << " (void);" << be_nl;
      if (!node->is_nested ())
        *os << "ACE_INLINE ";
      *os << storage_class << "void " << node->nested_type_name (scope, "_free") << " (";
      *os << node->nested_type_name (scope, "_slice") << " *_tao_slice);" << be_nl;
      *os << storage_class << node->nested_type_name (scope, "_slice") << " *";
      *os << node->nested_type_name (scope, "_dup") << " (const ";
      *os << node->nested_type_name (scope, "_slice") << " *_tao_slice);" << be_nl;
      *os << storage_class << "void " << node->nested_type_name (scope, "_copy") << " (";
      *os << node->nested_type_name (scope, "_slice") << " *_tao_to, const ";
      *os << node->nested_type_name (scope, "_slice") << " *_tao_from);" << be_nl;
    }
  else
    {
      // anonymous array
      if (!node->is_nested ())
        *os << "ACE_INLINE ";
      *os << storage_class << node->nested_type_name (scope, "_slice", "_") << " *";
      *os << node->nested_type_name (scope, "_alloc", "_") << " (void);" << be_nl;
      if (!node->is_nested ())
        *os << "ACE_INLINE ";
      *os << storage_class << "void " 
          << node->nested_type_name (scope, "_free", "_") << " (";
      *os << node->nested_type_name (scope, "_slice", "_") << " *_tao_slice);" << be_nl;
      *os << storage_class << node->nested_type_name (scope, "_slice", "_") << " *";
      *os << node->nested_type_name (scope, "_dup", "_") << " (const ";
      *os << node->nested_type_name (scope, "_slice", "_") << " *_tao_slice);" << be_nl;
      *os << storage_class << "void " 
          << node->nested_type_name (scope, "_copy", "_") << " (";
      *os << node->nested_type_name (scope, "_slice", "_") << " *_tao_to, const ";
      *os << node->nested_type_name (scope, "_slice", "_") << " *_tao_from);" << be_nl;
    }

  *os << "\n";

#if 0 
  // Typecode for an anonymous array will not be required anywhere since we do
  // not generate the Any operators for anonymous types

  // is this a typedefined array? if so, then let the typedef deal with
  // generation of the typecode
  if (!this->ctx_->tdef ())
    {
      // by using a visitor to declare and define the TypeCode, we have the
      // added advantage to conditionally not generate any code. This will be
      // based on the command line options. This is still TO-DO
      be_visitor_context ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_TYPECODE_DECL);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_ch::"
                             "visit_array - "
                             "TypeCode declaration failed\n"
                             ), -1);
        }
      delete visitor;
    }
#endif /* 0 */

  // generate the endif macro
  os->gen_endif ();

  node->cli_hdr_gen (1);
  return 0;
}

// generate the var defn
int
be_visitor_array_ch::gen_var_defn (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  char namebuf [NAMEBUFSIZE];  // names
  char varnamebuf [NAMEBUFSIZE];  // var names

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  ACE_OS::memset (varnamebuf, '\0', NAMEBUFSIZE);
  if (this->ctx_->tdef ())
    {
      // typedefed array
      ACE_OS::sprintf (namebuf, "%s", node->local_name ()->get_string ());
      ACE_OS::sprintf (varnamebuf, "%s_var", node->local_name ()->get_string ());
    }
  else
    {
      ACE_OS::sprintf (namebuf, "_%s", node->local_name ()->get_string ());
      ACE_OS::sprintf (varnamebuf, "_%s_var", node->local_name ()->get_string ());
    }

  // generate the var definition (always in the client header).
  // Depending upon the data type, there are some differences which we account
  // for over here.

  os->indent (); // start with whatever was our current indent level
  *os << "class " << idl_global->export_macro ()
      << " " << varnamebuf << be_nl;
  *os << "{" << be_nl;
  *os << "public:" << be_idt_nl;
  // default constr
  *os << varnamebuf << " (void); // default constructor" << be_nl;
  // constr from pointer to slice
  *os << varnamebuf << " (" << namebuf << "_slice *);" << be_nl;
  // copy constructor
  *os << varnamebuf << " (const " << varnamebuf <<
    " &); // copy constructor" << be_nl;
  // destructor
  *os << "~" << varnamebuf << " (void); // destructor" << be_nl;
  *os << be_nl;
  // assignment operator from a pointer to slice
  *os << varnamebuf << " &operator= (" << namebuf << "_slice *);" 
      << be_nl;
  // assignment from _var
  *os << varnamebuf << " &operator= (const " << varnamebuf << " &);" << be_nl;

  // arrow operator
  // nothing here
  *os << be_nl;

  // other extra types (cast operators, [] operator, and others)
  // overloaded [] operator
  *os << namebuf << "_slice &operator[] (CORBA::ULong index);" 
      << be_nl;
  *os << "const " << namebuf 
      << "_slice &operator[] (CORBA::ULong index) const;" << be_nl;

  // cast operators
  *os << "operator " << namebuf << "_slice * const &() const;" 
      << be_nl;
  *os << "operator " << namebuf << "_slice *&();" << be_nl;

  // in, inout, out and _retn
  *os << "// in, inout, out, _retn " << be_nl;
  *os << "const " << namebuf << "_slice *in (void) const;" << be_nl;
  *os << namebuf << "_slice *inout (void);" << be_nl;
  *os << namebuf << "_slice *&out (void);" << be_nl;
  *os << namebuf << "_slice *_retn (void);" << be_nl;

  // generate an additional member function that returns the underlying pointer
  *os << namebuf << "_slice *ptr (void) const;" << be_uidt_nl;

  // generate the private section
  *os << "private:" << be_idt_nl;
  *os << namebuf << "_slice *ptr_;" << be_uidt_nl;
  *os << "};\n\n";

  return 0;
}

// generate the _out definition
int
be_visitor_array_ch::gen_out_defn (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  char namebuf [NAMEBUFSIZE];  // to hold the name
  char outnamebuf [NAMEBUFSIZE];  // to hold the _out name

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  ACE_OS::memset (outnamebuf, '\0', NAMEBUFSIZE);
  if (this->ctx_->tdef ())
    {
      ACE_OS::sprintf (namebuf, "%s", node->local_name ()->get_string ());
      ACE_OS::sprintf (outnamebuf, "%s_out", node->local_name ()->get_string ());
    }
  else
    {
      // anonymous array
      ACE_OS::sprintf (namebuf, "_%s", node->local_name ()->get_string ());
      ACE_OS::sprintf (outnamebuf, "_%s_out", node->local_name ()->get_string ());
    }

  // generate the out definition (always in the client header)
  os->indent (); // start with whatever was our current indent level

  *os << "class " << idl_global->export_macro ()
      << " " << outnamebuf << be_nl;
  *os << "{" << be_nl;
  *os << "public:" << be_idt_nl;

  // No default constructor

  // constructor from a pointer to slice
  *os << outnamebuf << " (" << namebuf << "_slice *&);" << be_nl;
  // constructor from a _var &
  *os << outnamebuf << " (" << namebuf << "_var &);" << be_nl;
  // constructor from a _out &
  *os << outnamebuf << " (const " << outnamebuf << " &);" << be_nl;
  // assignment operator from a _out &
  *os << outnamebuf << " &operator= (const " << outnamebuf << " &);" << be_nl;
  // assignment from slice *
  *os << outnamebuf << " &operator= (" << namebuf << "_slice *);" 
      << be_nl;
  // cast
  *os << "operator " << namebuf << "_slice *&();" << be_nl;
  // ptr fn
  *os << namebuf << "_slice *&ptr (void);" << be_nl;
  // operator [] instead of ->
  *os << namebuf << "_slice &operator[] (CORBA::ULong index);" << be_nl;
  *os << "const " << namebuf << "_slice &operator[] " 
      << "(CORBA::ULong index) const;" << be_uidt_nl;

  *os << "private:" << be_idt_nl;
  *os << namebuf << "_slice *&ptr_;" << be_nl;
  *os << "// assignment from T_var not allowed" << be_nl;
  *os << "void operator= (const " << namebuf << "_var &);" << be_uidt_nl;
  *os << "};\n\n";
  return 0;
}

// generate the _var definition for ourself
int
be_visitor_array_ch::gen_forany_defn (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  char namebuf [NAMEBUFSIZE];  // names
  char foranyname [NAMEBUFSIZE];  // forany class names

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  ACE_OS::memset (foranyname, '\0', NAMEBUFSIZE);

  if (this->ctx_->tdef ())
    {
      ACE_OS::sprintf (namebuf, "%s", node->local_name ()->get_string ());
      ACE_OS::sprintf (foranyname, "%s_forany", node->local_name ()->get_string ());
    }
  else
    {
      // anonymous array case
      ACE_OS::sprintf (namebuf, "_%s", node->local_name ()->get_string ());
      ACE_OS::sprintf (foranyname, "_%s_forany", 
                       node->local_name ()->get_string ());
    }

  // generate the forany definition (always in the client header).
  // Depending upon the data type, there are some differences which we account
  // for over here.

  os->indent (); // start with whatever was our current indent level
  *os << "class " << idl_global->export_macro ()
      << " " << foranyname << be_nl;
  *os << "{" << be_nl;
  *os << "public:" << be_idt_nl;

  // default constr
  *os << foranyname << " (void); // default constructor" << be_nl;
  // constr from pointer to slice
  *os << foranyname << " (" << namebuf << "_slice *, " 
      << "CORBA::Boolean nocopy=0);" << be_nl;
  // copy constructor
  *os << foranyname << " (const " << foranyname 
      << " &); // copy constructor" << be_nl;
  // destructor
  *os << "~" << foranyname << " (void); // destructor" << be_nl;
  *os << be_nl;
  // assignment operator from a pointer to slice
  *os << foranyname << " &operator= (" << namebuf << "_slice *);" 
      << be_nl;
  // assignment from _var
  *os << foranyname << " &operator= (const " << foranyname << " &);" << be_nl;

  // arrow operator
  // nothing here
  *os << be_nl;

  // other extra types (cast operators, [] operator, and others)
  // overloaded [] operator
  *os << namebuf << "_slice &operator[] (CORBA::ULong index);" 
      << be_nl;
  *os << "const " << namebuf 
      << "_slice &operator[] (CORBA::ULong index) const;" << be_nl;

  // cast operators
  *os << "operator " << namebuf << "_slice * const &() const;" 
      << be_nl;
  *os << "operator " << namebuf << "_slice *&();" << be_nl;

  // in, inout, out and _retn
  *os << "// in, inout, out, _retn " << be_nl;
  *os << "const " << namebuf << "_slice *in (void) const;" << be_nl;
  *os << namebuf << "_slice *inout (void);" << be_nl;
  *os << namebuf << "_slice *&out (void);" << be_nl;
  *os << namebuf << "_slice *_retn (void);" << be_nl;

  // generate an additional member function that returns the underlying pointer
  *os << namebuf << "_slice *ptr (void) const;" << be_nl;

  // additional member function that returns the NOCOPY flag
  *os << "CORBA::Boolean nocopy (void) const;" << be_uidt_nl;

  // generate the private section
  *os << "private:" << be_idt_nl;
  *os << "/* friend class CORBA_Any; */" << be_nl;
  *os << namebuf << "_slice *ptr_;" << be_nl;
  *os << "CORBA::Boolean nocopy_;" << be_uidt_nl;
  *os << "};\n\n";

  return 0;
}

