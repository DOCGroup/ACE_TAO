// -*- C++ -*-
// $Id$

#include "PSDL_Enum_Type_Visitor.h"
#include "PSDL_Stream.h"



TAO_PSDL_Enum_Type_Visitor::TAO_PSDL_Enum_Type_Visitor (void)
{
}

TAO_PSDL_Enum_Type_Visitor::~TAO_PSDL_Enum_Type_Visitor (void)
{
}

int
TAO_PSDL_Enum_Type_Visitor::visit_identifier (TAO_PSDL_Identifier *identifier)
{
  // Save the identifiers for future use.
  this->identifiers_.size (this->count_ + 1);

  this->identifiers_ [this->count_] = identifier->value ();

  ++this->count_;
  return 0;
}

int
TAO_PSDL_Enum_Type_Visitor::visit_enumerator (TAO_PSDL_Enumerator *enumerator)
{
  // Gets the enum name and saves it in the global array of
  // identifiers.
  if (enumerator->identifier ()->accept (this) == -1)
    return -1;

  // Get a pointer to PSDL_Stream of the stub header.
  TAO_PSDL_Stream *ps_sh = TAO_PSDL_Scope::instance ()->get_sh ();

  *ps_sh<< "  " << this->identifiers_ [this->count_ - 1]
       << ";\n";

  // Get the enum members and save them.
  if (enumerator->enumerator () != 0)
    if (enumerator->enumerator ()->accept (this) == -1)
      return -1;

  return 0;
}

int
TAO_PSDL_Enum_Type_Visitor::visit_predefined_type (TAO_PSDL_Predefined_Type *predefined_type)
{
  if (predefined_type->type_of_variable_one () != 0)
    {
      if (predefined_type->type_of_variable_one ()->accept (this) == -1)
          return -1;
    }

  ACE_CString enum_type_name = this->identifiers_ [this->count_ - 1];

  // Get a pointer to PSDL_Stream of the stub header.
  TAO_PSDL_Stream *ps_sh = TAO_PSDL_Scope::instance ()->get_sh ();

  *ps_sh<< "enum " << enum_type_name.c_str () << "\n";
  *ps_sh<< "{\n";

  if (predefined_type->type_of_variable_two () != 0)
    {
      if (predefined_type->type_of_variable_two ()->accept (this) == -1)
        return -1;
    }

  *ps_sh<< "};\n";

  // Print the code that should go into the generated header file.
  // @@ Update this method to get the code that should go to
  // implementation method (as applies).
  this->print_class_for_enum_type (enum_type_name);

  return 0;
}

void
TAO_PSDL_Enum_Type_Visitor::print_class_for_enum_type (ACE_CString enum_type_name)
{
// Get a pointer to PSDL_Stream of the stub header.
  TAO_PSDL_Stream *ps_sh = TAO_PSDL_Scope::instance ()->get_sh ();

  ps_sh->nl ();

  *ps_sh << "typedef " << enum_type_name << " &"
         << enum_type_name << "_out;\n";

  *ps_sh<< "TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr "
       << "_tc_" << enum_type_name << ";\n";

  ps_sh->nl ();

}
