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

// AST_PredefinedType nodes denote the various predefined types such
// as long, short, etc. that are available in IDL. Each AST_PredefinedType
// node has a field (the value of this field is from the enum
// AST_PredefinedType::PredefinedType) which denotes the specific predefined
// type that this node represents. There is only one node in the entire
// AST which represents each predefined type, such as long etc.

#include "ast_predefined_type.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "global_extern.h"
#include "ace/Log_Msg.h"

ACE_RCSID (ast, 
           ast_predefined_type, 
           "$Id$")

AST_PredefinedType::AST_PredefinedType (void)
  : pd_pt (PT_long)
{
}

AST_PredefinedType::AST_PredefinedType (PredefinedType t,
                                        UTL_ScopedName *n)
  : AST_Decl (AST_Decl::NT_pre_defined,
              n,
              I_TRUE),
    pd_pt (t)
{
  UTL_ScopedName *new_name = 0;
  Identifier *id = 0;

  // Generate a new Scoped Name for us such that we belong to the CORBA
  // namespace.
  if (t == AST_PredefinedType::PT_void)
    {
      ACE_NEW (id,
               Identifier (n->last_component ()->get_string ()));

      ACE_NEW (new_name,
               UTL_ScopedName (id,
                               0));
    }
  else
    {
      ACE_NEW (id,
               Identifier (idl_global->nest_orb () ? "NORB" : "CORBA"));

      ACE_NEW (new_name,
               UTL_ScopedName (id,
                               0));

      UTL_ScopedName *conc_name = 0;

      switch (this->pt ())
        {
        case AST_PredefinedType::PT_long:
          ACE_NEW (id,
                   Identifier ("Long"));
          break;
        case AST_PredefinedType::PT_ulong:
          ACE_NEW (id,
                   Identifier ("ULong"));
          break;
        case AST_PredefinedType::PT_short:
          ACE_NEW (id,
                   Identifier ("Short"));
          break;
        case AST_PredefinedType::PT_ushort:
          ACE_NEW (id,
                   Identifier ("UShort"));
          break;
        case AST_PredefinedType::PT_float:
          ACE_NEW (id,
                   Identifier ("Float"));
          break;
        case AST_PredefinedType::PT_double:
          ACE_NEW (id,
                   Identifier ("Double"));
          break;
        case AST_PredefinedType::PT_char:
          ACE_NEW (id,
                   Identifier ("Char"));
          break;
        case AST_PredefinedType::PT_octet:
          ACE_NEW (id,
                   Identifier ("Octet"));
          break;
        case AST_PredefinedType::PT_wchar:
          ACE_NEW (id,
                   Identifier ("WChar"));
          break;
        case AST_PredefinedType::PT_boolean:
          ACE_NEW (id,
                   Identifier ("Boolean"));
          break;
        case AST_PredefinedType::PT_longlong:
          ACE_NEW (id,
                   Identifier ("LongLong"));
          break;
        case AST_PredefinedType::PT_ulonglong:
          ACE_NEW (id,
                   Identifier ("ULongLong"));
          break;
        case AST_PredefinedType::PT_longdouble:
          ACE_NEW (id,
                   Identifier ("LongDouble"));
          break;
        case AST_PredefinedType::PT_any:
          ACE_NEW (id,
                   Identifier ("Any"));
          break;
        case AST_PredefinedType::PT_object:
          ACE_NEW (id,
                   Identifier ("Object"));
          break;
        case AST_PredefinedType::PT_value:
          ACE_NEW (id,
                   Identifier ("ValueBase"));
          break;
        case AST_PredefinedType::PT_pseudo:
          ACE_NEW (id,
                   Identifier (n->last_component ()->get_string ()));
          break;
        default:
          ACE_ERROR ((LM_ERROR,
                      "AST_PredefinedType - bad enum value\n"));
        }

      ACE_NEW (conc_name,
               UTL_ScopedName (id,
                               0));

      new_name->nconc (conc_name);
    }

  this->set_name (new_name);
}

AST_PredefinedType::~AST_PredefinedType (void)
{
}

// Redefinition of inherited virtual operations.

// Dump this AST_PredefinedType node to the ostream o.
void
AST_PredefinedType::dump (ACE_OSTREAM_TYPE &o)
{
  AST_Decl::dump (o);
}

// Compute the size type of the node in question.
int
AST_PredefinedType::compute_size_type (void)
{
  switch (this->pd_pt)
  {
    case AST_PredefinedType::PT_any:
    case AST_PredefinedType::PT_pseudo:
    case AST_PredefinedType::PT_object:
      this->size_type (AST_Type::VARIABLE);
      break;
    default:
      this->size_type (AST_Type::FIXED);
      break;
  }

  return 0;
}

int
AST_PredefinedType::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_predefined_type (this);
}

void
AST_PredefinedType::destroy (void)
{
  this->AST_Type::destroy ();
}

// Data accessors.

AST_PredefinedType::PredefinedType
AST_PredefinedType::pt (void)
{
  return this->pd_pt;
}

// Narrowing.
IMPL_NARROW_METHODS1(AST_PredefinedType, AST_ConcreteType)
IMPL_NARROW_FROM_DECL(AST_PredefinedType)
