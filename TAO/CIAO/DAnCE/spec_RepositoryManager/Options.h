// $Id$

/* -*- C++ -*- */
// Options.h, Stoyan

//============================================================================
//
// = LIBRARY
//    JAWS
//
// = FILENAME
//    Options.h
//
// = DESCRIPTION
//     Options is an Singleton for blobby
//
// = AUTHOR
//    Stoyan Paunov
//
//============================================================================

#ifndef RM_OPTIONS_H
#define RM_OPTIONS_H

#include "ace/Get_Opt.h"
#include "ace/SString.h"    //for ACE_CString

class Options
{
public:
  /// Returns the singleton instance
  static Options *instance (void);

  static void destroy (void);

  /// parses commandline arguments
  void parse_args (int argc, ACE_TCHAR *argv[]);

  ACE_CString name_;
  //Name of package

  ACE_CString uuid_;
  //Name of package

  ACE_CString path_;
  //specifies the local path for install

  //delete the name_
  bool delete_;

  //installs the name_
  bool install_;

  //finds the name_
  bool find_;

  //get all Names
  bool all_names_;

  //get all types
  bool all_types_;

  //find all names by type
  bool names_by_type_;

  /// shutdown the RepositoryManagerDemon
  bool shutdown_;


protected:
  /// protected constructor, singleton
  Options (void);

  /// the singleton
  static Options *instance_;


  void usage (void);
};

#endif /* RM_OPTIONS_H */
