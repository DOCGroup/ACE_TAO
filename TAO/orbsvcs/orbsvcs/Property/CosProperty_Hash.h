/* -*- C++ -*- */

//$Id$

// ===================================================================================================
//
// = FILE
//     CosProperty_Hash.h
//
// = DESCRITION
//     Defines CosProperty_Hash_Value and CosProperty_Hash_Key classes
//     for the Hash_Map_Manager to be used in the CosPropertyService.
//
// = AUTHOR
//     Alexander B. Arulanthu <alex@cs.wustl.edu>
//
// ===================================================================================================

#if !defined (COSPROPERTY_HASH_H)
#define	COSPROPERTY_HASH_H

#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"
#include "orbsvcs/CosPropertyServiceS.h"
#include "orbsvcs/orbsvcs_export.h"

class TAO_ORBSVCS_Export CosProperty_Hash_Key 
{
  // = TITLE
  //     Key for the Hash Table.
public: 
  // = Initialization and termination methods.
  CosProperty_Hash_Key (void);
  // Default constructor.
  
  CosProperty_Hash_Key (const char * &name);
  // Constructor from a const string.
  
  CosProperty_Hash_Key (const CosPropertyService::PropertyName &name);
  // Costructor that takes PropertyName.
  
  CosProperty_Hash_Key (const CosProperty_Hash_Key &hash_key);
  // Copy constructor.
  
  virtual ~CosProperty_Hash_Key (void);
  // Destructor.

  virtual int operator == (const CosProperty_Hash_Key &hash_key) const;
  // The operator for hash binding and "find"ing.

  virtual u_long hash (void) const;
  // The function that computes a hash value.

  CosPropertyService::PropertyName_var pname_; 
  // This is still the public variable.
};

class TAO_ORBSVCS_Export CosProperty_Hash_Value
{
  // = TITLE 
  //     This will be the value part in the Hash_Table.
public: 
  // = Initialization and termination methods.
  
  CosProperty_Hash_Value (void);
  // Default constructor.
  
  CosProperty_Hash_Value (const CORBA::Any &any,
                          const CosPropertyService::PropertyModeType &mode);
  // Constructor with the property_value and the property_mode.
  
  CosProperty_Hash_Value (const CosProperty_Hash_Value &hash_value);
  // Copy constructor.
  
  virtual ~CosProperty_Hash_Value (void);
  // Destructor.
  
  CORBA::Any_var pvalue_;
  // property value.
  
  CosPropertyService::PropertyModeType pmode_;
  // Property Mode.
};

#endif /* COSPROPERTY_HASH_H */





