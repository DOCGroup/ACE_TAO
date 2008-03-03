// -*- C++ -*-

//=============================================================================
/**
 * @file LowLevelMonitors_T.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef LOWLEVELMONITORS_T_H
#define LOWLEVELMONITORS_T_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Singleton.h"

#include "MonitorControl/MonitorPoint_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @note This file contains class declarations for specialized monitor
     *       points we will want to have at the OS level. The purpose of
     *       each class should be self-evident from its name.
     */

    /**
     * @class CPULoadMonitor
     *
     * @brief Monitors the load on the CPU.
     */
    template<bool ENABLED>
    class CPULoadMonitor : public MonitorPoint<ENABLED>
    {
      friend class ACE_Singleton<CPULoadMonitor <ENABLED>, ACE_Null_Mutex>;

    private:
      CPULoadMonitor (void);
      virtual ~CPULoadMonitor (void);
    };

    //======================================================

    /**
     * @class MemoryUsageMonitor
     *
     * @brief Monitors the amount of memory in use.
     */
    template<bool ENABLED>
    class MemoryUsageMonitor : public MonitorPoint<ENABLED>
    {
      friend class ACE_Singleton<MemoryUsageMonitor <ENABLED>, ACE_Null_Mutex>;

    private:
      MemoryUsageMonitor (void);
      virtual ~MemoryUsageMonitor (void);
    };

    //=======================================================

    /**
     * @class NumThreadsMonitor
     *
     * @brief Monitors the number of threads.
     */
    template<bool ENABLED>
    class NumThreadsMonitor : public MonitorPoint<ENABLED>
    {
      friend class ACE_Singleton<NumThreadsMonitor <ENABLED>, ACE_Null_Mutex>;

    private:
      NumThreadsMonitor (void);
      virtual ~NumThreadsMonitor (void);
    };

    //=======================================================

    /**
     * @class BytesSentMonitor
     *
     * @brief Counter for the number of bytes sent over the wire.
     */
    template<bool ENABLED>
    class BytesSentMonitor : public MonitorPoint<ENABLED>
    {
      friend class ACE_Singleton<BytesSentMonitor <ENABLED>, ACE_Null_Mutex>;

    private:
      BytesSentMonitor (void);
      virtual ~BytesSentMonitor (void);
    };

    //=======================================================

    /**
     * @class BytesReceivedMonitor
     *
     * @brief Counter for the number of bytes sent over the wire.
     */
    template<bool ENABLED>
    class BytesReceivedMonitor : public MonitorPoint<ENABLED>
    {
      friend class ACE_Singleton<BytesReceivedMonitor <ENABLED>, ACE_Null_Mutex>;

    private:
      BytesReceivedMonitor (void);
      virtual ~BytesReceivedMonitor (void);
    };

    //=======================================================

    /**
     * @class PacketsSentMonitor
     *
     * @brief Counter for the number of packets sent over the wire.
     */
    template<bool ENABLED>
    class PacketsSentMonitor : public MonitorPoint<ENABLED>
    {
      friend class ACE_Singleton<PacketsSentMonitor <ENABLED>, ACE_Null_Mutex>;

    private:
      PacketsSentMonitor (void);
      virtual ~PacketsSentMonitor (void);
    };

    //=======================================================

    /**
     * @class PacketsReceivedMonitor
     *
     * @brief Counter for the number of packets sent over the wire.
     */
    template<bool ENABLED>
    class PacketsReceivedMonitor : public MonitorPoint<ENABLED>
    {
      friend class ACE_Singleton<PacketsReceivedMonitor <ENABLED>, ACE_Null_Mutex>;

    private:
      PacketsReceivedMonitor (void);
      virtual ~PacketsReceivedMonitor (void);
    };
  }
}

typedef ACE::MonitorControl::CPULoadMonitor<true>
  ENABLED_CPU_LOAD_MONITOR;

typedef ACE::MonitorControl::CPULoadMonitor<false>
  DISABLED_CPU_LOAD_MONITOR;

typedef ACE::MonitorControl::MemoryUsageMonitor<true>
  ENABLED_MEMORY_USAGE_MONITOR;

typedef ACE::MonitorControl::MemoryUsageMonitor<false>
  DISABLED_MEMORY_USAGE_MONITOR;

typedef ACE::MonitorControl::NumThreadsMonitor<true>
  ENABLED_NUM_THREADS_MONITOR;

typedef ACE::MonitorControl::NumThreadsMonitor<false>
  DISABLED_NUM_THREADS_MONITOR;

typedef ACE::MonitorControl::BytesSentMonitor<true>
  ENABLED_BYTES_SENT_MONITOR;

typedef ACE::MonitorControl::BytesSentMonitor<false>
  DISABLED_BYTES_SENT_MONITOR;

typedef ACE::MonitorControl::PacketsSentMonitor<true>
  ENABLED_PACKETS_SENT_MONITOR;

typedef ACE::MonitorControl::PacketsSentMonitor<false>
  DISABLED_PACKETS_SENT_MONITOR;

typedef ACE::MonitorControl::BytesReceivedMonitor<true>
  ENABLED_BYTES_RECEIVED_MONITOR;

typedef ACE::MonitorControl::BytesReceivedMonitor<false>
  DISABLED_BYTES_RECEIVED_MONITOR;

typedef ACE::MonitorControl::PacketsReceivedMonitor<true>
  ENABLED_PACKETS_RECEIVED_MONITOR;

typedef ACE::MonitorControl::PacketsReceivedMonitor<false>
  DISABLED_PACKETS_RECEIVED_MONITOR;

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "MonitorControl/LowLevelMonitors_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("LowLevelMonitors_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"

#endif // LOWLEVELMONITORS_T_H
