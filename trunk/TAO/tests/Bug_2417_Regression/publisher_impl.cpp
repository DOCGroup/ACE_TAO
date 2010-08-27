// $Id$

#include "publisher_impl.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Task.h"
#include "ace/Process_Mutex.h"
#include "ace/OS_NS_unistd.h"
#include <vector>

using namespace std;

struct _Subscriber
{
  Subscriber_var subscriber;
  bool unsubscribed;
  unsigned int count;
};

class Publisher_impl::Worker : public ACE_Task_Base
{
public:
  Worker(Publisher_impl * _owner);
  ~Worker();
  void addSubscriber(Subscriber_ptr subscriber);
  virtual int svc (void);
  void terminate();
private:
  bool terminated;
  vector<_Subscriber> subscribers;
  TAO_SYNCH_MUTEX mutex;
  Publisher_impl * owner;
};

Publisher_impl::Worker::Worker(Publisher_impl * _owner)
  : owner(_owner)
{
  terminated = false;
}

Publisher_impl::Worker::~Worker()
{
  terminated = true;
}

void Publisher_impl::Worker::addSubscriber(Subscriber_ptr subscriber)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex);
  subscribers.push_back(_Subscriber());
  _Subscriber& s = subscribers.back();
  s.unsubscribed = false;
  s.subscriber = Subscriber::_duplicate(subscriber);
  s.count = 0;
}

int Publisher_impl::Worker::svc (void)
{
  double data = 0.0;
  bool doShutdown = false;
  unsigned long iteration = 0;
  ACE_Time_Value tv;
  tv.set(0.01);
  while (!terminated)
    {
      data += 0.01;
      ++iteration;
      {
        ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex);
        doShutdown = subscribers.size() > 0;
        for (vector<_Subscriber>::iterator iter = subscribers.begin();
             iter != subscribers.end(); ++iter)
          {
            if (!iter->unsubscribed)
              {
                doShutdown = false;
                try
                  {
                    if (!CORBA::is_nil(iter->subscriber.in ()))
                      iter->subscriber->onData(data);
                    else
                      iter->unsubscribed = true;
                    ++iter->count;
                  }
                catch (...)
                  {
                    iter->unsubscribed = true;
                  }
              }
          }
      }
      if (iteration % 200 == 0)
        {
          ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex);
          for (vector<_Subscriber>::iterator iter = subscribers.begin();
               iter != subscribers.end(); ++iter)
            {
              if (!iter->unsubscribed)
                {
                  try
                    {
                      iter->subscriber->isAlive();
                    }
                  catch (...)
                    {
                      iter->unsubscribed = true;
                    }
                }
            }
        }
      if (doShutdown)
        owner->shutdown();
      else
        ACE_OS::sleep(tv);
    }
  return 0;
}

void Publisher_impl::Worker::terminate()
{
  terminated = true;
}

Publisher_impl::Publisher_impl(CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
  worker = new Worker(this);
  worker->activate();
}

Publisher_impl::~Publisher_impl()
{
  worker->terminate();
  worker->thr_mgr()->wait();
  delete worker;
}

void subscribe (
                ::Subscriber_ptr subscriber
                );

void
Publisher_impl::subscribe(
                          ::Subscriber_ptr subscriber
                          )
{
  worker->addSubscriber(subscriber);
}

void
Publisher_impl::shutdown (
                          )
{
  this->orb_->shutdown (0);
  worker->terminate();
}
