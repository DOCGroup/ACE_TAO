// $Id$

#include "ace/Date_Time.h"
#include "ace/streams.h"

#include "examples/Monitor/MC_Test_Utilities.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

void
MC_Test_Utilities::display_timestamp (
  const Monitor_Control_Types::Data &data)
{
  /// The heavy lifting of converting an ACE_Time_Value
  /// to something that's easily outputted in a pretty
  /// format has already been done in ACE_Date_Time.
  ACE_Date_Time dt (data.timestamp_);
  cout << setfill ('0')
       << setw (2) << dt.month () << '-'
       << setw (2) << dt.day () << '-'
       << dt.year () << ' '
       << setw (2) << dt.hour () << ':'
       << setw (2) << dt.minute () << ':'
       << setw (2) << dt.second () << '.'
       << setw (6) << dt.microsec () << ":   ";
}

void
MC_Test_Utilities::display_num_threads (
  const Monitor_Control_Types::Data &data)
{
  cout << "# of threads:             ";
  display_timestamp (data);
  cout << static_cast<unsigned int> (data.value_) << endl;
}

void
MC_Test_Utilities::display_mq_size (
  const Monitor_Control_Types::Data &data)
{
  cout << "Message queue size:       ";
  display_timestamp (data);
  cout << static_cast<unsigned int> (data.value_) << endl;
}

void
MC_Test_Utilities::display_memory_usage (
  const Monitor_Control_Types::Data &data)
{
  cout << "% memory used:            ";
  display_timestamp (data);
  cout << setiosflags (ios::showpoint | ios::fixed)
       << setprecision (2) << data.value_ << endl;
}

void
MC_Test_Utilities::display_cpu_load (
  const Monitor_Control_Types::Data &data)
{
  cout << "% CPU load:               ";
  display_timestamp (data);
  cout << setiosflags (ios::showpoint | ios::fixed)
       << setprecision (2) << data.value_ << endl;
}

void
MC_Test_Utilities::display_bytes_sent (
  const Monitor_Control_Types::Data &data)
{
  cout << "total bytes sent:         ";
  display_timestamp (data);
  cout << static_cast<ACE_UINT64> (data.value_) << endl;
}
void
MC_Test_Utilities::display_bytes_received (
  const Monitor_Control_Types::Data &data)
{
  cout << "total bytes received:         ";
  display_timestamp (data);
  cout << static_cast<ACE_UINT64> (data.value_) << endl;
}

void
MC_Test_Utilities::display_packets_sent (
  const Monitor_Control_Types::Data &data)
{
  cout << "total packets sent:         ";
  display_timestamp (data);
  cout << static_cast<ACE_UINT64> (data.value_) << endl;
}
void
MC_Test_Utilities::display_packets_received (
  const Monitor_Control_Types::Data &data)
{
  cout << "total packets received:         ";
  display_timestamp (data);
  cout << static_cast<ACE_UINT64> (data.value_) << endl;
}

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */



