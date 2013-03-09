// -*- C++ -*-

//=============================================================================
/**
 *  @file     miopconf.h
 *
 *  $Id$
 *
 *  Build configuration file.
 *
 *  @author  Copyright 1995 by Sun Microsystems, Inc.
 *  @author  Chris Cleeland
 *  @author  Douglas C. Schmidt
 */
//=============================================================================

#ifndef TAO_MIOPCONF_H
#define TAO_MIOPCONF_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"

// This is the version of the MIOP spec that TAO supports. The
// exact usage of the version has not been emphasized. But TAO should
// get a TaggedComponents for a group with this version number. So, for
// the present, we do a sanity check for our version and raise an error on
// a mismatch.

#if !defined (TAO_DEF_MIOP_MAJOR)
#define TAO_DEF_MIOP_MAJOR 1
#endif /* TAO_DEF_MIOP_MAJOR */
#if !defined (TAO_DEF_MIOP_MINOR)
#define TAO_DEF_MIOP_MINOR 0
#endif /* TAO_DEF_MIOP_MINOR */

// MIOP Definitions:

// Default value for the delay in milliseconds before a fragmented
// MIOP message will be considered as expired on the server.
#if !defined (TAO_DEFAULT_MIOP_FRAGMENTS_CLEANUP_DELAY)
static int const TAO_DEFAULT_MIOP_FRAGMENTS_CLEANUP_DELAY = 1000;
#endif

// Default number of incomplete messages waiting for reassembly before
// cleanup on the server side.
#if !defined (TAO_DEFAULT_MIOP_FRAGMENTS_CLEANUP_NUMBER)
static int const TAO_DEFAULT_MIOP_FRAGMENTS_CLEANUP_NUMBER = 5;
#endif

// Default memory taken by incomplete messages waiting for reassembly
// before cleanup on the server side.
#if !defined (TAO_DEFAULT_MIOP_FRAGMENTS_CLEANUP_MEMORY)
static int const TAO_DEFAULT_MIOP_FRAGMENTS_CLEANUP_MEMORY = 3000000;
#endif

// We don't use the constants below but let they stay for the documentation.
// static u_long const MIOP_MAGIC_OFFSET             = 0u;
// static u_long const MIOP_VERSION_OFFSET           = 4u;
// static u_long const MIOP_FLAGS_OFFSET             = 5u;
// static u_long const MIOP_PACKET_LENGTH_OFFSET     = 6u;
// static u_long const MIOP_PACKET_NUMBER_OFFSET     = 8u;
// static u_long const MIOP_NUMBER_OF_PACKETS_OFFSET = 12u;
// static u_long const MIOP_ID_LENGTH_OFFSET         = 16u;
static u_long const MIOP_ID_CONTENT_OFFSET         = 20u;

// Max ID length 252 is defined in the MIOP spec. We use 12.
static u_long const MIOP_MIN_ID_LENGTH             = 0u;
static u_long const MIOP_MAX_ID_LENGTH             = 252u;
static u_long const MIOP_DEFAULT_ID_LENGTH         = 12u;

// MIOP_DEFAULT_HEADER_SIZE, MIOP_MIN_HEADER_SIZE and MIOP_MAX_HEADER_SIZE
// must be multiple of 8.
static u_long const MIOP_DEFAULT_HEADER_SIZE       =
  MIOP_ID_CONTENT_OFFSET + MIOP_DEFAULT_ID_LENGTH +
  (MIOP_ID_CONTENT_OFFSET + MIOP_DEFAULT_ID_LENGTH) % 8u;
static u_long const MIOP_MIN_HEADER_SIZE           =
  MIOP_ID_CONTENT_OFFSET + MIOP_MIN_ID_LENGTH +
  (MIOP_ID_CONTENT_OFFSET + MIOP_MIN_ID_LENGTH) % 8u;
static u_long const MIOP_MAX_HEADER_SIZE           =
  MIOP_ID_CONTENT_OFFSET + MIOP_MAX_ID_LENGTH +
  (MIOP_ID_CONTENT_OFFSET + MIOP_MAX_ID_LENGTH) % 8u;

static u_long const MIOP_MAX_DGRAM_SIZE            = ACE_MAX_UDP_PACKET_SIZE;

// Default value for the size of MIOP fragment used by the client.
// This can be considered same as MTU.
#if !defined (TAO_DEFAULT_MIOP_FRAGMENT_SIZE)
static u_long const TAO_DEFAULT_MIOP_FRAGMENT_SIZE     = MIOP_MAX_DGRAM_SIZE;
#endif

#if !defined (TAO_DEFAULT_MIOP_MAX_FRAGMENTS)
static u_long const TAO_DEFAULT_MIOP_MAX_FRAGMENTS = 0u; // Zero is unlimited
#endif

#if !defined (TAO_DEFAULT_MIOP_SEND_THROTTLING)
static bool const TAO_DEFAULT_MIOP_SEND_THROTTLING = true; // Enabled
#endif

#if !defined (TAO_DEFAULT_MIOP_EAGER_DEQUEUEING)
static bool const TAO_DEFAULT_MIOP_EAGER_DEQUEUEING = true; // Enabled
#endif

static CORBA::Octet const miop_magic[4] = {
  0x4d, 0x49, 0x4f, 0x50
}; // in ASCII this is 'M', 'I', 'O', 'P'

#include /**/ "ace/post.h"
#endif /*TAO_MIOPCONF_H*/
