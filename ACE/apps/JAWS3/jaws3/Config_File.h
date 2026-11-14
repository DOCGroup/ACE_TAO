/* -*- c++ -*- */
#ifndef JAWS_CONFIG_FILE_H
#define JAWS_CONFIG_FILE_H

#include "jaws3/Export.h"
#include <memory>

class JAWS_Config_File;
class JAWS_Config_File_Impl;

/// Reads and manages a configuration file with this syntax:
/// @code
/// <configuration-file>   := <configuration-seq>
/// <configuration-seq>    := <configuration-line> [<configuration-seq>]
/// <configuration-line>   := <name-value-pair-line> | <comment-line>
/// <name-value-pair-line> := <name> '=' <value-line>
/// <name>                 := <char-seq>
/// <value-line>           := [<char-seq>] <LF> [<continuation-seq>]
/// <continuation-seq>     := <continuation-line> [<continuation-seq>]
/// <continuation-line>    := <LWSP> [<char-seq>] <LF>
/// <comment-line>         := [<comment>] <LF>
/// <comment>              := '#' <char-seq>
/// <char-seq>             := <char> [<char-seq>]
/// @endcode
/// Configuration lines cannot exceed 4094 characters.
class JAWS_Export JAWS_Config_File
{
public:
  /// @name Initialization
  /// @{

  /// Parses <config_file> from <config_dir>.
  explicit JAWS_Config_File (ACE_TCHAR const *config_file,
                             ACE_TCHAR const *config_dir = "./");
  ~JAWS_Config_File ();
  JAWS_Config_File (JAWS_Config_File const &) = delete;
  JAWS_Config_File (JAWS_Config_File &&) = delete;
  JAWS_Config_File &operator= (JAWS_Config_File const &) = delete;
  JAWS_Config_File &operator= (JAWS_Config_File &&) = delete;

  /// @}
  /// @name Searching
  /// @{

  /// Finds the value associated with <key>.
  int find (const ACE_TCHAR *key, const ACE_TCHAR *&value);

  /// Re-reads the configuration file.
  void reset ();

  /// Dumps the values of all configuration variables.
  void dump ();

  /// @}

private:
  /// Opaque implementation.
  std::unique_ptr<JAWS_Config_File_Impl> impl_;
};

#endif /* JAWS_CONFIG_FILE_H */
