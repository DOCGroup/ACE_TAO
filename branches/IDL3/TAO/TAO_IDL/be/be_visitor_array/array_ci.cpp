//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    array_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Arrays in the client inline.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_array, 
           array_ci, 
           "$Id$")


// ************************************************************************
//  visitor for array declaration in client inline
// ************************************************************************

be_visitor_array_ci::be_visitor_array_ci (be_visitor_context *ctx)
  : be_visitor_array (ctx)
{
}

be_visitor_array_ci::~be_visitor_array_ci (void)
{
}

int be_visitor_array_ci::visit_array (be_array *node)
{
  // Nothing to do if we are imported or code is already generated.
  if (node->imported () || (node->cli_inline_gen ()))
    {
      return 0;
    }

  this->ctx_->node (node); // save the array node

  // If we contain an anonymous sequence, generate code for it here.
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_array_ci::"
                         "visit_array - "
                         "bad base type\n"),
                        -1);
    }

  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      if (this->gen_anonymous_base_type (bt,
                                         TAO_CodeGen::TAO_SEQUENCE_CI)
          == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_ci::"
                             "visit_array - "
                             "gen_anonymous_base_type failed\n"),
                            -1);
        }
    }

  // No _var or _out class for an anonymous (non-typedef'd) array.
  if (this->ctx_->tdef () != 0)
    {
      // Generate code for the _var and _out types
      if (this->gen_var_impl (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_array_ci::"
                             "visit_array - "
                             "var_defn failed\n"),
                            -1);
        }
      if (node->size_type () == be_decl::VARIABLE)
        {
          if (this->gen_out_impl (node) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_array_ci::"
                                 "visit_array - "
                                 "out_defn failed\n"),
                                -1);
            }
        }
    }

  // Generate code for the forany type.
  if (this->gen_forany_impl (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ci::"
                         "visit_array - "
                         "forany_defn failed\n"),
                        -1);
    }

  node->cli_inline_gen (1);
  return 0;
}

// implementation of the _var class. All of these get generated in the inline
// file
int
be_visitor_array_ci::gen_var_impl (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  char nodename [NAMEBUFSIZE];
  char fname [NAMEBUFSIZE];
  char lname [NAMEBUFSIZE];

  ACE_OS::memset (nodename, 
                  '\0', 
                  NAMEBUFSIZE);
  ACE_OS::memset (fname, 
                  '\0', 
                  NAMEBUFSIZE);
  ACE_OS::memset (lname, 
                  '\0', 
                  NAMEBUFSIZE);

  if (this->ctx_->tdef ())
    {
      // typedefed node
      ACE_OS::sprintf (nodename, "%s", node->full_name ());
      ACE_OS::sprintf (fname, "%s_var", node->full_name ());
      ACE_OS::sprintf (lname, "%s_var",
                       node->local_name ()->get_string ());
    }
  else
    {
      // for anonymous arrays ...
      // we have to generate a name for us that has an underscope prepended to
      // our local name. This needs to be inserted after the parents's name
      if (node->is_nested ())
        {
          be_decl *parent = 
            be_scope::narrow_from_scope (node->defined_in ())->decl ();
          ACE_OS::sprintf (nodename, "%s::_%s", parent->full_name (),
                           node->local_name ()->get_string ());
          ACE_OS::sprintf (fname, "%s::_%s_var", parent->full_name (),
                           node->local_name ()->get_string ());
          ACE_OS::sprintf (lname, "_%s_var",
                           node->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (nodename, "_%s", node->full_name ());
          ACE_OS::sprintf (fname, "_%s_var", node->full_name ());
          ACE_OS::sprintf (lname, "_%s_var",
                           node->local_name ()->get_string ());
        }
    }

  // generate the var implementation in the inline file

  os->indent (); // start with whatever was our current indent level

  *os << "// *************************************************************"
      << be_nl;
  *os << "// Inline operations for class " << fname << be_nl;
  *os << "// *************************************************************\n\n";

  // default constr
  *os << "ACE_INLINE" << be_nl;
  *os << fname << "::" << lname <<
    " (void) // default constructor" << be_nl;
  *os << "  " << ": ptr_ (0)" << be_nl;
  *os << "{}\n\n";

  // constr from a _slice *
  os->indent ();
  *os << "ACE_INLINE" << be_nl;
  *os << fname << "::" << lname << " (" << nodename << "_slice *p)" << be_nl;
  *os << "  : ptr_ (p)" << be_nl;
  *os << "{}\n\n";

  // copy constructor (deep copy)
  os->indent ();
  *os << "ACE_INLINE" << be_nl;
  *os << fname << "::" << lname << " (const " << fname
      << " &p) // copy constructor" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "this->ptr_ = " << nodename << "_dup " << "(ACE_const_cast (const "
      << nodename << "_slice *, p.ptr_));" << be_uidt_nl;
  *os << "}\n\n";

  // destructor
  os->indent ();
  *os << "ACE_INLINE" << be_nl;
  *os << fname << "::~" << lname << " (void) // destructor" << be_nl;
  *os << "{" << be_idt_nl;
  *os << nodename << "_free (this->ptr_);" << be_uidt_nl;
  *os << "}\n\n";

  // assignment operator
  os->indent ();
  *os << "ACE_INLINE " << fname << " &" << be_nl;
  *os << fname << "::operator= (" << nodename
      << "_slice *p)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "// is what we own the same that is being assigned to us?"
      << be_nl;
  *os << "if (this->ptr_ != p)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "// delete our stuff and assume ownership of p" << be_nl;
  *os << nodename << "_free (this->ptr_);" << be_nl;
  *os << "this->ptr_ = p;" << be_uidt_nl;
  *os << "}" << be_nl;
  *os << "return *this;" << be_uidt_nl;
  *os << "}\n\n";

  // assignment operator from _var
  os->indent ();
  *os << "ACE_INLINE " << fname << " &" << be_nl;
  *os << fname << "::operator= (const " << fname
      << " &p)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "if (this != &p)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "// not assigning to ourselves" << be_nl;
  *os << nodename << "_free (this->ptr_); // free old stuff" << be_nl;
  *os << "// deep copy" << be_nl;
  *os << "this->ptr_ = " << nodename << "_dup (ACE_const_cast (const "
      << nodename << "_slice *, p.ptr_));" << be_uidt_nl;
  *os << "}" << be_nl;
  *os << "return *this;" << be_uidt_nl;
  *os << "}\n\n";

  // other extra methods - cast operators ()
  os->indent ();
  *os << "ACE_INLINE " << be_nl;
  *os << fname << "::operator " << nodename
      << "_slice * const &() const // cast" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return this->ptr_;" << be_uidt_nl;
  *os << "}\n\n";

  if (node->size_type () == be_decl::VARIABLE)
    {
      os->indent ();
      *os << "ACE_INLINE " << be_nl;
      *os << fname << "::operator " << nodename
          << "_slice *&() // cast " << be_nl;
      *os << "{" << be_idt_nl;
      *os << "return this->ptr_;" << be_uidt_nl;
      *os << "}\n\n";
    }

  // two operator []s instead of ->
  os->indent ();
  *os << "ACE_INLINE " << be_nl;
  *os << "const " << nodename << "_slice &" << be_nl;
  *os << fname << "::operator[] (CORBA::ULong index) const" << be_nl;
  *os << "{" << be_nl;

  *os << "#if defined (ACE_HAS_BROKEN_IMPLICIT_CONST_CAST)" << be_idt_nl;
  *os << "return ACE_const_cast (const " << nodename
      << "_slice &, this->ptr_[index]);" << be_uidt_nl;
  *os << "#else" << be_idt_nl;
  *os << "const " << nodename << "_slice &tmp = this->ptr_[index];" << be_nl;
  *os << "return tmp;" << be_uidt_nl;
  *os << "#endif /* ACE_HAS_BROKEN_IMPLICIT_CONST_CAST */" << be_nl;

  *os << "}\n\n";

  os->indent ();
  *os << "ACE_INLINE " << be_nl;
  *os << nodename << "_slice &" << be_nl;
  *os << fname << "::operator[] (CORBA::ULong index)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return this->ptr_[index];" << be_uidt_nl;
  *os << "}\n\n";

  // copy (in case we are a sequence element)
  os->indent ();
  *os << "ACE_INLINE void" << be_nl;
  *os << fname << "::copy (" << nodename << "_slice *_tao_to, "
      << "const " << nodename << "_slice *_tao_from)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << nodename << "_copy (_tao_to, _tao_from);" << be_uidt_nl;
  *os << "}\n\n";

  // in, inout, out, and _retn
  os->indent ();
  *os << "ACE_INLINE const " << nodename << "_slice *" << be_nl;
  *os << fname << "::in (void) const" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return ACE_const_cast (const " << nodename
      << "_slice *, this->ptr_);" << be_uidt_nl;
  *os << "}\n\n";

  os->indent ();
  if (node->size_type () == be_decl::FIXED)
    {
      *os << "ACE_INLINE " << nodename << "_slice *" << be_nl;
    }
  else
    {
      *os << "ACE_INLINE " << nodename << "_slice * &" << be_nl;
    }

  *os << fname << "::inout (void)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return this->ptr_;" << be_uidt_nl;
  *os << "}\n\n";

  os->indent ();
  *os << "ACE_INLINE " << nodename << "_slice * &" << be_nl;
  *os << fname << "::out (void)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << nodename << "_free (this->ptr_);" << be_nl;
  *os << "this->ptr_ = (" << nodename << "_slice *)0;" << be_nl;
  *os << "return this->ptr_;" << be_uidt_nl;
  *os << "}\n\n";

  os->indent ();
  *os << "ACE_INLINE " << nodename << "_slice *" << be_nl;
  *os << fname << "::_retn (void)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << nodename << "_slice *tmp = this->ptr_;" << be_nl;
  *os << "this->ptr_ = (" << nodename << "_slice *)0;" << be_nl;
  *os << "return tmp;" << be_uidt_nl;
  *os << "}\n\n";

  // the additional ptr () member function
  os->indent ();
  *os << "ACE_INLINE " << nodename << "_slice *" << be_nl;
  *os << fname << "::ptr (void) const" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return this->ptr_;" << be_uidt_nl;
  *os << "}\n\n";

  return 0;
}

int
be_visitor_array_ci::gen_out_impl (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  char nodename [NAMEBUFSIZE]; // node name
  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local names of the out class
  // save the node's local name and full name in a buffer for quick use later
  // on
  ACE_OS::memset (nodename, '\0', NAMEBUFSIZE);
  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  if (this->ctx_->tdef ())
    {
      // typedefed node
      ACE_OS::sprintf (nodename, "%s", node->full_name ());
      ACE_OS::sprintf (fname, "%s_out", node->full_name ());
      ACE_OS::sprintf (lname, "%s_out",
                       node->local_name ()->get_string ());
    }
  else
    {
      // for anonymous arrays ...
      // we have to generate a name for us that has an underscope prepended to
      // our local name. This needs to be inserted after the parents's name
      if (node->is_nested ())
        {
          be_decl *parent = 
            be_scope::narrow_from_scope (node->defined_in ())->decl ();
          ACE_OS::sprintf (nodename, "%s::_%s", parent->full_name (),
                           node->local_name ()->get_string ());
          ACE_OS::sprintf (fname, "%s::_%s_out", parent->full_name (),
                           node->local_name ()->get_string ());
          ACE_OS::sprintf (lname, "_%s_out",
                           node->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (nodename, "_%s", node->full_name ());
          ACE_OS::sprintf (fname, "_%s_out", node->full_name ());
          ACE_OS::sprintf (lname, "_%s_out",
                           node->local_name ()->get_string ());
        }
    }

  // generate the out implementation in the inline file

  os->indent (); // start with whatever was our current indent level

  *os << "// *************************************************************"
      << be_nl;
  *os << "// Inline operations for class " << fname << be_nl;
  *os << "// *************************************************************\n\n";

  // constr from a pointer to slice
  os->indent ();
  *os << "ACE_INLINE" << be_nl;
  *os << fname << "::" << lname << " (" << nodename << "_slice *&p)" << be_nl;
  *os << "  : ptr_ (p)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "this->ptr_ = 0;" << be_uidt_nl;
  *os << "}\n\n";

  // constructor from _var &
  os->indent ();
  *os << "ACE_INLINE" << be_nl;
  *os << fname << "::" << lname << " (" << nodename
      << "_var &p) // constructor from _var" << be_nl;
  *os << "  : ptr_ (p.out ())" << be_nl;
  *os << "{" << be_idt_nl;
  *os << nodename << "_free (this->ptr_);" << be_nl;
  *os << "this->ptr_ = 0;" << be_uidt_nl;
  *os << "}\n\n";

  // copy constructor
  os->indent ();
  *os << "ACE_INLINE" << be_nl;
  *os << fname << "::" << lname << " (const " << fname
      << " &p) // copy constructor" << be_nl;
  *os << "  : ptr_ (ACE_const_cast ("
      << fname << "&,p).ptr_)" << be_nl;
  *os << "{}\n\n";

  // assignment operator from _out &
  os->indent ();
  *os << "ACE_INLINE " << fname << " &" << be_nl;
  *os << fname << "::operator= (const " << fname
      << " &p)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "this->ptr_ = ACE_const_cast ("
      << fname << "&,p).ptr_;" << be_nl;
  *os << "return *this;" << be_uidt_nl;
  *os << "}\n\n";

  // assignment from _var is not allowed

  // assignment operator from _ptr
  os->indent ();
  *os << "ACE_INLINE " << fname << " &" << be_nl;
  *os << fname << "::operator= (" << nodename
      << "_slice *p)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "this->ptr_ = p;" << be_nl;
  *os << "return *this;" << be_uidt_nl;
  *os << "}\n\n";

  // other extra methods - cast operator ()
  os->indent ();
  *os << "ACE_INLINE " << be_nl;
  *os << fname << "::operator " << nodename
      << "_slice *&() // cast" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return this->ptr_;" << be_uidt_nl;
  *os << "}\n\n";

  // ptr function
  os->indent ();
  *os << "ACE_INLINE " << nodename << "_slice *&" << be_nl;
  *os << fname << "::ptr (void) // ptr" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return this->ptr_;" << be_uidt_nl;
  *os << "}\n\n";

  // operator [] instead of ->
  os->indent ();
  *os << "ACE_INLINE " << nodename << "_slice &" << be_nl;
  *os << fname << "::operator[] (CORBA::ULong index)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return this->ptr_[index];" << be_uidt_nl;
  *os << "}\n\n";

  return 0;
}

int
be_visitor_array_ci::gen_forany_impl (be_array *node)
{
  if (node->is_local ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream (); // output stream

  char nodename [NAMEBUFSIZE]; // node name
  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local names of the var
  // save the node's local name and full name in a buffer for quick use later
  // on
  ACE_OS::memset (nodename, '\0', NAMEBUFSIZE);
  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  if (this->ctx_->tdef ())
    {
      // typedefed node
      ACE_OS::sprintf (nodename, "%s", node->full_name ());
      ACE_OS::sprintf (fname, "%s_forany", node->full_name ());
      ACE_OS::sprintf (lname, "%s_forany",
                       node->local_name ()->get_string ());
    }
  else
    {
      // for anonymous arrays ...
      // we have to generate a name for us that has an underscope prepended to
      // our local name. This needs to be inserted after the parents's name
      if (node->is_nested ())
        {
          be_decl *parent = 
            be_scope::narrow_from_scope (node->defined_in ())->decl ();
          ACE_OS::sprintf (nodename, "%s::_%s", parent->full_name (),
                           node->local_name ()->get_string ());
          ACE_OS::sprintf (fname, "%s::_%s_forany", parent->full_name (),
                           node->local_name ()->get_string ());
          ACE_OS::sprintf (lname, "_%s_forany",
                           node->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (nodename, "_%s", node->full_name ());
          ACE_OS::sprintf (fname, "_%s_forany", node->full_name ());
          ACE_OS::sprintf (lname, "_%s_forany",
                           node->local_name ()->get_string ());
        }
    }

  // generate the var implementation in the inline file

  os->indent (); // start with whatever was our current indent level

  *os << "// *************************************************************"
      << be_nl;
  *os << "// Inline operations for class " << fname << be_nl;
  *os << "// *************************************************************\n\n";

  // default constr
  *os << "ACE_INLINE" << be_nl;
  *os << fname << "::" << lname <<
    " (void) // default constructor" << be_nl;
  *os << "  " << ": ptr_ (0)," << be_nl;
  *os << "    nocopy_ (0)" << be_nl;
  *os << "{}\n\n";

  // constr from a _slice *
  os->indent ();
  *os << "ACE_INLINE" << be_nl;
  *os << fname << "::" << lname << " (" << nodename << "_slice *p, "
      << "CORBA::Boolean nocopy)" << be_nl;
  *os << "  : ptr_ (p)," << be_nl;
  *os << "    nocopy_ (nocopy)" << be_nl;
  *os << "{}\n\n";

  // copy constructor (deep copy)
  os->indent ();
  *os << "ACE_INLINE" << be_nl;
  *os << fname << "::" << lname << " (const " << fname
      << " &p) // copy constructor" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "this->ptr_ = " << nodename << "_dup (ACE_const_cast (const "
      << nodename << "_slice *, p.ptr_));" << be_nl;
  *os << "this->nocopy_ = p.nocopy_;" << be_uidt_nl;
  *os << "}\n\n";

  // destructor
  os->indent ();
  *os << "ACE_INLINE" << be_nl;
  *os << fname << "::~" << lname << " (void) // destructor" << be_nl;
  *os << "{" << be_nl;
  *os << "   // don't do anything" << be_nl;
  *os << "}\n\n";

  // assignment operator
  os->indent ();
  *os << "ACE_INLINE " << fname << " &" << be_nl;
  *os << fname << "::operator= (" << nodename
      << "_slice *p)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "// is what we own the same that is being assigned to us?"
      << be_nl;
  *os << "if (this->ptr_ != p)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "// delete our stuff and assume ownership of p" << be_nl;
  *os << nodename << "_free (this->ptr_);" << be_nl;
  *os << "this->ptr_ = p;" << be_uidt_nl;
  *os << "}" << be_nl;
  *os << "return *this;" << be_uidt_nl;
  *os << "}\n\n";

  // assignment operator from _forany
  os->indent ();
  *os << "ACE_INLINE " << fname << " &" << be_nl;
  *os << fname << "::operator= (const " << fname
      << " &p)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "if (this != &p)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "// not assigning to ourselves" << be_nl;
  *os << nodename << "_free (this->ptr_); // free old stuff" << be_nl;
  *os << "// deep copy" << be_nl;
  *os << "this->ptr_ = " << nodename << "_dup (ACE_const_cast (const "
      << nodename << "_slice *, p.ptr_));" << be_nl;
  *os << "this->nocopy_ = p.nocopy_;" << be_uidt_nl;
  *os << "}" << be_nl;
  *os << "return *this;" << be_uidt_nl;
  *os << "}\n\n";

  // other extra methods - cast operators ()
  os->indent ();
  *os << "ACE_INLINE " << be_nl;
  *os << fname << "::operator " << nodename
      << "_slice * const &() const // cast" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return this->ptr_;" << be_uidt_nl;
  *os << "}\n\n";

  os->indent ();
  *os << "ACE_INLINE " << be_nl;
  *os << fname << "::operator " << nodename
      << "_slice *&() // cast " << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return this->ptr_;" << be_uidt_nl;
  *os << "}\n\n";

  // two operator []s instead of ->
  os->indent ();
  *os << "ACE_INLINE " << be_nl;
  *os << "const " << nodename << "_slice &" << be_nl;
  *os << fname << "::operator[] (CORBA::ULong index) const" << be_nl;
  *os << "{" << be_idt_nl;

  // MSVC requires an explicit cast for this. SunCC will
  // not accept one, but will do it implicitly with a temporary.
  // It's only a problem with multidimensional arrays.
#if defined (ACE_HAS_BROKEN_IMPLICIT_CONST_CAST)
  *os << "return ACE_const_cast (const " << nodename
      << "_slice &, this->ptr_[index]);" << be_uidt_nl;
#else
  *os << "const " << nodename << "_slice &tmp = this->ptr_[index];" << be_nl;
  *os << "return tmp;" << be_uidt_nl;
#endif /* ACE_HAS_BROKEN_IMPLICIT_CONST_CAST */

  *os << "}\n\n";

  os->indent ();
  *os << "ACE_INLINE " << be_nl;
  *os << nodename << "_slice &" << be_nl;
  *os << fname << "::operator[] (CORBA::ULong index)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return this->ptr_[index];" << be_uidt_nl;
  *os << "}\n\n";

  // in, inout, out, and _retn
  os->indent ();
  *os << "ACE_INLINE const " << nodename << "_slice *" << be_nl;
  *os << fname << "::in (void) const" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return ACE_const_cast (const " << nodename
      << "_slice *, this->ptr_);" << be_uidt_nl;
  *os << "}\n\n";

  os->indent ();
  *os << "ACE_INLINE " << nodename << "_slice *" << be_nl;
  *os << fname << "::inout (void)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return this->ptr_;" << be_uidt_nl;
  *os << "}\n\n";

  os->indent ();
  *os << "ACE_INLINE " << nodename << "_slice * &" << be_nl;
  *os << fname << "::out (void)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return this->ptr_;" << be_uidt_nl;
  *os << "}\n\n";

  os->indent ();
  *os << "ACE_INLINE " << nodename << "_slice *" << be_nl;
  *os << fname << "::_retn (void)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return this->ptr_;" << be_uidt_nl;
  *os << "}\n\n";

  // the additional ptr () member function
  os->indent ();
  *os << "ACE_INLINE " << nodename << "_slice *" << be_nl;
  *os << fname << "::ptr (void) const" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return this->ptr_;" << be_uidt_nl;
  *os << "}\n\n";

  // the additional nocopy member function
  os->indent ();
  *os << "ACE_INLINE CORBA::Boolean" << be_nl;
  *os << fname << "::nocopy (void) const" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return this->nocopy_;" << be_uidt_nl;
  *os << "}\n\n";

  return 0;
}
