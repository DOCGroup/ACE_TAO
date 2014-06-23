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

#include "tao/ImR_Client/ImplRepoC.h"
#include "tao/corba.h"
#include "ace/SString.h"
#include "ace/Auto_Ptr.h"

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
  int init (int argc, ACE_TCHAR **argv);

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
  ImplementationRepository::Administration_var imr_;

  /// What we need to do.
  ACE_Auto_Ptr<TAO_IMR_Op> op_;
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
  enum RETURN_CODES {
    NORMAL = 0,
    UNKNOWN,
    NO_PERMISSION,
    ALREADY_REGISTERED,
    CANNOT_ACTIVATE,
    NOT_FOUND,
    CANNOT_COMPLETE
  };

  /// Factory.
  static TAO_IMR_Op *make_op (const ACE_TCHAR *op_name);

  /// Destructor.
  virtual ~TAO_IMR_Op (void);

  /// Parse arguments.
  virtual int parse (int argc, ACE_TCHAR **argv) = 0;

  /// Do the work.
  virtual int run (void) = 0;

  /// Sets the implrepo locator pointer
  void set_imr (ImplementationRepository::Administration_ptr imr);

protected:
  /// Reference to our implementation repository.
  ImplementationRepository::Administration_ptr imr_;

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
  virtual int parse (int argc, ACE_TCHAR **argv);
  virtual int run (void);

protected:
  /// Prints a message about the usage
  void print_usage (void);

  /// POA server name.
  ACE_CString server_name_;
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
  virtual int parse (int argc, ACE_TCHAR **argv);
  virtual int run (void);

protected:
  /// Prints a message about the usage
  void print_usage (void);

  /// POA server name.
  ACE_CString server_name_;

  /// Filename to output to.
  ACE_TString filename_;
};


/**
 * @class TAO_IMR_Op_Kill
 *
 * @brief Kill Operation
 *
 * Kill is used to unregister a server in the IMR.
 */
class TAO_IMR_Op_Kill : public TAO_IMR_Op
{
public:
  virtual int parse (int argc, ACE_TCHAR **argv);
  virtual int run (void);

protected:
  /// Prints a message about the usage
  void print_usage (void);

  ACE_CString server_name_;
  int signum_;
};


/**
 * @class TAO_IMR_Op_Link
 *
 * @brief Link Operation
 *
 * Link is used to unregister a server in the IMR.
 */
class TAO_IMR_Op_Link : public TAO_IMR_Op
{
public:
  virtual int parse (int argc, ACE_TCHAR **argv);
  virtual int run (void);

protected:
  /// Prints a message about the usage
  void print_usage (void);

  ACE_CString server_name_;

  CORBA::StringSeq peers_;
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

  virtual int parse (int argc, ACE_TCHAR **argv);
  virtual int run (void);

protected:
  /// Prints a message about the usage
  void print_usage (void);

  /// POA server name.
  ACE_CString server_name_;

  /// If true, more server information is displayed.
  int verbose_server_information_;

  /// If true, only show servers that are currently active.
  int list_only_active_servers_;

  /// retrieve no more than this number of entries at a time
  CORBA::ULong how_many_;

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
  virtual int parse (int argc, ACE_TCHAR **argv);
  virtual int run (void);

protected:
  /// Prints a message about the usage
  void print_usage (void);

  ACE_CString server_name_;
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
  virtual int parse (int argc, ACE_TCHAR **argv);
  virtual int run (void);

protected:
  /// Prints a message about the usage
  void print_usage (void);

  ACE_CString server_name_;
};

/**
 * Shutdown the ImR and optionally any registered activators.
 */
class TAO_IMR_Op_ShutdownRepo : public TAO_IMR_Op
{
public:
  TAO_IMR_Op_ShutdownRepo();
  virtual int parse (int argc, ACE_TCHAR **argv);
  virtual int run (void);

protected:
  void print_usage (void);

  bool activators_;
};

/**
 * @class TAO_IMR_Op_Register
 *
 * @brief Register Operation
 *
 * Register is used to update/add information for a server
 * with the IMR.
 */
class TAO_IMR_Op_Register : public TAO_IMR_Op
{
public:
  TAO_IMR_Op_Register(bool is_add);

  virtual int parse (int argc, ACE_TCHAR **argv);
  virtual int run (void);

protected:

  /// Enables pre-registration checks
  bool is_add_;

  /// Sets one environment variable.
  void addenv (ACE_TCHAR *opt);

  /// Prints a message about the usage.
  void print_usage (void);

  /// POA server name.
  ACE_CString server_name_;

  bool set_command_line_;
  ACE_CString command_line_;

  bool set_environment_vars_;
  ImplementationRepository::EnvironmentList environment_vars_;

  bool set_working_dir_;
  ACE_CString working_dir_;

  bool set_activation_;
  ImplementationRepository::ActivationMode activation_;

  bool set_retry_count_;
  int retry_count_;

  bool set_activator_;
  ACE_CString activator_;
};

#endif /* TAO_IMR_I_H */
