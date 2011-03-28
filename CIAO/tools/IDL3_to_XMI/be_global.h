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
  /// Constructor.
  BE_GlobalData (void);

  /// Destructor.
  virtual ~BE_GlobalData (void);

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

  /// Parse args that affect the backend.
  void parse_args (long &i, char **av);

  /// Cleanup.
  void destroy (void);

private:
  bool ir_filter_;

  /// Name of the IDL file we are processing.
  char *filename_;

  /**
   * Directory where the generated file is to be
   * kept. Default value is 0 for this string which means the current
   * directory from which the <tao_picml> is called.
   */
  char *output_dir_;

  /// full path to dtd to be used.
  ACE_CString dtd_;

  /// generate xmi for local entities only
  bool local_only_;
};

#endif /* TAO_IFR_BE_GLOBAL_H */
