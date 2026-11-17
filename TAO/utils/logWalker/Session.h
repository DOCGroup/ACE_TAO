// -*- C++ -*-

//

#ifndef LOG_WALKER_SESSION_H
#define LOG_WALKER_SESSION_H

#include "ace/Hash_Map_Manager.h"
#include "ace/RB_Tree.h"
#include "ace/Functor.h"
#include "ace/SString.h"
#include "ace/Synch.h"

#include <memory>
#include <ostream>
#include <utility>

class Log;
class HostProcess;
class Endpoint;

using Processes = ACE_RB_Tree<long, HostProcess *, ACE_Less_Than<long>, ACE_Null_Mutex>;
using Procs_By_Name = ACE_RB_Tree<ACE_CString, HostProcess *, ACE_Less_Than<ACE_CString>, ACE_Null_Mutex>;
using AltAddresses = ACE_Hash_Map_Manager<const ACE_CString, ACE_CString, ACE_Null_Mutex>;

class Session
{
public:
  class Output_Stream
  {
  public:
    Output_Stream () = default;
    explicit Output_Stream (std::ostream *stream);
    explicit Output_Stream (std::unique_ptr<std::ostream> stream);
    Output_Stream (Output_Stream &&) = default;
    Output_Stream &operator= (Output_Stream &&) = default;
    Output_Stream (Output_Stream const &) = delete;
    Output_Stream &operator= (Output_Stream const &) = delete;

    std::ostream *get () const;
    std::ostream &operator* () const;

  private:
    std::unique_ptr<std::ostream> owned_;
    // When owned_ is empty, stream_ refers to a borrowed stream such as stdout.
    std::ostream *stream_ {};
  };

  Session ();

  ~Session ();

  void add_process (HostProcess *proc);
  HostProcess *find_process (long pid);
  HostProcess *find_host (const Endpoint &endpoint, bool server);
  void reconcile ();

  static bool set_tao_version (ACE_TCHAR const *str);
  static long tao_version ();
  static void alternate_address (const char *string);
  static bool is_equivalent (const ACE_CString &primary,
                             const ACE_CString &alternate);
  void default_service (const char *string);

  void make_dir (const char * ,bool );
  void outfile (const char * );

  bool has_dir () const;
  bool has_outfile () const;

  void dump ();
  Output_Stream stream_for (Output_Stream old,
                        HostProcess *hp = nullptr,
                        const char *sub = nullptr,
                        const char *detail = nullptr);

private:
  HostProcess *find_host_i (const Endpoint &endpoint, bool server);

  // processes_ owns its HostProcess pointers; procs_by_name_ is a non-owning index.
  Processes processes_;
  Procs_By_Name procs_by_name_;
  ACE_CString base_dir_;
  ACE_CString outfile_;
  bool split_details_;
  static long tao_version_;
  static AltAddresses alt_addrs_;
};

#endif // LOG_WALKER_SESSION_H
