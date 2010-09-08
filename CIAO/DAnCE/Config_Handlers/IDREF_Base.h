/**
* $Id$
* @file IDREF_Base.h
* @author Will Otte <wotte@dre.vanderbilt.edu>
*
* Defines the IDREF_Base class, which is an abstract class that serves
* as a base for any config handler that must map IDREFS.
*/

#ifndef CIAO_CONFIG_HANDLERS_IDREF_BASE_H
#define CIAO_CONFIG_HANDLERS_IDREF_BASE_H

#include /**/ "ace/pre.h"

#include "Common.h"
#include "Config_Handlers_Export.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace DAnCE
{
namespace Config_Handlers
{

/**
* @class IDREF_Handler
* @brief Base class for any handler that must process idrefs.
*
*/
template <typename T>
class IDREF_Base
{
public:
IDREF_Base ()
{
}

IDREF_Base (T start_point)
: next_ (start_point)
{
}

/// Bind an IDREF to an index
 void bind_ref (ACE_TString& id, T value);

void bind_next_available (ACE_TString &id);

/// Get the index associated with an IDREF
 void find_ref (const ACE_TString& id, T& value);

/// get the IDREF associated with an index
 void find_ref (const T &value, ACE_TString& id);

/// Unbind all IDREFS
bool
unbind_refs (void);

typedef ACE_Hash_Map_Manager<ACE_TString,
T,
ACE_Null_Mutex> IDREF_MAP;


typedef ACE_Hash_Map_Manager<T,
ACE_TString,
ACE_Null_Mutex> POS_MAP;
protected:
/// The map used to store and look up the indexes of elements
/// referenced by their IDREF.
IDREF_MAP idref_map_;
/// The map used to store and look up the IDREFS of elements
/// referenced by their index.
POS_MAP pos_map_;

T next_;
};

}

}

#include "IDREF_Base.cpp"

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_IDREF_BASE_H */
