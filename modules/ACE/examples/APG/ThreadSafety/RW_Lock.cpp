// $Id$

#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/Synch.h"
#include "ace/Containers.h"
#include "ace/Task.h"

class Device
{
public:
  Device (int id) : id_(id)
  { }

  int id_;
};

typedef ACE_DLList<Device> DeviceList;
typedef ACE_DLList_Iterator<Device> DeviceListIterator;

// Listing 1 code/ch14
class HA_DiscoveryAgent
{
public:
  void add_device (Device *device)
  {
    ACE_WRITE_GUARD (ACE_RW_Thread_Mutex, mon, rwmutex_);
    list_add_item_i (device);
  }

  void remove_device (Device *device)
  {
    ACE_READ_GUARD (ACE_RW_Thread_Mutex, mon, rwmutex_);
    list_remove_item_i(device);
  }

  int contains_device (Device *device)
  {
    ACE_READ_GUARD_RETURN
      (ACE_RW_Thread_Mutex, mon, rwmutex_, -1);
    return list_contains_item_i (device);
  }

private:
  void list_add_item_i (Device * device);
  int list_contains_item_i (Device * device);
  void list_remove_item_i (Device* device);

private:
  DeviceList deviceList_;
  ACE_RW_Thread_Mutex rwmutex_;
};
// Listing 1

void
HA_DiscoveryAgent::list_add_item_i (Device *device)
{
  deviceList_.insert_tail (device);
}

int
HA_DiscoveryAgent::list_contains_item_i (Device *device)
{
  DeviceListIterator iter (deviceList_);
  while (!iter.done ())
    {
      if (iter.next () == device)
        return 1;
      iter++;
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
      iter++;
    }
}

static Device *devices[100];

class Runner : public ACE_Task_Base
{
public:
  Runner(HA_DiscoveryAgent &agent) : agent_(agent)
  { }

  virtual int svc (void)
  {
    ACE_ASSERT(agent_.contains_device(devices[9]) == 1);
    agent_.remove_device (devices[9]);
    ACE_ASSERT(agent_.contains_device(devices[9]) == 0);
    return 0;
  }

private:
  HA_DiscoveryAgent &agent_;
};

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  HA_DiscoveryAgent agent;

  for (int i = 0; i < 100; i++)
    {
      devices[i]  = new Device (i);
      agent.add_device (devices[i]);
    }

  Runner runner (agent);
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
