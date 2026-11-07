// -*- C++ -*-

//=============================================================================
/**
 *  @file    ParserContext.h
 *
 *  @author Krishnakumar B <kitty@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACEXML_PARSER_CONTEXT_H
#define ACEXML_PARSER_CONTEXT_H

#include /**/ "ace/pre.h"
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
#include <memory>

class ACEXML_PARSER_Export ACEXML_Parser_Context
{
public:
  /// Default constructor
  ACEXML_Parser_Context () = default;

  ACEXML_Parser_Context (ACEXML_Parser_Context const&) = delete;
  ACEXML_Parser_Context (ACEXML_Parser_Context&&) = delete;
  ACEXML_Parser_Context& operator= (ACEXML_Parser_Context const&) = delete;
  ACEXML_Parser_Context& operator= (ACEXML_Parser_Context&&) = delete;

  /// Constructor which initializes the context
  ACEXML_Parser_Context (ACEXML_InputSource* instream,
                         ACEXML_LocatorImpl* locator);

  /// Comparison operator
  bool operator!= (ACEXML_Parser_Context const& src) const;

  /// Destructor
  virtual ~ACEXML_Parser_Context();

  /// Reset the parser context. This does not free up the memory. Only sets
  /// it to zero. Meant to be called after a context is pushed on to a
  /// stack.
  void reset ();

  /// Get the underlying input source.
  virtual ACEXML_InputSource* getInputSource();

  /// Get the underlying locator.
  virtual ACEXML_LocatorImpl* getLocator();

  /// Set the underlying input source.
  virtual void setInputSource(ACEXML_InputSource* ip);

  /// Set the underlying locator.
  virtual void setLocator(ACEXML_LocatorImpl* locator);

private:
  /// Current input char stream.
  std::unique_ptr<ACEXML_InputSource> instream_ {};

  /// Current Locator which provides line no., column no. systemId and publicId
  std::unique_ptr<ACEXML_LocatorImpl> locator_ {};
};

#if defined (__ACEXML_INLINE__)
# include "ACEXML/parser/parser/ParserContext.inl"
#endif /* __ACEXML_INLINE__ */

#include /**/ "ace/post.h"

#endif /* ACEXML_PARSER_CONTEXT_H */
