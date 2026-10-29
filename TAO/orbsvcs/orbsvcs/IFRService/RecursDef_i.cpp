#include "orbsvcs/IFRService/RecursDef_i.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

//-----------------------------------------------------------------------------
// Class TAO_RecursiveDef_OuterScopes is a support class for StructDef_i.h and
// UnionDef_i.h files. It creates a stack of outer structure definitions, used
// by TAO_StructDef_i::type_i(), and TAO_UnionDef_i::type_i() during nested
// structure processing.
//-----------------------------------------------------------------------------

TAO_RecursiveDef_OuterScopes::TAO_RecursiveDef_OuterScopes (ACE_TString const &id)
  : id_ (id), pNextOuterID_ (pIDsSeenAlready)
{
  pIDsSeenAlready = this; // "Push" this scope
}

TAO_RecursiveDef_OuterScopes::~TAO_RecursiveDef_OuterScopes()
{
  pIDsSeenAlready = pNextOuterID_; // "Pop" this scope
}

bool TAO_RecursiveDef_OuterScopes::SeenBefore (ACE_TString const &id)
{
  for (TAO_RecursiveDef_OuterScopes const *pOuterScope = pIDsSeenAlready;
        pOuterScope;
        pOuterScope = pOuterScope->pNextOuterID_)
  {
    if (pOuterScope->id_ == id)
      return true;
  }

  return false;
}

TAO_RecursiveDef_OuterScopes const *TAO_RecursiveDef_OuterScopes::pIDsSeenAlready = nullptr;

TAO_END_VERSIONED_NAMESPACE_DECL
