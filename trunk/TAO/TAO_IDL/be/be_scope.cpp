//
// $Id$
//
#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

// Default Constructor
be_scope::be_scope (void)
{
}

// Constructor
be_scope::be_scope (AST_Decl::NodeType type)
  : UTL_Scope (type)
{
}

be_scope::~be_scope (void)
{
}

// Code generation methods
int
be_scope::gen_client_header (void)
{
  UTL_ScopeActiveIterator *si;
  AST_Decl *d;
  be_decl *bd;

  if (this->nmembers () > 0)
    {
      // if there are elements in this scope

      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);
      // instantiate a scope iterator.

      while (!(si->is_done ()))
        {
          // get the next AST decl node
          d = si->item ();
          if (!d->imported ())
            {
              // we are not imported.

              // NOTE: Our assumption here is that whatever scope we are in, the
              // node type that shows up here *MUST* be valid according to the
              // IDL grammar. We do not check for this since the front end must
              // have taken care of weeding out such errors

              // The FE does some strange things in which items get added to
              // the enclosing scope as well as the scope that encloses the
              // enclosing scope. We do not want to generate code for items
              // that do not belong to the appropriate scope

              bd = be_decl::narrow_from_decl (d);
              // @@ Despite the comment above the following code will
              // crash without the if() around it.
              if (bd != 0)
                {
                  if (bd->gen_client_header () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         "(%N:%l) be_scope::gen_client_header failed\n"),
                                        -1);
                    }
                }
              else
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "WARNING (%N:%l) be_scope::gen_client_header - "
                              "narrow_from_decl returned 0\n"));
                }
            } // no imported
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

int
be_scope::gen_client_stubs (void)
{
  UTL_ScopeActiveIterator *si;
  AST_Decl *d;
  be_decl *bd;

  if (this->nmembers () > 0)
    {
      // if there are elements in this scope

      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);
      // instantiate a scope iterator.

      while (!(si->is_done ()))
        {
          // get the next AST decl node
          d = si->item ();
          if (!d->imported ())
            {
              // we are not imported.
              // NOTE: Our assumptin here is that whatever scope we are in, the
              // node type that shows up here *MUST* be valid according to the
              // IDL grammar. We do not check for this since the front end must
              // have taken care of weeding out such errors

              bd = be_decl::narrow_from_decl (d);
              // @@ Despite the comment above the following code will
              // crash without the if() around it.
              if (bd != 0)
                {
                  if (bd->gen_client_stubs () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         "(%N:%l) be_scope::gen_client_stubs failed\n"),
                                        -1);
                    }
                }
              else
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "WARNING (%N:%l) be_scope::gen_client_stubs - "
                              "narrow_from_decl returned 0\n"));
                }
            } // not imported
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

int
be_scope::gen_client_inline (void)
{
  UTL_ScopeActiveIterator *si;
  AST_Decl *d;
  be_decl *bd;

  if (this->nmembers () > 0)
    {
      // if there are elements in this scope

      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);
      // instantiate a scope iterator.

      while (!(si->is_done ()))
        {
          // get the next AST decl node
          d = si->item ();
          if (!d->imported ())
            {
              // we are not imported.
              // NOTE: Our assumptin here is that whatever scope we are in, the
              // node type that shows up here *MUST* be valid according to the
              // IDL grammar. We do not check for this since the front end must
              // have taken care of weeding out such errors

              bd = be_decl::narrow_from_decl (d);
              if (bd != 0)
                {
                  if (bd->gen_client_inline () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         "(%N:%l) be_scope::gen_client_inline failed\n"),
                                        -1);
                    }
                }
              else
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "WARNING (%N:%l) be_scope::gen_client_inline - "
                              "narrow_from_decl returned 0\n"));
                }
            } // not imported
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

int
be_scope::gen_server_header (void)
{
  UTL_ScopeActiveIterator *si;
  AST_Decl *d;
  be_decl *bd;

  if (this->nmembers () > 0)
    {
      // if there are elements in this scope

      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);
      // instantiate a scope iterator.

      while (!(si->is_done ()))
        {
          // get the next AST decl node
          d = si->item ();
          if (!d->imported ())
            {
              // we are not imported.
              // we are not imported.

          // NOTE: Our assumptin here is that whatever scope we are in, the
          // node type that shows up here *MUST* be valid according to the
          // IDL grammar. We do not check for this since the front end must
          // have taken care of weeding out such errors

              bd = be_decl::narrow_from_decl (d);
              if (bd != 0)
                {
                  if (bd->gen_server_header () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         "(%N:%l) be_scope::gen_server_header failed\n"),
                                        -1);
                    }
                }
              else
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "WARNING (%N:%l) be_scope::gen_server_header - "
                              "narrow_from_decl returned 0\n"));
                }
            } // not imported
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

int
be_scope::gen_server_skeletons (void)
{
  UTL_ScopeActiveIterator *si;
  AST_Decl *d;
  be_decl *bd;

  if (this->nmembers () > 0)
    {
      // if there are elements in this scope

      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);
      // instantiate a scope iterator.

      while (!(si->is_done ()))
        {
          // get the next AST decl node
          d = si->item ();
          if (!d->imported ())
            {
              // we are not imported.

              // NOTE: Our assumptin here is that whatever scope we are in, the
              // node type that shows up here *MUST* be valid according to the
              // IDL grammar. We do not check for this since the front end must
              // have taken care of weeding out such errors

              bd = be_decl::narrow_from_decl (d);
              if (bd != 0)
                {
                  if (bd->gen_server_skeletons () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         "(%N:%l) be_scope::gen_server_skeletons failed\n"),
                                        -1);
                    }
                }
              else
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "WARNING (%N:%l): be_scope::gen-server_skeletons - "
                              "narrow_from_decl returned 0\n"));
                }
            } // not imported
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

int
be_scope::gen_server_inline (void)
{
  UTL_ScopeActiveIterator *si;
  AST_Decl *d;
  be_decl *bd;

  if (this->nmembers () > 0)
    {
      // if there are elements in this scope

      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);
      // instantiate a scope iterator.

      while (!(si->is_done ()))
        {
          // get the next AST decl node
          d = si->item ();

          if (!d->imported ())
            {
              // we are not imported.
              // NOTE: Our assumptin here is that whatever scope we are in, the
              // node type that shows up here *MUST* be valid according to the
              // IDL grammar. We do not check for this since the front end must
              // have taken care of weeding out such errors

              bd = be_decl::narrow_from_decl (d);
              if (bd != 0)
                {
                  if (bd->gen_server_inline () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         "(%N:%l) be_scope::gen_server_inline failed\n"),
                                        -1);
                    }
                }
              else
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "WARNING (%N:%l): be_scope::gen_server_inline - "
                              "narrow_from_decl returned 0\n"));
                }
            }
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

int
be_scope::gen_encapsulation (void)
{
  UTL_ScopeActiveIterator *si;
  AST_Decl *d;
  be_decl *bd;

  // if there are elements in this scope
  if (this->nmembers () > 0)
    {
      // instantiate a scope iterator.
      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);

      while (!(si->is_done ()))
        {
          // get the next AST decl node
          d = si->item ();
          bd = be_decl::narrow_from_decl (d);
          if (bd->gen_encapsulation () == -1)
            {
              // failure
              return -1;
            }
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

long
be_scope::tc_encap_len (void)
{
  UTL_ScopeActiveIterator *si;
  AST_Decl *d;
  be_decl *bd;
  long encap_len = 0;

  if (this->nmembers () > 0)
    {
      // if there are elements in this scope

      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);
      // instantiate a scope iterator.

      while (!(si->is_done ()))
        {
          // get the next AST decl node
          d = si->item ();

          // NOTE: Our assumptin here is that whatever scope we are in, the
          // node type that shows up here *MUST* be valid according to the
          // IDL grammar. We do not check for this since the front end must
          // have taken care of weeding out such errors

          bd = be_decl::narrow_from_decl (d);
          if (bd != 0)
            {
              encap_len += bd->tc_encap_len ();
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "WARNING (%N:%l): be_scope::tc_encap_len - "
                          "narrow_from_decl returned 0\n"));
            }
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return encap_len;
}

// narrowing methods
IMPL_NARROW_METHODS1 (be_scope, UTL_Scope)
IMPL_NARROW_FROM_SCOPE (be_scope)
