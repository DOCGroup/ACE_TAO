#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/Synch.h"
#include "ace/Containers.h"
#include "ace/Task.h"
#include <array>
#include <memory>

class Device
{
public:
  explicit Device (int const id) : id_ (id) {}

  int id_ {};
};

using DeviceList = ACE_DLList<Device>;
using DeviceListIterator = ACE_DLList_Iterator<Device>;

// Listing 1 code/ch14
class HA_DiscoveryAgent
{
public:
  HA_DiscoveryAgent () = default;
  HA_DiscoveryAgent (HA_DiscoveryAgent const &) = delete;
  HA_DiscoveryAgent (HA_DiscoveryAgent &&) = delete;
  HA_DiscoveryAgent &operator= (HA_DiscoveryAgent const &) = delete;
  HA_DiscoveryAgent &operator= (HA_DiscoveryAgent &&) = delete;

  void add_device (Device *device)
  {
    ACE_WRITE_GUARD (ACE_RW_Thread_Mutex, mon, rwmutex_);
    list_add_item_i (device);
  }

  void remove_device (Device *device)
  {
    ACE_WRITE_GUARD (ACE_RW_Thread_Mutex, mon, rwmutex_);
    list_remove_item_i (device);
  }

  int contains_device (Device const *device)
  {
    ACE_READ_GUARD_RETURN
      (ACE_RW_Thread_Mutex, mon, rwmutex_, -1);
    return list_contains_item_i (device);
  }

private:
  void list_add_item_i (Device *device);
  int list_contains_item_i (Device const *device);
  void list_remove_item_i (Device *device);

private:
  DeviceList deviceList_;
  ACE_RW_Thread_Mutex rwmutex_ {};
};
// Listing 1

void
HA_DiscoveryAgent::list_add_item_i (Device *device)
{
  deviceList_.insert_tail (device);
}

int
HA_DiscoveryAgent::list_contains_item_i (Device const *device)
{
  DeviceListIterator iter (deviceList_);
  while (!iter.done ())
    {
      if (iter.next () == device)
        return 1;
      iter.advance ();
    }

  return 0;
}

void
HA_DiscoveryAgent::list_remove_item_i (Device *device)
{
  DeviceListIterator iter (deviceList_);
  while (!iter.done ())
    {
      if (iter.next () == device)
        {
          iter.remove ();
          break;
        }
      iter.advance ();
    }
}

static constexpr int DEVICE_COUNT {100};

class Runner : public ACE_Task_Base
{
public:
  Runner (HA_DiscoveryAgent &agent,
          std::array<std::unique_ptr<Device>, DEVICE_COUNT> const &devices)
    : agent_ (agent), devices_ (devices) {}
  Runner (Runner const &) = delete;
  Runner (Runner &&) = delete;
  Runner &operator= (Runner const &) = delete;
  Runner &operator= (Runner &&) = delete;

  int svc () override
  {
    ACE_ASSERT (agent_.contains_device (devices_[9].get ()) == 1);
    agent_.remove_device (devices_[9].get ());
    ACE_ASSERT (agent_.contains_device (devices_[9].get ()) == 0);
    return 0;
  }

private:
  HA_DiscoveryAgent &agent_;
  std::array<std::unique_ptr<Device>, DEVICE_COUNT> const &devices_;
};

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  std::array<std::unique_ptr<Device>, DEVICE_COUNT> devices {};
  HA_DiscoveryAgent agent;

  for (int i = 0; i < DEVICE_COUNT; ++i)
    {
      devices[i] = std::make_unique<Device> (i);
      agent.add_device (devices[i].get ());
    }

  Runner runner (agent, devices);
  runner.activate ();

  runner.wait ();

  return 0;
}

#else
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::puts (ACE_TEXT ("This example requires threads."));
  return 0;
}

#endif /* ACE_HAS_THREADS */
