// $Id$

#include "be_connector.h"
#include "be_visitor.h"

#include "utl_identifier.h"

be_connector::be_connector (
      UTL_ScopedName *n,
      AST_Connector *base_connector)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_connector,
              n),
    AST_Type (AST_Decl::NT_connector,
              n),
    UTL_Scope (AST_Decl::NT_connector),
    AST_Interface (n,
                   0,
                   0,
                   0,
                   0,
                   false,
                   false),
    AST_Component (n,
                   base_connector,
                   0,
                   0,
                   0,
                   0),
    AST_Connector (n,
                   base_connector),
    be_scope (AST_Decl::NT_connector),
    be_decl (AST_Decl::NT_connector,
              n),
    be_type (AST_Decl::NT_connector,
             n),
    be_interface (n,
                  0,
                  0,
                  0,
                  0,
                  false,
                  false),
    be_component (n,
                  base_connector,
                  0,
                  0,
                  0,
                  0),
    dds_connector_ (false),
    ami_connector_ (false)
{
  if (! this->imported ())
    {
      this->check_ancestors ();
    }
}

be_connector::~be_connector (void)
{
}

void
be_connector::destroy (void)
{
  this->AST_Connector::destroy ();
  this->be_component::destroy ();
}

int
be_connector::accept (be_visitor *visitor)
{
  return visitor->visit_connector (this);
}

bool
be_connector::dds_connector (void)
{
  return this->dds_connector_;
}

bool
be_connector::ami_connector (void)
{
  return this->ami_connector_;
}

void
be_connector::check_ancestors (void)
{
  /// Start from derived connector since we are no longer
  /// (2010-05-05) putting derived DDS4CCM connector declarations
  /// in application IDL.
  AST_Connector *base = this;

  while (base != 0)
    {
      const char *base_fname = base->full_name ();

      if (ACE_OS::strcmp (base_fname, "CCM_DDS::DDS_TopicBase") == 0)
        {
          this->dds_connector_ = true;
          idl_global->dds_connector_seen_ = true;
          break;
        }
      else if (ACE_OS::strcmp (base_fname, "CCM_AMI::AMI4CCM_Base") == 0)
        {
          this->ami_connector_ = true;
          idl_global->ami_connector_seen_ = true;
          break;
        }

      base = base->base_connector ();
    }
}

IMPL_NARROW_FROM_DECL (be_connector)
IMPL_NARROW_FROM_SCOPE (be_connector)

