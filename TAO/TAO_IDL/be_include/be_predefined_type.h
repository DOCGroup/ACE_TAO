/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_predefined_type.h
//
// = DESCRIPTION
//    Extension of class AST_PredefinedType that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (_BE_PREDEFINED_TYPE_H)
#define _BE_PREDEFINED_TYPE_H

// be_classes.hh
//
// Defines all classes for dummy BE

/*
 * BE_PredefinedType
 */
class be_predefined_type : public virtual AST_PredefinedType,
                           public virtual be_type
{
public:
  // =Operations

  be_predefined_type (void);
  // default constructor

  be_predefined_type (AST_PredefinedType::PredefinedType t, UTL_ScopedName *n,
                      UTL_StrList *p);
  // constructor

  virtual int gen_client_header (void);
  // Generates the client-side header information for the predefined type 

  virtual int gen_client_stubs (void);
  // Generates the client-side stubs for the predefined type

  virtual int gen_server_header (void);
  // Generates the server-side header information for the predefined type 

  virtual int gen_server_skeletons (void);
  // Generates the server-side skeletons for the predefined type

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the predefined type

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the predefined type

  virtual int gen_typecode (void);
  // generate the typecode

  const char *const gen_corba_mapping (void);
  // helper for all the above virtual methods

  // Narrowing
  DEF_NARROW_METHODS2 (be_predefined_type, AST_PredefinedType, be_type);
  DEF_NARROW_FROM_DECL (be_predefined_type);

protected:
  virtual void compute_tc_name (void);
  // overridden method
};

#endif
