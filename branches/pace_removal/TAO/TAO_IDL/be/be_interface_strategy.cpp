// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_interface_strategy.cpp
//
// = DESCRIPTION
//    A strategy to cover the differences between various variants
//    of an interface
//
// = AUTHOR
//    Michael Kircher
//
// ============================================================================

#include "be_interface_strategy.h"
#include "be_interface.h"
#include "be_extern.h"
#include "utl_identifier.h"

ACE_RCSID (be, 
           be_interface_strategy, 
           "$Id$")


be_interface_strategy::be_interface_strategy (be_interface *node,
                                              Strategy_Kind strategy_type)
  : local_name_(0),
    full_name_(0),
    flat_name_(0),
    repoID_(0),
    full_skel_name_(0),
    full_coll_name_(0),
    local_coll_name_(0),
    relative_skel_name_(0),
    node_ (node),
    cached_type_ (-1),
    strategy_type_ (strategy_type)
{
}

be_interface_strategy::~be_interface_strategy (void)
{
  if (this->local_name_ != 0)
    {
      delete [] this->local_name_;
      this->local_name_ = 0;
    }

  if (this->full_name_ != 0)
    {
      delete [] this->full_name_;
      this->full_name_ = 0;
    }

  if (this->flat_name_ != 0)
    {
      delete [] this->flat_name_;
      this->flat_name_ = 0;
    }

  if (this->repoID_ != 0)
    {
      delete [] this->repoID_;
      this->repoID_ = 0;
    }

  if (this->full_skel_name_ != 0)
    {
      delete [] this->full_skel_name_;
      this->full_skel_name_ = 0;
    }

  if (this->full_coll_name_ != 0)
    {
      delete [] this->full_coll_name_;
      this->full_coll_name_ = 0;
    }

  if (this->local_coll_name_ != 0)
    {
      delete [] this->local_coll_name_;
      this->local_coll_name_ = 0;
    }

  if (this->relative_skel_name_ != 0)
    {
      delete [] this->relative_skel_name_;
      this->relative_skel_name_ = 0;
    }
}

// Interface Type Strategy Base Class

// Relative skeleton name.
const char *
be_interface_strategy::relative_skel_name (const char *skel_name)
{
  return be_interface::relative_name (this->full_skel_name (),
                                      skel_name);
}



// Compute stringified fully qualified collocated class name.
void
be_interface_strategy::compute_coll_names (int type,
                                           const char *prefix,
                                           const char *suffix)
{
  if (type == this->cached_type_ && this->full_coll_name_ != 0)
    {
      return;
    }
  else
    {
      this->cached_type_ = type;
      delete [] this->full_coll_name_;
      delete [] this->local_coll_name_;
    }

  static const char *collocated_names[] = {"_tao_thru_poa_collocated_",
                                           "_tao_direct_collocated_"};
  static const char *poa = "POA_";

  // Reserve enough room for the "POA_" prefix, the "_tao_collocated_"
  // prefix and the local name and the (optional) "::"
  const char *collocated = collocated_names[type];

  int name_len = ACE_OS::strlen (collocated)
                 + ACE_OS::strlen (poa)
                 + 1;

  if (prefix)
    {
      name_len += ACE_OS::strlen (prefix);
    }

  if (suffix)
    {
      name_len += ACE_OS::strlen (suffix);
    }

  for (UTL_IdListActiveIterator i (this->node_->name ());
       !i.is_done ();
       i.next ())
    {
      // Reserve 2 characters for "::".
      name_len += ACE_OS::strlen (i.item ()->get_string ()) + 2;
    }

  ACE_NEW (this->full_coll_name_,
           char[name_len + 1]);

  // Null terminate the string.
  this->full_coll_name_[0] = 0;

  // Only the first component get the "POA_" preffix.
  int poa_added = 0;

  // Iterate again.
  // Must advance the iterator explicitly inside the loop.
  for (UTL_IdListActiveIterator j (this->node_->name ());
       !j.is_done ();)
    {
      const char *item = j.item ()->get_string ();

      // Increase right away, so we can test for the final component
      // in the loop.
      j.next ();

      // We add the POA_ preffix only if the first component is not
      // the global scope...
      if (ACE_OS::strcmp (item, "") != 0)
        {
          if (!j.is_done ())
            {
              // We only add the POA_ preffix if there are more than
              // two components in the name, in other words, if the
              // class is inside some scope.
              if (!poa_added)
                {
                  ACE_OS::strcat (this->full_coll_name_, poa);
                  poa_added = 1;
                }
              ACE_OS::strcat (this->full_coll_name_, item);
              ACE_OS::strcat (this->full_coll_name_, "::");
            }
          else
            {
              ACE_OS::strcat (this->full_coll_name_, collocated);

              if (prefix)
                {
                  ACE_OS::strcat (this->full_coll_name_, prefix);
                }

              ACE_OS::strcat (this->full_coll_name_, item);

              if (suffix)
                {
                  ACE_OS::strcat (this->full_coll_name_, suffix);
                }
            }
        }
    }

  // Compute the local name for the collocated class.
  char *local_name = this->node_->AST_Interface::local_name ()->get_string ();
  int local_len = ACE_OS::strlen (collocated)
                  + ACE_OS::strlen (local_name)
                  + 1;
  if (prefix)
    {
      local_len += ACE_OS::strlen (prefix);
    }

  if (suffix)
    {
      local_len += ACE_OS::strlen (suffix);
    }

  ACE_NEW (this->local_coll_name_,
           char[local_len]);

  ACE_OS::strcpy (this->local_coll_name_,
                  collocated);

  if (prefix)
    {
      ACE_OS::strcat (this->local_coll_name_,
                      prefix);
    }

  ACE_OS::strcat (this->local_coll_name_,
                  node_->AST_Interface::local_name ()->get_string ());

  if (suffix)
    {
      ACE_OS::strcat (this->local_coll_name_, suffix);
    }
}


void
be_interface_strategy::compute_names (const char *name,
                                      const char *prefix,
                                      const char *suffix,
                                      char *&new_name)
{
  if (!prefix || !suffix)
    {
      return;
    }

  int name_length = ACE_OS::strlen (name) +
    ACE_OS::strlen (prefix) +
    ACE_OS::strlen (suffix);

  ACE_NEW (new_name,
           char[name_length + 1]);

  // Copy it in.
  ACE_OS::strcpy (new_name, name);

  const char *interface_name = 0;
  int i = ACE_OS::strlen (name);

  for (;i >= 1; i--)
    {
      if (name[i-1] == ':' && name[i] == ':')
        {
          interface_name = &name[i+1];
          break;
        }
      else if (i >= 3)
        {
          if (name[i-3] == 'P' &&
              name[i-2] == 'O' &&
              name[i-1] == 'A' &&
              name[i] == '_')
            {
              interface_name = &name[i+1];
              break;
            }
        }
    }

  if (interface_name == 0)
    {
      interface_name = name;
    }

  ACE_OS::strcpy (&new_name[name_length
                           - ACE_OS::strlen (prefix)
                           - ACE_OS::strlen (interface_name)
                           - ACE_OS::strlen (suffix)],
                  prefix);

  ACE_OS::strcpy (&new_name[name_length
                           - ACE_OS::strlen (interface_name)
                           - ACE_OS::strlen (suffix)],
                  interface_name);

  ACE_OS::strcpy (&new_name[name_length
                           - ACE_OS::strlen(suffix)],
                  suffix);
}


TAO_OutStream *
be_interface_strategy::get_out_stream (void)
{
  // Outstream.
  return tao_cg->server_skeletons ();
}

const char *
be_interface_strategy::get_out_stream_fname (void)
{
  return be_global->be_get_server_skeleton_fname ();
}

int
be_interface_strategy::strategy_type (void)
{
  return strategy_type_;
}

TAO_CodeGen::CG_STATE
be_interface_strategy::next_state (TAO_CodeGen::CG_STATE current_state,
                                   int /* is_extra_state */)
{
  return current_state;
}

int
be_interface_strategy::has_extra_code_generation (
    TAO_CodeGen::CG_STATE /* current_state */
  )
{
  return 0;
}

be_interface *
be_interface_strategy::replacement (void)
{
  return 0;
}


// ****************************************************************
// AMI Hander Strategy

be_interface_ami_handler_strategy::be_interface_ami_handler_strategy (
    be_interface *node
  )
  : be_interface_default_strategy (node,
                                   AMI_HANDLER)
{
}

be_interface_ami_handler_strategy::~be_interface_ami_handler_strategy (void)
{
}


TAO_CodeGen::CG_STATE
be_interface_ami_handler_strategy::next_state (
    TAO_CodeGen::CG_STATE current_state,
    int /*is_extra_state */
  )
{
  return current_state;
}


// ****************************************************************
// AMI Exception Holder Strategy

be_interface_ami_exception_holder_strategy::
be_interface_ami_exception_holder_strategy (be_interface *node)
  : be_interface_default_strategy (node,
                                   AMI_EXCEPTION_HOLDER)
{
}

be_interface_ami_exception_holder_strategy
::~be_interface_ami_exception_holder_strategy (void)
{
}


TAO_CodeGen::CG_STATE
be_interface_ami_exception_holder_strategy::next_state (
    TAO_CodeGen::CG_STATE current_state,
    int is_extra_state
  )
{
  if (is_extra_state)
    {
      switch (current_state)
        {
        case TAO_CodeGen::TAO_VALUETYPE_OBV_CH:
          return TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_VALUETYPE_CH;
        case TAO_CodeGen::TAO_VALUETYPE_OBV_CS:
          return TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_VALUETYPE_CS;
        default:
          return current_state;
        }
    }
  else
    {
      return current_state;
    }
}


int
be_interface_ami_exception_holder_strategy::has_extra_code_generation (
    TAO_CodeGen::CG_STATE current_state
  )
{
  if (current_state == TAO_CodeGen::TAO_VALUETYPE_OBV_CH
      || current_state == TAO_CodeGen::TAO_VALUETYPE_OBV_CS)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}


// ****************************************************************
// AMI Strategy

be_interface_ami_strategy::be_interface_ami_strategy (be_interface *node,
                                                      be_interface *handler)
  : be_interface_default_strategy (node,
                                   AMI_INTERFACE),
  handler_ (handler)
{
}

be_interface_ami_strategy::~be_interface_ami_strategy (void)
{
}


TAO_CodeGen::CG_STATE
be_interface_ami_strategy::next_state (TAO_CodeGen::CG_STATE current_state,
                                       int is_extra_state)
{
  if (is_extra_state)
    {
      switch (current_state)
        {
        case TAO_CodeGen::TAO_AMI_INTERFACE_CH:
          return TAO_CodeGen::TAO_INTERFACE_CH;
        default:
          return current_state;
        }
    }
  else
    {
      switch (current_state)
        {
        case TAO_CodeGen::TAO_INTERFACE_CH:
          return TAO_CodeGen::TAO_AMI_INTERFACE_CH;
        default:
          return current_state;
        }
    }
}


int
be_interface_ami_strategy::has_extra_code_generation (
    TAO_CodeGen::CG_STATE current_state
  )
{
  if (current_state == TAO_CodeGen::TAO_AMI_INTERFACE_CH)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

be_interface *
be_interface_ami_strategy::replacement (void)
{
  return handler_;
}

// ****************************************************************
// Default Strategy

const char *be_interface_default_strategy::suffix_table_[] =
{
  "_Proxy_Impl",
  "_Proxy_Broker"
};

const char *be_interface_default_strategy::tag_table_[] =
{
  "_Remote",
  "_ThruPOA",
  "_Direct",
  "_Strategized",
  "_TAO_"
};


be_interface_default_strategy::be_interface_default_strategy (
    be_interface *node,
    Strategy_Kind strategy_kind
  )
  : be_interface_strategy (node,
                           strategy_kind),
    base_proxy_impl_name_ (0),
    remote_proxy_impl_name_ (0),
    thruPOA_proxy_impl_name_ (0),
    direct_proxy_impl_name_ (0),
    full_base_proxy_impl_name_ (0),
    full_remote_proxy_impl_name_ (0),
    full_thruPOA_proxy_impl_name_ (0),
    full_direct_proxy_impl_name_ (0),
    base_proxy_broker_ (0),
    remote_proxy_broker_ (0),
    strategized_proxy_broker_ (0),
    full_base_proxy_broker_name_ (0),
    full_remote_proxy_broker_name_ (0),
    full_strategized_proxy_broker_name_(0),
    client_scope_ (0),
    flat_client_scope_ (0),
    server_scope_ (0),
    flat_server_scope_ (0)
{
}

be_interface_default_strategy::~be_interface_default_strategy (void)
{
  if (this->base_proxy_impl_name_ != 0)
    {
      delete this->base_proxy_impl_name_;
      this->base_proxy_impl_name_ = 0;
    }

  if (this->remote_proxy_impl_name_ != 0)
    {
      delete this->remote_proxy_impl_name_;
      this->remote_proxy_impl_name_ = 0;
    }

  if (this->thruPOA_proxy_impl_name_ != 0)
    {
      delete this->thruPOA_proxy_impl_name_;
      this->thruPOA_proxy_impl_name_ = 0;
    }

  if (this->direct_proxy_impl_name_ != 0)
    {
      delete this->direct_proxy_impl_name_;
      this->direct_proxy_impl_name_ = 0;
    }

  if (this->full_base_proxy_impl_name_ != 0)
    {
      delete this->full_base_proxy_impl_name_;
      this->full_base_proxy_impl_name_ = 0;
    }

  if (this->full_remote_proxy_impl_name_ != 0)
    {
      delete this->full_remote_proxy_impl_name_;
      this->full_remote_proxy_impl_name_ = 0;
    }

  if (this->full_thruPOA_proxy_impl_name_ != 0)
    {
      delete this->full_thruPOA_proxy_impl_name_;
      this->full_thruPOA_proxy_impl_name_ = 0;
    }

  if (this->full_direct_proxy_impl_name_ != 0)
    {
      delete this->full_direct_proxy_impl_name_;
      this->full_direct_proxy_impl_name_ = 0;
    }

  if (this->base_proxy_broker_ != 0)
    {
      delete this->base_proxy_broker_;
      this->base_proxy_broker_ = 0;
    }

  if (this->remote_proxy_broker_ != 0)
    {
      delete this->remote_proxy_broker_;
      this->remote_proxy_broker_ = 0;
    }

  if (this->strategized_proxy_broker_ != 0)
    {
      delete this->strategized_proxy_broker_;
      this->strategized_proxy_broker_ = 0;
    }

  if (this->full_base_proxy_broker_name_ != 0)
    {
      delete this->full_base_proxy_broker_name_;
      this->full_base_proxy_broker_name_ = 0;
    }

  if (this->full_remote_proxy_broker_name_ != 0)
    {
      delete this->full_remote_proxy_broker_name_;
      this->full_remote_proxy_broker_name_ = 0;
    }

  if (this->full_strategized_proxy_broker_name_ != 0)
    {
      delete this->full_strategized_proxy_broker_name_;
      this->full_strategized_proxy_broker_name_ = 0;
    }

  if (this->client_scope_ != 0)
    {
      delete this->client_scope_;
      this->client_scope_ = 0;
    }

  if (this->flat_client_scope_ != 0)
    {
      delete this->flat_client_scope_;
      this->flat_client_scope_ = 0;
    }

  if (this->server_scope_ != 0)
    {
      delete this->server_scope_;
      this->server_scope_ = 0;
    }

  if (this->flat_server_scope_ != 0)
    {
      delete this->flat_server_scope_;
      this->flat_server_scope_ = 0;
    }
}

const char *
be_interface_default_strategy::full_name (void)
{
  if (this->full_name_ == 0)
    {
      int len = ACE_OS::strlen (node_->be_decl::full_name ());

      ACE_NEW_RETURN (this->full_name_,
                      char[len + 1],
                      0);

      ACE_OS::strcpy (this->full_name_,
                      node_->be_decl::full_name ());
    }

  return this->full_name_;
}

const char *
be_interface_default_strategy::local_name (void)
{
  if (!this->local_name_)
    {
      int len = 
        ACE_OS::strlen (node_->AST_Interface::local_name ()->get_string ());

      ACE_NEW_RETURN (this->local_name_,
                      char[len + 1],
                      0);

      ACE_OS::strcpy (this->local_name_,
                      node_->AST_Interface::local_name ()->get_string ());
    }

  return this->local_name_;
}

const char *
be_interface_default_strategy::flat_name (void)
{
  if (!this->flat_name_)
    {
      int len = ACE_OS::strlen (node_->be_decl::flat_name ());

      ACE_NEW_RETURN (this->flat_name_,
                      char[len + 1],
                      0);

      ACE_OS::strcpy (this->flat_name_,
                      node_->be_decl::flat_name ());
    }

  return this->flat_name_;
}

const char *
be_interface_default_strategy::repoID (void)
{
  if (this->repoID_ == 0)
    {
      int len = ACE_OS::strlen (node_->be_decl::repoID ());

      ACE_NEW_RETURN (this->repoID_,
                      char[len + 1],
                      0);

      ACE_OS::strcpy (this->repoID_,
                      node_->be_decl::repoID ());
    }

  return this->repoID_;
}

const char *
be_interface_default_strategy::full_skel_name (void)
{
  if (this->full_skel_name_ == 0)
    {
      // the following method is inherited from the base class
      node_->compute_full_skel_name ("POA_",
                                     this->full_skel_name_);
    }

  return this->full_skel_name_;
}


const char *
be_interface_default_strategy::full_coll_name (int type)
{
  this->compute_coll_names (type,
                            0,  // prefix
                            0); // suffix

  return this->full_coll_name_;
}

const char *
be_interface_default_strategy::local_coll_name (int type)
{
  this->compute_coll_names (type,
                            0,  // prefix
                            0); // suffix

  return this->local_coll_name_;
}

char *
be_interface_default_strategy::create_with_prefix_suffix (
    const char *prefix,
    const char *str,
    const char *suffix,
    const char *separator
  )
{
  char *cat_string = 0;
  unsigned int length =
    ACE_OS::strlen (str) +
    ACE_OS::strlen (prefix) +
    ACE_OS::strlen (suffix) +
    ACE_OS::strlen (separator) +
    1; // The '/0'

  ACE_NEW_RETURN (cat_string, 
                  char[length], 
                  0);

  ACE_OS::strcpy (cat_string, prefix);
  ACE_OS::strcat (cat_string, str);
  ACE_OS::strcat (cat_string, separator);
  ACE_OS::strcat (cat_string, suffix);

  return cat_string;
}

const char *
be_interface_default_strategy::base_proxy_impl_name (void)
{
  if (this->base_proxy_impl_name_ != 0)
    {
      return this->base_proxy_impl_name_;
    }

  this->base_proxy_impl_name_ =
    this->create_with_prefix_suffix (this->tag_table_[GC_PREFIX],
                                     this->node_->local_name (),
                                     this->suffix_table_[PROXY_IMPL]);

  return this->base_proxy_impl_name_;
}


const char *
be_interface_default_strategy::client_scope (void)
{
  if (this->client_scope_ != 0)
    {
      return this->client_scope_;
    }

  const char *full_name = this->full_name ();
  const char *name = this->local_name ();

  int offset = ACE_OS::strlen (name);
  int length = ACE_OS::strlen (full_name) - offset;
  ACE_NEW_RETURN (this->client_scope_,
                  char[length + 1],
                  0);

  ACE_OS::strncpy (this->client_scope_, full_name, length);
  this->client_scope_[length] = '\0';

  return this->client_scope_;
}
const char *
be_interface_default_strategy::flat_client_scope (void)
{
  if (this->flat_client_scope_ != 0)
    {
      return this->flat_client_scope_;
    }

  const char *full_name = this->flat_name ();
  const char *name = this->local_name ();

  int offset = ACE_OS::strlen (name);
  int length = ACE_OS::strlen (full_name) - offset;
  ACE_NEW_RETURN (this->flat_client_scope_,
                  char[length + 1],
                  0);

  ACE_OS::strncpy (this->flat_client_scope_, full_name, length);
  this->flat_client_scope_[length] = '\0';

  return this->flat_client_scope_;
}


const char *
be_interface_default_strategy::server_scope (void)
{
  if (this->server_scope_ != 0)
    {
      return this->server_scope_;
    }

  const char *full_name =
    this->node_->full_coll_name (be_interface::DIRECT);

  const char *name = this->node_->local_coll_name (be_interface::DIRECT);

  int offset = ACE_OS::strlen (name);
  int length = ACE_OS::strlen (full_name) - offset;
  ACE_NEW_RETURN (this->server_scope_,
                  char[length + 1],
                  0);

  ACE_OS::strncpy (this->server_scope_, full_name, length);
  this->server_scope_[length] = '\0';

  return this->server_scope_;
}

const char *
be_interface_default_strategy::flat_server_scope (void)
{
  if (this->flat_server_scope_ != 0)
    {
      return this->flat_server_scope_;
    }

  const char *full_name = this->flat_name ();
  const char *name = this->local_name ();

  int offset = ACE_OS::strlen (name);
  int length = ACE_OS::strlen (full_name) - offset;
  ACE_NEW_RETURN (this->flat_client_scope_,
                  char[length + 1],
                  0);

  ACE_OS::strncpy (this->flat_server_scope_, full_name, length);
  this->flat_server_scope_[length] = '\0';

  return this->flat_server_scope_;
}



const char *
be_interface_default_strategy::full_base_proxy_impl_name (void)
{
  if (this->full_base_proxy_impl_name_ != 0)
    {
      return this->full_base_proxy_impl_name_;
    }

  const char *scope = this->client_scope ();
  const char *base_name = this->base_proxy_impl_name ();
  int length = ACE_OS::strlen (scope) + ACE_OS::strlen (base_name);
  ACE_NEW_RETURN (this->full_base_proxy_impl_name_,
                  char[length + 1],
                  0);

  ACE_OS::strcpy (this->full_base_proxy_impl_name_, scope);
  ACE_OS::strcat (this->full_base_proxy_impl_name_, base_name);

  return this->full_base_proxy_impl_name_;
}



const char *
be_interface_default_strategy::remote_proxy_impl_name (void)
{
  if (this->remote_proxy_impl_name_ != 0)
    {
      return this->remote_proxy_impl_name_;
    }


  this->remote_proxy_impl_name_ =
    this->create_with_prefix_suffix (this->tag_table_[GC_PREFIX],
                                     this->node_->local_name (),
                                     this->suffix_table_[PROXY_IMPL],
                                     this->tag_table_[REMOTE]);

  return this->remote_proxy_impl_name_;
}

const char *
be_interface_default_strategy::full_remote_proxy_impl_name (void)
{
  if (this->full_remote_proxy_impl_name_ != 0)
    {
      return this->full_remote_proxy_impl_name_;
    }

  const char *scope = this->client_scope ();
  const char *base_name = this->remote_proxy_impl_name ();
  int length = ACE_OS::strlen (scope) + ACE_OS::strlen (base_name);
  ACE_NEW_RETURN (this->full_remote_proxy_impl_name_,
                  char[length + 1],
                  0);

  ACE_OS::strcpy (this->full_remote_proxy_impl_name_, scope);
  ACE_OS::strcat (this->full_remote_proxy_impl_name_, base_name);

  return this->full_remote_proxy_impl_name_;
}

const char *
be_interface_default_strategy::thru_poa_proxy_impl_name (void)
{
  if (this->thruPOA_proxy_impl_name_ != 0)
    {
      return this->thruPOA_proxy_impl_name_;
    }

  this->thruPOA_proxy_impl_name_ =
    this->create_with_prefix_suffix (this->tag_table_[GC_PREFIX],
                                     this->node_->local_name (),
                                     this->suffix_table_[PROXY_IMPL],
                                     this->tag_table_[THRU_POA]);

  return this->thruPOA_proxy_impl_name_;
}

const char *
be_interface_default_strategy::full_thru_poa_proxy_impl_name (void)
{
  if (this->full_thruPOA_proxy_impl_name_ != 0)
    {
      return this->full_thruPOA_proxy_impl_name_;
    }

  const char *scope = this->server_scope ();
  const char *base_name = this->thru_poa_proxy_impl_name ();
  int length = ACE_OS::strlen (scope) + ACE_OS::strlen (base_name);
  ACE_NEW_RETURN (this->full_thruPOA_proxy_impl_name_,
                  char[length + 1],
                  0);

  ACE_OS::strcpy (this->full_thruPOA_proxy_impl_name_, scope);
  ACE_OS::strcat (this->full_thruPOA_proxy_impl_name_, base_name);

  return this->full_thruPOA_proxy_impl_name_;
}


const char *
be_interface_default_strategy::direct_proxy_impl_name (void)
{
  if (this->direct_proxy_impl_name_ != 0)
    {
      return this->direct_proxy_impl_name_;
    }

  this->direct_proxy_impl_name_ =
    this->create_with_prefix_suffix (this->tag_table_[GC_PREFIX],
                                     this->node_->local_name (),
                                     this->suffix_table_[PROXY_IMPL],
                                     this->tag_table_[DIRECT]);

  return this->direct_proxy_impl_name_;
}


const char *
be_interface_default_strategy::full_direct_proxy_impl_name (void)
{
  if (this->full_direct_proxy_impl_name_ != 0)
    {
      return this->full_direct_proxy_impl_name_;
    }

  const char *scope = this->server_scope ();
  const char *base_name = this->direct_proxy_impl_name ();

  int length = ACE_OS::strlen (scope) + ACE_OS::strlen (base_name);
  ACE_NEW_RETURN (this->full_direct_proxy_impl_name_,
                  char[length + 1],
                  0);

  ACE_OS::strcpy (this->full_direct_proxy_impl_name_, scope);
  ACE_OS::strcat (this->full_direct_proxy_impl_name_, base_name);

  return this->full_direct_proxy_impl_name_;
}


const char *
be_interface_default_strategy::base_proxy_broker_name (void)
{
  if (this->base_proxy_broker_ != 0)
    {
      return this->base_proxy_broker_;
    }

  this->base_proxy_broker_ =
    this->create_with_prefix_suffix (this->tag_table_[GC_PREFIX],
                                     this->node_->local_name (),
                                     this->suffix_table_[PROXY_BROKER]);

  return this->base_proxy_broker_;
}


const char *
be_interface_default_strategy::full_base_proxy_broker_name (void)
{
  if (this->full_base_proxy_broker_name_ != 0)
    {
      return this->full_base_proxy_broker_name_;
    }

  const char *scope = this->client_scope ();
  const char *base_name = this->base_proxy_broker_name ();
  int length = ACE_OS::strlen (scope) + ACE_OS::strlen (base_name);
  ACE_NEW_RETURN (this->full_base_proxy_broker_name_,
                  char[length + 1],
                  0);

  ACE_OS::strcpy (this->full_base_proxy_broker_name_, scope);
  ACE_OS::strcat (this->full_base_proxy_broker_name_, base_name);

  return this->full_base_proxy_broker_name_;
}

const char *
be_interface_default_strategy::remote_proxy_broker_name (void)
{
  if (this->remote_proxy_broker_ != 0)
    {
      return this->remote_proxy_broker_;
    }

  this->remote_proxy_broker_ =
    this->create_with_prefix_suffix (this->tag_table_[GC_PREFIX],
                                     this->node_->local_name (),
                                     this->suffix_table_[PROXY_BROKER],
                                     this->tag_table_[REMOTE]);

  return this->remote_proxy_broker_;
}


const char *
be_interface_default_strategy::full_remote_proxy_broker_name (void)
{
  if (this->full_remote_proxy_broker_name_ != 0)
    {
      return this->full_remote_proxy_broker_name_;
    }

  const char *scope = this->client_scope ();
  const char *base_name = this->remote_proxy_broker_name ();
  int length = ACE_OS::strlen (scope) + ACE_OS::strlen (base_name);
  ACE_NEW_RETURN (this->full_remote_proxy_broker_name_,
                  char[length + 1],
                  0);

  ACE_OS::strcpy (this->full_remote_proxy_broker_name_, scope);
  ACE_OS::strcat (this->full_remote_proxy_broker_name_, base_name);

  return this->full_remote_proxy_broker_name_;
}


const char *
be_interface_default_strategy::strategized_proxy_broker_name (void)
{
  if (this->strategized_proxy_broker_ != 0)
    {
      return this->strategized_proxy_broker_;
    }

  this->strategized_proxy_broker_ =
    this->create_with_prefix_suffix (this->tag_table_[GC_PREFIX],
                                     this->node_->local_name (),
                                     this->suffix_table_[PROXY_BROKER],
                                     this->tag_table_[STRATEGIZED]);

  return this->strategized_proxy_broker_;
}

const char *
be_interface_default_strategy::full_strategized_proxy_broker_name (void)
{
  if (this->full_strategized_proxy_broker_name_ != 0)
    {
      return this->full_strategized_proxy_broker_name_;
    }

  const char *scope = this->server_scope ();
  const char *base_name = this->strategized_proxy_broker_name ();
  int length = ACE_OS::strlen (scope) + ACE_OS::strlen (base_name);
  ACE_NEW_RETURN (this->full_strategized_proxy_broker_name_,
                  char[length + 1],
                  0);

  ACE_OS::strcpy (this->full_strategized_proxy_broker_name_, scope);
  ACE_OS::strcat (this->full_strategized_proxy_broker_name_, base_name);

  return this->full_strategized_proxy_broker_name_;
}
