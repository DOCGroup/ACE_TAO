/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/ImplRepo_Service
//
// = FILENAME
//    tao_ir_i.h
//
// = DESCRIPTION
//    This class implements the Implementation Repository helper application.
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#if !defined (TAO_IR_I_H)
#define TAO_IR_I_H

#include "tao/corba.h"
#include "orbsvcs/ImplRepoC.h"

class TAO_IR_Op;
// Forward Declaration

class TAO_IR_i
{
  // = TITLE
  //     TAO's command line helper application
  //
  // = DESCRIPTION
  //     This class talks to the IR and registers/lists/etc.
public:
  // = Constructor and destructor.
  TAO_IR_i (void);
  ~TAO_IR_i (void);

  int run (void);
  // Execute client code.

  int init (int argc, char **argv);
  // Initialize the client communication endpoint with server.

private:
  void print_usage (void);
  // Print out information about all operations.

  int parse_args (void);
  // Parses the arguments passed on the command line.

  int argc_;
  // # of arguments on the command line.

  ASYS_TCHAR **argv_;
  // arguments from command line.

  CORBA::ORB_var orb_;
  // Remember our orb.

  ImplementationRepository::Administration_var implrepo_;
  // Reference to our implementation repository.

  TAO_IR_Op *op_;
  // What we need to do.
};

class TAO_IR_Op
{
public:
  static TAO_IR_Op *make_op (const ASYS_TCHAR *op_name, ImplementationRepository::Administration_ptr ir);
  // Factory.
  
  TAO_IR_Op (ImplementationRepository::Administration_ptr implrepo);
  // Constructor.
  
  virtual ~TAO_IR_Op (void);
  // Virtual Destructor.

  virtual int parse (int argc, ASYS_TCHAR **argv) = 0;
  // Parse arguments.

  virtual int run (void) = 0;
  // Do the work.

protected:
  ImplementationRepository::Administration_ptr implrepo_;
  // Reference to our implementation repository.

  // = Helper methods

  void display_server_information (const ImplementationRepository::ServerInformation &info);
  // Prints out the information contained in a ServerInformation structure.
};

class TAO_IR_Op_Activate : public TAO_IR_Op
{
public:
  TAO_IR_Op_Activate (ImplementationRepository::Administration_ptr implrepo);
  ~TAO_IR_Op_Activate (void);

  virtual int parse (int argc, ASYS_TCHAR **argv);
  virtual int run (void);

protected:
  void print_usage (void);
  // Prints a message about the usage

  ACE_TString server_name_;
  // POA server name.
};

class TAO_IR_Op_Add : public TAO_IR_Op
{
public:
  TAO_IR_Op_Add (ImplementationRepository::Administration_ptr implrepo);
  ~TAO_IR_Op_Add (void);

  virtual int parse (int argc, ASYS_TCHAR **argv);
  virtual int run (void);

protected:
  void print_usage (void);
  // Prints a message about the usage.

  ACE_TString server_name_;
  // POA server name.

  ACE_TString command_line_;
  // Command line.

  ACE_TString working_dir_;
  // Working directory.
};


// Used to list the entries in the IR

class TAO_IR_Op_List : public TAO_IR_Op
{
public:
  TAO_IR_Op_List (ImplementationRepository::Administration_ptr implrepo);
  ~TAO_IR_Op_List (void);

  virtual int parse (int argc, ASYS_TCHAR **argv);
  virtual int run (void);

protected:
  void print_usage (void);
  // Prints a message about the usage

  ACE_TString server_name_;
  // POA server name.

  int verbose_server_information_;
  // If true, more server information is displayed.

  void display_server_information (const ImplementationRepository::ServerInformation &info);
  // Prints out the information contained in a ServerInformation structure.
  // Specialized to only print server information
};

class TAO_IR_Op_Remove : public TAO_IR_Op
{
public:
  TAO_IR_Op_Remove (ImplementationRepository::Administration_ptr implrepo);
  ~TAO_IR_Op_Remove (void);

  virtual int parse (int argc, ASYS_TCHAR **argv);
  virtual int run (void);

protected:
  void print_usage (void);
  // Prints a message about the usage

  ACE_TString server_name_;
  // POA server name.
};

class TAO_IR_Op_Shutdown : public TAO_IR_Op
{
public:
  TAO_IR_Op_Shutdown (ImplementationRepository::Administration_ptr implrepo);
  ~TAO_IR_Op_Shutdown (void);

  virtual int parse (int argc, ASYS_TCHAR **argv);
  virtual int run (void);

protected:
  void print_usage (void);
  // Prints a message about the usage

  ACE_TString server_name_;
  // POA server name.
};

class TAO_IR_Op_Update : public TAO_IR_Op
{
public:
  TAO_IR_Op_Update (ImplementationRepository::Administration_ptr implrepo);
  ~TAO_IR_Op_Update (void);

  virtual int parse (int argc, ASYS_TCHAR **argv);
  virtual int run (void);

protected:
  void print_usage (void);
  // Prints a message about the usage.

  ACE_TString server_name_;
  // POA server name.

  int set_command_line_;
  // True if the command_line_ needs to be updated.
  
  ACE_TString command_line_;
  // Startup command.

  int set_working_dir_;
  // True if the working_dir_ needs to be updated.

  ACE_TString working_dir_;
  // Working directory.
};

#endif /* TAO_IR_I_H */
