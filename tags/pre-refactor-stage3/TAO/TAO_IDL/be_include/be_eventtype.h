/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_eventtype.h
//
// = DESCRIPTION
//    Extension of class AST_EventType and be_valuetype that provides
//    additional means for C++ mapping of an valuetype.
//
// ============================================================================

#ifndef TAO_BE_EVENTTYPE_H
#define TAO_BE_EVENTTYPE_H

#include "be_valuetype.h"
#include "ast_eventtype.h"

class be_eventtype : public virtual be_valuetype,
                     public virtual AST_EventType
{
  // = TITLE
  //    Backend-class for eventtypes
  //
  // = DESCRIPTION
  //    Extends be_valuetype.
  //
public:
  be_eventtype (void);
  // Default constructor.

  be_eventtype (UTL_ScopedName *n,
                AST_Interface **inherits,
                long n_inherits,
                AST_ValueType *inherits_concrete,
                AST_Interface **inherits_flat,
                long n_inherits_flat,
                AST_Interface **supports,
                long n_supports,
                AST_Interface *supports_concrete,
                idl_bool abstract,
                idl_bool truncatable);
  // Constructor that sets its scoped name <n>, a list of inherited valuetypes
  // and supported interfaces <ih>, and the number of inherited interfaces <nih>

  ~be_eventtype (void);
  // Destructor.

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Cleanup.
  virtual void destroy (void);

 // Narrowing.
  DEF_NARROW_METHODS2 (be_eventtype, be_valuetype, AST_EventType);
  DEF_NARROW_FROM_DECL (be_eventtype);
  DEF_NARROW_FROM_SCOPE (be_eventtype);
};

#endif  // if !defined
