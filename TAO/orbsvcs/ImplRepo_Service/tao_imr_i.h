/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/ImplRepo_Service
//
// = FILENAME
//    tao_imr_i.h
//
// = DESCRIPTION
//    This class implements the Implementation Repository helper application.
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#if !defined (TAO_IMR_I_H)
#define TAO_IMR_I_H

#include "tao/corba.h"
#include "tao/ImplRepoC.h"

class TAO_IMR_Op;
// Forward Declaration

class TAO_IMR_i
{
  // = TITLE
  //     TAO's command line helper application
  //
  // = DESCRIPTION
  //     This class talks to the IMR and registers/lists/etc.
public:
  // = Constructor and destructor.
  TAO_IMR_i (void);
  ~TAO_IMR_i (void);

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

  TAO_IMR_Op *op_;
  // What we need to do.
};

class TAO_IMR_Op
{
public:
  static TAO_IMR_Op *make_op (const ASYS_TCHAR *op_name, ImplementationRepository::Administration_ptr ir);
  // Factory.
  
  TAO_IMR_Op (ImplementationRepository::Administration_ptr implrepo);
  // Constructor.
  
  virtual ~TAO_IMR_Op (void);
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

class TAO_IMR_Op_Activate : public TAO_IMR_Op
{
public:
  TAO_IMR_Op_Activate (ImplementationRepository::Administration_ptr implrepo);
  ~TAO_IMR_Op_Activate (void);

  virtual int parse (int argc, ASYS_TCHAR **argv);
  virtual int run (void);

protected:
  void print_usage (void);
  // Prints a message about the usage

  ACE_TString server_name_;
  // POA server name.
};

class TAO_IMR_Op_Add : public TAO_IMR_Op
{
public:
  TAO_IMR_Op_Add (ImplementationRepository::Administration_ptr implrepo);
  ~TAO_IMR_Op_Add (void);

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

  ImplementationRepository::ActivationMode activation_;
  // Activation mode (0 = NORMAL, 1 = MANUAL, 2 = PER_CLIENT, 3 = AUTO_START)
};

class TAO_IMR_Op_Autostart : public TAO_IMR_Op
{
public:
  TAO_IMR_Op_Autostart (ImplementationRepository::Administration_ptr implrepo);
  ~TAO_IMR_Op_Autostart (void);

  virtual int parse (int argc, ASYS_TCHAR **argv);
  virtual int run (void);

protected:
  void print_usage (void);
  // Prints a message about the usage
};

class TAO_IMR_Op_List : public TAO_IMR_Op
// Used to list the entries in the IMR
{
public:
  TAO_IMR_Op_List (ImplementationRepository::Administration_ptr implrepo);
  ~TAO_IMR_Op_List (void);

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



class TAO_IMR_Op_Remove : public TAO_IMR_Op
// Used to remove entries in the IMR
{
public:
  TAO_IMR_Op_Remove (ImplementationRepository::Administration_ptr implrepo);
  ~TAO_IMR_Op_Remove (void);

  virtual int parse (int argc, ASYS_TCHAR **argv);
  virtual int run (void);

protected:
  void print_usage (void);
  // Prints a message about the usage

  ACE_TString server_name_;
  // POA server name.
};

class TAO_IMR_Op_Shutdown : public TAO_IMR_Op
{
public:
  TAO_IMR_Op_Shutdown (ImplementationRepository::Administration_ptr implrepo);
  ~TAO_IMR_Op_Shutdown (void);

  virtual int parse (int argc, ASYS_TCHAR **argv);
  virtual int run (void);

protected:
  void print_usage (void);
  // Prints a message about the usage

  ACE_TString server_name_;
  // POA server name.
};

class TAO_IMR_Op_Update : public TAO_IMR_Op
{
public:
  TAO_IMR_Op_Update (ImplementationRepository::Administration_ptr implrepo);
  ~TAO_IMR_Op_Update (void);

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

  int set_activation_;
  // True if the activation mode needs to be updated.

  ImplementationRepository::ActivationMode activation_;
  // Activation mode (0 = NORMAL, 1 = MANUAL, 2 = PER_CLIENT, 3 = AUTO_START)
};

#endif /* TAO_IMR_I_H */
