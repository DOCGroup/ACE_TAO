// -*- C++ -*-
// $Id$

#include "PSDL_Struct_Visitor.h"
#include "PSDL_Stream.h"
#include "PSDL_y.h"
#include "tao/CORBA_String.h"



TAO_PSDL_Struct_Visitor::TAO_PSDL_Struct_Visitor (void)
  : identifiers_count_ (0),
    struct_name_ ()
{
}

TAO_PSDL_Struct_Visitor::~TAO_PSDL_Struct_Visitor (void)
{
}

int
TAO_PSDL_Struct_Visitor::visit_identifier (TAO_PSDL_Identifier *identifier)
{
  // The first time it will enter this function is to get the name of
  // the struct. And, then on, for the members in the structure.
  // so this->identifiers_[0] will be the name of the struct.
  if (this->identifiers_.size () == 0)
    {
      this->identifiers_.size (1);
      this->count_ = 0;
    }

  if (this->identifiers_.size () < this->count_ + 1)
    this->identifiers_.size (this->count_ + 1);

  this->identifiers_ [this->count_] = identifier->value ();
  ++this->count_;

  return 0;
}

int
TAO_PSDL_Struct_Visitor::visit_type_spec (TAO_PSDL_Type_Spec *type_spec)
{
  if (type_spec->type_of_type_spec ()->accept (this) == -1)
    return -1;

  ACE_CString type;
  // Get a pointer to PSDL_Stream of the stub header.
  TAO_PSDL_Stream *ps_sh = TAO_PSDL_Scope::instance ()->get_sh ();

  if (this->base_type (this->identifiers_[this->identifiers_count_], type))
    {
      // If this is a predefined type like long or char *
      if (ACE_OS::strcmp (type.c_str (), "char *") == 0)
        {
          ps_sh->indent ();
          *ps_sh << " TAO::String_Manager ";
        }
      else
        {
          *ps_sh << " "
                 << this->version_for_stub (type) << " ";
        }
    }
  else
    {
      *ps_sh << "ACE_NESTED_CLASS ("
           << TAO_PSDL_Scope::instance ()->get_name_space ()
           << ", " << this->identifiers_ [this->identifiers_count_] << ") ";
    }

  ++this->identifiers_count_;
  return 0;
}

int
TAO_PSDL_Struct_Visitor::visit_declarator (TAO_PSDL_Declarator *declarator)
{
  if (declarator->type_of_declarator ()->accept (this) == -1)
    return -1;

  // Get a pointer to PSDL_Stream of the stub header.
  TAO_PSDL_Stream *ps_sh = TAO_PSDL_Scope::instance ()->get_sh ();

  *ps_sh << this->identifiers_ [this->identifiers_count_].c_str ()
       << ";\n";

  ++this->identifiers_count_;
  return 0;
}

int
TAO_PSDL_Struct_Visitor::visit_predefined_type (TAO_PSDL_Predefined_Type *predefined_type)
{
  // Get a pointer to PSDL_Stream of the stub header.
  TAO_PSDL_Stream *ps_sh = TAO_PSDL_Scope::instance ()->get_sh ();

  // Reset the indentation.
  ps_sh->reset ();

  if (predefined_type->type_of_variable_one () != 0)
    {
      if (predefined_type->type_of_variable_one ()->accept (this) == -1)
        return -1;

      this->struct_name_ = CORBA::string_dup (this->identifiers_ [0].c_str ());

      *ps_sh << "struct " << this->struct_name_
           << ";\n";

      *ps_sh << "class " << this->struct_name_
           << "_var;";

      ps_sh->nl ();
      ps_sh->nl ();

      *ps_sh << "struct " << this->struct_name_;
      ps_sh->nl ();

      *ps_sh << "{";
      ps_sh->nl ();

      ps_sh->nl ();

      *ps_sh << "typedef " << this->struct_name_ << "_var _var_type;\n";

      ps_sh->nl ();

      ps_sh->nl ();

      *ps_sh << "static void _tao_any_destructor (void*);";

      ps_sh->nl ();
      ps_sh->nl ();
    }

  if (predefined_type->type_of_variable_two () != 0)
    {
      // Resetting the counter to be used while printing out the
      // type_spec and declarator informations. See the corresponding
      // visit_* methods.

      if (this->count_ > 0)
        this->identifiers_count_ = 1;

      if (predefined_type->type_of_variable_two ()->accept (this) == -1)
        return -1;

      *ps_sh << "\n};\n\n";

      this->print_class_for_structure (this->struct_name_);
    }

  if (predefined_type->type_one () != 0)
    {
      // If the member declarations in the structure include any
      // predefined types like long, save them and increment the counter.
      if (this->identifiers_.size () < this->count_ + 1)
        this->identifiers_.size (this->count_ + 1);

      this->identifiers_ [this->count_] =
        TAO_PSDL_Scope::instance ()->convert_str (predefined_type->type_one ());

      ++this->count_;
    }

  return 0;
}

void
TAO_PSDL_Struct_Visitor::gen_code_for_si (void)
{
  TAO_PSDL_Stream *ps_si = TAO_PSDL_Scope::instance ()->get_si ();

  ps_si->reset ();
  ps_si->indent (); // start from current indentation level

  // Generate the typecode information here
  *ps_si << "static const CORBA::Long _oc_";

  // Flat name generation.
  *ps_si << this->struct_name_;
  *ps_si << "[] =";
  ps_si->nl ();
  *ps_si << "{";
  ps_si->nl ();

  // Some invocation call to put in the stuff that goes between
  // braces.
  *ps_si << "TAO_ENCAP_BYTE_ORDER";
  ps_si->nl ();
  *ps_si << "// Typecode related information should go in here";
  ps_si->nl ();

  *ps_si << "};";
  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "static CORBA::TypeCode _tc_TAO_tc_";
  *ps_si << this->struct_name_;
  *ps_si << " (";
  ps_si->nl ();
  ps_si->incr_indent (0);
  *ps_si << "CORBA::tk_struct,";
  ps_si->nl ();
  *ps_si << "sizeof (_oc_" << this->struct_name_ << "),";
  ps_si->nl ();
  *ps_si << "(char *) &_oc_";
  *ps_si << this->struct_name_ << ",";
  ps_si->nl ();
  *ps_si << "0,";
  ps_si->nl ();
  *ps_si << "sizeof (" << this->struct_name_ << ")";
  ps_si->nl ();
  *ps_si << ");";
  ps_si->decr_indent ();

  ps_si->nl ();
  ps_si->nl ();
  *ps_si << "::CORBA::TypeCode_ptr _tc_" << this->struct_name_ << " =";
  ps_si->nl ();
  ps_si->incr_indent (0);

  *ps_si << "&_tc_TAO_tc_" << this->struct_name_ << ";";
  ps_si->decr_indent ();
  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "void ";
  *ps_si << this->struct_name_
         << "::_tao_any_destructor (void *_tao_void_pointer)";
  ps_si->nl ();

  *ps_si << "{";
  ps_si->nl ();
  ps_si->incr_indent (0);

  *ps_si << this->struct_name_ << " *tmp = "
         << "ACE_static_cast ("
         << this->struct_name_
         << "*, _tao_void_pointer);";

  ps_si->nl ();
  *ps_si << "delete tmp;";
  ps_si->decr_indent ();
  ps_si->nl ();
  ps_si->decr_indent ();
  *ps_si <<"}";
  ps_si->nl ();
  ps_si->nl ();

  // Copying Insertion

  *ps_si << "// Copying insertion.";
  ps_si->nl ();

  *ps_si << "void operator<<= (";

  ps_si->incr_indent (0); ps_si->nl ();

  *ps_si << "CORBA::Any &_tao_any,"; ps_si->nl ();
  *ps_si << "const " << this->struct_name_ << " &_tao_elem"; ps_si->nl ();

  ps_si->decr_indent ();

  *ps_si << ")"; ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0); ps_si->nl ();
  *ps_si << "TAO_OutputCDR stream;"; ps_si->nl ();

  *ps_si << "if (stream << _tao_elem)"; ps_si->nl ();
  *ps_si << "{";

  ps_si->incr_indent (0); ps_si->nl ();

  *ps_si << "_tao_any._tao_replace (";

  ps_si->incr_indent (0); ps_si->nl ();

  *ps_si << "_tc_" << this->struct_name_ << ","; ps_si->nl ();
  *ps_si << "TAO_ENCAP_BYTE_ORDER,"; ps_si->nl ();
  *ps_si << "stream.begin ()";

  ps_si->decr_indent (0); ps_si->nl ();
  *ps_si << ");";

  ps_si->decr_indent (0); ps_si->nl ();
  *ps_si << "}";

  ps_si->decr_indent (0); ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();

  ps_si->reset ();
  ps_si->nl ();

  *ps_si << "// Non-copying insertion."; ps_si->nl ();
  *ps_si << "void operator<<= (";
  ps_si->incr_indent (0); ps_si->nl ();

  *ps_si << "CORBA::Any &_tao_any,"; ps_si->nl ();
  *ps_si << this->struct_name_ << " *_tao_elem";

  ps_si->nl ();
  *ps_si << ")";

  ps_si->decr_indent (0);  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0); ps_si->nl ();

  *ps_si << "TAO_OutputCDR stream;"; ps_si->nl ();

  *ps_si << "if (stream << *_tao_elem)"; ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0); ps_si->nl ();

  *ps_si << "_tao_any._tao_replace (";
  ps_si->incr_indent (0);ps_si->nl ();

  *ps_si << "_tc_" << this->struct_name_ << ","; ps_si->nl ();
  *ps_si << "TAO_ENCAP_BYTE_ORDER,"; ps_si->nl ();
  *ps_si << "stream.begin (),"; ps_si->nl ();
  *ps_si << "1,"; ps_si->nl ();
  *ps_si << "_tao_elem,"; ps_si->nl ();
  *ps_si << this->struct_name_ << "::_tao_any_destructor";

  ps_si->decr_indent (0); ps_si->nl ();
  *ps_si << ");";

  ps_si->decr_indent (0); ps_si->nl ();
  *ps_si << "}";

  ps_si->decr_indent (0); ps_si->nl ();
  *ps_si << "}"; ps_si->nl ();

  ps_si->reset ();
  ps_si->nl ();

  *ps_si << "// Extraction to const pointer."; ps_si->nl ();

  *ps_si << "CORBA::Boolean operator>>= (";
  ps_si->incr_indent (0); ps_si->nl ();

  *ps_si << "const CORBA::Any &_tao_any,"; ps_si->nl ();
  *ps_si << "const " << this->struct_name_ << " *&_tao_elem"; ps_si->nl ();

  *ps_si << ")";
  ps_si->decr_indent (0); ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);  ps_si->nl ();

  *ps_si << "_tao_elem = 0;"; ps_si->nl ();

  *ps_si << "ACE_TRY_NEW_ENV"; ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0); ps_si->nl ();

  *ps_si << "CORBA::TypeCode_var type = _tao_any.type ();"; ps_si->nl ();
  *ps_si << "CORBA::Boolean result ="; ps_si->nl ();

  *ps_si << "type->equivalent (";
  ps_si->incr_indent (0); ps_si->nl ();

  *ps_si << "_tc_" << this->struct_name_ << ""; ps_si->nl ();
  *ps_si << "";

  ps_si->decr_indent (0); ps_si->nl ();

  *ps_si << ");"; ps_si->nl ();
; ps_si->nl ();

  *ps_si << "if (result == 0)"; ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0); ps_si->nl ();

  *ps_si << "return 0; // not equivalent";

  ps_si->decr_indent (0); ps_si->nl ();
  *ps_si << "}"; ps_si->nl ();

  *ps_si << "if (_tao_any.any_owns_data ())"; ps_si->nl ();
  *ps_si << "{";
  ps_si->incr_indent (0); ps_si->nl ();

  *ps_si << "_tao_elem = ACE_static_cast (";
  ps_si->incr_indent (0); ps_si->nl ();

  *ps_si << "const " << this->struct_name_ << "*,"; ps_si->nl ();
  *ps_si << "_tao_any.value ()";

  ps_si->decr_indent (0); ps_si->nl ();
  *ps_si << ");"; ps_si->nl ();

  *ps_si << "return 1;"; ps_si->nl ();

  ps_si->decr_indent (0); ps_si->nl ();
  *ps_si << "}"; ps_si->nl ();

  *ps_si << "else"; ps_si->nl ();
  *ps_si << "{";
  ps_si->incr_indent (0); ps_si->nl ();

  *ps_si << this->struct_name_ << " *tmp;"; ps_si->nl ();

  *ps_si << "ACE_NEW_RETURN (";
  ps_si->incr_indent (0); ps_si->nl ();

  *ps_si << "tmp,"; ps_si->nl ();
  *ps_si << this->struct_name_ << ","; ps_si->nl ();
  *ps_si << "0"; ps_si->nl ();
  ps_si->decr_indent (0); ps_si->nl ();
  *ps_si << ");"; ps_si->nl ();
  *ps_si << ""; ps_si->nl ();

  *ps_si << "TAO_InputCDR stream (";
  ps_si->incr_indent (0); ps_si->nl ();

  *ps_si << "_tao_any._tao_get_cdr (),"; ps_si->nl ();
  *ps_si << "_tao_any._tao_byte_order ()";

  ps_si->decr_indent (0); ps_si->nl ();
  *ps_si << ");"; ps_si->nl ();

  *ps_si << "if (stream >> *tmp)"; ps_si->nl ();
  *ps_si << "{";
  ps_si->incr_indent (0); ps_si->nl ();

  *ps_si << "((CORBA::Any *)&_tao_any)->_tao_replace (";
  ps_si->incr_indent (0); ps_si->nl ();
  *ps_si << "_tc_" << this->struct_name_ << ","; ps_si->nl ();
  *ps_si << "1,"; ps_si->nl ();
  *ps_si << "static_cast<void *> (tmp),"; ps_si->nl ();
  *ps_si << this->struct_name_ << "::_tao_any_destructor";

  ps_si->decr_indent (0); ps_si->nl ();
  *ps_si << ");"; ps_si->nl ();

  *ps_si << "_tao_elem = tmp;"; ps_si->nl ();
  *ps_si << "return 1;";

  ps_si->decr_indent (0); ps_si->nl ();
  *ps_si << "}"; ps_si->nl ();

  *ps_si << "else"; ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0); ps_si->nl ();

  *ps_si << "delete tmp;";

  ps_si->decr_indent (0); ps_si->nl ();
  *ps_si << "}";

  ps_si->decr_indent (0); ps_si->nl ();
  *ps_si << "}";

  ps_si->decr_indent (0); ps_si->nl ();
  *ps_si << "}"; ps_si->nl ();

  *ps_si << "ACE_CATCHANY"; ps_si->nl ();
  *ps_si << "{";
  ps_si->nl ();
  *ps_si << "}"; ps_si->nl ();

  *ps_si << "ACE_ENDTRY;"; ps_si->nl ();
  *ps_si << "return 0;"; ps_si->nl ();
  ps_si->decr_indent (0); ps_si->nl ();
  *ps_si << "}";

  ps_si->reset ();
  ps_si->nl (); ps_si->nl ();

  return;
}

void
TAO_PSDL_Struct_Visitor::print_class_for_structure (ACE_CString struct_name)
{
  // Get a pointer to PSDL_Stream of the stub header.
  TAO_PSDL_Stream *ps_sh = TAO_PSDL_Scope::instance ()->get_sh ();

  *ps_sh << "class " << struct_name.c_str () << "_var\n";
  *ps_sh << "{\n";

  *ps_sh << "public:\n";
  *ps_sh << "   " << struct_name << "_var (void);\n";
  *ps_sh << "   " << struct_name << "_var (" << struct_name
         << " *);\n";
  *ps_sh << "   " << struct_name << "_var (const " << struct_name
       << "_var &);\n";
  *ps_sh << "   ~" << struct_name << "_var (void"
       << ");\n";

    ps_sh->nl ();


  *ps_sh << "   " << struct_name << "_var"
       << " &operator= (" << struct_name
       << " *);\n";

  *ps_sh << "   " << struct_name << "_var"
       << " &operator= (const " << struct_name
       << "_var &);\n";

  *ps_sh << "   " << struct_name
       << " *operator-> (void);\n";

  *ps_sh << "   const " << struct_name
       << " *operator-> (void) const;\n";

    ps_sh->nl ();


  *ps_sh << "   operator const "
       << struct_name << "&() const;\n";

  *ps_sh << "   operator "
       << struct_name << "&();\n";

  *ps_sh << "   operator "
       << struct_name << "&() const;\n";

  *ps_sh << "   // Variable-size types only.\n";

  *ps_sh << "   operator "
       << struct_name << "*&();\n";

  *ps_sh << "  // in inout, out, _retn\n";
  *ps_sh << "  const " << struct_name << "&in (void) const;\n";
  *ps_sh << "  " << struct_name << " &inout (void);\n";
  *ps_sh << "  " << struct_name << " *&out (void);\n";
  *ps_sh << "  " << struct_name << " *_retn (void);\n";
  *ps_sh << "  " << struct_name << " *ptr (void) const;\n";

  *ps_sh << "private:\n";
  *ps_sh << "  " << struct_name << " *ptr_;\n";

  *ps_sh << "};\n";

  *ps_sh << "class  " << struct_name << "_out\n";
  *ps_sh << "{\n";
  *ps_sh << "  public:\n";
  *ps_sh << "    " << struct_name << "_out ("
         << struct_name << " *&);\n";
  *ps_sh << "    " << struct_name << "_out ("
         << struct_name << "_var &);\n";
  *ps_sh << "    " << struct_name << "_out (const "
         << struct_name << "_out &);\n";
  *ps_sh << "    " << struct_name << "_out &operator= (const "
         << struct_name << "_out &);\n";
  *ps_sh << "    " << struct_name << "_out &operator= ("
         << struct_name << " *);\n";
  *ps_sh << "    operator " << struct_name << " *&();\n";
  *ps_sh << "    " << struct_name << " *&ptr (void);\n";
  *ps_sh << "    " << struct_name << " *operator-> (void);\n";
  ps_sh->nl ();

  *ps_sh << "  private:\n";
  *ps_sh << "    " << struct_name << " *&ptr_;\n";
  *ps_sh << "    // Assignment from T_var not allowed.\n";
  *ps_sh << "    void operator= (const " << struct_name << "_var &);\n";
  *ps_sh << "  };\n";

  *ps_sh << "TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr"
         << " _tc_" << struct_name << ";";
  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << " void operator<<= (CORBA::Any &, const " << struct_name << " &);"
         << "// copying version";
  ps_sh->nl ();

  *ps_sh << " void operator<<= (CORBA::Any &, " << struct_name << "*);"
         << "// noncopying version";
  ps_sh->nl ();

  *ps_sh << " CORBA::Boolean operator>>= "
         << "(const CORBA::Any &, " << struct_name << " *&);"
         << "// deprecated";
  ps_sh->nl ();

  *ps_sh << " CORBA::Boolean operator>>= "
         << "(const CORBA::Any &, const " << struct_name << " *&);";
  ps_sh->nl ();

  ps_sh->nl ();

  *ps_sh << "#ifndef __ACE_INLINE__";
  ps_sh->incr_indent (0);
  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << " CORBA::Boolean operator<< "
         << "(TAO_OutputCDR &, const " << struct_name << " &);";
  ps_sh->nl ();

  *ps_sh << " CORBA::Boolean operator>> (TAO_InputCDR &, " << struct_name << " &);";

  ps_sh->decr_indent (0);
  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "#endif /* __ACE_INLINE__ */";

  ps_sh->nl ();
  ps_sh->nl ();
}

void
TAO_PSDL_Struct_Visitor::gen_code_for_sinline (void)
{
  TAO_PSDL_Stream *ps_sin = TAO_PSDL_Scope::instance ()->get_sinline ();

  if (ps_sin == 0)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Null pointer to the psdl inline file\n"));
  }
  ps_sin->reset ();

  *ps_sin << "// ********************************************************";
  ps_sin->nl ();
  *ps_sin << "// Inline operations for class " << this->struct_name_ << "_var"; ps_sin->nl ();
  *ps_sin << "// ********************************************************";
  ps_sin->nl ();

  ps_sin->nl ();

  *ps_sin << "ACE_INLINE"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_var::" << this->struct_name_ << "_var (void) // default constructor";

  ps_sin->incr_indent (0);
  ps_sin->nl ();

  *ps_sin << ": ptr_ (0)";

  ps_sin->decr_indent (0);
  ps_sin->nl ();

  *ps_sin << "{}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_var::" << this->struct_name_ << "_var (" << this->struct_name_ << " *p)";

  ps_sin->incr_indent (0);
  ps_sin->nl ();

  *ps_sin << ": ptr_ (p)";

  ps_sin->decr_indent (0);
  ps_sin->nl ();

  *ps_sin << "{}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_var::" << this->struct_name_ << "_var (const ::" << this->struct_name_ << "_var &p) // copy constructor"; ps_sin->nl ();
  *ps_sin << "{";

  ps_sin->incr_indent (0);
  ps_sin->nl ();

  *ps_sin << "if (p.ptr_)";

  ps_sin->incr_indent (0);
  ps_sin->nl ();

  *ps_sin << "ACE_NEW (this->ptr_, ::" << this->struct_name_ << " (*p.ptr_));";

  ps_sin->decr_indent (0);
  ps_sin->nl ();

  *ps_sin << "else";

  ps_sin->incr_indent (0);
  ps_sin->nl ();

  *ps_sin << "this->ptr_ = 0;";

  ps_sin->decr_indent (0);
  ps_sin->decr_indent (0);
  ps_sin->nl ();

  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_var::~" << this->struct_name_ << "_var (void) // destructor"; ps_sin->nl ();
  *ps_sin << "{";

  ps_sin->incr_indent (0);
  ps_sin->nl ();

  *ps_sin << "delete this->ptr_;";

  ps_sin->decr_indent (0);
  ps_sin->nl ();

  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE " << this->struct_name_ << "_var &"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_var::operator= (" << this->struct_name_ << " *p)"; ps_sin->nl ();
  *ps_sin << "{";

  ps_sin->incr_indent (0);
  ps_sin->nl ();

  *ps_sin << "delete this->ptr_;"; ps_sin->nl ();
  *ps_sin << "this->ptr_ = p;"; ps_sin->nl ();
  *ps_sin << "return *this;";

  ps_sin->decr_indent (0);
  ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE ::" << this->struct_name_ << "_var &"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_var::operator= (const ::" << this->struct_name_ << "_var &p)"; ps_sin->nl ();
  *ps_sin << "{";

  ps_sin->incr_indent (0);
  ps_sin->nl ();

  *ps_sin << "if (this != &p)";

  ps_sin->incr_indent (0);
  ps_sin->nl ();

  *ps_sin << "{";

  ps_sin->incr_indent (0);
  ps_sin->nl ();

  *ps_sin << "if (p.ptr_ == 0)";

  ps_sin->incr_indent (0);
  ps_sin->nl ();

  *ps_sin << "{";

  ps_sin->incr_indent (0);
  ps_sin->nl ();

  *ps_sin << "delete this->ptr_;"; ps_sin->nl ();
  *ps_sin << "this->ptr_ = 0;";

  ps_sin->decr_indent (0);
  ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->decr_indent (0); ps_sin->nl ();

  *ps_sin << "else";

  ps_sin->incr_indent (0);
  ps_sin->nl ();
  *ps_sin << "{";

  ps_sin->incr_indent (0);
  ps_sin->nl ();

  *ps_sin << this->struct_name_ << " *deep_copy =";
  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "new " << this->struct_name_ << " (*p.ptr_);";

  ps_sin->decr_indent (0);ps_sin->nl ();

  ps_sin->nl ();

  *ps_sin << "if (deep_copy != 0)";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "{";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << this->struct_name_ << " *tmp = deep_copy;"; ps_sin->nl ();
  *ps_sin << "deep_copy = this->ptr_;"; ps_sin->nl ();
  *ps_sin << "this->ptr_ = tmp;"; ps_sin->nl ();
  *ps_sin << "delete deep_copy;";

  ps_sin->decr_indent (0);ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->decr_indent (0);
  ps_sin->decr_indent (0);ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->decr_indent (0);
  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "return *this;";

  ps_sin->decr_indent (0);
  ps_sin->decr_indent (0);
  ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE const ::" << this->struct_name_ << " *"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_var::operator-> (void) const"; ps_sin->nl ();
  *ps_sin << "{";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "return this->ptr_;";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE ::" << this->struct_name_ << " *"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_var::operator-> (void)"; ps_sin->nl ();
  *ps_sin << "{";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "return this->ptr_;";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_var::operator const ::" << this->struct_name_ << " &() const // cast"; ps_sin->nl ();
  *ps_sin << "{";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "return *this->ptr_;";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_var::operator ::" << this->struct_name_ << " &() // cast "; ps_sin->nl ();
  *ps_sin << "{";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "return *this->ptr_;";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_var::operator ::" << this->struct_name_ << " &() const // cast "; ps_sin->nl ();
  *ps_sin << "{";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "return *this->ptr_;";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "// variable-size types only"; ps_sin->nl ();
  *ps_sin << "ACE_INLINE"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_var::operator ::" << this->struct_name_ << " *&() // cast "; ps_sin->nl ();
  *ps_sin << "{";
  ps_sin->incr_indent (0); ps_sin->nl ();

  *ps_sin << "return this->ptr_;";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE const ::" << this->struct_name_ << " &"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_var::in (void) const"; ps_sin->nl ();
  *ps_sin << "{";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "return *this->ptr_;";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE ::" << this->struct_name_ << " &"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_var::inout (void)"; ps_sin->nl ();
  *ps_sin << "{";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "return *this->ptr_;";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "// mapping for variable size "; ps_sin->nl ();
  *ps_sin << "ACE_INLINE ::" << this->struct_name_ << " *&"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_var::out (void)"; ps_sin->nl ();
  *ps_sin << "{";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "delete this->ptr_;"; ps_sin->nl ();
  *ps_sin << "this->ptr_ = 0;"; ps_sin->nl ();
  *ps_sin << "return this->ptr_;";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE ::" << this->struct_name_ << " *"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_var::_retn (void)"; ps_sin->nl ();
  *ps_sin << "{";

  ps_sin->incr_indent (0); ps_sin->nl ();

  *ps_sin << "::" << this->struct_name_ << " *tmp = this->ptr_;"; ps_sin->nl ();
  *ps_sin << "this->ptr_ = 0;"; ps_sin->nl ();
  *ps_sin << "return tmp;";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE ::" << this->struct_name_ << " *"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_var::ptr (void) const"; ps_sin->nl ();

  *ps_sin << "{";
  ps_sin->incr_indent (0); ps_sin->nl ();

  *ps_sin << "return this->ptr_;";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "// ************************************************************";
  ps_sin->nl ();
  *ps_sin << "// Inline operations for class " << this->struct_name_ << "_out";
  ps_sin->nl ();
  *ps_sin << "// ************************************************************"; ps_sin->nl ();

  ps_sin->nl ();

  *ps_sin << "ACE_INLINE"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_out::" << this->struct_name_ << "_out (::" << this->struct_name_ << " *&p)";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << ": ptr_ (p)";
  ps_sin->decr_indent (0); ps_sin->nl ();

  *ps_sin << "{";
  ps_sin->incr_indent (0); ps_sin->nl ();

  *ps_sin << "this->ptr_ = 0;";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_out::" << this->struct_name_ << "_out (" << this->struct_name_ << "_var &p) // constructor from _var";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << ": ptr_ (p.out ())";
  ps_sin->decr_indent (0); ps_sin->nl ();

  *ps_sin << "{";
  ps_sin->incr_indent (0);ps_sin->nl ();

  *ps_sin << "delete this->ptr_;"; ps_sin->nl ();
  *ps_sin << "this->ptr_ = 0;";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_out::" << this->struct_name_ << "_out (const ::" << this->struct_name_ << "_out &p) // copy constructor";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << ": ptr_ (ACE_const_cast (" << this->struct_name_ << "_out&, p).ptr_)";
  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "{}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE " << this->struct_name_ << "_out &"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_out::operator= (const ::" << this->struct_name_ << "_out &p)"; ps_sin->nl ();

  *ps_sin << "{";
  ps_sin->incr_indent (0);ps_sin->nl ();

  *ps_sin << "this->ptr_ = ACE_const_cast (" << this->struct_name_ << "_out&, p).ptr_;"; ps_sin->nl ();
  *ps_sin << "return *this;";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE " << this->struct_name_ << "_out &"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_out::operator= (" << this->struct_name_ << " *p)"; ps_sin->nl ();
  *ps_sin << "{";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "this->ptr_ = p;"; ps_sin->nl ();
  *ps_sin << "return *this;";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_out::operator ::" << this->struct_name_ << " *&() // cast"; ps_sin->nl ();
  *ps_sin << "{";
  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "return this->ptr_;";
  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE ::" << this->struct_name_ << " *&"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_out::ptr (void) // ptr"; ps_sin->nl ();
  *ps_sin << "{";
  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "return this->ptr_;";
  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE ::" << this->struct_name_ << " *"; ps_sin->nl ();
  *ps_sin << this->struct_name_ << "_out::operator-> (void)"; ps_sin->nl ();
  *ps_sin << "{";
  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "return this->ptr_;";
  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE CORBA::Boolean operator<< "
          << "(TAO_OutputCDR &strm, const " << this->struct_name_ << " &_tao_aggregate)";
  ps_sin->nl ();
  *ps_sin << "{";

  ps_sin->incr_indent (0); ps_sin->nl ();

  *ps_sin << "if (";
  ps_sin->incr_indent (0); ps_sin->nl ();

  *ps_sin << "(strm << _tao_aggregate.name.in ()) &&"; ps_sin->nl ();
  *ps_sin << "(strm << _tao_aggregate.obj_ref.in ())";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << ")";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "return 1;";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "else";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "return 0;";


  ps_sin->decr_indent (0);
  ps_sin->decr_indent (0);ps_sin->nl ();
  *ps_sin << "}";

  ps_sin->nl ();
  ps_sin->nl ();

  *ps_sin << "ACE_INLINE CORBA::Boolean operator>> "
          << "(TAO_InputCDR &strm, " << this->struct_name_ << " &_tao_aggregate)";
  ps_sin->nl ();

  *ps_sin << "{";
  ps_sin->incr_indent (0); ps_sin->nl ();

  *ps_sin << "if (";
  ps_sin->incr_indent (0);ps_sin->nl ();

  *ps_sin << "(strm >> _tao_aggregate.name.out ()) &&"; ps_sin->nl ();
  *ps_sin << "(strm >> _tao_aggregate.obj_ref.out ())";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << ")";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "return 1;";

  ps_sin->decr_indent (0); ps_sin->nl ();
  *ps_sin << "else";

  ps_sin->incr_indent (0); ps_sin->nl ();
  *ps_sin << "return 0;";

  ps_sin->decr_indent (0);
  ps_sin->decr_indent (0);
  ps_sin->nl ();

  *ps_sin << "}";

  ps_sin->nl ();

}
