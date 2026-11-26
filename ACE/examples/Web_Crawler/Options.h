/**
 *  @file    Options.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */

#ifndef _OPTIONS_H
#define _OPTIONS_H

#include "ace/Null_Mutex.h"
#include "ace/Singleton.h"
#include "ace/Time_Value.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward decls.
class Command_Processor;
class URL_Visitor;

/**
 * @class Options
 *
 * @brief Maintains the global options.
 *
 * This class is converted into a Singleton by the
 * @c ACE_Singleton template.
 */
class Options
{
public:
  /// Parse the command-line arguments and initialize the options.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// If non-0 and the link is an HTML file then recursively check all
  /// links that are embedded in the body of file.
  bool recurse () const;

  /// Return the hostname of the initial Web server.
  ACE_TCHAR const *hostname () const;

  /// Return the initial URI.
  ACE_TCHAR const *path_name () const;

  /// String used to filter out which URLs to validate.
  ACE_TCHAR const *url_filter () const;

  /// Are we debugging?
  bool debug () const;

  /// Are we being verbose?
  bool verbose () const;

  /// Which order? LIFO|FIFO??
  ACE_TCHAR const *order () const;

  /// Port #
  int port_no () const;

  /// Return the timeout used to prevent hanging on @c recv and
  /// @c connect calls to broken servers.
  ACE_Time_Value const *timeout () const;

  /// @name Command processor
  /// @{

  Command_Processor *command_processor () const;
  void command_processor (Command_Processor *);

  /// @}

  /// @name URL visitor
  /// @{

  URL_Visitor *visitor () const;
  void visitor (URL_Visitor *);

  /// @}

  /// Get the handle limit.
  int handle_limit () const;
private:
  /// Are we recursving.
  bool recurse_ {};

  /// Initial Web server name.
  ACE_TCHAR const *hostname_ {};

  /// Initial URI name.
  ACE_TCHAR const *uri_ {};

  /// Are we debugging?
  bool debug_ {};

  /// Are we being verbose?
  bool verbose_ {};

  /// Whether the URLs are traversed in FIFO or LIFO order.
  ACE_TCHAR const *order_ {};

  /// Timeout on @c recv and @c connect to broken Web servers.
  ACE_Time_Value timeout_;

  /// String used to filter out which URLs to validate.
  ACE_TCHAR const *url_filter_ {};

  /// Pointer to the Command_Processor.
  Command_Processor *command_processor_ {};

  /// Pointer to the @c URL_Visitor.
  URL_Visitor *visitor_ {};

  /// Port no.
  int port_no_ {};

  /// The limit of the number of descriptors to be given for this process.
  int handle_limit_ {};
};

// Typedef an Options Singleton.
using OPTIONS = ACE_Singleton<Options, ACE_Null_Mutex>;

#endif /* _OPTIONS_H */
