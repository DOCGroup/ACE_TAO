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

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

ACE_RCSID(be, be_interface_strategy, "$Id$")


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

be_interface_strategy::~be_interface_strategy ()
{
  if (this->local_name_ != 0)
    delete [] this->local_name_;
  if (this->full_name_ != 0)
    delete [] this->full_name_;
  if (this->flat_name_ != 0)
    delete [] this->flat_name_;
  if (this->repoID_ != 0)
    delete [] this->repoID_;
  if (this->full_skel_name_ != 0)
    delete [] this->full_skel_name_;
  if (this->full_coll_name_ != 0)
    delete [] this->full_coll_name_;
  if (this->local_coll_name_ != 0)
    delete [] this->local_coll_name_;
  if (this->relative_skel_name_ != 0)
    delete [] this->relative_skel_name_;
}

// Interface Type Strategy Base Class

const char *
be_interface_strategy::relative_skel_name (const char *skel_name)
// relative skeleton name
{
  return be_interface::relative_name (this->full_skel_name (),
                              skel_name);
}



// compute stringified fully qualified collocated class name.
void
be_interface_strategy::compute_coll_names (int type,
                                                const char *prefix,
                                                const char *suffix)
{
  if (type == this->cached_type_ && this->full_coll_name_ != 0)
    return;
  else
    {
      this->cached_type_ = type;
      delete this->full_coll_name_;
      delete this->local_coll_name_;
    }

  static const char *collocated_names[] = { "_tao_thru_poa_collocated_",
                                            "_tao_direct_collocated_" };
  const char poa[] = "POA_";
  // Reserve enough room for the "POA_" prefix, the "_tao_collocated_"
  // prefix and the local name and the (optional) "::"
  const char *collocated = collocated_names[type];

  int name_len = ACE_OS::strlen (collocated) +
                sizeof (poa) +
                1;

  if (prefix)
    name_len += ACE_OS::strlen (prefix);

  if (suffix)
    name_len += ACE_OS::strlen (suffix);

  {
    UTL_IdListActiveIterator *i;
    ACE_NEW (i, UTL_IdListActiveIterator (node_->name ()));
    while (!i->is_done ())
      {
        // reserve 2 characters for "::".
        name_len += ACE_OS::strlen (i->item ()->get_string ()) + 2;
        i->next ();
      }
    delete i;
  }

  ACE_NEW (this->full_coll_name_,
           char[name_len+1]);
  this->full_coll_name_[0] = 0; // null terminate the string...

  // Iterate again....
  UTL_IdListActiveIterator *i;
  ACE_NEW (i, UTL_IdListActiveIterator (node_->name ()));

  // Only the first component get the "POA_" preffix.
  int poa_added = 0;
  while (!i->is_done ())
    {
      const char* item = i->item ()->get_string ();

      // Increase right away, so we can test for the final component
      // in the loop.
      i->next ();

      // We add the POA_ preffix only if the first component is not
      // the global scope...
      if (ACE_OS::strcmp (item, "") != 0)
        {
          if (!i->is_done ())
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
                ACE_OS::strcat (this->full_coll_name_, prefix);

              ACE_OS::strcat (this->full_coll_name_, item);

              if (suffix)
                ACE_OS::strcat (this->full_coll_name_, suffix);
            }
        }
    }
  delete i;

  // Compute the local name for the collocated class.
  int local_len = ACE_OS::strlen (collocated) +
                  ACE_OS::strlen (node_->AST_Interface::local_name ()->get_string ()) +
                  1;
  if (prefix)
    local_len += ACE_OS::strlen (prefix);

  if (suffix)
    local_len += ACE_OS::strlen (suffix);

  ACE_NEW (this->local_coll_name_, char[local_len]);

  ACE_OS::strcpy(this->local_coll_name_, collocated);

  if (prefix)
    ACE_OS::strcat (this->local_coll_name_, prefix);

  ACE_OS::strcat(this->local_coll_name_,
                 node_->AST_Interface::local_name ()->get_string ());

  if (suffix)
    ACE_OS::strcat (this->local_coll_name_, suffix);
}


void
be_interface_strategy::compute_names (const char *name,
                                           const char *prefix,
                                           const char *suffix,
                                           char *&new_name)
{
  if (!prefix || !suffix)
    return;

  int name_length = ACE_OS::strlen (name) +
                    ACE_OS::strlen (prefix) +
                    ACE_OS::strlen (suffix);

  ACE_NEW (new_name,
           char[name_length + 1]);

  // copy it in
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
        if (name[i-3] == 'P' &&
            name[i-2] == 'O' &&
            name[i-1] == 'A' &&
            name[i] == '_')
          {
            interface_name = &name[i+1];
            break;
          }
  }

  if (interface_name == 0)
    interface_name = name;

  ACE_OS::strcpy(&new_name[name_length -
                           ACE_OS::strlen(prefix) -
                           ACE_OS::strlen(interface_name) -
                           ACE_OS::strlen(suffix)],prefix);

  ACE_OS::strcpy(&new_name[name_length -
                           ACE_OS::strlen(interface_name) -
                           ACE_OS::strlen(suffix)],interface_name);

  ACE_OS::strcpy(&new_name[name_length -
                           ACE_OS::strlen(suffix)],suffix);
}


TAO_OutStream *
be_interface_strategy::get_out_stream ()
{
  // Codegen singleton.
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // Outstream.
  return cg->server_skeletons ();
}

const char *
be_interface_strategy::get_out_stream_fname ()
{
  return idl_global->be_get_server_skeleton_fname ();
}

int
be_interface_strategy::strategy_type ()
{
  return strategy_type_;
}

TAO_CodeGen::CG_STATE
be_interface_strategy::next_state (TAO_CodeGen::CG_STATE current_state,
                                   int is_extra_state)
{
  ACE_UNUSED_ARG (is_extra_state);
  return current_state;
}



int 
be_interface_strategy::has_extra_code_generation (TAO_CodeGen::CG_STATE current_state)
{
  ACE_UNUSED_ARG (current_state);
  return 0;
}

be_interface *
be_interface_strategy::replacement (void)
{
  return 0;
}


// ****************************************************************
// AMI Hander Strategy

be_interface_ami_handler_strategy::be_interface_ami_handler_strategy (be_interface *node)
  : be_interface_default_strategy (node, 
                                   AMI_HANDLER)
{
}

be_interface_ami_handler_strategy::~be_interface_ami_handler_strategy ()
{
}


TAO_CodeGen::CG_STATE
be_interface_ami_handler_strategy::next_state (TAO_CodeGen::CG_STATE current_state,
                                               int is_extra_state)
{
  ACE_UNUSED_ARG (is_extra_state);
  return current_state;
}


// ****************************************************************
// AMI Exception Holder Strategy

be_interface_ami_exception_holder_strategy::be_interface_ami_exception_holder_strategy (be_interface *node)
  : be_interface_default_strategy (node, 
                                   AMI_EXCEPTION_HOLDER)

{
}

be_interface_ami_exception_holder_strategy::~be_interface_ami_exception_holder_strategy ()
{
}


TAO_CodeGen::CG_STATE
be_interface_ami_exception_holder_strategy::next_state (TAO_CodeGen::CG_STATE current_state,
                                                        int is_extra_state)
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
    return current_state;
}


int 
be_interface_ami_exception_holder_strategy::has_extra_code_generation (TAO_CodeGen::CG_STATE current_state)
{
  if (current_state == TAO_CodeGen::TAO_VALUETYPE_OBV_CH
   || current_state == TAO_CodeGen::TAO_VALUETYPE_OBV_CS)
    return 1;
  else
    return 0;
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

be_interface_ami_strategy::~be_interface_ami_strategy ()
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
be_interface_ami_strategy::has_extra_code_generation (TAO_CodeGen::CG_STATE current_state)
{
  if (current_state == TAO_CodeGen::TAO_AMI_INTERFACE_CH)
    return 1;
  else
    return 0;
}

be_interface *
be_interface_ami_strategy::replacement (void)
{
  return handler_;
}

// ****************************************************************
// Default Strategy

be_interface_default_strategy::be_interface_default_strategy (be_interface *node,
                                                              Strategy_Kind strategy_kind)
  : be_interface_strategy (node, strategy_kind)
{
}

be_interface_default_strategy::~be_interface_default_strategy ()
{
}

const char *
be_interface_default_strategy::full_name (void)
{
  if (!this->full_name_)
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
    int len = ACE_OS::strlen (node_->AST_Interface::local_name()->get_string ());

    ACE_NEW_RETURN (this->local_name_,
                    char[len + 1],
                    0);

    ACE_OS::strcpy (this->local_name_,
                    node_->AST_Interface::local_name()->get_string ());
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
  if (!this->repoID_)
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



