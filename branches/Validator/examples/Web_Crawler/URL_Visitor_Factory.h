/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    examples/Web_Crawler
//
// = FILENAME
//    URL_Visitor_Factory.h
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#ifndef _URL_VISITOR_FACTORY_H
#define _URL_VISITOR_FACTORY_H

#include "URL_Visitor.h"
#include "Command_Processor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

class URL_Visitor_Factory
{
  // = TITLE
  //   Abstract base class that creates URL visitors.
  //
  // = DESCRIPTION
  //   Subclasses define each of the Factory Methods to
  //   make the right objects, which all "vary" together.
public:
  virtual URL_Visitor *make_visitor (void) = 0;
  // Factory Method that makes the appropriate type of <URL_Visitor>.
  
  virtual Command_Processor *make_command_processor (void) = 0;
  // Factory Method that makes the appropriate type of
  // <Command_Processor>.
};

class URL_Validation_Visitor_Factory : public URL_Visitor_Factory
{
  // = TITLE
  //   Create a URL visitor that validates URL links.
public:
  virtual URL_Visitor *make_visitor (void);
  // Factory Method that makes a <URL_Validation_Visitor>.
  
  virtual Command_Processor *make_command_processor (void);
  // Factory Method that makes a <FIFO_Command_Processor>.

 
};

class URL_Download_Visitor_Factory : public URL_Visitor_Factory
{
  // = TITLE
  //   Create a URL visitor that downloads URL links.
public:
  virtual URL_Visitor *make_visitor (void);
  // Factory Method that makes a <URL_Download_Visitor>.
  
  virtual Command_Processor *make_command_processor (void);
  // Factory Method that makes a <FIFO_Command_Processor>.
};

#endif /* _URL_VISITOR_FACTORY_H */
