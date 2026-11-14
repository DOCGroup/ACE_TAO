/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    File_Parser.h
 *
 *  @author Doug Schmidt
 */
//=============================================================================

#ifndef _FILE_PARSER
#define _FILE_PARSER

#include <memory>

#include "ace/Basic_Types.h"
#include "ace/OS_NS_stdio.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class FPRT
 *
 * @brief This class serves as a namespace for the <Return_Type>.
 */
class FPRT
{
public:
  enum Return_Type
  {
    RT_EOLINE,
    RT_EOFILE,
    RT_SUCCESS,
    RT_COMMENT,
    RT_DEFAULT,
    RT_PARSE_ERROR
  };
};

/**
 * @class File_Parser
 *
 * @brief Class used to parse the configuration file for the
 * <Consumer_Map>.
 */
template <class ENTRY>
class File_Parser
{
  struct File_Deleter
  {
    void operator() (FILE *file) const
    {
      if (file != nullptr)
        ACE_OS::fclose (file);
    }
  };

public:
  File_Parser () = default;

  virtual ~File_Parser () = default;
  File_Parser (File_Parser const &) = delete;
  File_Parser (File_Parser &&) = delete;
  File_Parser &operator= (File_Parser const &) = delete;
  File_Parser &operator= (File_Parser &&) = delete;

  /// Open or close the configured input file.
  int open (const ACE_TCHAR filename[]);
  int close ();

  /// Pure virtual hook for subclasses to fill in an entry using the
  /// protected parsing methods.
  virtual FPRT::Return_Type read_entry (ENTRY &entry,
                                        int &line_number) = 0;

protected:
  /// Read the next ASCII word.
  FPRT::Return_Type getword (char buf[]);

  /// Read the next integer.
  FPRT::Return_Type getint (ACE_INT32 &value);

  /**
   * Read the next "word," which is demarcated by <delimiter>s.
   *
   * @@ This function is inherently flawed since it doesn't take a
   * count of the size of <buf>...
   */
  FPRT::Return_Type readword (char buf[]);

  /// Returns true if <ch> is a delimiter, i.e., ' ', ',', or '\t'.
  int delimiter (char ch);

  /// Returns true if <ch> is the comment character, i.e., '#'.
  int comments (char ch);

  /// Skips to the remainder of a line, e.g., when we find a comment
  /// character.
  int skipline ();

  /// Owns the file being read and closes it if the parser exits early.
  std::unique_ptr<FILE, File_Deleter> infile_;
};

#include "File_Parser.cpp"

#endif /* _FILE_PARSER */
