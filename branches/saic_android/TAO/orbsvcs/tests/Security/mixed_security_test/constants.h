// -*- C++ -*-

//=============================================================================
/**
 * @file constants.h
 *
 * $Id$
 *
 * Set of constants shared between client and server
 *
 * @author Chris Cleeland <cleeland@ociweb.com>
 */
//=============================================================================

#ifndef SECURE_INVOCATION_CONSTANTS_H
#define SECURE_INVOCATION_CONSTANTS_H

namespace TAO_Mixed_Security_Test
{
  const char *cert_file = "cacert.pem";

  const ACE_TCHAR *permitted_ior_file = ACE_TEXT("permitted.ior");
  const ACE_TCHAR *permitted_ior = ACE_TEXT("file://permitted.ior");

  const ACE_TCHAR *restricted_ior_file = ACE_TEXT("restricted.ior");
  const ACE_TCHAR *restricted_ior = ACE_TEXT("file://restricted.ior");
}

#endif
