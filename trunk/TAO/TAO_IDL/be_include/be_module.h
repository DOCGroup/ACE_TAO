/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_module.h
//
// = DESCRIPTION
//    Extension of class AST_Module that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (_BE_MODULE_H)
#define _BE_MODULE_H

/*
 * BE_Module
 */

class be_module : public virtual AST_Module, 
                  public virtual be_scope,
                  public virtual be_decl
{
public:
  // =Operations

  be_module (void);
  // default constructor

  be_module (UTL_ScopedName *n, UTL_StrList *p);
  // constructor

  virtual int gen_client_header (void);
  // Generates the client-side header information for the module 

  virtual int gen_client_stubs (void);
  // Generates the client-side stubs for the module

  virtual int gen_server_header (void);
  // Generates the server-side header information for the module 

  virtual int gen_server_skeletons (void);
  // Generates the server-side skeletons for the module

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the module

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the module

  // Narrowing
  DEF_NARROW_METHODS3 (be_module, AST_Module, be_scope, be_decl);
  DEF_NARROW_FROM_DECL (be_module);
  DEF_NARROW_FROM_SCOPE (be_module);
};

#endif
