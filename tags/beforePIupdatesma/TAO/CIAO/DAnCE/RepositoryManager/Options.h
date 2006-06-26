// $Id$

/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Options.h
 *
 *  $Id$
 *
 *   TheOptions is an Options class wrapped into an ACE_Singleton
 *   with Null_Mutex because the client is single-threaded.
 *
 *
 *  @author Stoyan Paunov
 */
//=============================================================================


#ifndef RM_OPTIONS_H
#define RM_OPTIONS_H

#include "ace/Get_Opt.h"
#include "ace/SString.h"    //for ACE_CString
#include "ace/Singleton.h"  //for ACE_Singleton
#include "ace/Null_Mutex.h" //for ACE_Null_Mutex

//forward declaration
class Options;

typedef ACE_Singleton <Options, ACE_Null_Mutex> TheOptions;

class Options
{
public:

  ///constructor
  Options (void);

  /// parses commandline arguments
  bool parse_args (int argc, ACE_TCHAR *argv[]);

  /// Name of package
  ACE_CString name_;

  /// Name of package
  ACE_CString uuid_;

  /// Type of package
  ACE_CString type_;

  /// specifies the local path for install
  ACE_CString path_;

  /// delete the name_
  bool delete_;

  /// installs the name_
  bool install_;

  /// creates the name_
  bool create_;

  /// finds the name_
  bool find_;

  /// get all Names
  bool all_names_;

  /// get all types
  bool all_types_;

  /// find all names by type
  bool names_by_type_;

  /// shutdown the RepositoryManagerDemon
  bool shutdown_;

  // use naming service
  bool register_with_ns_;

  // use ior file
  bool write_to_ior_;

  // Name of RepoMan
  ACE_CString repoman_name_;

protected:
  //usage function
  void usage (void);
};

#endif /* RM_OPTIONS_H */
