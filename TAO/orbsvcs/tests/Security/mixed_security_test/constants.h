// -*- C++ -*-

//=============================================================================
/**
 * @file constants.h
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
  char const *cert_file = "cacert.pem";

  ACE_TCHAR const *permitted_ior_file = ACE_TEXT ("permitted.ior");
  ACE_TCHAR const *permitted_ior = ACE_TEXT ("file://permitted.ior");

  ACE_TCHAR const *restricted_ior_file = ACE_TEXT ("restricted.ior");
  ACE_TCHAR const *restricted_ior = ACE_TEXT ("file://restricted.ior");
}

#endif
