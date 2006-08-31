// -*- C++ -*-

//=============================================================================
/**
 * @file      Export_File_Generator.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_EXPORT_FILE_GENERATOR_H_
#define _CUTS_EXPORT_FILE_GENERATOR_H_

#include <string>

//=============================================================================
/**
 * @class CUTS_Export_File_Generator
 *
 * Generates macro definitions for exporting entries from a
 * shared library.
 */
//=============================================================================

class CUTS_Export_File_Generator
{
public:
  /**
   * Constructor. This version of the constructor will initialize
   * the export file and export macro based on the value of
   * \a export.
   *
   * @param[in]      export     Name of the export
   */
  CUTS_Export_File_Generator (const std::string & name);

  /// Destructor.
  ~CUTS_Export_File_Generator (void);

  /**
   * Get the name of the export marco.
   *
   * @return      Name of the export macro.
   */
  const std::string & export_macro (void) const;

  /**
   * Get the name of the export file.
   *
   * @return      Name of the export file.
   */
  const std::string & export_file (void) const;

  /**
   * Get the original name used to construct the export
   * macro name and file.
   *
   * @return      Name of the export.
   */
  const std::string & name (void) const;

  const std::string & build_flag (void) const;

  bool generate (const std::string & outdir);

  static std::string export_macro (const std::string & name);

  static std::string export_file (const std::string & name);

  static std::string build_flag (const std::string & name);

private:
  /// Name used to construct export data.
  std::string name_;

  /// Name of the export file.
  std::string export_file_;

  /// Name of the export marco.
  std::string export_macro_;

  /// Flag to toggling export/import.
  std::string build_flag_;
};

#endif  // !defined _CUTS_EXPORT_FILE_GENERATOR_H_
