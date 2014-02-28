// -*- C++ -*-

//=============================================================================
/**
 *  @file    LocatorImpl.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author Krishnakumar Balasubramanian <kitty@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACEXML_LOCALTORIMPL_H
#define ACEXML_LOCALTORIMPL_H

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/Locator.h"
#include "ace/Copy_Disabled.h"

/**
 * @class ACEXML_LocatorImpl
 *
 * @brief ACEXML_LocatorImpl is an implementation of ACEXML_Locator.
 *
 * This class is available mainly for application writers, who can
 * use it to make a persistent snapshot of a locator at any point
 * during a document parse:
 *
 * @code
 *   ACEXML_Locator locator;
 *   ACEXML_Locator startloc;
 *
 *   public void setLocator (ACEXML_Locator locator)
 *   {
 *      // note the locator
 *      this.locator = locator;
 *   }
 *
 *   public void startDocument ()
 *   {
 *      // save the location of the start of the document
 *      // for future use.
 *      ACEXML_Locator startloc = new ACEXML_LocatorImpl(locator);
 *   }
 * @endcode
 *
 * Normally, parser writers will not use this class, since it is more
 * efficient to provide location information only when requested,
 * rather than constantly updating a Locator object.
 *
 * @todo ACEXML parser doesn't support the use of Locator yet.
 *
 * @sa ACEXML_Locator
 */
class ACEXML_Export ACEXML_LocatorImpl :
  public ACEXML_Locator,
  private ACE_Copy_Disabled
{
public:
  /*
   * Default constructor.
   */
  ACEXML_LocatorImpl (void);

  /**
   * Construct a locator with systemId and publicId
   *
   */
  ACEXML_LocatorImpl (const ACEXML_Char* systemId,
                      const ACEXML_Char* publicId);
  /**
   * Copy constructor.  Create a persistent copy of the current state
   * of a locator. When the original locator changes, this copy will
   * still keep the original values (and it can be used outside the
   * scope of DocumentHandler methods).
   */
  ACEXML_LocatorImpl (const ACEXML_Locator& locator);

  /*
   * Destructor.
   */
  virtual ~ACEXML_LocatorImpl (void);

  /*
   * Return the column number where the current document event ends.
   */
  virtual int getColumnNumber (void) const;

  /*
   * Return the line number where the current document event ends.
   */
  virtual int getLineNumber (void) const;

  /*
   * Return the public identifier for the current document event.
   */
  virtual const ACEXML_Char *getPublicId (void) const;

  /*
   * Return the system identifier for the current document event.
   */
  virtual const ACEXML_Char *getSystemId (void) const;

  /*
   * Set the column number of this locator.
   */
  void setColumnNumber (int cn);

  /*
   * Set the line number of this locator.
   */
  void setLineNumber (int ln);

  /*
   * Set the public identifier of this locator.
   */
  void setPublicId (const ACEXML_Char *id);

  /*
   * Set the system identifier of this locator.
   */
  void setSystemId (const ACEXML_Char *id);

  /*
   * Increment the line number.
   */
  void incrLineNumber ();

  /*
   * Increment the column number.
   */
  void incrColumnNumber ();

  /*
   * Reset the Locator information. This is necessary because one might
   * want to use the same parser to parse multiple files. In that case,
   * tying the life of the Locator with the parser is not appropriate. The
   * parser calls this method as soon as issuing an endDocument() call.
   */
  void reset (void);

private:
  ACEXML_Char *publicId_;
  ACEXML_Char *systemId_;
  int lineNumber_;
  int columnNumber_;
};

#if defined (__ACEXML_INLINE__)
# include "ACEXML/common/LocatorImpl.inl"
#endif /* __ACEXML_INLINE__ */

#include /**/ "ace/post.h"

#endif /* ACEXML_LOCALTORIMPL_H */
