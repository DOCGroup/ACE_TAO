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

/*
 * fe_init.cc - Initialize the FE
 *
 * The FE initialization is carried out in two stages, with the BE
 * initialization protocol sandwiched between the two stages.
 *
 * The first stage is responsible for creating the scopes stack.
 * The second stage is run after the BE initialization has created
 * and returned an instance of AST_Generator (or a subclass). This
 * instance is used to create the root node for the AST, and to
 * populate it with AST_PredefinedType nodes which represent the
 * predefined IDL types. This AST root is then pushed on the scopes
 * stack as the outermost scope.
 */

#include        "idl.h"
#include        "idl_extern.h"

#include        "fe_private.h"

ACE_RCSID(fe, fe_init, "$Id$")

/*
 * Create a scoped name
 */
static UTL_ScopedName *
create_scoped_name(char *s)
{
  return new UTL_ScopedName(new Identifier(s, 1, 0, I_FALSE), NULL);
}

/*
 * Populate the global scope with all predefined entities
 */
void
fe_populate(AST_Module *m)
{
  AST_PredefinedType *pdt;

  pdt = idl_global->gen()->create_predefined_type(AST_PredefinedType::PT_long,
                                                  create_scoped_name("long"),
                                                  NULL);
  m->fe_add_predefined_type(pdt);

  pdt =
    idl_global->gen()->create_predefined_type(AST_PredefinedType::PT_ulong,
                                           create_scoped_name("unsigned long"),
                                           NULL);
  m->fe_add_predefined_type(pdt);
  pdt =
    idl_global->gen()
        ->create_predefined_type(AST_PredefinedType::PT_longlong,
                                 create_scoped_name("long long"),
                                 NULL);
  m->fe_add_predefined_type(pdt);
  pdt =
    idl_global->gen()
        ->create_predefined_type(AST_PredefinedType::PT_ulonglong,
                                 create_scoped_name("unsigned long long"),
                                 NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()->create_predefined_type(AST_PredefinedType::PT_short,
                                                  create_scoped_name("short"),
                                                  NULL);
  m->fe_add_predefined_type(pdt);
  pdt =
   idl_global->gen()->create_predefined_type(AST_PredefinedType::PT_ushort,
                                          create_scoped_name("unsigned short"),
                                          NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()->create_predefined_type(AST_PredefinedType::PT_float,
                                                  create_scoped_name("float"),
                                                  NULL);
  m->fe_add_predefined_type(pdt);
  pdt =
    idl_global->gen()->create_predefined_type(AST_PredefinedType::PT_double,
                                              create_scoped_name("double"),
                                              NULL);
  m->fe_add_predefined_type(pdt);
  pdt =
    idl_global->gen()
        ->create_predefined_type(AST_PredefinedType::PT_longdouble,
                                 create_scoped_name("long double"),
                                 NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()->create_predefined_type(AST_PredefinedType::PT_char,
                                                  create_scoped_name("char"),
                                                  NULL);
  m->fe_add_predefined_type(pdt);
  pdt =
    idl_global->gen()
        ->create_predefined_type(AST_PredefinedType::PT_wchar,
                                 create_scoped_name("wchar"),
                                 NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()->create_predefined_type(AST_PredefinedType::PT_octet,
                                                  create_scoped_name("octet"),
                                                  NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()->create_predefined_type(AST_PredefinedType::PT_any,
                                                  create_scoped_name("any"),
                                                  NULL);
  m->fe_add_predefined_type(pdt);
  pdt =
    idl_global->gen()->create_predefined_type(AST_PredefinedType::PT_boolean,
                                              create_scoped_name("boolean"),
                                              NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()->create_predefined_type(AST_PredefinedType::PT_void,
                                                  create_scoped_name("void"),
                                                  NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("Object"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("TypeCode"),
                                         NULL);
  m->fe_add_predefined_type(pdt);

  /*
   * Add these to make all keywords protected even in different spellings
   */

  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("attribute"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("case"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("const"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("context"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("default"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("enum"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("exception"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("in"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("out"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("inout"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("interface"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("module"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("oneway"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("raises"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("readonly"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("sequence"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("string"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("wstring"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("struct"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("switch"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("typedef"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("union"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("unsigned"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("TRUE"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
  pdt = idl_global->gen()
                ->create_predefined_type(AST_PredefinedType::PT_pseudo,
                                         create_scoped_name("FALSE"),
                                         NULL);
  m->fe_add_predefined_type(pdt);
}

/*
 * Initialization stage 1: create global scopes stack
 */
void
FE_init_stage1()
{
  idl_global->set_scopes(new UTL_ScopeStack());
  if (idl_global->scopes() == NULL) {
    cerr << GTDEVEL("IDL: FE init failed to create scope stack, exiting\n");
    ACE_OS::exit (99);
  }
}

/*
 * Initialization stage 2: create global scope and populate it
 */
void
FE_init_stage2()
{
  AST_Root      *r;

  /*
   * Check that the BE init created a generator object
   */
  if (idl_global->gen() == NULL) {
    cerr << GTDEVEL("IDL: BE did not initialize idl_global->gen(), exiting\n");
    ACE_OS::exit (99);
  }
  /*
   * Create a global root for the AST. Note that the AST root has no
   * name
   */
  r = idl_global->gen()->create_root(create_scoped_name(""), NULL);
  idl_global->set_root(r);
  if (r == NULL) {
    cerr << GTDEVEL("IDL: FE init failed to create AST root, exiting\n");
    ACE_OS::exit (99);
  }
  /*
   * Push it on the stack
   */
  idl_global->scopes()->push(idl_global->root());
  /*
   * Populate it with nodes for predefined types
   */
  fe_populate(idl_global->root());
  /*
   * Set flag to indicate we are processing the main file now
   */
  idl_global->set_in_main_file(I_TRUE);
}
