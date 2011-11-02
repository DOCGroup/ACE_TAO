/**
 *  @file StreamFactory.h
 *
 *  $Id$
 *
 *  @author Krishnakumar B <kitty@cs.wustl.edu>
 */

#ifndef _ACEXML_STREAM_FACTORY_H
#define _ACEXML_STREAM_FACTORY_H

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"

// Forward declarations

class ACEXML_CharStream;

/**
 *  @class ACEXML_StreamFactory
 *
 *  @brief A generic factory used to create an appropriate @sa
 *  ACEXML_CharStream from a SYSTEM id. This class creates a @sa
 *  ACEXML_FileCharStream or a @sa ACEXML_HttpCharStream depending on the
 *  URI supplied.
 *
 *  @todo Write a stream abstraction for handling ftp:// type URIs and add
 *  a function to create and return such streams. That is the only chunk
 *  missing in the armour.
 */

class ACEXML_Export ACEXML_StreamFactory
{
public:

  // Destructor
  virtual ~ACEXML_StreamFactory (void);

  /**
   *  Create the appropriate stream from the @a uri passed and return the
   *  stream. The caller is responsible for deallocating the returned
   *  stream.
   *
   *  @param uri SYSTEM id or a stream of characters (in the case of a
   *  StrCharStream).
   */
  virtual ACEXML_CharStream* create_stream (const ACEXML_Char* uri);
};

#include /**/ "ace/post.h"

#endif /* _ACEXML_STREAM_FACTORY_H */
