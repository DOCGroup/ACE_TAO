/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    PG_Properties_Decoder.h
 *
 *  $Id$
 *
 *  This file declares classes to help manage the Properties
 *  defined in the Portable Object Group.
 *
 *  Note: this started as a simple helper class to make decoding sets of properties
 *  easier, but expanded to provide more general support for managing sets of properties.
 *
 *  A more appropriate name would be PG_Properties_Set.  Maybe this can be changed someday.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
#ifndef TAO_PG_PROPERTIES_DECODER_H
#define TAO_PG_PROPERTIES_DECODER_H
#include "portablegroup_export.h"
#include <orbsvcs/PortableGroupS.h>
#include <orbsvcs/CosNamingC.h>
#include <ace/Hash_Map_Manager.h>
#include <ace/SString.h>

namespace TAO_PG
{

  /**
   * The Properties_Decoder captures the set of properties from a PortableGroup::Properties
   * structure in a more usable format (a hash map), and provides methods for
   * operating on these properties.
   *
   * It supports "chains" of property sets to implement default value semantics.
   * If a requested property is not found in this set, the default set(s) are searched.
   * Thus, any property found at this level overrides the defaults.
   * 
   * See: PG_Properties_Support for more details on use of this object.
   *
   * A Properties_Decoder may also be used for it's original purpose as a stand-alone
   * helper class for extracting values from PortableGroup::Properties.
   */

  class TAO_PortableGroup_Export Properties_Decoder
  {
    typedef ACE_Hash_Map_Manager<
      ACE_CString,
      const PortableGroup::Value *,
      ACE_SYNCH_NULL_MUTEX> ValueMap;
    typedef ACE_Hash_Map_Iterator<
      ACE_CString,
      const PortableGroup::Value *,
      ACE_SYNCH_NULL_MUTEX> ValueMapIterator;

  public:

    /**
     * constructor: empty set with no defaults.
     */
    Properties_Decoder();

    /**
     * constructor
     * @param property_set the properties to be decoded
     */
    Properties_Decoder (const PortableGroup::Properties & property_set ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /**
     * constructor with defaults
     * @param property_set the properties to be decoded
     * @param defaults a propert set decoder that supplies default values.
     */
    Properties_Decoder (const PortableGroup::Properties & property_set, Properties_Decoder * defaults ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /**
     * constructor with defaults, but no properties (yet)
     * (note this is not a copy constructor)
     * @param defaults a propert set decoder that supplies default values.
     */
    Properties_Decoder (Properties_Decoder * defaults);


    ~Properties_Decoder ();

    /**
     * general purpose find. returns a pointer to an Any
     *  if templated methods were available:
     *    template <typename TYPE >
     *    int find (const ACE_CString & key, TYPE & value) const;
     *    instead, see global function below
     * @param key the (simple) name of the property
     * @param pValue an out parameter to receive a pointer to the Any containing the value
     * @returns boolean true if found
     */
    int find (const ACE_CString & key, const PortableGroup::Value *& pValue)const;


    /**
     * Decode additional properties
     * Duplicate values replace previous values.
     * @param property_set the properties to be decoded
     */
    void decode (const PortableGroup::Properties & property_set)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /**
     * Clear properties
     * Does not clear default properties.
     */
    void clear ();

    void remove (const PortableGroup::Properties & property_set)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /**
     * Export the properties to a PortableGroup::Properties
     *
     * This method is intended to be used to implement the PropertyManager::get_*_properties
     * methods.  If you want to access the properties for any purpose other than exporting
     * them across a CORBA interface, it is much more efficient to use the find interface.
     *
     */
    void export_properties(PortableGroup::Properties & property_set) const;

    /////////////////////////
    // Implementation Methods
 private:
    /**
     * populate a ValueMap with the properties known to this decoder
     * including but overriding default values
     */
    void merge_properties (ValueMap & merged_values) const;

    ////////////////////
    // Forbidden methods
  private:
    Properties_Decoder(const Properties_Decoder & rhs);
    Properties_Decoder & operator = (const Properties_Decoder & rhs);

    ///////////////
    // Data Members
  private:
    /**
     * Protect internal state.
     */
    TAO_SYNCH_MUTEX internals_;
    typedef ACE_Guard<TAO_SYNCH_MUTEX> InternalGuard;

    ValueMap values_;
    /**
     * a parent to another property decoder that provides default values
     * these can be chained indefinitely.
     * @@ TODO: reference counted pointers would be a good idea here.
     */
    Properties_Decoder * defaults_;
  };

#ifdef PG_PS_UNIT_TEST

  /**
   * unit test: encode and decode properties.
   * Initialize CORBA before calling this function.
   * Success is silent, failure prints on cerr.
   * @returns 1 if test passed; 0 if test failed.
   */
  int test_encode_decode();
#endif // PG_PS_UNIT_TEST
} //namespace TAO_PG

////////////////////////////////////
// include templated helper function
#include "PG_Properties_Decoder_Find.h"

#endif // TAO_PG_PROPERTIES_DECODER_H
