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
  /// Constructor.
  BE_GlobalData (void);

  /// Destructor.
  virtual ~BE_GlobalData (void);

  //=======================================

  // Data accessors.
  const char* output_dir (void) const;
  void output_dir (const char* s);

  bool encapsulate_idl2 (void) const;

  ACE_CString & excluded_filenames (void);

  void set_excluded_filenames (const char *filenames);

  bool gen_copyright (void) const;

  //=========================================

  /// Parse args that affect this backend.
  virtual void parse_args (long &i, char **av);

  /// Create the output file, the associated stream, and
  /// generate the initial #ifndef.
  int outfile_init (TAO_OutStream *&,
                    const char *file_prefix,
                    const char *file_suffix,
                    const char *guard_prefix,
                    const char *guard_suffix);

  /// Cleanup.
  void destroy (void);

protected:
  /// So it can be turned off in backends that inherit from this one.
  bool gen_copyright_;

private:
  /// Name of the IDL file we are processing.
  char *filename_;

  /**
   * Directory where the generated file is to be
   * kept. Default value is 0 for this string which means the current
   * directory from which the <tao_picml> is called.
   */
  char *output_dir_;

  /// Have the generated file just include the original file if
  /// no IDL3 declarations are found by the checking visitor.
  bool encapsulate_idl2_;

  /**
   * Whitespace-separated list of included IDL files
   * to be passed to the output IDL file without
   * adding the '_IDL2" suffix.
   */
  ACE_CString excluded_filenames_;
};

#endif /* TAO_IFR_BE_GLOBAL_H */
