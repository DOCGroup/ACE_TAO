/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_interface_strategy.h
//
// = DESCRIPTION
//    Strategy to cover differences between operations, e.g.
//    the sendc_ and raise_ operations in the AMI spec.
//
// = AUTHOR
//    Michael Kircher
//
// ============================================================================

#ifndef TAO_BE_INTERFACE_STRATEGY_H
#define TAO_BE_INTERFACE_STRATEGY_H

#include "be_codegen.h"

class be_interface;

// This class serves as a strategy base class for the differences
// in generating e.g. ami reply handlers.

class be_interface_strategy
{
public:
  enum Strategy_Kind {
      DEFAULT = 0,
      AMI_INTERFACE,
      AMI_HANDLER,
      AMI_EXCEPTION_HOLDER,
      //      AMH_INTERFACE
  };

  be_interface_strategy (be_interface *node,
                         Strategy_Kind strategy_type);

  virtual ~be_interface_strategy ();

  virtual const char *local_name (void) = 0;
  // return the local name

  virtual const char *full_name (void) = 0;
  // return the stringified full name

  virtual const char *flat_name (void) = 0;
  // return the flattened full scoped name.

  virtual const char *repoID (void) = 0;
  // retrieve the repository ID.

  virtual const char *full_skel_name (void) = 0;
  // retrieve the fully scoped skel class name.

  virtual const char *full_coll_name (int) = 0;
  // retrieve the fully qualified collocated class name

  virtual const char *local_coll_name (int) = 0;
  // retrieve the fully qualified collocated class name.

  virtual const char *base_proxy_impl_name (void) = 0;
  // retrieve the name of the base proxy implementation.

  virtual const char *full_base_proxy_impl_name (void) = 0;
  // retrieve the fully qualified name of the base proxy
  // implementation.

  virtual const char *remote_proxy_impl_name (void) = 0;
  // retrieve the name of the remote  proxy implementation.

  virtual const char *full_remote_proxy_impl_name (void) = 0;
  // retrieve the fully qualified name of the remote
  // proxy implementation.

  virtual const char *thru_poa_proxy_impl_name (void) = 0;
  // retrieve the name of the ThruPOA  proxy implementation.

  virtual const char *full_thru_poa_proxy_impl_name (void) = 0;
  // retrieve the fully qualified name of the ThruPOA  proxy
  // implementation.

  virtual const char *direct_proxy_impl_name (void) =0;
  // retrieve the name of the Directx  proxy implementation.

  virtual const char *full_direct_proxy_impl_name (void) =0;
  // retrieve the fully qualified name of the Directx  proxy
  // implementation.

  virtual const char *base_proxy_broker_name (void) = 0;
  // retrieve the name of the base proxy broker.

  virtual const char *full_base_proxy_broker_name (void) = 0;
  // retrieve the fully qualified name of the base proxy broker.

  virtual const char *remote_proxy_broker_name (void) = 0;
  // retrieve the name of the remote  proxy broker implementation.

  virtual const char *full_remote_proxy_broker_name (void) = 0;
  // retrieve the fully qualified name of the remote  proxy broker
  // implementation.

  virtual const char *strategized_proxy_broker_name (void) = 0;
  // retrieve the name of the strategized proxy broker implementation.

  virtual const char *full_strategized_proxy_broker_name (void) = 0;
  // retrieve the fully qualified name of the strategized proxy broker
  // implementation.

  virtual const char *client_scope (void) = 0;
  // Return the scope that encloses the client related
  // interface.

  virtual const char *flat_client_scope (void) = 0;
  // Return the "flat" version of the scope
  //that encloses the interface.

  virtual const char *server_scope (void) = 0;
  // Return the scope that encloses the server related
  // interface.

  virtual const char *flat_server_scope (void) = 0;
  // Return the "flat" version of the scope
  //that encloses the interface.


  const char *relative_skel_name (const char *skel_name);
  // relative skeleton name

  const char* relative_name (const char *localname,
                             const char *othername);

  void compute_names (const char* name,
                     const char* prefix,
                     const char* suffix,
                     char *&new_name);
  // compute the names using the local prefix and suffix

  void compute_coll_names (int type,
                           const char *prefix,
                           const char *suffix);
  // compute the collocation names

  virtual TAO_OutStream *get_out_stream ();
  // return the out stream depending on the strategy
  // @@ Michael: Right now every strategy behaves
  // the same way, but it might be in the future that we
  // have to differentiate.

  virtual const char *get_out_stream_fname ();
  // return the file name of the output stream.
  // @@ Michael: Right now every strategy behaves the
  // same way.

  int strategy_type ();
  // Return the type of the strategy.

  virtual TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                            int is_extra_state = 0);
  // Change the sate if necessary

  virtual int has_extra_code_generation (TAO_CodeGen::CG_STATE current_state);

  virtual be_interface *replacement (void);
  // Returns the node, which is used instead of the original node,
  // e.g. the reply handler to the actual node.

protected:

  char *local_name_;

  char *full_name_;

  char *flat_name_;

  char *repoID_;

  char *full_skel_name_;

  char *full_coll_name_;

  char *local_coll_name_;

  char *relative_skel_name_;

  be_interface *node_;
  // The node we strategize

  int cached_type_;
  // Current cached collocated name.

  Strategy_Kind strategy_type_;
  // The type of strategy


};



class be_interface_default_strategy
  : public be_interface_strategy
{
public:
  // begin overridden methods.
  be_interface_default_strategy (be_interface *node,
                                 Strategy_Kind strategy_type = DEFAULT);

  virtual ~be_interface_default_strategy ();

  const char * local_name (void);
  // return the local name

  virtual const char *full_name (void);
  // return the stringified full name

  virtual const char *flat_name (void);
  // return the flattened full scoped name.

  virtual const char *repoID (void);
  // retrieve the repository ID.

  virtual const char *full_skel_name (void);
  // retrieve the fully scoped skel class name.

  virtual const char *full_coll_name (int);
  // retrieve the fully qualified collocated class name

  virtual const char *local_coll_name (int);
  // retrieve the fully qualified collocated class name.

  virtual const char *base_proxy_impl_name (void);
  // retrieve the name of the base proxy implementation.

  virtual const char *full_base_proxy_impl_name (void);
  // retrieve the fully qualified name of the base proxy
  // implementation.

  virtual const char *remote_proxy_impl_name (void);
  // retrieve the name of the remote  proxy implementation.

  virtual const char *full_remote_proxy_impl_name (void);
  // retrieve the fully qualified name of the remote
  // proxy implementation.

  virtual const char *thru_poa_proxy_impl_name (void);
  // retrieve the name of the ThruPOA  proxy implementation.

  virtual const char *full_thru_poa_proxy_impl_name (void);
  // retrieve the fully qualified name of the ThruPOA  proxy
  // implementation.

  virtual const char *direct_proxy_impl_name (void);
  // retrieve the name of the Directx  proxy implementation.

  virtual const char *full_direct_proxy_impl_name (void);
  // retrieve the fully qualified name of the Directx  proxy
  // implementation.

  virtual const char *base_proxy_broker_name (void);
  // retrieve the name of the base proxy broker.

  virtual const char *full_base_proxy_broker_name (void);
  // retrieve the fully qualified name of the base proxy broker.

  virtual const char *remote_proxy_broker_name (void);
  // retrieve the name of the remote  proxy broker implementation.

  virtual const char *full_remote_proxy_broker_name (void);
  // retrieve the fully qualified name of the remote  proxy broker
  // implementation.

  virtual const char *strategized_proxy_broker_name (void);
  // retrieve the name of the strategized proxy broker implementation.

  virtual const char *full_strategized_proxy_broker_name (void);
  // retrieve the fully qualified name of the strategized proxy broker
  // implementation.

  virtual const char *client_scope (void);
  // Return the scope that encloses the client related
  // interface.

  virtual const char *flat_client_scope (void);
  // Return the "flat" version of the scope
  //that encloses the interface.

  virtual const char *server_scope (void);
  // Return the scope that encloses the server related
  // interface.

  virtual const char *flat_server_scope (void);
  // Return the "flat" version of the scope
  //that encloses the interface.


  // end of overridden methods

protected:

  char *create_with_prefix_suffix (const char *prefix,
                                   const char *str,
                                   const char *suffix,
                                   const char *separator = "");
  // create a new string made by the concatenation of "str" and "suffix" and using the
  // "separator" to concatenate the two.



protected:

  enum Suffix_Code
  {
    PROXY_IMPL = 0,
    PROXY_BROKER = 1
  };

  enum Tag_Code
  {
    REMOTE = 0,
    THRU_POA = 1,
    DIRECT = 2,
    STRATEGIZED = 3,
    GC_PREFIX = 4 // Prefix used for the generated class
                  // This prefix is used to avoid name conflicts
                  // with the user classes.
  };



  static const char *suffix_table_[];
  static const char *tag_table_[];

  // Proxy Implementation names.
  char *base_proxy_impl_name_;
  char *remote_proxy_impl_name_;
  char *thruPOA_proxy_impl_name_;
  char *direct_proxy_impl_name_;

  char *full_base_proxy_impl_name_;
  char *full_remote_proxy_impl_name_;
  char *full_thruPOA_proxy_impl_name_;
  char *full_direct_proxy_impl_name_;

  // Proxy Broker Names.
  char *base_proxy_broker_;
  char *remote_proxy_broker_;
  char *strategized_proxy_broker_;

  char *full_base_proxy_broker_name_;
  char *full_remote_proxy_broker_name_;
  char *full_strategized_proxy_broker_name_;

  char *client_scope_;
  char *flat_client_scope_;

  char *server_scope_;
  char *flat_server_scope_;
};


class be_interface_ami_handler_strategy
  : public be_interface_default_strategy
{
public:
  be_interface_ami_handler_strategy (be_interface *node);

  virtual ~be_interface_ami_handler_strategy ();

  // overridden methods.
  virtual TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                            int is_extra_state = 0);
};


class be_interface_ami_exception_holder_strategy
  : public be_interface_default_strategy
{
public:
  be_interface_ami_exception_holder_strategy (be_interface *node);

  virtual ~be_interface_ami_exception_holder_strategy ();

  // overridden methods.
  virtual TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                            int is_extra_state = 0);

  virtual int has_extra_code_generation (TAO_CodeGen::CG_STATE current_state);
};

class be_interface_ami_strategy
  : public be_interface_default_strategy
{
public:
  be_interface_ami_strategy (be_interface *node,
                             be_interface *handler);

  virtual ~be_interface_ami_strategy ();

  // overridden methods.
  virtual TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                            int is_extra_state = 0);

  virtual int has_extra_code_generation (TAO_CodeGen::CG_STATE current_state);

  virtual be_interface *replacement (void);

private:
  be_interface *handler_;
};

#endif  // if !defined
