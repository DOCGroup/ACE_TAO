// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    DIOP_Connector.h
 *
 *  $Id$
 *
 *  DIOP specific connector processing
 *
 *  @author  Michael Kircher
 */
//=============================================================================

#ifndef TAO_DIOP_CONNECTOR_H
#define TAO_DIOP_CONNECTOR_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "tao/Pluggable.h"
#include "DIOP_Connection_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// ****************************************************************

/**
 * @class TAO_DIOP_Connector
 *
 * @brief DIOP-specific Connector bridge for pluggable protocols.
 *
 * Concrete instance of the TAO_Connector class.  Responsible
 * for establishing a connection with a server and is called from the
 * Connector_Registory.
 */
class TAO_Strategies_Export TAO_DIOP_Connector : public TAO_Connector
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_DIOP_Connector (CORBA::Boolean flag = 0);

  /// Destructor.
  ~TAO_DIOP_Connector (void);

  // = The TAO_Connector methods, please check the documentation on
  // Pluggable.h
  int open (TAO_ORB_Core *orb_core);
  int close (void);
  int connect (TAO_GIOP_Invocation *invocation,
               TAO_Transport_Descriptor_Interface *desc
               TAO_ENV_ARG_DECL);
  int preconnect (const char *preconnections);
  TAO_Profile *create_profile (TAO_InputCDR& cdr);

  virtual int check_prefix (const char *endpoint);

  virtual char object_key_delimiter (void) const;

protected:

  /// = More TAO_Connector methods, please check the documentation on
  ///   Pluggable.h
  virtual TAO_Profile * make_profile (TAO_ENV_SINGLE_ARG_DECL);

  /// Obtain tcp properties that must be used by this connector, i.e.,
  /// initialize <tcp_properties_>.
  int init_tcp_properties (void);

protected:

  /// TCP configuration properties to be used for all
  /// connections established by this connector.
  TAO_DIOP_Properties tcp_properties_;

  /// Do we need to use a GIOP_Lite for sending messages?
  CORBA::Boolean lite_flag_;

private:
  // @@ Michael: UDP Addition
  ACE_Hash_Map_Manager_Ex < ACE_INET_Addr,
                            TAO_DIOP_Connection_Handler *,
                            ACE_Hash < ACE_INET_Addr >,
                            ACE_Equal_To < ACE_INET_Addr >,
                            ACE_Null_Mutex > svc_handler_table_;

  typedef ACE_Hash_Map_Iterator_Ex < ACE_INET_Addr,
                                     TAO_DIOP_Connection_Handler *,
                                     ACE_Hash < ACE_INET_Addr >,
                                     ACE_Equal_To < ACE_INET_Addr >,
                                     ACE_Null_Mutex > SvcHandlerIterator;
};

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */

#include "ace/post.h"
#endif  /* TAO_DIOP_CONNECTOR_H */
