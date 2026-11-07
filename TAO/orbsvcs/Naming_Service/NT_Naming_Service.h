// -*- C++ -*-


//=============================================================================
/**
 *  @file    NT_Naming_Service.h
 *
 *  Run the TAO Naming Service as a Windows NT Service.
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
  using MUTEX = TAO_SYNCH_RECURSIVE_MUTEX;

  TAO_NT_Naming_Service ();
  ~TAO_NT_Naming_Service () override;

  TAO_NT_Naming_Service (TAO_NT_Naming_Service const &) = delete;
  TAO_NT_Naming_Service (TAO_NT_Naming_Service &&) = delete;
  TAO_NT_Naming_Service &operator= (TAO_NT_Naming_Service const &) = delete;
  TAO_NT_Naming_Service &operator= (TAO_NT_Naming_Service &&) = delete;

  /// We override <handle_control> because it handles stop requests
  /// privately.
  void handle_control (DWORD control_code) override;

  /// We override <handle_exception> so a 'stop' control code can pop
  /// the reactor off of its wait.
  int handle_exception (ACE_HANDLE h) override;

  /// This is a virtual method inherited from ACE_NT_Service.
  int svc () override;

  /// Initialize the objects argc_ and argv_ attributes values.
  int init (int argc,
            ACE_TCHAR *argv[]) override;

  /// Helper function for setting or reporting command line args
  int set_args (ACE_TCHAR const *args);

private:
  void arg_manip (char *args, DWORD arglen, bool query);
  void report_error (ACE_TCHAR const *format,
                     ACE_TCHAR const *val,
                     LONG result);

  // = Keep track of the "command-line" arguments.
  /// Argument count.
  int argc_ {};
  int argc_save_ {};

  /// Argument list.
  ACE_TCHAR **argv_ {};
  ACE_TCHAR **argv_save_ {};

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
    explicit AutoFinalizer (TAO_NT_Naming_Service &service);
    ~AutoFinalizer ();

private:
    TAO_NT_Naming_Service &service_;
};


using SERVICE = ACE_Singleton<TAO_NT_Naming_Service, TAO_NT_Naming_Service::MUTEX>;

#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_SERVICES */
#endif /* TAO_NT_NAMING_SERVER_H */
