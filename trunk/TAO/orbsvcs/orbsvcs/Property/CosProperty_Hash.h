/* -*- C++ -*- */

//$Id$

// ===================================================================================================
//
// = FILE
//     CosProperty_Hash.h
//
// = DESCRITION
//     Defines CosProperty_Hash_Value and CosProperty_Hash_Key classes for the Hash_Map_Manager to
//     be used in the CosPropertyService. 
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

class TAO_ORBSVCS_Export CosProperty_Hash_Key 
{
  // = TITLE
  //     Key for the Hash Table.
public: 
  CosProperty_Hash_Key (void);
  // Default constructor
  
  CosProperty_Hash_Key (const CosPropertyService::PropertyName &name);
  // Costructor that takes PropertyName

  //CosProperty_Hash_Key (const CosProperty_Hash_Key &CosProperty_Hash_Key);
  // Copy constructor
  
  virtual int operator == (const CosProperty_Hash_Key &CosProperty_Hash_Key) const;
  // The vital operator for hash binding and "find"ing.

  virtual u_long hash (void) const;
  // The vital function

  virtual ~CosProperty_Hash_Key (void);
  // Destructor

  CosPropertyService::PropertyName_var pname_; 
  // This is still the public variable... 
};

class TAO_ORBSVCS_Export CosProperty_Hash_Value
{
  // = TITLE 
  //     This is the Hash Entry.
public: 
  CosProperty_Hash_Value (void);
  // Default constructor 
  
  CosProperty_Hash_Value (const CORBA::Any &value);
  // Constructor with Any value

  //CosProperty_Hash_Value (const CosProperty_Hash_Value &CosProperty_Hash_Value);
  // Copy constructor
  
  virtual ~CosProperty_Hash_Value (void);
  // Destructor

  CORBA::Any pvalue_;
  // Public data -- the property value
};

#endif  /* COSPROPERTY_HASH_H */





