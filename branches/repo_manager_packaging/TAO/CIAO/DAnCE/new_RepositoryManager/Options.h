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

#ifndef HTTP_OPTIONS_H
#define HTTP_OPTIONS_H

#include "ace/Get_Opt.h"
#include "ace/String_Base.h"
#include "ace/OS_NS_string.h"				//for ACE_CString 

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ARGV.h"

class Options
{
public:
  static Options *instance (void);
  // Returns the singleton instance

  void parse_args (int argc, ACE_TCHAR *argv[]);
  // parses commandline arguments

  ACE_CString package_;
  // Name of package

  ACE_CString local_path_;
  // specifies the local path for install

  ACE_CString artifact_;
  // Implementation Artifact requested

  bool delete_;
  // delete the package_ name

  bool install_;
  // installs the package_ name

  bool plan_;
  // retrieves the DeploymentPlan specified in the package_ name

  bool shutdown_;
  //shutdown the RepositoryManagerDemon

protected:
  Options (void);
  // protected constructor, singleton

  static Options *instance_;
  // the singleton

  void usage (void);
};

#endif /* HTTP_OPTIONS_H */
