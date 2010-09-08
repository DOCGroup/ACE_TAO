// -*- C++ -*-
// $Id$

#include "PSDL_Exception_Visitor.h"
#include "PSDL_Stream.h"



TAO_PSDL_Exception_Visitor::TAO_PSDL_Exception_Visitor (void)
  : exception_name_ ()
{
  // Constructor
}

TAO_PSDL_Exception_Visitor::~TAO_PSDL_Exception_Visitor (void)
{
  // Destructor
}

int
TAO_PSDL_Exception_Visitor::visit_identifier (TAO_PSDL_Identifier *identifier)
{
  // Save the identifiers for future use.
  this->identifiers_.size (this->count_ +1);

  this->identifiers_ [this->count_] = identifier->value ();

  // Increment the count..for future purposes.
  ++this->count_;

  return 0;
}

int
TAO_PSDL_Exception_Visitor::visit_except_dcl (TAO_PSDL_Except_Dcl *except_dcl)
{
  // This is where the control first comes to this class. Set the
  // initial size of this->identifiers_
  this->identifiers_.size (1);

  if (except_dcl->identifier ()->accept (this) == -1)
    return -1;

  // Save the exception name.
  this->exception_name_ = this->identifiers_ [this->count_ - 1];

  if (except_dcl->member_list () != 0)
    if (except_dcl->member_list ()->accept (this) == -1)
      return -1;

  // Print the code that should go into the generated header file.
  // @@ Update this method to get the code that should go to
  // implementation method (as applies).
  this->print_class_for_exception ();

  return 0;
}

int
TAO_PSDL_Exception_Visitor::visit_member_list (TAO_PSDL_Member_List *member_list)
{
  if (member_list->member ()->accept (this) == -1)
    return -1;

  if (member_list->member_list () != 0)
    {
      if (member_list->member_list ()->accept (this) == -1)
        return -1;
    }
  return 0;
}

int
TAO_PSDL_Exception_Visitor::visit_predefined_type (TAO_PSDL_Predefined_Type *predefined_type)
{
  if (predefined_type->type_one () != 0)
    {
      this->identifiers_.size (this->count_ +1);

      // Save the type of the identifier.
      this->identifiers_ [this->count_] =
        TAO_PSDL_Scope::instance ()->convert_str (predefined_type->type_one ());
      ++this->count_;
    }
  return 0;
}

void
TAO_PSDL_Exception_Visitor::print_class_for_exception (void)
{
  // Get a pointer to PSDL_Stream of the stub header.
  TAO_PSDL_Stream *ps_sh = TAO_PSDL_Scope::instance ()->get_sh ();

  // Code that should be generated to the stub (idlC.h) according to
  // the specification.
  ps_sh->nl ();
  ps_sh->nl ();

  ACE_CString name_space =
    TAO_PSDL_Scope::instance ()->get_name_space ();

  ACE_CString interface_name =
    TAO_PSDL_Scope::instance ()->get_interface_name ();

  *ps_sh << "#if !defined (_" << name_space << "_"
       << interface_name << "_" << this->exception_name_ << "_CH_)\n";
  *ps_sh << "#define _" << name_space << "_" << interface_name
       << "_" << this->exception_name_ << "_CH_\n";
    ps_sh->nl ();
  *ps_sh << "    class  " << this->exception_name_
       << " : public CORBA::UserException\n";
  *ps_sh << "    {\n";

  *ps_sh << "    public:\n";

  ACE_CString type;
  for (unsigned int i = 1; i < this->count_; ++i)
    {
      if (this->base_type (this->identifiers_[i], type))
        {
          if (ACE_OS::strcmp (type.c_str (), "char *") == 0)
            {
              *ps_sh << "      " << "TAO::String_Manager"
                   << " " << this->identifiers_[i+1]
                   << ";\n";
            }
          else
            {
              *ps_sh << "      " << this->version_for_stub (type)
               << " " << this->identifiers_[i+1]
               << ";\n";
            }
        }
      else
        {
          // Types defined previously in the idl.
          *ps_sh << "      ACE_NESTED_CLASS ("
            //@@@deal      << base_node_info.module_name << "::"
            //@@@deal   << base_node_info.interface_name
               << ", " << this->identifiers_[i]
               << ") " << this->identifiers_[i+1]
               << ";\n";
        }
      ++i;
    }

  *ps_sh << "      " << this->exception_name_
       << " (void);\n";
  *ps_sh << "      " << this->exception_name_
       << " (const " << this->exception_name_ << " &);\n";
  *ps_sh << "      ~" << this->exception_name_ << " (void);\n";
  *ps_sh << "\n";
  *ps_sh << "      " << this->exception_name_
         << " &operator= (const " << this->exception_name_ << " &);\n";
  *ps_sh << "\n";
  *ps_sh << "      static void _tao_any_destructor (void*);\n";
  *ps_sh << "\n";
  *ps_sh << "      static " << this->exception_name_
       << " *_downcast (CORBA::Exception *);\n";
  *ps_sh << "      static CORBA::Exception *_alloc (void);\n";
  *ps_sh << "\n";
  *ps_sh << "      virtual CORBA::Exception *_tao_duplicate (void) const;\n";
  *ps_sh << "\n";
  *ps_sh << "      virtual void _raise (void);\n";
  *ps_sh << "\n";
  *ps_sh << "      virtual void _tao_encode (\n";
  *ps_sh << "          TAO_OutputCDR &\n";
  *ps_sh << "\n";
  *ps_sh << "        ) const;\n";
  *ps_sh << "     \n";
  *ps_sh << "      virtual void _tao_decode (\n";
  *ps_sh << "          TAO_InputCDR &\n";
  *ps_sh << "\n";
  *ps_sh << "        );\n";
  *ps_sh << "     \n";

  if (this->identifiers_.size () > 1)
    {
      *ps_sh << "      " << this->exception_name_ << " (\n";

      for (unsigned int i = 1; i < this->count_; ++i)
        {
          if (this->base_type (this->identifiers_[i], type))
            {
              if (ACE_OS::strcmp (type.c_str (), "char *") == 0)
                {
                  *ps_sh << "      " << "const char *"
                       << " " << this->identifiers_[i+1]
                       << ";\n";
                }
              else
                {
                  *ps_sh << "      " << this->version_for_stub (type)
                       << " " << this->identifiers_[i+1]
                       << ";\n";
                }
            }
          else
            {
              // Types defined previously in the idl.
              *ps_sh << "      ACE_NESTED_CLASS ("
                   << name_space << "::" << interface_name
                   << ", " << this->identifiers_[i]
                   << ") " << this->identifiers_[i+1]
                   << ";\n";
            }
          ++i;
        }
    }

  *ps_sh << "     \n";
  *ps_sh << "      virtual CORBA::TypeCode_ptr _type (void) const;\n";
  *ps_sh << "    };\n";
  *ps_sh << "\n";
  *ps_sh << "static ::CORBA::TypeCode_ptr _tc_"
       << this->exception_name_ << ";\n";
  *ps_sh << "   \n";
  *ps_sh << "   \n";
  *ps_sh << "#endif /* end #if !defined */\n";
  *ps_sh << "   \n";
}
