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

#include "CIAO_IDL3_TO_XMI_Export.h"
#include "idl_defines.h"

#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Defines a class containing all back end global data.

class CIAO_IDL3_TO_XMI_Export BE_GlobalData
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
  bool ir_filter (void) const;
  void ir_filter (bool flag);

  const char *filename (void) const;
  void filename (char *fname);

  const char* output_dir (void) const;
  void output_dir (const char* s);

  const char * dtd (void) const;
  void dtd (const char *s);

  bool local_only (void) const;
  void local_only (bool);

  void parse_args (long &i, char **av);
  // Parse args that affect the backend.

  void destroy (void);
  // Cleanup.

private:
  bool ir_filter_;

  char *filename_;
  // Name of the IDL file we are processing.

  char *output_dir_;
  // Directory where the generated file is to be
  // kept. Default value is 0 for this string which means the current
  // directory from which the <tao_picml> is called.

  /// full path to dtd to be used.
  ACE_CString dtd_;

  /// generate xmi for local entities only
  bool local_only_;
};

#endif /* TAO_IFR_BE_GLOBAL_H */
