// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     varbase.h
//
// = DESCRIPTION
//     Contains a base class for the object _var classes. 
//
// = AUTHOR
//     Jeff Parsons.
//
// ============================================================================

#ifndef TAO_VARBASE_H
#define TAO_VARBASE_H
#include "ace/pre.h"

class TAO_Base_var
{
//
// = TITLE
//     TAO_Base_var
//
// = DESCRIPTION
//     Base class for object _var classes. Used to prevent the
//     _var class of a CORBA interface from accepting the _var
//     of a more derived interface in its copy constructor or
//     assignment operator.
//
public:
  TAO_Base_var (void) {}
};

#include "ace/post.h"
#endif /* TAO_VARBASE_H */
