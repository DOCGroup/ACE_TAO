// -*- C++ -*-

//=============================================================================
/**
 *  @file     UIPMC_Connector.h
 *
 *  $Id$
 *
 *  MIOP specific connector processing.
 *
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_UIPMC_CONNECTOR_H
#define TAO_UIPMC_CONNECTOR_H
#include /**/ "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Transport_Connector.h"
#include "UIPMC_Connection_Handler.h"
#include "portablegroup_export.h"

// ****************************************************************

/**
 * @class TAO_UIPMC_Connector
 *
 * @brief UIPMC-specific Connector bridge for pluggable protocols.
 *
 * Concrete instance of the TAO_Connector class.  Responsible
 * for establishing a connection with a server and is called from the
 * Connector_Registory.
 */
class TAO_PortableGroup_Export TAO_UIPMC_Connector : public TAO_Connector
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_UIPMC_Connector (CORBA::Boolean flag = 0);

  /// Destructor.
  ~TAO_UIPMC_Connector (void);

  // = The TAO_Connector methods, please check the documentation on
  // Transport_Connector.h
  int open (TAO_ORB_Core *orb_core);
  int close (void);
  TAO_Profile *create_profile (TAO_InputCDR& cdr);

  virtual int check_prefix (const char *endpoint);

  virtual char object_key_delimiter (void) const;

protected:
  /// = More TAO_Connector methods, please check the documentation on
  ///   Transport_Connector.h
  int set_validate_endpoint (TAO_Endpoint *endpoint);

  int make_connection (TAO_GIOP_Invocation *,
                       TAO_Transport_Descriptor_Interface *,
                       ACE_Time_Value *timeout);

  virtual TAO_Profile * make_profile (ACE_ENV_SINGLE_ARG_DECL);

  /// Obtain UIPMC properties that must be used by this connector, i.e.,
  /// initialize <uipmc_properties_>.
  int init_uipmc_properties (void);


protected:

  /// UIPMC configuration properties to be used for all
  /// connections established by this connector.
  TAO_UIPMC_Properties uipmc_properties_;

private:
  // @@ Michael: UDP Addition
  ACE_Hash_Map_Manager_Ex < ACE_INET_Addr,
                            TAO_UIPMC_Connection_Handler *,
                            ACE_Hash < ACE_INET_Addr >,
                            ACE_Equal_To < ACE_INET_Addr >,
                            ACE_Null_Mutex > svc_handler_table_;

  typedef ACE_Hash_Map_Iterator_Ex < ACE_INET_Addr,
                                     TAO_UIPMC_Connection_Handler *,
                                     ACE_Hash < ACE_INET_Addr >,
                                     ACE_Equal_To < ACE_INET_Addr >,
                                     ACE_Null_Mutex > SvcHandlerIterator;

};

#include /**/ "ace/post.h"
#endif  /* TAO_UIPMC_CONNECTOR_H */
