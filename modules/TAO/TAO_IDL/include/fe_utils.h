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

  struct T_ARGLIST : ACE_Unbounded_Queue<AST_Decl *>
  {
    void destroy (void);
  };
};

#endif /* FE_UTILS_H */

