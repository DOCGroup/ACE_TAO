//=============================================================================
/**
*  @file   amh_sh.cpp
*
*  Specialized interface visitor for AMH generates code that is
*  specific to AMH interfaces.
*
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*/
//=============================================================================

#include "interface.h"

#include <string>

be_visitor_amh_interface_sh::be_visitor_amh_interface_sh (
    be_visitor_context *ctx)
  : be_visitor_interface_sh (ctx)
{
}

be_visitor_amh_interface_sh::~be_visitor_amh_interface_sh () = default;

// The node is the original interface node but we 'tweak' with the
// local_name and the the operation signatures to generate the AMH
// skeleton on the 'fly'.

int
be_visitor_amh_interface_sh::visit_interface (be_interface *node)
{
  if (node->srv_hdr_gen () || node->imported () || node->is_local ())
    {
      return 0;
    }

  // Do not generate AMH classes for any sort of implied IDL.
  if (node->original_interface () != nullptr)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  std::string class_name;

  TAO_INSERT_COMMENT (os);

  // We shall have a POA_ prefix only if we are at the topmost level.
  if (!node->is_nested ())
    {
      // We are outermost.
      class_name += "POA_AMH_";
      class_name += node->local_name ();
    }
  else
    {
      class_name += "AMH_";
      class_name += node->local_name ();
    }

  // Generate the skeleton class name.
  *os << "class " << class_name << ";" << be_nl;

  // Generate the _ptr declaration.
  *os << "using " << class_name << "_ptr = "
      << class_name << " *;" << be_nl_2;

  // Now generate the class definition.
  *os << "class " << be_global->skel_export_macro ()
      << " " << class_name << be_idt_nl << ": " << be_idt;

  long const n_parents = node->n_inherits ();

  if (n_parents > 0)
    {
      for (int i = 0; i < n_parents; ++i)
        {
          std::string amh_name ("POA_");

          be_interface *base =
            dynamic_cast<be_interface*> (node->inherits ()[i]);
          std::string const full_name = base->compute_full_name ("AMH_", "");
          amh_name += full_name;

          if (i != 0)
            {
              *os << ", ";
            }

          *os << "public virtual "
              << amh_name
              << be_nl;
        }
    }
  else
    {
      // We don't inherit from another user defined object, hence our
      // base class is the ServantBase class.
      *os << "public virtual PortableServer::ServantBase";
    }

  *os << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "protected:" << be_idt_nl
      << class_name << " ();" << be_uidt_nl << be_nl
      << "public:" << be_idt_nl;

  // No copy constructor for locality constraint interface.
  *os << class_name << " (const " << class_name
      << "& rhs);" << be_nl
      << "virtual ~" << class_name << " () = default;\n\n"
      << be_nl
      << "virtual ::CORBA::Boolean _is_a (const char* logical_type_id);" << be_nl_2;

  // Add the dispatch method.
  *os << "virtual void _dispatch (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &req," << be_nl
      << "TAO::Portable_Server::Servant_Upcall *_servant_upcall);" << be_uidt
      << be_uidt_nl << be_nl;

  this->this_method (node);

  // The _interface_repository_id method.
  *os << be_nl
      << "virtual const char* _interface_repository_id "
      << "() const;";

  if (this->visit_scope (node) ==  -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_amh_interface_sh::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_amh_interface_sh::visit_operation (be_operation *node)
{
  be_visitor_amh_operation_sh visitor (this->ctx_);
  return visitor.visit_operation (node);
}

int
be_visitor_amh_interface_sh::visit_attribute (be_attribute *node)
{
  be_visitor_amh_operation_sh visitor (this->ctx_);
  return visitor.visit_attribute (node);
}

void
be_visitor_amh_interface_sh::this_method (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  std::string non_amh_name (node->client_enclosing_scope ());
  non_amh_name += node->local_name ();

  // Print out the _this() method.  The _this() method for AMH
  // interfaces is "special", because the returned type is not exactly
  // the type of the class, but the original class that "implied" the
  // AMH one.
  *os << non_amh_name << " *_this ();\n" << be_uidt;
}
