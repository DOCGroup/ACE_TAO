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

#pragma ident "%@(#)utl_global.cc	1.19% %92/06/10% Sun Microsystems"

// utl_global.cc - Implementation of class IDL_GlobalData
//

#include	"idl.h"
#include	"idl_extern.h"

// Define an increment for the size of the array used to store names of
// included files
#undef		INCREMENT
#define		INCREMENT	64

IDL_GlobalData::IDL_GlobalData()
    : pd_scopes(NULL),
      pd_root(NULL),
      pd_gen(NULL),
      pd_err(NULL),
      pd_err_count(0),
      pd_lineno(0),
      pd_filename(NULL),
      pd_main_filename(NULL),
      pd_real_filename(NULL),
      pd_in_main_file(I_FALSE),
      pd_prog_name(NULL),
      pd_cpp_location(NULL),
      pd_compile_flags(0),
      pd_be(NULL),
      pd_local_escapes(NULL),
      pd_indent(NULL),
      pd_pragmas(NULL),
      pd_read_from_stdin(I_FALSE),
      pd_include_file_names(NULL),
      pd_n_include_file_names(0),
      pd_n_alloced_file_names(0),
      pd_parse_state(PS_NoState)
{
  // empty
}

// Get or set scopes stack
UTL_ScopeStack *
IDL_GlobalData::scopes()
{
  return pd_scopes;
}
void
IDL_GlobalData::set_scopes(UTL_ScopeStack *s)
{
  pd_scopes = s;
}

// Get or set root of AST
AST_Root *
IDL_GlobalData::root()
{
  return pd_root;
}
void
IDL_GlobalData::set_root(AST_Root *r)
{
  pd_root = r;
}

// Get or set generator object
AST_Generator *
IDL_GlobalData::gen()
{
  return pd_gen;
}
void
IDL_GlobalData::set_gen(AST_Generator *g)
{
  pd_gen = g;
}

// Get or set error object
UTL_Error *
IDL_GlobalData::err()
{
  return pd_err;
}
void
IDL_GlobalData::set_err(UTL_Error *e)
{
  pd_err = e;
}

// Get or set error count
long
IDL_GlobalData::err_count()
{
  return pd_err_count;
}
void
IDL_GlobalData::set_err_count(long c)
{
  pd_err_count = c;
}

// Get or set line number
long
IDL_GlobalData::lineno()
{
  return pd_lineno;
}
void
IDL_GlobalData::set_lineno(long n)
{
  pd_lineno = n;
}

// Get or set file name being read now
String *
IDL_GlobalData::filename()
{
  return pd_filename;
}
void
IDL_GlobalData::set_filename(String *f)
{
  pd_filename = f;
}

// Get or set main file name
String *
IDL_GlobalData::main_filename()
{
  return pd_main_filename;
}
void
IDL_GlobalData::set_main_filename(String *n)
{
  pd_main_filename = n;
}

// Get or set real file name
String *
IDL_GlobalData::real_filename()
{
  return pd_real_filename;
}
void
IDL_GlobalData::set_real_filename(String *n)
{
  pd_real_filename = n;
}

// Get or set indicator whether import is on
idl_bool
IDL_GlobalData::imported()
{
  return pd_in_main_file ? I_FALSE : pd_import;
}
idl_bool
IDL_GlobalData::import()
{
  return pd_import;
}
void
IDL_GlobalData::set_import(idl_bool is_in)
{
  pd_import = is_in;
}

// Get or set indicator whether we're reading the main file now
idl_bool
IDL_GlobalData::in_main_file()
{
  return pd_in_main_file;
}
void
IDL_GlobalData::set_in_main_file(idl_bool is_in)
{
  pd_in_main_file = is_in;
}

// Get or set stripped file name
String *
IDL_GlobalData::stripped_filename()
{
  return pd_stripped_filename;
}
void
IDL_GlobalData::set_stripped_filename(String *nm)
{
  pd_stripped_filename = nm;
}

// Get or set cache value for argv[0]
char *
IDL_GlobalData::prog_name()
{
  return pd_prog_name;
}
void
IDL_GlobalData::set_prog_name(char *pn)
{
  pd_prog_name = pn;
}

// Get or set location to find C preprocessor
char *
IDL_GlobalData::cpp_location()
{
  return pd_cpp_location;
}
void
IDL_GlobalData::set_cpp_location(char *l)
{
  pd_cpp_location = l;
}

// Get or set IDL compiler flags
long
IDL_GlobalData::compile_flags()
{
  return pd_compile_flags;
}
void
IDL_GlobalData::set_compile_flags(long cf)
{
  pd_compile_flags = cf;
}

// Get or set BE to be used
char *
IDL_GlobalData::be()
{
  return pd_be;
}
void
IDL_GlobalData::set_be(char *nbe)
{
  pd_be = nbe;
}

// Get or set local escapes string. This provides additional mechanism
// to pass information to a BE.
char *
IDL_GlobalData::local_escapes()
{
  return pd_local_escapes;
}
void
IDL_GlobalData::set_local_escapes(char *e)
{
  pd_local_escapes = e;
}

// Get or set indent object
UTL_Indenter *
IDL_GlobalData::indent()
{
  return pd_indent;
}
void
IDL_GlobalData::set_indent(UTL_Indenter *i)
{
  pd_indent = i;
}

// Get or set list of pragmas being parsed
UTL_StrList *
IDL_GlobalData::pragmas()
{
  UTL_StrList *p = pd_pragmas;

  pd_pragmas = NULL;
  return p;
}
void
IDL_GlobalData::set_pragmas(UTL_StrList *p)
{
  pd_pragmas = p;
}

// Get or set indicator whether we're reading from stdin
idl_bool
IDL_GlobalData::read_from_stdin()
{
  return pd_read_from_stdin;
}
void
IDL_GlobalData::set_read_from_stdin(idl_bool r)
{
  pd_read_from_stdin = r;
}

// Have we seen this include file name before?
long
IDL_GlobalData::seen_include_file_before(String *n)
{
  unsigned long i;

  for (i = 0; i < pd_n_include_file_names; i++)
    if (n->compare(pd_include_file_names[i]))
      return I_TRUE;
  return I_FALSE;
}

// Store a name of an #include file
void
IDL_GlobalData::store_include_file_name(String *n)
{
  String	**o_include_file_names;
  unsigned long	  o_n_alloced_file_names,
		  i;

  /*
   * Check if we need to store it at all or whether we've seen it already
   */
  if (seen_include_file_before(n))
    return;
  /*
   * OK, need to store. Make sure there's space for one more string
   */
  if (pd_n_include_file_names == pd_n_alloced_file_names) {
    if (pd_n_alloced_file_names == 0) {
      pd_n_alloced_file_names = INCREMENT;
      pd_include_file_names = new String *[pd_n_alloced_file_names];
    } else {
      o_include_file_names = pd_include_file_names;
      o_n_alloced_file_names = pd_n_alloced_file_names;
      pd_n_alloced_file_names += INCREMENT;
      pd_include_file_names = new String *[pd_n_alloced_file_names];
      for (i = 0; i < o_n_alloced_file_names; i++)
        pd_include_file_names[i] = o_include_file_names[i];
      delete [] o_include_file_names;
    }
  }
  /*
   * Store it
   */
  pd_include_file_names[pd_n_include_file_names++] = n;
}

void
IDL_GlobalData::set_include_file_names(String **ns)
{
  pd_include_file_names = ns;
}

String **
IDL_GlobalData::include_file_names()
{
  return pd_include_file_names;
}

void
IDL_GlobalData::set_n_include_file_names(unsigned long n)
{
  pd_n_include_file_names = n;
}

unsigned long
IDL_GlobalData::n_include_file_names()
{
  return pd_n_include_file_names;
}

void
IDL_GlobalData::set_parse_state(ParseState ps)
{
  pd_parse_state = ps;
}

IDL_GlobalData::ParseState
IDL_GlobalData::parse_state()
{
  return pd_parse_state;
}

/*
 * Convert a PredefinedType to an ExprType
 */
AST_Expression::ExprType
IDL_GlobalData::PredefinedTypeToExprType(AST_PredefinedType::PredefinedType pt)
{
  switch (pt) {
  case AST_PredefinedType::PT_long:
    return AST_Expression::EV_long;
  case AST_PredefinedType::PT_ulong:
    return AST_Expression::EV_ulong;
  case AST_PredefinedType::PT_short:
    return AST_Expression::EV_short;
  case AST_PredefinedType::PT_ushort:
    return AST_Expression::EV_ushort;
  case AST_PredefinedType::PT_float:
    return AST_Expression::EV_float;
  case AST_PredefinedType::PT_double:
    return AST_Expression::EV_double;
  case AST_PredefinedType::PT_char:
    return AST_Expression::EV_char;
  case AST_PredefinedType::PT_octet:
    return AST_Expression::EV_octet;
  case AST_PredefinedType::PT_boolean:
    return AST_Expression::EV_bool;
  default:
    return AST_Expression::EV_any;
  }
}

// returns the IDL source file being copiled
String* IDL_GlobalData::idl_src_file()
{
  return this->pd_idl_src_file;
}

// set the source IDL file that is being parsed
void IDL_GlobalData::idl_src_file(String *s)
{
  this->pd_idl_src_file = s;
}

/************ Helper functions **************/
const char *
IDL_GlobalData::be_get_client_hdr_fname ()
{
  String *s;
  char *base;
  static char fname[MAXNAMELEN];

  ACE_OS::memset (fname, 0, MAXNAMELEN);
  s = idl_global->idl_src_file (); 
  // get the IDL source file name
  base = ACE_OS::strstr (s->get_string (), ".idl");
  // get the base part of the filename
  if (base == NULL)
    return 0;

  ACE_OS::strncpy (fname, s->get_string (), base - s->get_string ());
  ACE_OS::strcat (fname, "C.h");
  return fname;
}

const char * 
IDL_GlobalData::be_get_client_stub_fname ()
{
  String *s;
  char *base;
  static char fname[MAXNAMELEN];

  ACE_OS::memset (fname, 0, MAXNAMELEN);
  s = idl_global->idl_src_file (); 
  // get the IDL source file name
  base = ACE_OS::strstr (s->get_string (), ".idl");
  // get the base part of the filename
  if (base == NULL)
    return 0;

  ACE_OS::strncpy (fname, s->get_string (), base - s->get_string ());
  ACE_OS::strcat (fname, "C.cpp");
  return fname;
}

const char * 
IDL_GlobalData::be_get_client_inline_fname ()
{
  String *s;
  char *base;
  static char fname[MAXNAMELEN];

  ACE_OS::memset (fname, 0, MAXNAMELEN);
  s = idl_global->idl_src_file (); 
  // get the IDL source file name
  base = ACE_OS::strstr (s->get_string (), ".idl");
  // get the base part of the filename
  if (base == NULL)
    return 0;

  ACE_OS::strncpy (fname, s->get_string (), base - s->get_string ());
  ACE_OS::strcat (fname, "C.i");
  return fname;
}

const char * 
IDL_GlobalData::be_get_server_hdr_fname ()
{
  String *s;
  char *base;
  static char fname[MAXNAMELEN];

  ACE_OS::memset (fname, 0, MAXNAMELEN);
  s = idl_global->idl_src_file (); 
  // get the IDL source file name
  base = ACE_OS::strstr (s->get_string (), ".idl");
  // get the base part of the filename
  if (base == NULL)
    return 0;

  ACE_OS::strncpy (fname, s->get_string (), base - s->get_string ());
  ACE_OS::strcat (fname, "S.h");
  return fname;
}

const char * 
IDL_GlobalData::be_get_server_inline_fname ()
{
  String *s;
  char *base;
  static char fname[MAXNAMELEN];

  ACE_OS::memset (fname, 0, MAXNAMELEN);
  s = idl_global->idl_src_file (); 
  // get the IDL source file name
  base = ACE_OS::strstr (s->get_string (), ".idl");
  // get the base part of the filename
  if (base == NULL)
    return 0;

  ACE_OS::strncpy (fname, s->get_string (), base - s->get_string ());
  ACE_OS::strcat (fname, "S.i");
  return fname;
}

const char * 
IDL_GlobalData::be_get_server_skeleton_fname ()
{
  String *s;
  char *base;
  static char fname[MAXNAMELEN];

  ACE_OS::memset (fname, 0, MAXNAMELEN);
  s = idl_global->idl_src_file (); 
  // get the IDL source file name
  base = ACE_OS::strstr (s->get_string (), ".idl");
  // get the base part of the filename
  if (base == NULL)
    return 0;

  ACE_OS::strncpy (fname, s->get_string (), base - s->get_string ());
  ACE_OS::strcat (fname, "S.cpp");
  return fname;
}


