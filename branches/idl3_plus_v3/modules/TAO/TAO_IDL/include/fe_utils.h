// $Id$

#ifndef FE_UTILS_H
#define FE_UTILS_H

#include "ace/Unbounded_Queue.h"

#include "ast_decl.h"
#include "ast_expression.h"

class UTL_StrList;
class UTL_IdList;
typedef UTL_IdList UTL_ScopedName;
class UTL_NameList;
class AST_PortType;
class AST_Enum;

struct TAO_IDL_FE_Export FE_Utils
{
  struct T_Param_Info
  {
    AST_Decl::NodeType type_;
    AST_Expression::ExprType const_type_;
    AST_Enum *enum_const_type_decl_;
    ACE_CString name_;
    ACE_CString seq_param_ref_;

    T_Param_Info (void);
  };

  typedef ACE_Unbounded_Queue<T_Param_Info> T_PARAMLIST_INFO;

  static bool duplicate_param_id (T_PARAMLIST_INFO *params);

  struct T_Ref_Info
  {
    UTL_ScopedName *name_;
    UTL_StrList *params_;

    T_Ref_Info (void);
    T_Ref_Info (UTL_ScopedName *n, UTL_StrList *strs);

    void destroy (void);
  };

  typedef ACE_Unbounded_Queue<T_Ref_Info> T_REFLIST_INFO;

  struct T_Port_Info
  {
    ACE_CString name_;
    AST_PortType *type_;

    T_Port_Info (const char *name, AST_PortType *type);
  };

  struct T_Inst_Info
  {
    UTL_ScopedName *name_;
    UTL_NameList *args_;

    T_Inst_Info (UTL_ScopedName *n, UTL_NameList *args);

    void destroy (void);
  };

  struct T_ARGLIST : ACE_Unbounded_Queue<AST_Decl *>
  {
    void destroy (void);
  };
};

#endif /* FE_UTILS_H */

