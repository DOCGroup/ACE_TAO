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

  enum Port_Kind
  {
    PROVIDES,
    USES,
    PUBLISHES,
    CONSUMES,
    EMITS,
    NUM_PORT_KINDS
  };

  struct Ext_Port_Description
  {
    Port_Kind port_kind_;
    Identifier *port_name_;
    T_Ref_Info *obj_ref_;
    bool is_multiple_;

    Ext_Port_Description (void);
    Ext_Port_Description (Port_Kind port_kind,
                          Identifier *port_name,
                          T_Ref_Info *obj_ref,
                          bool is_multiple = false);

    void destroy (void);
  };

  typedef ACE_Unbounded_Queue<Ext_Port_Description> EXT_PORTLIST_INFO;
};

#endif /* FE_UTILS_H */