// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    SHMIOP_Acceptor.h
//
// = DESCRIPTION
//    SHMIOP specific acceptor processing
//
// = AUTHOR
//    Nanbor Wang
//
// ============================================================================

#ifndef TAO_SHMIOP_ACCEPTOR_H
#define TAO_SHMIOP_ACCEPTOR_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "tao/Pluggable.h"
#include "SHMIOP_Connection_Handler.h"
#include "tao/Acceptor_Impl.h"
#include "ace/Acceptor.h"
#include "ace/MEM_Acceptor.h"
#include "tao/GIOP_Message_Version.h"
// TAO SHMIOP_Acceptor concrete call defination

class TAO_Strategies_Export TAO_SHMIOP_Acceptor : public TAO_Acceptor
{
  // = TITLE
  //   TAO_SHMIOP_Acceptor
  //
  // = DESCRIPTION
  //   The SHMIOP-specific bridge class for the concrete acceptor.
  //
public:
  TAO_SHMIOP_Acceptor (CORBA::Boolean flag = 0);
  // Constructor.

  ~TAO_SHMIOP_Acceptor (void);
  // Destructor.

  typedef ACE_Strategy_Acceptor<TAO_SHMIOP_Connection_Handler, ACE_MEM_ACCEPTOR> TAO_SHMIOP_BASE_ACCEPTOR;
  typedef TAO_Creation_Strategy<TAO_SHMIOP_Connection_Handler> TAO_SHMIOP_CREATION_STRATEGY;
  typedef TAO_Concurrency_Strategy<TAO_SHMIOP_Connection_Handler> TAO_SHMIOP_CONCURRENCY_STRATEGY;
  typedef TAO_Accept_Strategy<TAO_SHMIOP_Connection_Handler, ACE_MEM_ACCEPTOR> TAO_SHMIOP_ACCEPT_STRATEGY;

  // = The TAO_Acceptor methods, check the documentation in
  //   Pluggable.h for details.
  virtual int open (TAO_ORB_Core *orb_core,
                    ACE_Reactor *reactor,
                    int version_major,
                    int version_minor,
                    const char *port,
                    const char *options = 0);
  virtual int open_default (TAO_ORB_Core *orb_core,
                            ACE_Reactor *reactor,
                            int version_major,
                            int version_minor,
                            const char *options = 0);
  virtual int close (void);
  virtual int create_profile (const TAO_ObjectKey &object_key,
                              TAO_MProfile &mprofile,
                              CORBA::Short priority);

  virtual int is_collocated (const TAO_Endpoint* endpoint);
  virtual CORBA::ULong endpoint_count (void);

  virtual int object_key (IOP::TaggedProfile &profile,
                          TAO_ObjectKey &key);

  int set_mmap_options (const ACE_TCHAR *prefix,
                        off_t size);
  // Set the MMAP options the MEM_Stream this acceptor creates will
  // use.

private:
  int open_i (TAO_ORB_Core* orb_core,
              ACE_Reactor *reactor);
  // Implement the common part of the open*() methods.

  virtual int parse_options (const char *options);
  // Parse protocol specific options.

  int create_new_profile (const TAO_ObjectKey &object_key,
                          TAO_MProfile &mprofile,
                          CORBA::Short priority);
  // Create a SHMIOP profile representing this acceptor.

  int create_shared_profile (const TAO_ObjectKey &object_key,
                             TAO_MProfile &mprofile,
                             CORBA::Short priority);
  // Add the endpoints on this acceptor to a shared profile.

protected:
  ACE_CString host_;
  // Cache the information about the endpoint serviced by this
  // acceptor.
  // @@ TODO there may in fact be multiple hostnames for this
  //    endpoint. For example it the IP address is INADDR_ANY
  //    (0.0.0.0) then there will be possibly a different hostname for
  //    each interface.

  ACE_MEM_Addr address_;
  // A local endpoint.

  TAO_GIOP_Message_Version version_;
  // The GIOP version for this endpoint

  TAO_ORB_Core *orb_core_;
  // ORB Core.

private:
  TAO_SHMIOP_BASE_ACCEPTOR base_acceptor_;
  // the concrete acceptor, as a pointer to it's base class.

  TAO_SHMIOP_CREATION_STRATEGY *creation_strategy_;
  TAO_SHMIOP_CONCURRENCY_STRATEGY *concurrency_strategy_;
  TAO_SHMIOP_ACCEPT_STRATEGY *accept_strategy_;
  // Acceptor strategies.

  const ACE_TCHAR *mmap_file_prefix_;
  // Determine the prefix (include path name) of the mmap file.

  off_t mmap_size_;
  // Determine the minimum size of mmap file.  This dictate the
  // maximum size of a CORBA method invocation.

  CORBA::Boolean lite_flag_;
  // Should we use GIOP lite??
};

#if defined(__ACE_INLINE__)
#include "SHMIOP_Acceptor.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */

#include "ace/post.h"
#endif  /* TAO_SHMIOP_ACCEPTOR_H */
