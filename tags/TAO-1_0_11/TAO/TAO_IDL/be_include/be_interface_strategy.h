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
      AMI_EXCEPTION_HOLDER
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

  // end of overridden methods
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
  be_interface_ami_strategy (be_interface *node);

  virtual ~be_interface_ami_strategy ();

  // overridden methods.
  virtual TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                            int is_extra_state = 0);

  virtual int has_extra_code_generation (TAO_CodeGen::CG_STATE current_state);
};


#endif  // if !defined
