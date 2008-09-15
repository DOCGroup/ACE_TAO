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

  const char *permitted_ior_file = "permitted.ior";
  const char *permitted_ior = "file://permitted.ior";

  const char *restricted_ior_file = "restricted.ior";
  const char *restricted_ior = "file://restricted.ior";
}

#endif
