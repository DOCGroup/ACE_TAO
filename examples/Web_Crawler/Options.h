/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps/Web
//
// = FILENAME
//    Options.h
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#ifndef _OPTIONS_H
#define _OPTIONS_H

#include "ace/Singleton.h"
#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward decls.
class Command_Processor;
class URL_Visitor;

class Options
{
  // = TITLE
  //   Maintains the global options.
  //
  // = DESCRIPTION
  //   This class is converted into a Singleton by the 
  //   <ACE_Singleton> template.
public:
  int parse_args (int argc, char *argv[]);
  // Parse the command-line arguments and initialize the options.

  int recurse (void) const;
  // If non-0 and the link is an HTML file then recursively check all
  // links that are embedded in the body of file.

  LPCTSTR hostname (void) const;
  // Return the hostname of the initial Web server.

  LPCTSTR path_name (void) const;
  // Return the initial URI.

  LPCTSTR url_filter (void) const;
  // String used to filter out which URLs to validate.

  int debug (void) const;
  // Are we debugging?

  int verbose (void) const;
  // Are we being verbose?

  LPCTSTR order (void) const;
  // Which order? LIFO|FIFO??

  int port_no (void) const;
  // Port #

  const ACE_Time_Value *timeout (void) const;
  // Return the timeout used to prevent hanging on <recv> and
  // <connect> calls to broken servers.

  // = Get/set the <Command_Processor>.
  Command_Processor *command_processor (void) const;
  void command_processor (Command_Processor *);

  // = Get/set the <URL_Visitor>.
  URL_Visitor *visitor (void) const;
  void visitor (URL_Visitor *);

  // Get the handle_limit.
  int handle_limit (void);
private:  
  int recurse_;
  // Are we recursving.

  LPCTSTR hostname_;
  // Initial Web server name.

  LPCTSTR uri_;
  // Initial URI name.

  int debug_;
  // Are we debugging?

  int verbose_;
  // Are we being verbose?

  LPCTSTR order_;
  // Whether the URLs are traversed in FIFO or LIFO order.

  ACE_Time_Value timeout_;
  // Timeout on <recv> and <connect> to broken Web servers.

  LPCTSTR url_filter_;
  // String used to filter out which URLs to validate.

  Command_Processor *command_processor_;
  // Pointer to the Command_Processor.

  URL_Visitor *visitor_;
  // Pointer to the <URL_Visitor>.

  int port_no_;
  // Port no.
  
  int handle_limit_;
  // The limit of the number of descriptors to be given for this process.
};

// Typedef an Options Singleton. 
typedef ACE_Singleton <Options, ACE_Null_Mutex> OPTIONS;

#endif /* _OPTIONS_H */
