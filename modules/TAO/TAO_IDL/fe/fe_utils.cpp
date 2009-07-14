// $Id$

#include "fe_utils.h"

#include "utl_scoped_name.h"
#include "utl_strlist.h"
#include "utl_identifier.h"

FE_Utils::T_Ref_Info::T_Ref_Info (void)
  : name_ (0),
    params_ (0)
{
}

FE_Utils::T_Ref_Info::T_Ref_Info (UTL_ScopedName *n,
                                  UTL_StrList *strs)
  : name_ (n),
    params_ (strs)
{
}

void
FE_Utils::T_Ref_Info::destroy (void)
{
  if (name_ != 0)
    {
      name_->destroy ();
      delete name_;
      name_ = 0;
    }

  if (params_ != 0)
    {
      params_->destroy ();
      delete params_;
      params_ = 0;
    }
}
  
FE_Utils::Ext_Port_Description::Ext_Port_Description (void)
  : port_kind_ (NUM_PORT_KINDS),
    port_name_ (0),
    obj_ref_ (0),
    is_multiple_ (false)
{
}
  
FE_Utils::Ext_Port_Description::Ext_Port_Description (
      Port_Kind port_kind,
      Identifier *port_name,
      T_Ref_Info *obj_ref,
      bool is_multiple)
  : port_kind_ (port_kind),
    port_name_ (port_name),
    obj_ref_ (obj_ref),
    is_multiple_ (is_multiple)
{
}

void
FE_Utils::Ext_Port_Description::destroy (void)
{
  if (port_name_ != 0)
    {
      port_name_->destroy ();
      delete port_name_;
      port_name_ = 0;
    }
    
  if (obj_ref_ != 0)
    {
      obj_ref_->destroy ();
      delete obj_ref_;
      obj_ref_ = 0;
    }
}
