// $Id$

// ===============================================================================================
//
// = FILE
//     CosProperty_Hash.cpp
//
// = Description
//     Defines equal and hash functions for the Hash_Property_Map class.
//    
// = Author
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ================================================================================================

#include "CosProperty_Hash.h"

CosProperty_Hash_Key::CosProperty_Hash_Key (void)
{
}

CosProperty_Hash_Key::CosProperty_Hash_Key (const CosPropertyService::PropertyName &name)
  : pname_ (name)
{
}

int
CosProperty_Hash_Key::operator == (const CosProperty_Hash_Key &CosProperty_Hash_Key) const   
{
  ACE_DEBUG ((LM_DEBUG,
              "CosProperty_Hash_Key::operator == : %s == %s \n",
              this->pname_.in (),
              CosProperty_Hash_Key.pname_.in ()));

  return ACE_OS::strcmp (this->pname_.in (),
                         CosProperty_Hash_Key.pname_.in ()) == 0;
}

u_long
CosProperty_Hash_Key::hash (void) const 
{
  u_long ret = ACE::hash_pjw (this->pname_.in ());

  ACE_DEBUG ((LM_DEBUG, 
              "CosProperty_Hash_Key::hash : hasing %s : val : %d \n",
              this->pname_.in (),
              ret));

  return ret;
}

CosProperty_Hash_Key::~CosProperty_Hash_Key (void) 
{
}

CosProperty_Hash_Value::CosProperty_Hash_Value (void)
{
}

CosProperty_Hash_Value::CosProperty_Hash_Value (const CORBA::Any &anyvalue)
{
  TAO_TRY
    {
      pvalue_.replace (anyvalue.type (),
                       anyvalue.value (),
                       CORBA::B_FALSE,
                       TAO_TRY_ENV);
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      TAO_TRY_ENV.print_exception ("User Exception");
    }
  TAO_ENDTRY;
}

CosProperty_Hash_Value::~CosProperty_Hash_Value (void)
{
}

