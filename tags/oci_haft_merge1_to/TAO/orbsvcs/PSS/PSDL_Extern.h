/* -*- C++ -*- */
// $Id$
//
// ==================================================================
//
// = LIBRARY
//    PSS
//
// = FILENAME
//    PSDL_Extern
//
// = DESCRIPTION
//    Wrapper to make available yacc/lex related parameters outside
//    the PSDL library.
//
// = AUTHOR
//    Priyanka Gontla <gontla_p@ociweb.com>
//
// ==================================================================

#ifndef TAO_PSDL_EXTERN_H
#define TAO_PSDL_EXTERN_H
#include "ace/pre.h"

#include "psdl_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/FILE.h"

//Forward declaration
class TAO_PSDL_Node;

class TAO_PSDL_Export TAO_PSDL_Extern
{
public:

  TAO_PSDL_Extern (void);

  int TAO_PSDL_Extern_yyparse (void *);

  void TAO_PSDL_Extern_yyin (FILE *);

  TAO_PSDL_Node *TAO_PSDL_Extern_yyval (void);
};

#include "ace/post.h"
#endif /* TAO_PSDL_EXTERN_H */
