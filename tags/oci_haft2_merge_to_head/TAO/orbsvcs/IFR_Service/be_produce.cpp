// $Id$

/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.
2550 Garcia Avenue
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

 */

// BE_produce.cc - Produce the work of the BE - does nothing in the
//                 dummy BE

#include "global_extern.h"
#include "TAO_IFR_BE_Export.h"
#include "be_extern.h"
#include "ast_root.h"
#include "ifr_visitor_macro.h"
#include "ifr_removing_visitor.h"
#include "ifr_adding_visitor.h"

ACE_RCSID (be,
           be_produce,
           "$Id$")

// Clean up before exit, whether successful or not.
// Need not be exported since it is called only from this file.
void
BE_cleanup (void)
{
  idl_global->destroy ();
  delete idl_global;
  idl_global = 0;

  // Remove the holding scope entry from the repository.
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::Contained_var result =
        be_global->repository ()->lookup_id (be_global->holding_scope_name ()
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!CORBA::is_nil (result.in ()))
        {
          CORBA::ModuleDef_var scope =
            CORBA::ModuleDef::_narrow (result.in ()
                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          scope->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("BE_cleanup"));
    }
  ACE_ENDTRY;

  delete be_global;
  be_global = 0;
}

// Abort this run of the BE.
TAO_IFR_BE_Export void
BE_abort (void)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Fatal Error - Aborting\n")));

  BE_cleanup ();

  ACE_OS::exit (1);
}

// ac must be passed in by reference, because it is also
// passed by reference to ORB_init, which may modify it.
// After BE_ifr_init returns to main() the modified argc
// must be passed to DRV_parse_args().
TAO_IFR_BE_Export int
BE_ifr_init (int &ac,
             char *av[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      be_global->orb (CORBA::ORB_init (ac,
                                       av,
                                       0
                                       ACE_ENV_ARG_PARAMETER));
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        be_global->orb ()->resolve_initial_references ("InterfaceRepository"
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (object.in ()))
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("Null objref from resolve_initial_references\n")
            ),
            -1
          );
        }

      CORBA::Repository_var repo = 
        CORBA::Repository::_narrow (object.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (repo.in ()))
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("CORBA::Repository::_narrow failed\n")
            ),
            -1
          );
        }

      be_global->repository (repo._retn ());
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("BE_ifr_init"));

      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

void
BE_create_holding_scope (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::ModuleDef_ptr scope = CORBA::ModuleDef::_nil ();

  // If we are multi-threaded, it may already be created.
  CORBA::Contained_var result =
    be_global->repository ()->lookup_id (be_global->holding_scope_name ()
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (result.in ()))
    {
      scope =
        be_global->repository ()->create_module (
                                      be_global->holding_scope_name (),
                                      be_global->holding_scope_name (),
                                      "1.0"
                                      ACE_ENV_ARG_PARAMETER
                                    );
      ACE_CHECK;
    }
  else
    {
      scope = CORBA::ModuleDef::_narrow (result.in ()
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  be_global->holding_scope (scope);
}

// Do the work of this BE. This is the starting point for code generation.
TAO_IFR_BE_Export void
BE_produce (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
//      BE_create_holding_scope (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the root node.
      AST_Decl *d = idl_global->root ();
      AST_Root *root = AST_Root::narrow_from_decl (d);

      if (root == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%N:%l) BE_produce - ")
                      ACE_TEXT ("No Root\n")));

          BE_abort ();
        }

      if (be_global->removing ())
        {
          ifr_removing_visitor visitor;

          TAO_IFR_VISITOR_WRITE_GUARD;

          // If the visitor is dispatched this way, we need to override
          // only visit_scope() for the removing visitor.
          if (visitor.visit_scope (root) == -1)
            {
              ACE_ERROR ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) BE_produce -")
                  ACE_TEXT (" failed to accept removing visitor\n")
                ));

              BE_abort ();
            }
        }
      else
        {
          ifr_adding_visitor visitor (d);

          TAO_IFR_VISITOR_WRITE_GUARD;

          if (root->ast_accept (&visitor) == -1)
            {
              ACE_ERROR ((
                  LM_ERROR,
                  ACE_TEXT ("(%N:%l) BE_produce -")
                  ACE_TEXT (" failed to accept adding visitor\n")
                ));

              BE_abort ();
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("BE_produce"));

    }
  ACE_ENDTRY;

  // Clean up.
  BE_cleanup ();
}
