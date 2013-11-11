
//=============================================================================
/**
 *  @file    client.h
 *
 *  $Id$
 *
 *  Header file for the Alt_Mapping_Test client application.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef ALT_MAPPING_CLIENT_H
#define ALT_MAPPING_CLIENT_H

#include "tao/Codeset/Codeset.h"
#include "alt_mappingC.h"
#include "results.h"

/**
 * @class Alt_Mapping_Client
 *
 * @brief Alt_Mapping_Client
 *
 * This class declares an interface to run the example client for
 * Alt_Mapping CORBA server.  All the complexity for initializing
 * the client is hidden in the class.  Just the run () interface
 * is needed. The template class does the specific work of making
 * the request of the desired data type
 */
template <class T>
class Alt_Mapping_Client
{
public:
  typedef T TEST_OBJECT;

  // = Constructor and destructor.
  Alt_Mapping_Client (CORBA::ORB_ptr orb,
                     Alt_Mapping_ptr objref,
                     T *);

  ~Alt_Mapping_Client (void);

  /// run the SII test
  int run_sii_test (void);

private:
  /// underlying ORB
  CORBA::ORB_ptr orb_;

  /// alt mapping object reference
  Alt_Mapping_ptr alt_mapping_test_;

  /// object doing the actual work
  TEST_OBJECT *test_object_;

  /// results
  Results results_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "client.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("client.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ALT_MAPPING_CLIENT_H */
