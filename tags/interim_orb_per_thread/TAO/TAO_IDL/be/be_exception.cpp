#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Exception
 */
be_exception::be_exception (void)
{
}

be_exception::be_exception (UTL_ScopedName *n, UTL_StrList *p)
	    : AST_Decl (AST_Decl::NT_except, n, p),
              AST_Structure (AST_Decl::NT_except, n, p),
	      UTL_Scope (AST_Decl::NT_except),
              member_count_ (-1)
{
  // computes the repoID
  compute_repoID ();

  // computes the fully scoped name
  compute_fullname ();

  // computes the fully scoped typecode name
  compute_tc_name ();

  // compute the flattened fully scoped name 
  compute_flatname ();
}

// compute total number of members
int
be_exception::compute_member_count (void)
{
  UTL_ScopeActiveIterator *si;  // iterator
  AST_Decl *d;  // temp node

  this->member_count_ = 0;

  // if there are elements in this scope
  if (this->nmembers () > 0)
    {
      // instantiate a scope iterator.
      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);

      while (!(si->is_done ())) 
	{
	  // get the next AST decl node
	  d = si->item ();
	  if (!d->imported ()) 
	    {
              this->member_count_++;
            }
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

// return the member count
int
be_exception::member_count (void)
{
  if (this->member_count_ == -1)
    this->compute_member_count ();

  return this->member_count_;
}


int
be_exception::gen_client_header (void)
{
  return 0;
}

int
be_exception::gen_client_inline (void)
{
  return 0;
}

int
be_exception::gen_client_stubs (void)
{
  return 0;
}

int
be_exception::gen_server_header (void)
{
  return 0;
}

int
be_exception::gen_server_inline (void)
{
  return 0;
}

int
be_exception::gen_server_skeletons (void)
{
  return 0;
}

int
be_exception::gen_typecode (void)
{
  return 0;
}

long
be_exception::tc_encap_len (void)
{
  return 0;
}

// Narrowing
IMPL_NARROW_METHODS3 (be_exception, AST_Exception, be_scope, be_decl)
IMPL_NARROW_FROM_DECL (be_exception)
IMPL_NARROW_FROM_SCOPE (be_exception)

