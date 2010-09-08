// -*- C++ -*-
// $Id$

#include "PSDL_Type_Dcl_Visitor.h"
#include "PSDL_Stream.h"



TAO_PSDL_Type_Dcl_Visitor::TAO_PSDL_Type_Dcl_Visitor (void)
{
}

TAO_PSDL_Type_Dcl_Visitor::~TAO_PSDL_Type_Dcl_Visitor (void)
{
}

int
TAO_PSDL_Type_Dcl_Visitor::visit_identifier (TAO_PSDL_Identifier *identifier)
{
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
TAO_PSDL_Type_Dcl_Visitor::visit_type_declarator (TAO_PSDL_Type_Declarator *type_declarator)
{
  // Typedefs.
  this->count_ = 0;
  this->identifiers_.size (2);
  this->predefined_type_.size (2);

  this->predefined_type_[this->count_] = 0;

  if (type_declarator->type_spec ()->accept (this) == -1)
    return -1;

  if (type_declarator->declarators ()->accept (this) == -1)
    return -1;

  ACE_CString type;

  // This flag is used to distinguish between the predefined types and
  // types that are typedef'ed before.
  if (this->predefined_type_ [0] == 0)
    {
      if (this->base_type (this->identifiers_[this->count_ -1], type))
        {
          if (ACE_OS::strcmp (type.c_str (), "struct") != 0)
            this->identifiers_[0] = type;
        }
    }

  // If the typedef is for a sequence of something, the very first
  // element is going to be 'sequence' (check visit_predefined_type
  // method in this class). We are basing our selection
  // based on that.
  if (ACE_OS::strcmp (this->identifiers_[0].c_str (), "sequence") != 0)
    {
      this->print_for_simple_typedefs ();
    }
  else
    this->print_for_sequence_typedefs (this->identifiers_[1],
                                       this->identifiers_[2]);

  return 0;
}

/*
  There seems to be a difference with what shoudl be printed adn what
  is printed out as per the method. So commenting it for now. Will keep
  it around for some time nad remove when I am sure its is totally wrong
*/

void
TAO_PSDL_Type_Dcl_Visitor::print_for_sequence_typedefs (ACE_CString identifier_type,
                                                        ACE_CString identifier)
{
  ACE_CString name_space = TAO_PSDL_Scope::instance ()->get_name_space ();

  // Get a pointer to PSDL_Stream of the stub header.
  TAO_PSDL_Stream *ps_sh = TAO_PSDL_Scope::instance ()->get_sh ();

  ps_sh->nl ();
  *ps_sh << "#if !defined (TAO_USE_SEQUENCE_TEMPLATES)";
  ps_sh->nl ();
  *ps_sh << "#if !defined (__TAO_UNBOUNDED_SEQUENCE_" << name_space.c_str ()
         << "_" << identifier.c_str () << "_CH_)";
  ps_sh->nl ();
  *ps_sh << "#define __TAO_UNBOUNDED_SEQUENCE_" << name_space.c_str ()
         << "_" << identifier.c_str () << "_CH_";
  ps_sh->nl ();
  *ps_sh << "class TAO_EXPORT_MACRO _TAO_Unbounded_Sequence_CosNaming_"
         << identifier.c_str ();
  ps_sh->nl ();
  *ps_sh << ": public TAO_Unbounded_Base_Sequence";
  ps_sh->nl ();
  *ps_sh << "{";
  ps_sh->incr_indent ();
  ps_sh->nl ();
  *ps_sh << "public:";
  ps_sh->nl ();
  *ps_sh << "// = Initialization and termination methods.";
  ps_sh->nl ();
  *ps_sh << "_TAO_Unbounded_Sequence_" << name_space.c_str ()
         << "_" << identifier.c_str () << " (void);";
  ps_sh->nl ();
  *ps_sh << "_TAO_Unbounded_Sequence_" << name_space.c_str ()
         << "_" << identifier.c_str () << " (CORBA::ULong maximum);";
  ps_sh->nl ();
  *ps_sh << "_TAO_Unbounded_Sequence_" << name_space.c_str () << "_"
         << identifier.c_str () << " (";
  ps_sh->nl ();
  *ps_sh << "CORBA::ULong maximum,";
  ps_sh->nl ();
  *ps_sh << "CORBA::ULong length,";
  ps_sh->nl ();
  *ps_sh << identifier_type << " *data,";
  ps_sh->nl ();
  *ps_sh << "CORBA::Boolean release = 0";
  ps_sh->nl ();
  *ps_sh << ");";
  ps_sh->nl ();
  *ps_sh << "_TAO_Unbounded_Sequence_" << name_space.c_str () << "_" << identifier.c_str () << " (";
  ps_sh->nl ();
  *ps_sh << "const _TAO_Unbounded_Sequence_" << name_space.c_str () << "_" << identifier.c_str () << " &rhs\n";
  *ps_sh << ");\n";
  *ps_sh << "_TAO_Unbounded_Sequence_" << name_space.c_str () << "_" << identifier.c_str () << " &operator= (\n";
  *ps_sh << "const _TAO_Unbounded_Sequence_" << name_space.c_str () << "_" << identifier.c_str () << " &rhs\n";
  *ps_sh << ");\n";
  *ps_sh << "virtual ~_TAO_Unbounded_Sequence_" << name_space << "_" << identifier << " (void);";
  ps_sh->nl ();
  ps_sh->nl ();
  *ps_sh << "// = Accessors.\n";
  *ps_sh << identifier_type << " &operator[] (CORBA::ULong i);\n";
  *ps_sh << "const " << identifier_type << " &operator[] (CORBA::ULong i) const;\n";
  ps_sh->nl ();
  *ps_sh << "// = Static operations.\n";
  *ps_sh << "static " << identifier_type << " *allocbuf (CORBA::ULong size);\n";
  *ps_sh << "static void freebuf (" << identifier_type << " *buffer);\n";
  ps_sh->nl ();
  *ps_sh << "// Implement the TAO_Base_Sequence methods (see Sequence.h)";   ps_sh->nl ();
  *ps_sh << "virtual void _allocate_buffer (CORBA::ULong length);";   ps_sh->nl ();
  *ps_sh << "virtual void _deallocate_buffer (void);";   ps_sh->nl ();
  *ps_sh << identifier_type << " *get_buffer (CORBA::Boolean orphan = 0);";   ps_sh->nl ();
  *ps_sh << "const " << identifier_type << " *get_buffer (void) const;";   ps_sh->nl ();
  *ps_sh << "void replace (";   ps_sh->nl ();
  *ps_sh << "  CORBA::ULong max,";   ps_sh->nl ();
  *ps_sh << "  CORBA::ULong length,";   ps_sh->nl ();
  *ps_sh << identifier_type << " *data,";   ps_sh->nl ();
  *ps_sh << "CORBA::Boolean release";   ps_sh->nl ();
  *ps_sh << ");";   ps_sh->nl ();
  ps_sh->decr_indent ();
  *ps_sh << "};";   ps_sh->nl ();
  ps_sh->decr_indent ();
  ps_sh->nl ();
  *ps_sh << "#endif /* end #if !defined */";   ps_sh->nl ();
  ps_sh->nl ();
  ps_sh->nl ();
  *ps_sh << "#endif /* !TAO_USE_SEQUENCE_TEMPLATES */ ";   ps_sh->nl ();

  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "#if !defined (_" << name_space.c_str () << "_" << identifier.c_str () << "_CH_)";   ps_sh->nl ();
  *ps_sh << "#define _" << name_space.c_str () << "_" << identifier.c_str () << "_CH_";   ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "class " << identifier.c_str () << ";";   ps_sh->nl ();
  *ps_sh << "class " << identifier.c_str () << "_var;";   ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "// *************************************************************";   ps_sh->nl ();
  *ps_sh << "// " << identifier.c_str () ;   ps_sh->nl ();
  *ps_sh << "// *************************************************************";   ps_sh->nl ();

  ps_sh->nl ();

  *ps_sh << "class  " << identifier.c_str () << " : public";   ps_sh->nl ();

  ps_sh->nl ();

  *ps_sh << "#if !defined (TAO_USE_SEQUENCE_TEMPLATES)";   ps_sh->nl ();
  *ps_sh << "_TAO_Unbounded_Sequence_" << name_space.c_str () << "_" << identifier.c_str () ;   ps_sh->nl ();
  *ps_sh << "#else /* TAO_USE_SEQUENCE_TEMPLATES */";   ps_sh->nl ();
  *ps_sh << "TAO_Unbounded_Sequence<" << identifier.c_str () << "Component>";   ps_sh->nl ();
  *ps_sh << "#endif /* !TAO_USE_SEQUENCE_TEMPLATES */ ";   ps_sh->nl ();
  *ps_sh << "{";   ps_sh->nl ();
  ps_sh->incr_indent ();
  *ps_sh << "public:";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << " (void); // default ctor";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << " (CORBA::ULong max); // uses max size";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << " (";   ps_sh->nl ();
  *ps_sh << "CORBA::ULong max, ";   ps_sh->nl ();
  *ps_sh << "CORBA::ULong length, ";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << "Component *buffer, ";   ps_sh->nl ();
  *ps_sh << "CORBA::Boolean release = 0";   ps_sh->nl ();
  *ps_sh << ");";   ps_sh->nl ();

  *ps_sh  << identifier.c_str () << " (const " << identifier.c_str () << " &); // copy ctor";   ps_sh->nl ();
  *ps_sh << "~" << identifier.c_str () << " (void);";   ps_sh->nl ();
  *ps_sh << "static void _tao_any_destructor (void*);";   ps_sh->nl ();
  ps_sh->nl ();
  *ps_sh << "typedef " << identifier.c_str () << "_var _var_type;";   ps_sh->nl ();
  ps_sh->nl ();
  ps_sh->nl ();
  *ps_sh << "};";   ps_sh->nl ();
  ps_sh->decr_indent ();
  ps_sh->nl ();
  *ps_sh << "#endif /* end #if !defined */";   ps_sh->nl ();
  ps_sh->nl ();
  ps_sh->nl ();
  *ps_sh << "#if !defined (_" << name_space.c_str () << "_" << identifier.c_str () << "___VAR_CH_)";   ps_sh->nl ();
  *ps_sh << "#define _" << name_space.c_str () << "_" << identifier.c_str () << "___VAR_CH_";   ps_sh->nl ();
  ps_sh->nl ();
  *ps_sh << "// *************************************************************";   ps_sh->nl ();
  *ps_sh << "// class " << name_space.c_str () << "::" << identifier.c_str () << "_var";   ps_sh->nl ();
  *ps_sh << "// *************************************************************";   ps_sh->nl ();
  ps_sh->nl ();
  *ps_sh << "class  " << identifier.c_str () << "_var";   ps_sh->nl ();
  *ps_sh << "{";   ps_sh->nl ();
  ps_sh->incr_indent ();
  *ps_sh << "public:";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << "_var (void);";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << "_var (" << identifier.c_str () << " *);";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << "_var (const " << identifier.c_str () << "_var &);";   ps_sh->nl ();
  *ps_sh << "~" << identifier.c_str () << "_var (void);";   ps_sh->nl ();
  ps_sh->nl ();
  *ps_sh << identifier.c_str () << "_var &operator= (" << identifier.c_str () << " *);";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << "_var &operator= (const " << identifier.c_str () << "_var &);";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << " *operator-> (void);";   ps_sh->nl ();
  *ps_sh << "const " << identifier.c_str () << " *operator-> (void) const;";   ps_sh->nl ();
  ps_sh->nl ();
  *ps_sh << "operator const " << identifier.c_str () << " &() const;";   ps_sh->nl ();
  *ps_sh << "operator " << identifier.c_str () << " &();";   ps_sh->nl ();
  *ps_sh << "operator " << identifier.c_str () << " &() const;";   ps_sh->nl ();
  *ps_sh << "operator " << identifier.c_str () << " *&(); // variable-size base types only";   ps_sh->nl ();
  ps_sh->nl ();
  *ps_sh << identifier.c_str () << "Component & operator[] (CORBA::ULong index);";   ps_sh->nl ();
  *ps_sh << "const " << identifier.c_str () << "Component & operator[] (CORBA::ULong index) const;";

  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "// in, inout, out, _retn ";   ps_sh->nl ();
  *ps_sh << "const " << identifier.c_str () << " &in (void) const;";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << " &inout (void);";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << " *&out (void);";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << " *_retn (void);";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << " *ptr (void) const;";

  ps_sh->decr_indent (0);

  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "private:";

  ps_sh->incr_indent (0);
  ps_sh->nl ();

  *ps_sh << identifier.c_str () << " *ptr_;";   ps_sh->nl ();
  *ps_sh << "};";

  ps_sh->nl ();
  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "#endif /* end #if !defined */";   ps_sh->nl ();
  ps_sh->nl ();
  ps_sh->nl ();
  *ps_sh << "#if !defined (_" << name_space.c_str () << "_" << identifier.c_str () << "___OUT_CH_)";   ps_sh->nl ();
  *ps_sh << "#define _" << name_space.c_str () << "_" << identifier.c_str () << "___OUT_CH_";   ps_sh->nl ();
  ps_sh->nl ();
  *ps_sh << "class  " << identifier.c_str () << "_out";   ps_sh->nl ();
  *ps_sh << "{";   ps_sh->nl ();
  ps_sh->incr_indent ();
  *ps_sh << "public:";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << "_out (" << identifier.c_str () << " *&);";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << "_out (" << identifier.c_str () << "_var &);";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << "_out (const " << identifier.c_str () << "_out &);";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << "_out &operator= (const " << identifier.c_str () << "_out &);";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << "_out &operator= (" << identifier.c_str () << " *);";   ps_sh->nl ();
  *ps_sh << "operator " << identifier.c_str () << " *&();";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << " *&ptr (void);";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << " *operator-> (void);";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << "Component & operator[] (CORBA::ULong index);";   ps_sh->nl ();
  ps_sh->nl ();
  *ps_sh << "private:";   ps_sh->nl ();
  *ps_sh << identifier.c_str () << " *&ptr_;";   ps_sh->nl ();
  *ps_sh << "// Assignment from T_var not allowed.";   ps_sh->nl ();
  *ps_sh << "void operator= (const " << identifier.c_str () << "_var &);";   ps_sh->nl ();
  *ps_sh << "};";   ps_sh->nl ();
  ps_sh->decr_indent ();
  ps_sh->nl ();
  ps_sh->nl ();
  *ps_sh << "#endif /* end #if !defined */";   ps_sh->nl ();

  *ps_sh << "TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr "
         << "_tc_" << identifier.c_str () << ";";   ps_sh->nl ();
}

void
TAO_PSDL_Type_Dcl_Visitor::print_for_simple_typedefs (void)
{
  ACE_CString type;

  // Get the base type of the identifier name
  this->base_type (this->identifiers_ [0], type);

  // Get a pointer to PSDL_Stream of the stub header.
  TAO_PSDL_Stream *ps_sh = TAO_PSDL_Scope::instance ()->get_sh ();

  if (ACE_OS::strcmp (type.c_str (), "char *") == 0)
    {
      ps_sh->reset ();
      ps_sh->nl ();

      *ps_sh << "typedef " << type.c_str () << " "
             << this->identifiers_ [1].c_str () << ";";
      }

  ps_sh->nl ();
  *ps_sh << "typedef " << this->version_for_stub (type).c_str () << " "
         << this->identifiers_[1].c_str ();

  if (ACE_OS::strcmp (type.c_str (), "char *") == 0)
    *ps_sh << "_var;";
  else
    *ps_sh << ";";

  ps_sh->nl ();
  *ps_sh << "typedef " << this->version_for_stub (type).c_str () << "_out"
         << " " << this->identifiers_[1].c_str () << "_out;";   ps_sh->nl ();
  *ps_sh << "TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr"
         << " _tc_" << this->identifiers_[1].c_str () << ";";   ps_sh->nl ();
  ps_sh->nl ();

  // Generates code for the stub implementation files.
  this->gen_code_for_si ();
}

void
TAO_PSDL_Type_Dcl_Visitor::gen_code_for_si (void)
{
  TAO_PSDL_Stream *ps_si = TAO_PSDL_Scope::instance ()->get_si ();

  ps_si->reset ();
  ps_si->indent (); // start from current indentation level

  // Generate the typecode information here
  *ps_si << "static const CORBA::Long _oc_";

  // Flat name generation.
  *ps_si << this->identifiers_[1];
  *ps_si << "[] =";
  ps_si->nl ();
  *ps_si << "{";
  ps_si->nl ();

  // Some invocation call to put in the stuff that goes between
  // braces.
  *ps_si << "// Typecode related information should go in here";
  ps_si->nl ();

  *ps_si << "};";
  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "static CORBA::TypeCode _tc_TAO_tc_";
  *ps_si << this->identifiers_[1];
  *ps_si << " (";
  ps_si->nl ();
  ps_si->incr_indent ();
  *ps_si << "CORBA::tk_alias,";
  ps_si->nl ();
  *ps_si << "sizeof (_oc_" << this->identifiers_[1] << "),";
  ps_si->nl ();
  *ps_si << "(char *) &_oc_";
  *ps_si << this->identifiers_[1] << ",";
  ps_si->nl ();
  *ps_si << "0,";
  ps_si->nl ();
  *ps_si << "sizeof (" << this->identifiers_[1] << ")";
  ps_si->nl ();
  *ps_si << ");";
  ps_si->decr_indent ();

  ps_si->nl ();
  ps_si->nl ();
  *ps_si << "::CORBA::TypeCode_ptr _tc_" << this->identifiers_[1] << " =";
  ps_si->nl ();
  ps_si->incr_indent ();

  *ps_si << "&_tc_TAO_tc_" << this->identifiers_[1] << ";";
  ps_si->decr_indent ();
  ps_si->nl ();
  ps_si->nl ();


  return;
}

int
TAO_PSDL_Type_Dcl_Visitor::visit_predefined_type (TAO_PSDL_Predefined_Type *predefined_type)
{
  if (predefined_type->type_one () != 0)
    {
      this->identifiers_ [this->count_] =
        TAO_PSDL_Scope::instance ()->convert_str (predefined_type->type_one ());
      if (this->count_ == 0)
        {
          // @@ implies this is the type.
          this->predefined_type_ [this->count_] = 1;
        }
      ++this->count_;
    }

  if (predefined_type->type_two () != 0)
    {}

  if (predefined_type->type_three () != 0)
    {}

  if (predefined_type->type_of_variable_one () != 0)
    {
      if (predefined_type->type_of_variable_one ()->accept (this) == -1)
        return -1;
    }
  if (predefined_type->type_of_variable_two () != 0)
    {}
  if (predefined_type->type_of_variable_three () != 0)
    {}
  return 0;
}
