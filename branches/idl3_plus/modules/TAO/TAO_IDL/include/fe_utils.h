// $Id$

#ifndef FE_UTILS_H
#define FE_UTILS_H

#include "ace/Unbounded_Queue.h"

#include "ast_decl.h"

class UTL_StrList;
class UTL_IdList;
typedef UTL_IdList UTL_ScopedName;
class UTL_NameList;

struct TAO_IDL_FE_Export FE_Utils
{
  struct T_Param_Info
  {
    AST_Decl::NodeType type_;
    ACE_CString name_;
  };

  typedef ACE_Unbounded_Queue<T_Param_Info> T_PARAMLIST_INFO;

  struct T_Ref_Info
  {
    UTL_ScopedName *name_;
    UTL_StrList *params_;

    T_Ref_Info (void);
    T_Ref_Info (UTL_ScopedName *n, UTL_StrList *strs);

    void destroy (void);
  };

  typedef ACE_Unbounded_Queue<T_Ref_Info> T_REFLIST_INFO;

  struct T_Inst_Info
  {
    UTL_ScopedName *name_;
    UTL_NameList *args_;

    T_Inst_Info (UTL_ScopedName *n, UTL_NameList *args);

    void destroy (void);
  };
};

#endif /* FE_UTILS_H */