/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO_IFR_BE_DLL
//
// = FILENAME
//    be_global.h
//
// = DESCRIPTION
//    Header file for class containing compiler back end global data.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_IFR_BE_GLOBAL_H
#define TAO_IFR_BE_GLOBAL_H

#include "TAO_IFR_BE_Export.h"
#include "idl_bool.h"
#include "tao/ORB.h"
#include "tao/InterfaceC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Defines a class containing all back end global data.

class TAO_IFR_BE_Export BE_GlobalData
{
public:
  // = TITLE
  //    BE_GlobalData
  //
  // = DESCRIPTION
  //    Storage of global data specific to the compiler back end
  //
  BE_GlobalData (void);
  // Constructor.

  virtual ~BE_GlobalData (void);
  // Destructor.

  // Data accessors.

  idl_bool removing (void) const;

  void removing (idl_bool value);

  CORBA::ORB_ptr orb (void) const;

  void orb (CORBA::ORB_ptr orb);

  IR_Repository_ptr repository (void) const;

  void repository (IR_Repository_ptr repo);

  ACE_Unbounded_Stack<IR_Container_ptr> &ifr_scopes (void);

  const char *filename (void);

  void filename (char *fname);

  idl_bool enable_locking (void);

  void enable_locking (idl_bool value);

private:
  idl_bool removing_;
  // Are we removing IR objects from the repository?

  CORBA::ORB_var orb_;
  // Reference to our ORB.

  IR_Repository_var repository_;
  // Reference to the interface repository.

  ACE_Unbounded_Stack<IR_Container_ptr> ifr_scopes_;
  // IR object scope stack.

  char *filename_;
  // Name of the IDL file we are processing.

  idl_bool enable_locking_;
  // Option to lock at the IDL file level.
};

#endif /* TAO_IFR_BE_GLOBAL_H */
