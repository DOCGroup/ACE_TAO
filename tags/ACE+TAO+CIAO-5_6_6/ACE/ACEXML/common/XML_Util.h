// -*- C++ -*-

//=============================================================================
/**
 *  @file    XML_Util.h
 *
 * Initially contains a function to escape strings for use in XML files.
 *
 *  $Id$
 *
 *  @author Justin Michel <michel_j@ociweb.com>
 */
//=============================================================================

#ifndef _ACEXML_XML_UTIL_H_
#define _ACEXML_XML_UTIL_H_

#include /**/ "ace/pre.h"

#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"

ACEXML_Export ACEXML_String ACEXML_escape_string(const ACEXML_String& str);
ACEXML_Export void ACEXML_escape_string(const ACEXML_String& in, ACEXML_String& out);

#include /**/ "ace/post.h"

#endif /* _ACEXML_XML_UTIL_H_ */
