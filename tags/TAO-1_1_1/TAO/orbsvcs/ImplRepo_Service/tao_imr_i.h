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

#ifndef TAO_IMR_I_H
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
  // = TITLE
  //     IMR Operation Base Class
  //
  // = DESCRIPTION
  //     Provides a base class with virtual methods for each operation strategy.
public:
  static TAO_IMR_Op *make_op (const ASYS_TCHAR *op_name);
  // Factory.

  TAO_IMR_Op (void);
  // Constructor.

  virtual void set_imr (ImplementationRepository::Administration_ptr imr);
  // Sets the implrepo pointer

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
  // = TITLE
  //     Activation Operation
  //
  // = DESCRIPTION
  //     Activation is used to start servers via the Implementation Repository
public:
  TAO_IMR_Op_Activate (void);
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
  // = TITLE
  //     Add Operation
  //
  // = DESCRIPTION
  //     Add is used to register information about a server with the IMR.
public:
  TAO_IMR_Op_Add (void);
  ~TAO_IMR_Op_Add (void);

  virtual int parse (int argc, ASYS_TCHAR **argv);
  virtual int run (void);

protected:
  void setenv (ASYS_TCHAR *optarg);
  // Sets one of the environment variables

  void print_usage (void);
  // Prints a message about the usage.

  ACE_TString server_name_;
  // POA server name.

  ACE_TString command_line_;
  // Command line.

  ImplementationRepository::EnvironmentList environment_vars_;
  // Environment Variables.

  ACE_TString working_dir_;
  // Working directory.

  ImplementationRepository::ActivationMode activation_;
  // Activation mode (0 = NORMAL, 1 = MANUAL, 2 = PER_CLIENT, 3 = AUTO_START)
};


class TAO_IMR_Op_Autostart : public TAO_IMR_Op
{
  // = TITLE
  //     Autostart Operation
  //
  // = DESCRIPTION
  //     Autostart is used to activate all servers with the AUTO_START activation
  //     mode.
public:
  TAO_IMR_Op_Autostart (void);
  ~TAO_IMR_Op_Autostart (void);

  virtual int parse (int argc, ASYS_TCHAR **argv);
  virtual int run (void);

protected:
  void print_usage (void);
  // Prints a message about the usage
};


class TAO_IMR_Op_IOR : public TAO_IMR_Op
{
  // = TITLE
  //     IOR Operation
  //
  // = DESCRIPTION
  //     IOR is used to create a simple IOR for a server that uses the
  //     IMR and the Interoperable Naming Service.
public:
  TAO_IMR_Op_IOR (void);
  ~TAO_IMR_Op_IOR (void);

  virtual int parse (int argc, ASYS_TCHAR **argv);
  virtual int run (void);

protected:
  void print_usage (void);
  // Prints a message about the usage

  ACE_TString server_name_;
  // POA server name.

  ACE_TString filename_;
  // Filename to output to.
};


class TAO_IMR_Op_List : public TAO_IMR_Op
{
  // = TITLE
  //     List Operation
  //
  // = DESCRIPTION
  //     List is used to either list all the servers registered in the IMR or just
  //     look at one of them.
public:
  TAO_IMR_Op_List (void);
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
{
  // = TITLE
  //     Remove Operation
  //
  // = DESCRIPTION
  //     Remove is used to unregister a server in the IMR.
public:
  TAO_IMR_Op_Remove (void);
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
  // = TITLE
  //     Shutdown Operation
  //
  // = DESCRIPTION
  //     Shutdown is used to shutdown a server through the IMR.
public:
  TAO_IMR_Op_Shutdown (void);
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
  // = TITLE
  //     Update Operation
  //
  // = DESCRIPTION
  //     Update is used to update the information for a server registered
  //     with the IMR.
public:
  TAO_IMR_Op_Update (void);
  ~TAO_IMR_Op_Update (void);

  virtual int parse (int argc, ASYS_TCHAR **argv);
  virtual int run (void);

protected:
  void setenv (ASYS_TCHAR *optarg);
  // Sets one environment variable.

  void print_usage (void);
  // Prints a message about the usage.

  ACE_TString server_name_;
  // POA server name.

  int set_command_line_;
  // True if the command_line_ needs to be updated.

  ACE_TString command_line_;
  // Startup command.

  int set_environment_vars_;
  // True if the environment_vars_ needs to be updated.

  int set_working_dir_;
  // True if the working_dir_ needs to be updated.

  ImplementationRepository::EnvironmentList environment_vars_;
  // Environment Variables.

  ACE_TString working_dir_;
  // Working directory.

  int set_activation_;
  // True if the activation mode needs to be updated.

  ImplementationRepository::ActivationMode activation_;
  // Activation mode (0 = NORMAL, 1 = MANUAL, 2 = PER_CLIENT, 3 = AUTO_START)
};

#endif /* TAO_IMR_I_H */
