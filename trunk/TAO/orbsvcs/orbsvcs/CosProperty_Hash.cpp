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
//     Alexander Babu Arulanthu
//
// ================================================================================================

#include "CosProperty_Hash.h"

// @@ Alex, make sure you don't "pollute" the global namespace.
// Typedefs for the Hash_Map classes.
typedef ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_Null_Mutex> 
        Hash_Property_Map;

EXT_ID::EXT_ID (void)
{
}

EXT_ID::EXT_ID (const CosPropertyService::PropertyName &name)
  : pname_ (name)
{
}

int
EXT_ID::operator == (const EXT_ID &ext_id) const   
{
  ACE_DEBUG ((LM_DEBUG, "EXT_ID::operator == : %s == %s \n",
              this->pname_.in (), ext_id.pname_.in ()));

  return ACE_OS::strcmp(this->pname_.in (), ext_id.pname_.in ()) == 0;
}

u_long
EXT_ID::hash (void) const 
{
  u_long ret = ACE::hash_pjw (this->pname_.in ());

  ACE_DEBUG ((LM_DEBUG, "EXT_ID::hash : hasing %s : val : %d \n",
              this->pname_.in (), ret));
  return ret;
}

EXT_ID::~EXT_ID (void) 
{
}

INT_ID::INT_ID (void)
{
}

INT_ID::INT_ID (const CORBA::Any &anyvalue)
{
  TAO_TRY
    {
      pvalue_.replace (anyvalue.type (),
                       anyvalue.value (),
                       CORBA::B_FALSE, TAO_TRY_ENV);
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

INT_ID::~INT_ID (void)
{
}
