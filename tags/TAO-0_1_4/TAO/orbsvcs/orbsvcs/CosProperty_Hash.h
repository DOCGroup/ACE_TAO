//$Id$

// ===================================================================================================
// = FILE
//     CosProperty_Hash.h
//
// = DESCRITION
//     Defines INT_ID and EXT_ID classes for the Hash_Map_Manager to be used in the CosPropertyService.
//
// = AUTHOR
//     Alexander B. Arulanthu
// ===================================================================================================

#if !defined (COSPROPERTY_HASH_H)
#define	COSPROPERTY_HASH_H

#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"
#include "CosPropertyServiceS.h"
#include "tao/any.h"

class EXT_ID 
// Key for the Hash Table
{
public: 
  // Default constructor
  EXT_ID ();
  
  // Costructor that takes PropertyName
  EXT_ID (const CosPropertyService::PropertyName &name);

  // Copy constructor
  //EXT_ID (const EXT_ID &ext_id);
  
  // The vital operator for hash binding and "find"ing
  virtual int operator == (const EXT_ID &ext_id) const;

  // The vital function
  virtual long unsigned int hash (void) const;

  // Destructor
  virtual ~EXT_ID ();

  // This is still the public variable... 
  CosPropertyService::PropertyName_var pname_; 
};

class INT_ID
// This is the Hash Entry
{
public: 

  // Default constructor 
  INT_ID ();
  
  // Constructor with Any value
  INT_ID (const CORBA::Any &value);

  // Copy constructor
  //INT_ID (const INT_ID &int_id);
  
  // Destructor
  virtual ~INT_ID ();

  // Public data -- the property value
  CORBA::Any pvalue_;
};


// typedefs for the Hash_Map classes
typedef ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_Null_Mutex> Hash_Property_Map;

#endif  /* COSPROPERTY_HASH_H */





