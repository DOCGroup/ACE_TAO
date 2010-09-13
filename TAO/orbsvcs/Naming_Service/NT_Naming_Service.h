// -*- C++ -*-


//=============================================================================
/**
 *  @file    NT_Naming_Service.h
 *
 *  $Id$
 *
 *  Run the TAO Naming Service as a Windows NT Service.
 *
 *
 *  @author John Tucker <jtucker@infoglide.com> and Mike Vitalo <mvitalo@infoglide.com>
 */
//=============================================================================


#ifndef TAO_NT_NAMING_SERVICE_H
#define TAO_NT_NAMING_SERVICE_H

#include /**/ "ace/NT_Service.h"

#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_SERVICES)

#include /**/ "ace/Singleton.h"
#include /**/ "ace/Synch.h"
#include /**/ "tao/orbconf.h"

/**
 * @class TAO_NT_Naming_Service
 *
 * @brief Run the TAO Naming Service as a Windows NT Service.
 */
class TAO_NT_Naming_Service : public ACE_NT_Service
{
public:
  typedef TAO_SYNCH_RECURSIVE_MUTEX MUTEX;

  // = Initialization and termination hooks.
  TAO_NT_Naming_Service (void);
  virtual ~TAO_NT_Naming_Service (void);

  /// We override <handle_control> because it handles stop requests
  /// privately.
  virtual void handle_control (DWORD control_code);

  /// We override <handle_exception> so a 'stop' control code can pop
  /// the reactor off of its wait.
  virtual int handle_exception (ACE_HANDLE h);

  /// This is a virtual method inherited from ACE_NT_Service.
  virtual int svc (void);

  /// Initialize the objects argc_ and argv_ attributes values.
  virtual int init (int argc,
                    ACE_TCHAR *argv[]);

private:
  // = Keep track of the "command-line" arguments.
  /// Argument count.
  int argc_;
  int argc_save_;

  /// Argument list.
  ACE_TCHAR **argv_;
  ACE_TCHAR **argv_save_;

  friend class ACE_Singleton<TAO_NT_Naming_Service, MUTEX>;
  friend class AutoFinalizer;

};

/**
 * A class with a "magic" constructor/desructor to call the inherited
 * from ACE_NT_Service report_status() when an instance of it goes out
 * of scope, should an exception occurs.
  */
class AutoFinalizer
{

public:
    AutoFinalizer (TAO_NT_Naming_Service &service);
    ~AutoFinalizer ();

private:
    TAO_NT_Naming_Service &service_;
};



typedef ACE_Singleton<TAO_NT_Naming_Service, TAO_NT_Naming_Service::MUTEX> SERVICE;

#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_SERVICES */
#endif /* TAO_NT_NAMING_SERVER_H */
