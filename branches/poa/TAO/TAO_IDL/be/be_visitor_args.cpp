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
  this->stream () << ");\n";
  this->stream ().decr_indent (0);
  return 0;
}

int be_visitor_args_decl::visit_argument (be_argument *node)
{
  be_type *type = be_type::narrow_from_decl (node->field_type ());
  this->stream ().indent ();

  // Different types have different mappings when used as in/out or
  // inout parameters.
  // IMHO those should be methods on the type, reducing the amount of
  // switching just to the IN/OUT/INOUT on each type
  // @@ TODO.

  // LEVEL (1) switch on node type of the "type"
  switch (type->base_node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
    case AST_Decl::NT_interface_fwd: // type is an obj reference
      {
        switch (node->direction ())
          {
          case AST_Argument::dir_IN:
	    this->stream () << type->name () << "_ptr ";
	    break;
          case AST_Argument::dir_INOUT: // inout
	    this->stream () << type->name () << "_ptr &";
            break;
          case AST_Argument::dir_OUT:
	    this->stream () << type->name () << "_out ";
            break;
          }
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        be_predefined_type *bpd = be_predefined_type::narrow_from_decl (type);

        // check if the type is an any
        if (bpd->pt () == AST_PredefinedType::PT_any)
          {
            switch (node->direction ())
              {
              case AST_Argument::dir_IN:
		this->stream () << "const " << type->name () << " &";
		break;
              case AST_Argument::dir_INOUT:
		this->stream () << type->name () << " &";
                break;
              case AST_Argument::dir_OUT:
		this->stream () << type->name () << "_out ";
                break;
              } // end switch direction
          } // end of if
        else if (bpd->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
          {
            switch (node->direction ())
              {
              case AST_Argument::dir_IN:
		this->stream () << type->name () << "_ptr ";
                break;
              case AST_Argument::dir_INOUT:
		this->stream () << type->name () << "_ptr &";
		  ", ";
                break;
              case AST_Argument::dir_OUT:
		this->stream () << type->name () << "_out ";
                break;
              } // end switch direction
          } // end else if
        else // simple predefined types
          {
            switch (node->direction ())
              {
              case AST_Argument::dir_IN:
		this->stream () << type->name ();
                break;
              case AST_Argument::dir_INOUT:
		this->stream () << type->name () << " &";
                break;
              case AST_Argument::dir_OUT:
		this->stream () << type->name () << "_out ";
                break;
              } // end switch direction
          } // end of else
      } // end of case predefined
      break;
    case AST_Decl::NT_string: // type is a string
      {
        switch (node->direction ())
          {
          case AST_Argument::dir_IN:
	    this->stream () << "const ";
	    if (type->node_type () == AST_Decl::NT_typedef)
	      this->stream () << type->name ();
	    else
	      this->stream () << "char *";
	    break;
          case AST_Argument::dir_INOUT:
	    if (type->node_type () == AST_Decl::NT_typedef)
	      this->stream () << type->name ();
	    else
	      this->stream () << "char *";
            break;
          case AST_Argument::dir_OUT:
	    if (type->node_type () == AST_Decl::NT_typedef)
	      this->stream () << type->name () << "_out";
	    else
	      this->stream () << "CORBA::String_out";
            break;
          } // end switch direction
      } // end case string
      break;
    case AST_Decl::NT_array: // type is an array
      switch (node->direction ())
        {
        case AST_Argument::dir_IN:
	  this->stream () << "const " << type->name ();
          break;
        case AST_Argument::dir_INOUT:
	  this->stream () << type->name ();
	  if (type->size_type () == be_decl::VARIABLE)
	    {
	      this->stream () << "_slice *";
	    }
          break;
        case AST_Argument::dir_OUT:
	  this->stream () << type->name () << "_out";
	  break;
        } // end of switch direction
      break;
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      switch (node->direction ())
        {
        case AST_Argument::dir_IN:
	  this->stream () << "const " << type->name () << " &";
          break;
        case AST_Argument::dir_INOUT:
	  this->stream () << type->name () << " &";
          break;
        case AST_Argument::dir_OUT:
	  this->stream () << type->name () << "_out";
          break;
        } // end switch direction
      break;
    case AST_Decl::NT_enum: // type is an enum
      switch (node->direction ())
        {
        case AST_Argument::dir_IN:
	  this->stream () << type->name ();
          break;
        case AST_Argument::dir_INOUT:
	  this->stream () << type->name () << " &";
          break;
        case AST_Argument::dir_OUT:
	  this->stream () << type->name () << "_out";
          break;
        } // end switch direction
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG TODO: is this allowed ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
	ACE_ERROR_RETURN ((LM_ERROR,
			   "base_node_type cannot be typedef\n"), -1);
      } // end of case
      break;
    } //end switch node type
  this->stream () << " " << node->local_name () << ",\n";
  return 0;
}
