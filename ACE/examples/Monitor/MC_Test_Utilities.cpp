#include <iomanip>
#include <iostream>

#include "ace/Date_Time.h"

#include "MC_Test_Utilities.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

void
MC_Test_Utilities::display_timestamp (
  Monitor_Control_Types::Data const &data)
{
  /// The heavy lifting of converting an ACE_Time_Value
  /// to something that's easily outputted in a pretty
  /// format has already been done in ACE_Date_Time.
  ACE_Date_Time dt (data.timestamp_);
  std::cout << std::setfill ('0')
       << std::setw (2) << dt.month () << '-'
       << std::setw (2) << dt.day () << '-'
       << dt.year () << ' '
       << std::setw (2) << dt.hour () << ':'
       << std::setw (2) << dt.minute () << ':'
       << std::setw (2) << dt.second () << '.'
       << std::setw (6) << dt.microsec () << ":   ";
}

void
MC_Test_Utilities::display_num_threads (
  Monitor_Control_Types::Data const &data)
{
  std::cout << "# of threads:             ";
  display_timestamp (data);
  std::cout << static_cast<unsigned int> (data.value_) << std::endl;
}

void
MC_Test_Utilities::display_mq_size (
  Monitor_Control_Types::Data const &data)
{
  std::cout << "Message queue size:       ";
  display_timestamp (data);
  std::cout << static_cast<unsigned int> (data.value_) << std::endl;
}

void
MC_Test_Utilities::display_memory_usage (
  Monitor_Control_Types::Data const &data)
{
  std::cout << "% memory used:            ";
  display_timestamp (data);
  std::cout << std::setiosflags (std::ios::showpoint | std::ios::fixed)
       << std::setprecision (2) << data.value_ << std::endl;
}

void
MC_Test_Utilities::display_cpu_load (
  Monitor_Control_Types::Data const &data)
{
  std::cout << "% CPU load:               ";
  display_timestamp (data);
  std::cout << std::setiosflags (std::ios::showpoint | std::ios::fixed)
       << std::setprecision (2) << data.value_ << std::endl;
}

void
MC_Test_Utilities::display_bytes_sent (
  Monitor_Control_Types::Data const &data)
{
  std::cout << "total bytes sent:         ";
  display_timestamp (data);
  std::cout << static_cast<ACE_UINT64> (data.value_) << std::endl;
}

void
MC_Test_Utilities::display_bytes_received (
  Monitor_Control_Types::Data const &data)
{
  std::cout << "total bytes received:         ";
  display_timestamp (data);
  std::cout << static_cast<ACE_UINT64> (data.value_) << std::endl;
}

void
MC_Test_Utilities::display_packets_sent (
  Monitor_Control_Types::Data const &data)
{
  std::cout << "total packets sent:         ";
  display_timestamp (data);
  std::cout << static_cast<ACE_UINT64> (data.value_) << std::endl;
}

void
MC_Test_Utilities::display_packets_received (
  Monitor_Control_Types::Data const &data)
{
  std::cout << "total packets received:         ";
  display_timestamp (data);
  std::cout << static_cast<ACE_UINT64> (data.value_) << std::endl;
}

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

