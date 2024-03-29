/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    URL_Visitor_Factory.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================


#ifndef _URL_VISITOR_FACTORY_H
#define _URL_VISITOR_FACTORY_H

#include "URL_Visitor.h"
#include "Command_Processor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class URL_Visitor_Factory
 *
 * @brief Abstract base class that creates URL visitors.
 *
 * Subclasses define each of the Factory Methods to
 * make the right objects, which all "vary" together.
 */
class URL_Visitor_Factory
{
public:
  /// Destructor.
  virtual ~URL_Visitor_Factory ();

  /// Factory Method that makes the appropriate type of <URL_Visitor>.
  virtual URL_Visitor *make_visitor () = 0;

  /// Factory Method that makes the appropriate type of
  /// <Command_Processor>.
  virtual Command_Processor *make_command_processor () = 0;
};

/**
 * @class URL_Validation_Visitor_Factory
 *
 * @brief Create a URL visitor that validates URL links.
 */
class URL_Validation_Visitor_Factory : public URL_Visitor_Factory
{
public:
  /// Factory Method that makes a <URL_Validation_Visitor>.
  virtual URL_Visitor *make_visitor ();

  /// Factory Method that makes a <FIFO_Command_Processor>.
  virtual Command_Processor *make_command_processor ();
};

/**
 * @class URL_Download_Visitor_Factory
 *
 * @brief Create a URL visitor that downloads URL links.
 */
class URL_Download_Visitor_Factory : public URL_Visitor_Factory
{
public:
  /// Factory Method that makes a <URL_Download_Visitor>.
  virtual URL_Visitor *make_visitor ();

  /// Factory Method that makes a <FIFO_Command_Processor>.
  virtual Command_Processor *make_command_processor ();
};

#endif /* _URL_VISITOR_FACTORY_H */
