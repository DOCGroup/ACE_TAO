//
// $Id$
//

#include "idl.h"
#include "be.h"
#include "be_visitor_args.h"

be_visitor_args_decl::be_visitor_args_decl (TAO_OutStream *stream)
  :  stream_ (stream)
{
}

be_visitor_args_decl::~be_visitor_args_decl (void)
{
}

void be_visitor_args_decl::argument_direction (int direction)
{
  this->argument_direction_ = direction;
}

void be_visitor_args_decl::current_type_name (UTL_ScopedName* name)
{
  this->current_type_name_ = name;
}

inline TAO_OutStream& be_visitor_args_decl::stream (void) const
{
  return *this->stream_;
}

int be_visitor_args_decl::visit_operation (be_operation *node)
{
  this->stream ().incr_indent (0);
  this->stream ().incr_indent (0);
  this->stream () << "(\n";

  // This will iterate over the arguments and call back upon us.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) visit_operation as scope failed\n"), -1);
    }

  // last argument - is always CORBA::Environment
  this->stream ().indent ();
  this->stream () << "CORBA::Environment &_tao_environment\n";
  this->stream ().decr_indent ();
  this->stream () << ")";
  this->stream ().decr_indent (0);
  return 0;
}

int be_visitor_args_decl::visit_argument (be_argument *node)
{
  be_type *type = be_type::narrow_from_decl (node->field_type ());
  this->stream ().indent ();

  // Different types have different mappings when used as in/out or
  // inout parameters.

  this->argument_direction_ = node->direction ();

  // Use the actual type name, not the name for the node, this is
  // useful for typedefs where the typedef name must be used, but the
  // base type mapping rules must apply.
  this->current_type_name_ = type->name ();

  type->accept (this);

  this->stream () << " " << node->local_name () << ",\n";
  return 0;
}

int be_visitor_args_decl::visit_predefined_type (be_predefined_type *node)
{
  // check if the type is an any
  if (node->pt () == AST_PredefinedType::PT_any)
    {
      switch (this->argument_direction_)
	{
	case AST_Argument::dir_IN:
	  this->stream () << "const " << node->name () << " &";
	  break;
	case AST_Argument::dir_INOUT:
	  this->stream () << node->name () << " &";
	  break;
	case AST_Argument::dir_OUT:
	  this->stream () << node->name () << "_out ";
	  break;
	} // end switch direction
    } // end of if
  else if (node->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
    {
      switch (this->argument_direction_)
	{
	case AST_Argument::dir_IN:
	  this->stream () << node->name () << "_ptr ";
	  break;
	case AST_Argument::dir_INOUT:
	  this->stream () << node->name () << "_ptr &";
	  break;
	case AST_Argument::dir_OUT:
	  this->stream () << node->name () << "_out ";
	  break;
	} // end switch direction
    } // end else if
  else // simple predefined types
    {
      switch (this->argument_direction_)
	{
	case AST_Argument::dir_IN:
	  this->stream () << node->name ();
	  break;
	case AST_Argument::dir_INOUT:
	  this->stream () << node->name () << " &";
	  break;
	case AST_Argument::dir_OUT:
	  this->stream () << node->name () << "_out ";
	  break;
	} // end switch direction
    } // end of else

  return 0;
}

int be_visitor_args_decl::visit_interface (be_interface *)
{
  return this->dump_interface ();
}

int be_visitor_args_decl::visit_interface_fwd (be_interface_fwd *)
{
  return this->dump_interface ();
}

int be_visitor_args_decl::dump_interface (void) const
{
  switch (this->argument_direction_)
    {
    case AST_Argument::dir_IN:
      this->stream () << this->current_type_name_ << "_ptr ";
      break;
    case AST_Argument::dir_INOUT: // inout
      this->stream () << this->current_type_name_ << "_ptr &";
      break;
    case AST_Argument::dir_OUT:
      this->stream () << this->current_type_name_ << "_out ";
      break;
    }
  return 0;
}

int be_visitor_args_decl::visit_structure (be_structure *)
{
  return this->dump_structure ();
}

int be_visitor_args_decl::dump_structure () const
{
  switch (this->argument_direction_)
    {
    case AST_Argument::dir_IN:
      this->stream () << "const " << this->current_type_name_ << " &";
      break;
    case AST_Argument::dir_INOUT:
      this->stream () << this->current_type_name_ << " &";
      break;
    case AST_Argument::dir_OUT:
      this->stream () << this->current_type_name_ << "_out";
      break;
    }
  return 0;
}

int be_visitor_args_decl::visit_enum (be_enum *node)
{
  switch (this->argument_direction_)
    {
    case AST_Argument::dir_IN:
      this->stream () << node->name ();
      break;
    case AST_Argument::dir_INOUT:
      this->stream () << node->name () << " &";
      break;
    case AST_Argument::dir_OUT:
      this->stream () << node->name () << "_out";
      break;
    }
  return 0;
}

int be_visitor_args_decl::visit_union (be_union *)
{
  return this->dump_structure ();
}

int be_visitor_args_decl::visit_array (be_array *node)
{
  switch (this->argument_direction_)
    {
    case AST_Argument::dir_IN:
      this->stream () << "const " << this->current_type_name_;
      break;
    case AST_Argument::dir_INOUT:
      this->stream () << this->current_type_name_;
      if (node->size_type () == be_decl::VARIABLE)
	{
	  this->stream () << "_slice *";
	}
      break;
    case AST_Argument::dir_OUT:
      this->stream () << this->current_type_name_ << "_out";
      break;
    }
  return 0;
}

int be_visitor_args_decl::visit_sequence (be_sequence *)
{
  return this->dump_structure ();
}

int be_visitor_args_decl::visit_string (be_string *)
{
  switch (this->argument_direction_)
    {
    case AST_Argument::dir_IN:
      this->stream () << "const char*";
      break;
    case AST_Argument::dir_INOUT:
      this->stream () << "char*";
      break;
    case AST_Argument::dir_OUT:
      this->stream () << "CORBA::String_out";
      break;
    }
  return 0;
}

int be_visitor_args_decl::visit_typedef (be_typedef *node)
{
  return node->primitive_base_type ()->accept (this);
}

int be_visitor_args_decl::visit_native (be_native *)
{
  switch (this->argument_direction_)
    {
    case AST_Argument::dir_IN:
      this->stream () << this->current_type_name_;
      break;
    case AST_Argument::dir_INOUT:
      this->stream () << this->current_type_name_ << " &";
      break;
    case AST_Argument::dir_OUT:
      this->stream () << this->current_type_name_ << " &";
      break;
    }
  return 0;
}
