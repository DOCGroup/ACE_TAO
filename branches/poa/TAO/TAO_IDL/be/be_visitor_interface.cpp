//
// $Id$
//

#include "idl.h"
#include "nr_extern.h"
#include "be.h"
#include "be_visitor_args.h"
#include "be_visitor_interface.h"

be_visitor_collocated_sh::be_visitor_collocated_sh (void)
{
}

be_visitor_collocated_sh::~be_visitor_collocated_sh (void)
{
}

int be_visitor_collocated_sh::visit_interface (be_interface *node)
{
  TAO_NL  nl;        // end line

  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  TAO_OutStream *os = cg->server_header ();

  os->gen_ifdef_macro (node->flatname (), "_collocated");

  os->indent ();
  *os << "class " << node->local_coll_name ();
  os->incr_indent ();
  *os << ": public virtual " << node->name ();

  if (node->n_inherits () > 0)
    {
      for (int i = 0; i < node->n_inherits (); ++i)
	{
	  *os << "," << nl;
	  be_interface* parent =
	    be_interface::narrow_from_decl (node->inherits()[i]);
	  *os << "  public virtual " << parent->full_coll_name ();
	}
    }
  *os << "\n";
  os->decr_indent ();
  *os << "{" << nl;
  *os << "public:\n";
  os->incr_indent ();

  *os << node->local_coll_name () << " (";

  AST_Decl *d = ScopeAsDecl (node->defined_in ());
  if (d->node_type () == AST_Decl::NT_root)
    {
      // The skeleton name is the outermost, we need to printout the
      // POA_ prefix that goes with it.
      *os << "POA_";
    }

  *os << node->local_name () << "_ptr "
      << " servant);\n";

  os->indent ();
  if (d->node_type () == AST_Decl::NT_root)
    {
      // The skeleton name is the outermost, we need to printout the
      // POA_ prefix that goes with it.
      *os << "POA_";
    }
  *os << node->local_name ()
      << "_ptr _get_servant (void) const;\n";

  if (node->nmembers () > 0)
    {
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
		      UTL_ScopeActiveIterator (node,
					       UTL_Scope::IK_decls),
		      -1);
      while (!si->is_done ())
	{
	  AST_Decl *d = si->item ();
	  si->next ();
	  be_decl *bd = be_decl::narrow_from_decl (d);
	  if (d->imported ()
	      || bd == 0
	      || bd->node_type () != AST_Decl::NT_op)
	    {
	      continue;
	    }
	  if (bd->accept (this) == -1)
	    {
	      delete si;
	      return -1;
	    }
	}
      delete si;
    }

  os->decr_indent ();

  *os << "\nprivate:\n";
  os->incr_indent ();
  if (d->node_type () == AST_Decl::NT_root)
    {
      // The skeleton name is the outermost, we need to printout the
      // POA_ prefix that goes with it.
      *os << "POA_";
    }
  *os << node->local_name () << "_ptr servant_;\n";
  os->decr_indent ();
  *os << "};\n\n";

  os->gen_endif ();

  return 0;
}

int be_visitor_collocated_sh::visit_operation (be_operation *node)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_OPERATION_SH);

  TAO_OutStream *sh = cg->server_header ();
  sh->indent (); // start with the current indentation level

  // every operation is declared virtual
  *sh << "virtual ";

  // first generate the return type
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (bt->write_as_return (sh, bt) == -1)
    {
      return -1;
    }

  // generate the operation name
  *sh << " " << node->local_name () << " ";

  be_visitor_args_decl visitor (sh);
  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) args decl failed\n"), -1);
    }
  *sh << ";\n";

  return 0;
}



be_visitor_collocated_ss::be_visitor_collocated_ss (void)
  :  current_interface_ (0)
{
}

be_visitor_collocated_ss::~be_visitor_collocated_ss (void)
{
}

int be_visitor_collocated_ss::visit_interface (be_interface *node)
{
  TAO_NL nl;

  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  TAO_OutStream *ss = cg->server_skeletons ();

  this->current_interface_ = node;

  *ss << this->current_interface_->full_coll_name () << "::"
      << this->current_interface_->local_coll_name () << " ("
      << node->full_skel_name () << "_ptr "
      << " servant)\n";
  ss->incr_indent ();
  *ss << ": ";
  if (this->current_interface_->n_inherits () > 0)
    {
      for (int i = 0; i < node->n_inherits (); ++i)
	{
	  be_interface* parent =
	    be_interface::narrow_from_decl (this->current_interface_->inherits()[i]);
#if defined (ACE_WIN32)
	  // @@ TODO MSVC++ compiler has some kind of issue (read
	  // *bug*) wrt nested classes in constructors, if the fully
	  // qualified name is used it gets all confused. Quite to my
	  // dismay the work around is to use a non-qualified name for
	  // the base class!
	  // I wish I never have to know why the symbol table for
	  // MSVC++ can get so confused ;-) (coryan)
	  *ss << "  " << parent->local_coll_name () << " (servant),"
	      << nl;
#else
	  *ss << "  " << parent->full_coll_name () << " (servant),"
	      << nl;
#endif /* ACE_WIN32 */
	}
    }

  *ss << "  servant_ (servant)";

  *ss << "\n";
  ss->decr_indent ();
  *ss << "{\n";
  ss->incr_indent ();
  *ss << "this->set_parent (servant->get_parent ());\n";
  ss->decr_indent ();
  *ss << "}\n\n";

  ss->indent ();
  *ss << this->current_interface_->full_skel_name () << "_ptr "
      << this->current_interface_->full_coll_name () << "::"
      << "_get_servant (void) const\n"
      << "{\n";
  ss->incr_indent ();
  *ss << "return this->servant_;\n";
  ss->decr_indent ();
  *ss << "}\n\n";

  if (node->nmembers () > 0)
    {
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
		      UTL_ScopeActiveIterator (node,
					       UTL_Scope::IK_decls),
		      -1);
      while (!si->is_done ())
	{
	  AST_Decl *d = si->item ();
	  si->next ();
	  be_decl *bd = be_decl::narrow_from_decl (d);
	  // Only printout the operations, nested interfaces and
	  // structures only go in the main declaration.
	  if (d->imported () || bd == 0)
	    {
	      continue;
	    }
	  if (bd->accept (this) == -1)
	    {
	      delete si;
	      return -1;
	    }
	}
      delete si;
    }

  return 0;
}

int be_visitor_collocated_ss::visit_operation (be_operation *node)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  TAO_OutStream *ss = cg->server_skeletons ();
  TAO_NL  nl;

  // retrieve the return type again because we have used bt to also retrieve
  // the argument types
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (bt->write_as_return (ss, bt) == -1)
    {
      return -1;
    }

  *ss << " " << current_interface_->full_coll_name () << "::"
      << node->local_name () << " ";

  be_visitor_args_decl visitor (ss);
  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) args decl failed\n"), -1);
    }
  
  *ss << "\n";
  ss->indent ();
  *ss << "{\n";
  ss->incr_indent ();
  
  if (bt->node_type () != AST_Decl::NT_pre_defined
      || be_predefined_type::narrow_from_decl (bt)->pt () != AST_PredefinedType::PT_void)
    {
      *ss << "return ";
    }
  
  *ss << "this->servant_->" << node->local_name () << " (\n";
  ss->incr_indent (0);
  ss->incr_indent (0);

  this->visit_scope (node);

  ss->indent ();
  *ss << " _tao_environment\n";
  ss->decr_indent ();
  *ss << ");\n";
  ss->decr_indent (0);
  ss->decr_indent (0);
  *ss << "}\n\n";
  
  return 0;
}

int be_visitor_collocated_ss::visit_argument (be_argument *node)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  TAO_OutStream *ss = cg->server_skeletons ();

  ss->indent ();
  *ss << node->local_name () << ",\n";
  return 0;
}
