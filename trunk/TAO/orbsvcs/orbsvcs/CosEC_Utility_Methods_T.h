/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//   CosEC_Utility_Methods_T.h
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_COSEC_UTILITY_METHODS_T_H
#define TAO_COSEC_UTILITY_METHODS_T_H

#include "orbsvcs/CosNamingC.h"

class CosEC_Utility_NIL {};
// No-op template parameter.

template <class T>
class CosEC_Utility_Methods
{
  // = TITLE
  //   A class to act as a namespace for utility methods.
  //
public:
  // = Helpers methods
  static T* activate (PortableServer::POA_ptr poa,
                      PortableServer::Servant p_servant,
                      CORBA_Environment &ACE_TRY_ENV);
  // Activate a servant with the poa.

  static T* activate (PortableServer::POA_ptr poa,
                      PortableServer::Servant p_servant,
                      const char *servant_id,
                      CORBA_Environment &ACE_TRY_ENV);
  // Use the given id to activate a servant.

  static void deactivate (PortableServer::POA_ptr poa,
                          PortableServer::Servant p_servant,
                          CORBA::Environment &ACE_TRY_ENV);
  // Deactivate a servant.

  static T* locate (CosNaming::NamingContext_ptr naming,
                    const char *object_name,
                    CORBA::Environment &ACE_TRY_ENV);
  // Locate a corba object with the name service.

  static void bind (CosNaming::NamingContext_ptr naming,
                    const char *object_name,
                    CORBA::Object_ptr obj,
                    CORBA::Environment &ACE_TRY_ENV);
  // Bind a corba object with the name service.

  static void unbind (CosNaming::NamingContext_ptr naming,
                      const char *object_name,
                      CORBA::Environment &ACE_TRY_ENV);
  // Unbind a corba object from the name service.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "CosEC_Utility_Methods_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("CosEC_Utility_Methods_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_COSEC_UTILITY_METHODS_T_H */
