//$:Id$
//
//
// FILE
//   CosProperty_Hash.h
//
// DESCRITION
//   Defines INT_ID and EXT_ID classes for the Hash_Map_Manager to be used in the CosPropertyService.
//
// AUTHOR
//   Alexander B. Arulanthu
//
#if !defined (COSPROPERTY_HASH_H)
#define	COSPROPERTY_HASH_H

#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"
#include "CosPropertyServiceS.h"
#include "tao/any.h"

class EXT_ID 
// Key for the Hash Table
{
public:  // Functions
  EXT_ID ();
  
  EXT_ID (const CosPropertyService::PropertyName &name);

  EXT_ID (const EXT_ID &ext_id);
  
  virtual int operator == (const EXT_ID &ext_id) const;

  virtual long unsigned int hash (void) const;

  virtual ~EXT_ID ();

public:  // Data
  CosPropertyService::PropertyName_var pname_; 
};

class INT_ID
// This is the Hash Entry
{
public:  // Functions
  INT_ID ();
  
  INT_ID (const CORBA::Any &value);

  INT_ID (const INT_ID &int_id);

  virtual ~INT_ID ();

public:  // Data
  CORBA::Any pvalue_;
  
};


// typedefs for the Hash_Map classes
typedef ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_Null_Mutex> Hash_Property_Map;

#endif  /* COSPROPERTY_HASH_H */





