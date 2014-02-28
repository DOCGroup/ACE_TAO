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

// AST_String nodes represent IDL string declarations.
// AST_String is a subclass of AST_ConcreteType.
// AST_String nodes have a maximum size (an AST_Expression which must
// evaluate to a positive integer).

#include "ast_string.h"
#include "ast_expression.h"
#include "ast_visitor.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "utl_identifier.h"
#include "idl_defines.h"
#include "global_extern.h"

// Note we are overlooking the wstring case here.
AST_Decl::NodeType const
AST_String::NT = AST_Decl::NT_string;

AST_String::AST_String (AST_Decl::NodeType nt,
                        UTL_ScopedName *n,
                        AST_Expression *ms,
                        long wide)
  : COMMON_Base (),
    AST_Decl (nt, n, true),
    AST_Type (nt, n),
    AST_ConcreteType (nt, n),
    pd_max_size (ms),
    pd_width (wide)
{
  // Always the case.
  this->size_type (AST_Type::VARIABLE);

  Identifier *id = 0;
  UTL_ScopedName *new_name = 0;
  UTL_ScopedName *conc_name = 0;
  bool narrow = this->width () == (ssize_t) sizeof (char);

  ACE_NEW (id,
           Identifier (narrow ? "char *" : "WChar *"));

  ACE_NEW (conc_name,
           UTL_ScopedName (id,
                           0));

  if (narrow)
    {
      new_name = conc_name;
    }
  else
    {
      ACE_NEW (id,
               Identifier ("CORBA"));

      ACE_NEW (new_name,
               UTL_ScopedName (id,
                               conc_name));
    }

  this->set_name (new_name);

  unsigned long bound = ms->ev ()->u.ulval;

  static char namebuf[NAMEBUFSIZE];
  static char boundbuf[NAMEBUFSIZE];
  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);
  ACE_OS::memset (boundbuf,
                  '\0',
                  NAMEBUFSIZE);

  if (bound)
    {
      ACE_OS::sprintf (boundbuf,
                       "_%ld",
                       bound);
    }

  ACE_OS::sprintf (namebuf,
                   "CORBA_%sSTRING%s",
                   (wide == 1 ? "" : "W"),
                   boundbuf);

  this->flat_name_ = ACE::strnew (namebuf);
}

AST_String::~AST_String (void)
{
}

// Redefinition of inherited virtual operations.

// Dump this AST_String node to the ostream o.
void
AST_String::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "string <");
  this->pd_max_size->dump (o);
  this->dump_i (o, ">");
}

int
AST_String::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_string (this);
}

void
AST_String::destroy (void)
{
  this->pd_max_size->destroy ();
  delete this->pd_max_size;
  this->pd_max_size = 0;

  this->AST_ConcreteType::destroy ();
}

// Data accessors.

AST_Expression *
AST_String::max_size (void)
{
  return this->pd_max_size;
}

long
AST_String::width (void)
{
  return this->pd_width;
}



IMPL_NARROW_FROM_DECL(AST_String)
