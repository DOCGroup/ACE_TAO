// -*- C++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   SHMIOP_Factory.h
//
// = AUTHOR
//   Nanbor Wang <nanbor@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_SHMIOP_FACTORY_H
#define TAO_SHMIOP_FACTORY_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "tao/Protocol_Factory.h"
#include "strategies_export.h"

#include "ace/Service_Config.h"

class TAO_Acceptor;
class TAO_Connector;

class TAO_Strategies_Export TAO_SHMIOP_Protocol_Factory : public TAO_Protocol_Factory
{
public:
  TAO_SHMIOP_Protocol_Factory (void);
  virtual ~TAO_SHMIOP_Protocol_Factory (void);

  // = Service Configurator hooks.
  virtual int init (int argc, char* argv[]);
  // Dynamic linking hook

  virtual int match_prefix (const ACE_CString &prefix);
  // Verify prefix is a match

  virtual const char *prefix (void) const;
  // Returns the prefix used by the protocol.

  virtual char options_delimiter (void) const;
  // Return the character used to mark where an endpoint ends and
  // where its options begin.

  // = Check Protocol_Factory.h for a description of these methods.
  virtual TAO_Acceptor  *make_acceptor (void);
  virtual TAO_Connector *make_connector  (void);
  virtual int requires_explicit_endpoint (void) const;

  // = Set/get mmap file prefix
  void mmap_file_prefix (const ACE_TCHAR *prefix);
  const ACE_TCHAR *mmap_file_prefix (void);

private:
  int major_;
  int minor_;
  // Changing the version number can be used to provide backwards
  // compatibility with old clients.

  ACE_TCHAR *mmap_prefix_;
  // Specify the prefix (full path) for mmap files

  off_t min_bytes_;
  // Minimum bytes of the mmap files.
};

ACE_STATIC_SVC_DECLARE (TAO_SHMIOP_Protocol_Factory)
ACE_FACTORY_DECLARE (TAO_Strategies, TAO_SHMIOP_Protocol_Factory)

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */

#include "ace/post.h"
#endif /* TAO_SHMIOP_FACTORY_H */
