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
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;
  be_decl *scope = this->ctx_->scope ();

  // Nothing to do if we are imported or code is already generated.
  if (node->imported () || (node->cli_hdr_gen ()))
    return 0;

  this->ctx_->node (node);

  // Retrieve the type.
  bt = be_type::narrow_from_decl (node->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_ch::"
                         "visit_array - "
                         "bad base type\n"),
                        -1);
    }

  // Generate the ifdefined macro.
  os->gen_ifdef_macro (node->flat_name ());

  // If we contain an anonymous sequence,
  // generate code for the sequence here.
  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      if (this->gen_anonymous_base_type (bt,
                                         TAO_CodeGen::TAO_SEQUENCE_CH)
          == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_ch::"
                             "visit_array - "
                             "gen_anonymous_base_type failed\n"),
                            -1);
        }
    }

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
      // We are dealing with an anonymous array case. Generate a typedef with
      // an _ prepended to the name.
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

  // Now define the slice type and other required operations
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
      // We are dealing with an anonymous array case. Generate a typedef with
      // an _ prepended to the name.
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

  *os << ";" << be_nl << be_nl;

  // No _var or _out class for an anonymous (non-typedef'd) array.
  if (this->ctx_->tdef () != 0)
    {
      // Generate the _var and _out class declarations.
      if (this->gen_var_defn (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_array_ch::"
                             "visit_argument - "
                             "var_defn failed\n"),
                            -1);
        }

      // An out defn is generated only for a variable length struct
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
          *os << "typedef " << node->local_name () << " "
              << node->local_name () << "_out;" << be_nl << be_nl;
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

  // The _alloc, _dup, copy, and free methods. If the node is nested, the
  // methods become static
  const char *storage_class = 0;

  if (node->is_nested ())
    {
      if (scope->node_type () != AST_Decl::NT_module)
        {
          storage_class = "static ";
        }
      else
        {
          storage_class = "TAO_NAMESPACE_STORAGE_CLASS ";
        }
    }
  else
    {
      storage_class = "";
    }

  if (this->ctx_->tdef ())
    {
      // Typedefed array.
      *os << storage_class << node->nested_type_name (scope, "_slice")
          << " *";
      *os << node->nested_type_name (scope, "_alloc") << " (void);" << be_nl;
      *os << storage_class << "void "
          << node->nested_type_name (scope, "_free")
          << " (" << be_idt << be_idt_nl;
      *os << node->nested_type_name (scope, "_slice")
          << " *_tao_slice " << be_uidt_nl
          << ");" << be_uidt_nl;
      *os << storage_class << node->nested_type_name (scope, "_slice") << " *";
      *os << node->nested_type_name (scope, "_dup")
          << " (" << be_idt << be_idt_nl
          << "const ";
      *os << node->nested_type_name (scope, "_slice")
          << " *_tao_slice" << be_uidt_nl
          << ");" << be_uidt_nl;
      *os << storage_class << "void "
          << node->nested_type_name (scope, "_copy")
          << " (" << be_idt << be_idt_nl;
      *os << node->nested_type_name (scope, "_slice") << " *_tao_to," << be_nl
          << "const ";
      *os << node->nested_type_name (scope, "_slice")
          << " *_tao_from" << be_uidt_nl
          << ");" << be_uidt_nl;
    }
  else
    {
      // Anonymous array.
      *os << storage_class << node->nested_type_name (scope, "_slice", "_")
          << " *";
      *os << node->nested_type_name (scope, "_alloc", "_")
          << " (void);" << be_nl;
      *os << storage_class << "void "
          << node->nested_type_name (scope, "_free", "_")
          << " (" << be_idt << be_idt_nl;
      *os << node->nested_type_name (scope, "_slice", "_")
          << " *_tao_slice" << be_uidt_nl
          << ");" << be_uidt_nl;
      *os << storage_class << node->nested_type_name (scope, "_slice", "_")
          << " *";
      *os << node->nested_type_name (scope, "_dup", "_")
          << " (" << be_idt << be_idt_nl
          << "const ";
      *os << node->nested_type_name (scope, "_slice", "_")
          << " *_tao_slice" << be_uidt_nl
          << ");" << be_uidt_nl;
      *os << storage_class << "void "
          << node->nested_type_name (scope, "_copy", "_")
          << " (" << be_idt << be_idt_nl;
      *os << node->nested_type_name (scope, "_slice", "_")
          << " *_tao_to," << be_nl
          << "const ";
      *os << node->nested_type_name (scope, "_slice", "_")
          << " *_tao_from" << be_uidt_nl
          << ");" << be_uidt_nl;
    }

  *os << be_nl;

  // Generate the endif macro.
  os->gen_endif ();

  node->cli_hdr_gen (1);
  return 0;
}

// Generate the var definition.
int
be_visitor_array_ch::gen_var_defn (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  char namebuf [NAMEBUFSIZE];
  char varnamebuf [NAMEBUFSIZE];

  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);
  ACE_OS::memset (varnamebuf,
                  '\0',
                  NAMEBUFSIZE);

  if (this->ctx_->tdef ())
    {
      // Typedefed array.
      ACE_OS::sprintf (namebuf,
                       "%s",
                       node->local_name ()->get_string ());
      ACE_OS::sprintf (varnamebuf,
                       "%s_var",
                       node->local_name ()->get_string ());
    }
  else
    {
      ACE_OS::sprintf (namebuf,
                       "_%s",
                       node->local_name ()->get_string ());
      ACE_OS::sprintf (varnamebuf,
                       "_%s_var",
                       node->local_name ()->get_string ());
    }

  // Generate the var definition (always in the client header).
  // Depending upon the data type, there are some differences which we account
  // for over here.

  *os << "class " << be_global->stub_export_macro ()
      << " " << varnamebuf << be_nl;
  *os << "{" << be_nl;
  *os << "public:" << be_idt_nl;
  // default constr
  *os << varnamebuf << " (void);" << be_nl;
  // constr from pointer to slice
  *os << varnamebuf << " (" << namebuf << "_slice *);" << be_nl;
  // copy constructor
  *os << varnamebuf << " (const " << varnamebuf <<
    " &);" << be_nl;
  // destructor
  *os << "~" << varnamebuf << " (void);" << be_nl;
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

  if (node->size_type () == be_decl::VARIABLE)
    {
      *os << "operator " << namebuf << "_slice *&();" << be_nl;
    }

  *os << be_nl;

  // Non-spec helper function used if array is a sequence element.
  *os << "// Calls " << namebuf << "_copy "
      << "(used in sequences of " << namebuf << ")." << be_nl;
  *os << "static void copy (" << be_idt << be_idt_nl
      << namebuf << "_slice *_tao_to," << be_nl
      << "const " << namebuf << "_slice *_tao_from" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  // in, inout, out and _retn
  *os << "// in, inout, out, _retn " << be_nl;
  *os << "const " << namebuf << "_slice *in (void) const;" << be_nl;

  if (node->size_type () == be_decl::FIXED)
    {
      *os << namebuf << "_slice *inout (void);" << be_nl;
    }
  else
    {
      *os << namebuf << "_slice *&inout (void);" << be_nl;
    }

  *os << namebuf << "_slice *&out (void);" << be_nl;
  *os << namebuf << "_slice *_retn (void);" << be_nl;

  // Generate an additional member function that returns the
  // underlying pointer.
  *os << namebuf << "_slice *ptr (void) const;" << be_uidt_nl;

  // Generate the private section.
  *os << "private:" << be_idt_nl;
  *os << namebuf << "_slice *ptr_;" << be_uidt_nl;
  *os << "};" << be_nl << be_nl;

  return 0;
}

// Generate the _out definition.
int
be_visitor_array_ch::gen_out_defn (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  char namebuf [NAMEBUFSIZE];
  char outnamebuf [NAMEBUFSIZE];

  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);
  ACE_OS::memset (outnamebuf,
                  '\0',
                  NAMEBUFSIZE);

  if (this->ctx_->tdef ())
    {
      ACE_OS::sprintf (namebuf,
                       "%s",
                       node->local_name ()->get_string ());
      ACE_OS::sprintf (outnamebuf,
                       "%s_out",
                       node->local_name ()->get_string ());
    }
  else
    {
      // Anonymous array.
      ACE_OS::sprintf (namebuf,
                       "_%s",
                       node->local_name ()->get_string ());
      ACE_OS::sprintf (outnamebuf,
                       "_%s_out",
                       node->local_name ()->get_string ());
    }

  // Generate the out definition (always in the client header).
  *os << "class " << be_global->stub_export_macro ()
      << " " << outnamebuf << be_nl;
  *os << "{" << be_nl;
  *os << "public:" << be_idt_nl;

  // No default constructor

  // Constructor from a pointer to slice.
  *os << outnamebuf << " (" << namebuf << "_slice *&);" << be_nl;
  // Constructor from a _var &
  *os << outnamebuf << " (" << namebuf << "_var &);" << be_nl;
  // Constructor from a _out &
  *os << outnamebuf << " (const " << outnamebuf << " &);" << be_nl << be_nl;
  // Assignment operator from a _out &
  *os << outnamebuf << " &operator= (const " << outnamebuf << " &);" << be_nl;
  // Assignment from slice *
  *os << outnamebuf << " &operator= (" << namebuf << "_slice *);"
      << be_nl << be_nl;
  // Cast
  *os << "operator " << namebuf << "_slice *&();" << be_nl;
  // ptr function
  *os << namebuf << "_slice *&ptr (void);" << be_nl << be_nl;
  // operator [] instead of ->
  *os << namebuf << "_slice &operator[] (CORBA::ULong index);" << be_nl;
  *os << "const " << namebuf << "_slice &operator[] "
      << "(CORBA::ULong index) const;" << be_uidt_nl;

  *os << "private:" << be_idt_nl;
  *os << namebuf << "_slice *&ptr_;" << be_nl;
  *os << "// Assignment from T_var not allowed." << be_nl;
  *os << "void operator= (const " << namebuf << "_var &);" << be_uidt_nl;
  *os << "};" << be_nl << be_nl;

  return 0;
}

// generate the _forany definition for ourself.
int
be_visitor_array_ch::gen_forany_defn (be_array *node)
{
  if (node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  char namebuf [NAMEBUFSIZE];
  char foranyname [NAMEBUFSIZE];

  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);
  ACE_OS::memset (foranyname,
                  '\0',
                  NAMEBUFSIZE);

  if (this->ctx_->tdef ())
    {
      ACE_OS::sprintf (namebuf,
                       "%s",
                       node->local_name ()->get_string ());
      ACE_OS::sprintf (foranyname,
                       "%s_forany",
                       node->local_name ()->get_string ());
    }
  else
    {
      // Anonymous array case.
      ACE_OS::sprintf (namebuf,
                       "_%s",
                       node->local_name ()->get_string ());
      ACE_OS::sprintf (foranyname,
                       "_%s_forany",
                       node->local_name ()->get_string ());
    }

  // Generate the forany definition (always in the client header).
  // Depending upon the data type, there are some differences which we account
  // for over here.

  *os << "class " << be_global->stub_export_macro ()
      << " " << foranyname << be_nl;
  *os << "{" << be_nl;
  *os << "public:" << be_idt_nl;

  // Default constuctor.
  *os << foranyname << " (void);" << be_nl;
  // Constuctor from pointer to slice.
  *os << foranyname << " (" << be_idt << be_idt_nl
      << namebuf << "_slice *," << be_nl
      << "CORBA::Boolean nocopy = 0" << be_uidt_nl
      << ");" << be_uidt_nl;
  // Copy constructor.
  *os << foranyname << " (const " << foranyname
      << " &);" << be_nl;
  // Destructor.
  *os << "~" << foranyname << " (void);" << be_nl << be_nl;

  *os << "static void _tao_any_destructor (void*);" << be_nl << be_nl;

  // assignment operator from a pointer to slice
  *os << foranyname << " &operator= (" << namebuf << "_slice *);"
      << be_nl;
  // assignment from _var
  *os << foranyname << " &operator= (const " << foranyname << " &);"
      << be_nl << be_nl;

  // Other extra types (cast operators, [] operator, and others)
  // overloaded [] operator
  *os << namebuf << "_slice &operator[] (CORBA::ULong index);"
      << be_nl;
  *os << "const " << namebuf
      << "_slice &operator[] (CORBA::ULong index) const;" << be_nl << be_nl;

  // cast operators
  *os << "operator " << namebuf << "_slice * const &() const;"
      << be_nl;
  *os << "operator " << namebuf << "_slice *&();" << be_nl << be_nl;

  // in, inout, out and _retn
  *os << "// in, inout, out, _retn " << be_nl;
  *os << "const " << namebuf << "_slice *in (void) const;" << be_nl;
  *os << namebuf << "_slice *inout (void);" << be_nl;
  *os << namebuf << "_slice *&out (void);" << be_nl;
  *os << namebuf << "_slice *_retn (void);" << be_nl;

  // Generate an additional member function that returns the
  // underlying pointer.
  *os << namebuf << "_slice *ptr (void) const;" << be_nl;

  // Additional member function that returns the NOCOPY flag.
  *os << "CORBA::Boolean nocopy (void) const;" << be_uidt_nl;

  // Generate the private section.
  *os << "private:" << be_idt_nl;
  *os << namebuf << "_slice *ptr_;" << be_nl;
  *os << "CORBA::Boolean nocopy_;" << be_uidt_nl;
  *os << "};" << be_nl << be_nl;

  return 0;
}
