/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_global.h
 *
 *  $Id$
 *
 *  Header file for class containing compiler back end global data.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


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
  /// Constructor.
  BE_GlobalData (void);

  /// Destructor.
  virtual ~BE_GlobalData (void);

  // Data accessors.

  bool removing (void) const;
  void removing (bool value);

  CORBA::ORB_ptr orb (void) const;
  void orb (CORBA::ORB_ptr orb);

  CORBA::Repository_ptr repository (void) const;
  void repository (CORBA::Repository_ptr repo);

  ACE_Unbounded_Stack<CORBA::Container_ptr> &ifr_scopes (void);

  /// Cleanup function.
  void destroy (void);

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

  /// Command line passed to ACE_Process::spawn. Different
  /// implementations in IDL and IFR backends.
  ACE_CString spawn_options (void);

  /// Parse args that affect the backend.
  void parse_args (long &i, char **av);

private:
  /// Are we removing IR objects from the repository?
  bool removing_;

  /// Reference to our ORB.
  CORBA::ORB_var orb_;

  /// Reference to the interface repository.
  CORBA::Repository_var repository_;

  /// Must be something unlikely to clash.
  CORBA::String_var holding_scope_name_;

  /// IR object scope stack.
  ACE_Unbounded_Stack<CORBA::Container_ptr> ifr_scopes_;

  /// Name of the IDL file we are processing.
  char *filename_;

  /// Option to lock at the IDL file level.
  bool enable_locking_;

  /// Option to process included IDL files.
  bool do_included_files_;

  /// Option to enable duplicate typedefs in parsed IDL.
  bool allow_duplicate_typedefs_;

  /// Holder for -ORB args saved and passed to DRV_fork.
  ACE_CString orb_args_;
};

#endif /* TAO_IFR_BE_GLOBAL_H */
