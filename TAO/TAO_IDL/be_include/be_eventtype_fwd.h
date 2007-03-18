/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_eventtype_fwd.h
//
// = DESCRIPTION
//    Extension of class AST_EventType_Fwd that provides additional
//    means for C++ mapping of an eventtype.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef BE_EVENTTYPE_FWD_H
#define BE_EVENTTYPE_FWD_H

#include "be_valuetype_fwd.h"
#include "ast_eventtype_fwd.h"

class be_eventtype_fwd : public virtual be_valuetype_fwd,
                         public virtual AST_EventTypeFwd
{
  // =TITLE
  //   be_valuetype_fwd
  //
  // =DESCRIPTION
  //   Extensions to the be_interface_fwd class
public:
  be_eventtype_fwd (void);
  // Default constructor.

  be_eventtype_fwd (AST_Interface *dummy,
                    UTL_ScopedName *n);
  // Constructor.

  virtual ~be_eventtype_fwd (void);
  // Destructor.

  // Visiting.
  virtual int accept (be_visitor* visitor);

  // Cleanup
  virtual void destroy (void);

  // Narrowing.
  DEF_NARROW_METHODS2 (be_eventtype_fwd, be_valuetype_fwd, AST_EventTypeFwd);
  DEF_NARROW_FROM_DECL (be_eventtype_fwd);
};

#endif // if !defined
