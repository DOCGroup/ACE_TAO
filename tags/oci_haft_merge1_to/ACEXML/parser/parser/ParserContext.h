// -*- C++ -*-

//=============================================================================
/**
 *  @file    ParserContext.h
 *
 *  $Id$
 *
 *  @author Krishnakumar B <kitty@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACEXML_PARSER_CONTEXT_H
#define ACEXML_PARSER_CONTEXT_H

#include "ace/pre.h"
#include "ACEXML/parser/parser/Parser_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"
#include "ACEXML/common/InputSource.h"
#include "ACEXML/common/Locator.h"
#include "ACEXML/common/LocatorImpl.h"
#include "ace/Functor.h"
#include "ace/Containers_T.h"
#include "ace/Swap.h"

class ACEXML_PARSER_Export ACEXML_Parser_Context
{
public:
  /// Default constructor
  ACEXML_Parser_Context();

  /// Constructor which initializes the context
  ACEXML_Parser_Context (ACEXML_InputSource* instream,
                         ACEXML_LocatorImpl* locator);

  /// Copy constructor
  ACEXML_Parser_Context (const ACEXML_Parser_Context& src);

  /// Assignment operator
  ACEXML_Parser_Context& operator= (const ACEXML_Parser_Context& src);

  /// Comparison operator
  int operator!= (const ACEXML_Parser_Context& src);

  /// Destructor
  virtual ~ACEXML_Parser_Context();

  /// Reset the parser context. This does not free up the memory. Only sets
  /// it to zero. Meant to be called after a context is pushed on to a
  /// stack.
  void reset (void);

  /// Get the underlying input source.
  virtual ACEXML_InputSource* getInputSource(void);

  /// Get the underlying locator.
  virtual ACEXML_LocatorImpl* getLocator(void);

private:

  /// Current input char stream.
  ACEXML_InputSource *instream_;

  /// Current Locator which provides line no., column no. systemId and publicId
  ACEXML_LocatorImpl* locator_;
};

#if defined (__ACEXML_INLINE__)
# include "ACEXML/parser/parser/ParserContext.inl"
#endif /* __ACEXML_INLINE__ */

#include "ace/post.h"

#endif /* ACEXML_PARSER_CONTEXT_H */
