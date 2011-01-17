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
#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/ORB.h"
#include "ace/Containers.h"
#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class AST_Generator;

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

  bool removing (void) const;
  void removing (bool value);

  CORBA::ORB_ptr orb (void) const;
  void orb (CORBA::ORB_ptr orb);

  CORBA::Repository_ptr repository (void) const;
  void repository (CORBA::Repository_ptr repo);

  ACE_Unbounded_Stack<CORBA::Container_ptr> &ifr_scopes (void);

  void destroy (void);
  // Cleanup function.

  const char *filename (void) const;
  void filename (char *fname);

  bool enable_locking (void) const;
  void enable_locking (bool value);

  bool do_included_files (void) const;
  void do_included_files (bool val);

  bool allow_duplicate_typedefs () const;
  void allow_duplicate_typedefs (bool val);

  ACE_CString orb_args (void) const;
  void orb_args (const ACE_CString& args);

  ACE_CString spawn_options (void);
  // Command line passed to ACE_Process::spawn. Different
  // implementations in IDL and IFR backends.

  void parse_args (long &i, char **av);
  // Parse args that affect the backend.

private:
  bool removing_;
  // Are we removing IR objects from the repository?

  CORBA::ORB_var orb_;
  // Reference to our ORB.

  CORBA::Repository_var repository_;
  // Reference to the interface repository.

  CORBA::String_var holding_scope_name_;
  // Must be something unlikely to clash.

  ACE_Unbounded_Stack<CORBA::Container_ptr> ifr_scopes_;
  // IR object scope stack.

  char *filename_;
  // Name of the IDL file we are processing.

  bool enable_locking_;
  // Option to lock at the IDL file level.

  bool do_included_files_;
  // Option to process included IDL files.

  bool allow_duplicate_typedefs_;
  // Option to enable duplicate typedefs in parsed IDL.

  ACE_CString orb_args_;
  // Holder for -ORB args saved and passed to DRV_fork.
};

#endif /* TAO_IFR_BE_GLOBAL_H */
