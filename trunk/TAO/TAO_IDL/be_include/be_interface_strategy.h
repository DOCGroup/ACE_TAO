/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_interface.h
//
// = DESCRIPTION
//    Extension of class AST_Interface that provides additional means for C++
//    mapping of an interface.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale,
//    Michael Kircher
//
// ============================================================================

#ifndef TAO_BE_INTERFACE_STRATEGY_H
#define TAO_BE_INTERFACE_STRATEGY_H

#include "be_codegen.h"

class be_interface;

// This class serves as a strategy base class for the differences
// in generating e.g. ami reply handlers.

class be_interface_type_strategy
{
public:
  enum Strategy_Kind {
      DEFAULT = 0,
      AMI_HANDLER,
      AMI_EXCEPTION_HOLDER
  };

  be_interface_type_strategy (be_interface *node,
                              Strategy_Kind strategy_type);

  virtual ~be_interface_type_strategy ();

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

  TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state) = 0;
  // Change the sate if necessary

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
  // 
};

class be_interface_prefix_suffix_strategy
  : public be_interface_type_strategy
{
public:
  // begin overridden methods.
  be_interface_prefix_suffix_strategy (be_interface *node,
                                       Strategy_Kind strategy_type,
                                       const char *prefix,
                                       const char *suffix);

  virtual ~be_interface_prefix_suffix_strategy ();

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
protected:

  const char *prefix_;
  // The prefix to the interface

  const char *suffix_;
  // The suffix to the interface
};


class be_interface_ami_handler_strategy
  : public be_interface_prefix_suffix_strategy
{
public:
  // begin overridden methods.
  be_interface_ami_handler_strategy (be_interface *node);

  virtual ~be_interface_ami_handler_strategy ();

  // overridden methods.
  TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state);
};


class be_interface_ami_exception_holder_strategy
  : public be_interface_prefix_suffix_strategy
{
public:
  // begin overridden methods.
  be_interface_ami_exception_holder_strategy (be_interface *node);

  virtual ~be_interface_ami_exception_holder_strategy ();

  // overridden methods.
  TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state);
};


class be_interface_default_strategy
  : public be_interface_type_strategy
{
public:
  // begin overridden methods.
  be_interface_default_strategy (be_interface *node);

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

  TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state);
  // end of overridden methods
};

#endif  // if !defined
