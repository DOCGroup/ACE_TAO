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

#include "TAO_IDL3_TO_IDL2_BE_Export.h"
#include "idl_defines.h"
#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class AST_Generator;
class TAO_OutStream;

// Defines a class containing all back end global data.

class TAO_IDL3_TO_IDL2_BE_Export BE_GlobalData
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

  //=======================================

  // Data accessors.
  
  const char *filename (void) const;
  void filename (char *fname);

  const char* output_dir (void) const;
  void output_dir (const char* s);
  
  bool const encapsulate_idl2 (void) const;
  
  ACE_CString & excluded_filenames (void);
  
  //=========================================

  void parse_args (long &i, char **av);
  // Parse args that affect the backend.

  void prep_be_arg (char *s);
  // Special BE arg call factored out of DRV_args.

  void arg_post_proc (void);
  // Checks made after parsing args.

  void usage (void) const;
  // Usage message for backend options.

  AST_Generator *generator_init (void);
  // Create an AST node generator.
  
  int outfile_init (TAO_OutStream *&);
  // Create the output file, the associated stream, and
  // generate the initial #ifndef.

  void destroy (void);
  // Cleanup.

private:
  char *filename_;
  // Name of the IDL file we are processing.

  char *output_dir_;
  // Directory where the generated file is to be
  // kept. Default value is 0 for this string which means the current
  // directory from which the <tao_picml> is called.
  
  bool encapsulate_idl2_;
  // Have the generated file just include the original file if
  // no IDL3 declarations are found by the checking visitor.
  
  ACE_CString excluded_filenames_;
  // Whitespace-separated list of included IDL files
  // to be passed to the output IDL file without
  // adding the '_IDL2" suffix.
};

#endif /* TAO_IFR_BE_GLOBAL_H */
