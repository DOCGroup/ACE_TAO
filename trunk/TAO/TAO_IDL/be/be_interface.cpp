#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Interface
 */
be_interface::be_interface()
{
}

be_interface::be_interface(UTL_ScopedName *n, AST_Interface **ih, long nih,
			   UTL_StrList *p)
	    : AST_Interface(n, ih, nih, p),
	      AST_Decl(AST_Decl::NT_interface, n, p),
	      UTL_Scope(AST_Decl::NT_interface)
{
}

void be_interface::gen_client_header()
{
#if 0
  if (idl_global->in_main_file())
    {
      // if we are defined in the main file, then generate the code
      long	i;

      o << "interface ";
      local_name()->dump(o);
      o << " ";
      if (pd_n_inherits > 0) {
	o << ": ";
	for (i = 0; i < pd_n_inherits; i++) {
	  pd_inherits[i]->local_name()->dump(o);
	  if (i < pd_n_inherits - 1)
	    o << ", ";
	}
      }
      o << " {\n";
      UTL_Scope::dump(o);
      idl_global->indent()->skip_to(o);
      o << "}";
    }
#endif
}

void be_interface::gen_client_stubs()
{
}

void be_interface::gen_server_header()
{
}

void be_interface::gen_server_skeletons()
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_interface, AST_Interface)
IMPL_NARROW_FROM_DECL(be_interface)
IMPL_NARROW_FROM_SCOPE(be_interface)

