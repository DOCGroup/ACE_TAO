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
#include "tao/IFR_Client/IFR_BasicC.h"
#include "ace/Containers.h"

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

  CORBA::Repository_ptr repository (void) const;

  void repository (CORBA::Repository_ptr repo);

  CORBA::ModuleDef_ptr holding_scope (void) const;

  void holding_scope (CORBA::ModuleDef_ptr scope);

  const char *holding_scope_name (void) const;

  ACE_Unbounded_Stack<CORBA::Container_ptr> &ifr_scopes (void);

  const char *filename (void) const;

  void filename (char *fname);

  idl_bool enable_locking (void) const;

  void enable_locking (idl_bool value);

  idl_bool do_included_files (void) const;

  void do_included_files (idl_bool val);

private:
  idl_bool removing_;
  // Are we removing IR objects from the repository?

  CORBA::ORB_var orb_;
  // Reference to our ORB.

  CORBA::Repository_var repository_;
  // Reference to the interface repository.

  CORBA::ModuleDef_var holding_scope_;
  // Used to hold struct/union/exception member defns until
  // they are moved into their permanent scope.

  CORBA::String_var holding_scope_name_;
  // Must be something unlikely to clash.

  ACE_Unbounded_Stack<CORBA::Container_ptr> ifr_scopes_;
  // IR object scope stack.

  char *filename_;
  // Name of the IDL file we are processing.

  idl_bool enable_locking_;
  // Option to lock at the IDL file level.

  idl_bool do_included_files_;
  // Option to process included IDL files.
};

#endif /* TAO_IFR_BE_GLOBAL_H */
