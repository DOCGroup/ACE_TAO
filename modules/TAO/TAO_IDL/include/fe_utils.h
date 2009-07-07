// $Id$

#ifndef FE_UTILS_H
#define FE_UTILS_H

#include "ace/Unbounded_Queue.h"

#include "ast_decl.h"

class UTL_StrList;
class UTL_IdList;
typedef UTL_IdList UTL_ScopedName;

struct TAO_IDL_FE_Export FE_Utils
{
  struct T_Param_Info
  {
    AST_Decl::NodeType type;
    ACE_CString name;
  };

  typedef ACE_Unbounded_Queue<T_Param_Info> T_PARAMLIST_INFO;

  struct T_Ref_Info
  {
    UTL_ScopedName *name;
    UTL_StrList *params;

    T_Ref_Info (void);
    T_Ref_Info (UTL_ScopedName *n, UTL_StrList *strs);
    
    void destroy (void);
  };

  typedef ACE_Unbounded_Queue<T_Ref_Info> T_REFLIST_INFO;
};

#endif /* FE_UTILS_H */