/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    tao_imr_i.h
 *
 *  $Id$
 *
 *  This class implements the Implementation Repository helper application.
 *
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================
#ifndef TAO_IMR_I_H
#define TAO_IMR_I_H

#include "tao/PortableServer/ImR_LocatorC.h"
#include "tao/corba.h"
#include "ace/SString.h"

// Forward Declaration
class TAO_IMR_Op;

/**
 * @class TAO_IMR_i
 *
 * @brief TAO's command line helper application
 *
 * This class talks to the IMR and registers/lists/etc.
 */
class TAO_IMR_i
{
public:
  // = Constructor and destructor.
  TAO_IMR_i (void);
  ~TAO_IMR_i (void);

  /// Execute client code.
  int run (void);

  /// Initialize the client communication endpoint with server.
  int init (int argc, char **argv);

private:
  /// Print out information about all operations.
  void print_usage (void);

  /// Parses the arguments passed on the command line.
  int parse_args (void);

  /// # of arguments on the command line.
  int argc_;

  /// Arguments from command line.
  ACE_TCHAR **argv_;

  /// Remember our orb.
  CORBA::ORB_var orb_;

  /// Reference to our Locator interface of
  /// implementation repository.
  ImplementationRepository::Locator_var imr_locator_;

  /// What we need to do.
  TAO_IMR_Op *op_;
};


/**
 * @class TAO_IMR_Op
 *
 * @brief IMR Operation Base Class
 *
 * Provides a base class with virtual methods for each operation strategy.
 */
class TAO_IMR_Op
{
public:
  /// Factory.
  static TAO_IMR_Op *make_op (const ACE_TCHAR *op_name);

  /// Constructor.
  TAO_IMR_Op (void);

  /// Sets the implrepo locator pointer
  virtual void set_imr_locator (ImplementationRepository::Locator_ptr imr);

  /// Virtual Destructor.
  virtual ~TAO_IMR_Op (void);

  /// Parse arguments.
  virtual int parse (int argc, ACE_TCHAR **argv) = 0;

  /// Do the work.
  virtual int run (void) = 0;

protected:
  /// Reference to our implementation repository.
  ImplementationRepository::Locator_ptr imr_locator_;

  // = Helper methods

  /// Prints out the information contained in a ServerInformation structure.
  void display_server_information (const ImplementationRepository::ServerInformation &info);
};


/**
 * @class TAO_IMR_Op_Activate
 *
 * @brief Activation Operation
 *
 * Activation is used to start servers via the Implementation Repository
 */
class TAO_IMR_Op_Activate : public TAO_IMR_Op
{
public:
  TAO_IMR_Op_Activate (void);
  ~TAO_IMR_Op_Activate (void);

  virtual int parse (int argc, ACE_TCHAR **argv);
  virtual int run (void);

protected:
  /// Prints a message about the usage
  void print_usage (void);

  /// POA server name.
  ACE_CString server_name_;

  ACE_CString location_;
};


/**
 * @class TAO_IMR_Op_Add
 *
 * @brief Add Operation
 *
 * Add is used to register information about a server with the IMR.
 */
class TAO_IMR_Op_Add : public TAO_IMR_Op
{
public:
  TAO_IMR_Op_Add (void);
  ~TAO_IMR_Op_Add (void);

  virtual int parse (int argc, ACE_TCHAR **argv);
  virtual int run (void);

protected:
  /// Sets one of the environment variables
  void setenv (ACE_TCHAR *opt);

  /// Prints a message about the usage.
  void print_usage (void);

  /// POA server name.
  ACE_CString server_name_;

  /// Command line.
  ACE_CString command_line_;

  /// Environment Variables.
  ImplementationRepository::EnvironmentList environment_vars_;

  /// Working directory.
  ACE_CString working_dir_;

  /// Activation mode (0 = NORMAL, 1 = MANUAL, 2 = PER_CLIENT, 3 = AUTO_START)
  ImplementationRepository::ActivationMode activation_;

  /// Hostname where the activator is running.
  ACE_CString location_;
};


/**
 * @class TAO_IMR_Op_Autostart
 *
 * @brief Autostart Operation
 *
 * Autostart is used to activate all servers with the AUTO_START activation
 * mode.
 */
class TAO_IMR_Op_Autostart : public TAO_IMR_Op
{
public:
  TAO_IMR_Op_Autostart (void);
  ~TAO_IMR_Op_Autostart (void);

  virtual int parse (int argc, ACE_TCHAR **argv);
  virtual int run (void);

protected:
  /// Prints a message about the usage
  void print_usage (void);
};


/**
 * @class TAO_IMR_Op_IOR
 *
 * @brief IOR Operation
 *
 * IOR is used to create a simple IOR for a server that uses the
 * IMR and the Interoperable Naming Service.
 */
class TAO_IMR_Op_IOR : public TAO_IMR_Op
{
public:
  TAO_IMR_Op_IOR (void);
  ~TAO_IMR_Op_IOR (void);

  virtual int parse (int argc, ACE_TCHAR **argv);
  virtual int run (void);

protected:
  /// Prints a message about the usage
  void print_usage (void);

  /// POA server name.
  ACE_CString server_name_;

  /// Filename to output to.
  ACE_CString filename_;
};


/**
 * @class TAO_IMR_Op_List
 *
 * @brief List Operation
 *
 * List is used to either list all the servers registered in the IMR or just
 * look at one of them.
 */
class TAO_IMR_Op_List : public TAO_IMR_Op
{
public:
  TAO_IMR_Op_List (void);
  ~TAO_IMR_Op_List (void);

  virtual int parse (int argc, ACE_TCHAR **argv);
  virtual int run (void);

protected:
  /// Prints a message about the usage
  void print_usage (void);

  /// POA server name.
  ACE_CString server_name_;

  /// If true, more server information is displayed.
  int verbose_server_information_;

  /// Prints out the information contained in a ServerInformation structure.
  /// Specialized to only print server information
  void display_server_information (const ImplementationRepository::ServerInformation &info);
};


/**
 * @class TAO_IMR_Op_Remove
 *
 * @brief Remove Operation
 *
 * Remove is used to unregister a server in the IMR.
 */
class TAO_IMR_Op_Remove : public TAO_IMR_Op
{
public:
  TAO_IMR_Op_Remove (void);
  ~TAO_IMR_Op_Remove (void);

  virtual int parse (int argc, ACE_TCHAR **argv);
  virtual int run (void);

protected:
  /// Prints a message about the usage
  void print_usage (void);

  /// POA server name.
  ACE_CString server_name_;

  /// Hostname where the activator is running.
  ACE_CString location_;
};


/**
 * @class TAO_IMR_Op_Shutdown
 *
 * @brief Shutdown Operation
 *
 * Shutdown is used to shutdown a server through the IMR.
 */
class TAO_IMR_Op_Shutdown : public TAO_IMR_Op
{
public:
  TAO_IMR_Op_Shutdown (void);
  ~TAO_IMR_Op_Shutdown (void);

  virtual int parse (int argc, ACE_TCHAR **argv);
  virtual int run (void);

protected:
  /// Prints a message about the usage
  void print_usage (void);

  /// POA server name.
  ACE_CString server_name_;

  ACE_CString location_;
};


/**
 * @class TAO_IMR_Op_Update
 *
 * @brief Update Operation
 *
 * Update is used to update the information for a server registered
 * with the IMR.
 */
class TAO_IMR_Op_Update : public TAO_IMR_Op
{
public:
  TAO_IMR_Op_Update (void);
  ~TAO_IMR_Op_Update (void);

  virtual int parse (int argc, ACE_TCHAR **argv);
  virtual int run (void);

protected:
  /// Sets one environment variable.
  void setenv (ACE_TCHAR *opt);

  /// Prints a message about the usage.
  void print_usage (void);

  /// POA server name.
  ACE_CString server_name_;

  /// True if the command_line_ needs to be updated.
  int set_command_line_;

  /// Startup command.
  ACE_CString command_line_;

  /// True if the environment_vars_ needs to be updated.
  int set_environment_vars_;

  /// True if the working_dir_ needs to be updated.
  int set_working_dir_;

  /// Environment Variables.
  ImplementationRepository::EnvironmentList environment_vars_;

  /// Working directory.
  ACE_CString working_dir_;

  /// True if the activation mode needs to be updated.
  int set_activation_;

  /// Activation mode (0 = NORMAL, 1 = MANUAL, 2 = PER_CLIENT, 3 = AUTO_START)
  ImplementationRepository::ActivationMode activation_;

  /// True if the location mode needs to be updated.
  int set_location_;

  /// Hostname where the activator is running.
  ACE_CString location_;

};

#endif /* TAO_IMR_I_H */
