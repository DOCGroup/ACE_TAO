//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_init_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Valuetypes factory in the client header
//    (see IDL to C++ mapping)
//
// = AUTHOR
//   Boris Kolpackov <bosk@ipmce.ru>
//
//
// ============================================================================

ACE_RCSID (be_visitor_valuetype, 
           valuetype_init_ci, 
           "$Id$")

be_visitor_valuetype_init_ci::be_visitor_valuetype_init_ci (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype_init (ctx)
{
}

be_visitor_valuetype_init_ci::~be_visitor_valuetype_init_ci (void)
{
}

int
be_visitor_valuetype_init_ci::visit_valuetype (be_valuetype *node)
{
  // Just generate the factory _var impl.

  // To hold the full and local _var names.
  char fname [NAMEBUFSIZE];
  char lname [NAMEBUFSIZE];

  ACE_OS::memset (fname,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (fname,
                   "%s_init_var",
                   node->full_name ());

  ACE_OS::memset (lname,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (lname,
                   "%s_init_var",
                   node->local_name ());

  TAO_OutStream *ci = tao_cg->client_inline ();

  // Generate the var implementation in the stubs file
  // Depending upon the data type, there are some differences which we account
  // for over here.

  ci->indent (); // start with whatever was our current indent level

  *ci << "// *************************************************************"
      << be_nl;
  *ci << "// Operations for class " << fname << be_nl;
  *ci << "// *************************************************************\n\n";

  // Default constructor.
  *ci << fname << "::" << lname <<
    " (void) // default constructor" << be_nl;
  *ci << "  " << ": ptr_ (0)" << be_nl;
  *ci << "{}\n\n";

  // Constructor from a pointer.
  ci->indent ();
  *ci << fname << "::" << lname << " ("
      << node->local_name () << "_init" << "* p)" << be_nl;
  *ci << "  : ptr_ (p)" << be_nl;
  *ci << "{}" << be_nl << be_nl;

  // Constructor from a const pointer.
  // TAO extension - it appears that there are problems with at least g++
  // which reclaims amguity between T(T*) and T(const T_var &)
  *ci << fname << "::" << lname << " (const "
      << node->local_name () << "_init" << "* p)" << be_nl;
  *ci << "  : ptr_ (ACE_const_cast ("
      << node->local_name () << "_init" << "*, p))" << be_nl;
  *ci << "{}" << be_nl << be_nl;

  // The additional ptr () member function. This member function must be
  // defined before the remaining member functions including the copy
  // constructor because this inline function is used elsewhere. Hence to make
  // inlining of this function possible, we must define it before its use.
  *ci << node->full_name () << "_init" << "* " << be_nl;
  *ci << fname << "::ptr (void) const" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "return this->ptr_;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // Copy constructor.
  *ci << fname << "::" << lname << " (const " << lname <<
    " &p) // copy constructor" << be_nl;
  *ci << "{" << be_idt_nl
      <<    "p.ptr ()->_add_ref ();" << be_nl
      <<    "this->ptr_ = p.ptr ();" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Destructor.
  *ci << fname << "::~" << lname << " (void) // destructor" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "this->ptr_->_remove_ref ();" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // Assignment operator.
  *ci << fname << " &" << be_nl;
  *ci << fname << "::operator= (" << node->local_name () << "_init" 
      << "* p)" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "this->ptr_->_remove_ref ();" << be_nl;
  *ci << "this->ptr_ = p;" << be_nl;
  *ci << "return *this;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // Assignment operator from _var.
  *ci << fname << " &" << be_nl;
  *ci << fname << "::operator= (const " << lname
      << " &p)" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "if (this != &p)" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "this->ptr_->_remove_ref ();" << be_nl
      << node->local_name () << "_init" << "* tmp = p.ptr ();" << be_nl
      << "tmp->_add_ref ();" << be_nl
      << "this->ptr_ = tmp;" << be_uidt_nl;
  *ci << "}" << be_nl;
  *ci << "return *this;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // Other extra methods - cast operator ().
  *ci << fname << "::operator const " << node->full_name () << "_init" 
      << "* () const // cast" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "return this->ptr_;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  *ci << fname << "::operator " << node->full_name () << "_init" 
      << "* () // cast " << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "return this->ptr_;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // operator->
  *ci << node->full_name () << "_init" << "* " << be_nl;
  *ci << fname << "::operator-> (void) const" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "return this->ptr_;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // in, inout, out, and _retn.
  *ci << node->full_name () << "_init" << "*" << be_nl;
  *ci << fname << "::in (void) const" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "return this->ptr_;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  *ci << node->full_name () << "_init" << "* &" << be_nl;
  *ci << fname << "::inout (void)" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "return this->ptr_;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  *ci << node->full_name () << "_init" << "* &" << be_nl;
  *ci << fname << "::out (void)" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "this->ptr_->_remove_ref ();" << be_nl;
  *ci << "this->ptr_ = 0;" << be_nl;
  *ci << "return this->ptr_;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  *ci << node->full_name () << "_init" << "* " << be_nl;
  *ci << fname << "::_retn (void)" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "// yield ownership of managed obj reference" << be_nl;
  *ci << node->local_name () << "_init" << "* tmp = this->ptr_;" << be_nl;
  *ci << "this->ptr_ = 0;" << be_nl;
  *ci << "return tmp;" << be_uidt_nl;
  *ci << "}\n\n";

  return 0;
}

int
be_visitor_valuetype_init_ci::visit_factory (be_factory *)
{
  return 0;
}
