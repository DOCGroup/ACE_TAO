/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Options.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


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
 * <ACE_Singleton> template.
 */
class Options
{
public:
  /// Parse the command-line arguments and initialize the options.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// If non-0 and the link is an HTML file then recursively check all
  /// links that are embedded in the body of file.
  int recurse (void) const;

  /// Return the hostname of the initial Web server.
  const ACE_TCHAR *hostname (void) const;

  /// Return the initial URI.
  const ACE_TCHAR *path_name (void) const;

  /// String used to filter out which URLs to validate.
  const ACE_TCHAR *url_filter (void) const;

  /// Are we debugging?
  int debug (void) const;

  /// Are we being verbose?
  int verbose (void) const;

  /// Which order? LIFO|FIFO??
  const ACE_TCHAR *order (void) const;

  /// Port #
  int port_no (void) const;

  /// Return the timeout used to prevent hanging on <recv> and
  /// <connect> calls to broken servers.
  const ACE_Time_Value *timeout (void) const;

  // = Get/set the <Command_Processor>.
  Command_Processor *command_processor (void) const;
  void command_processor (Command_Processor *);

  // = Get/set the <URL_Visitor>.
  URL_Visitor *visitor (void) const;
  void visitor (URL_Visitor *);

  // Get the handle_limit.
  int handle_limit (void);
private:
  /// Are we recursving.
  int recurse_;

  /// Initial Web server name.
  const ACE_TCHAR *hostname_;

  /// Initial URI name.
  const ACE_TCHAR *uri_;

  /// Are we debugging?
  int debug_;

  /// Are we being verbose?
  int verbose_;

  /// Whether the URLs are traversed in FIFO or LIFO order.
  const ACE_TCHAR *order_;

  /// Timeout on <recv> and <connect> to broken Web servers.
  ACE_Time_Value timeout_;

  /// String used to filter out which URLs to validate.
  const ACE_TCHAR *url_filter_;

  /// Pointer to the Command_Processor.
  Command_Processor *command_processor_;

  /// Pointer to the <URL_Visitor>.
  URL_Visitor *visitor_;

  /// Port no.
  int port_no_;

  /// The limit of the number of descriptors to be given for this process.
  int handle_limit_;
};

// Typedef an Options Singleton.
typedef ACE_Singleton <Options, ACE_Null_Mutex> OPTIONS;

#endif /* _OPTIONS_H */
