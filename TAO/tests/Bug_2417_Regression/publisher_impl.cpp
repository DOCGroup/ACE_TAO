#include "publisher_impl.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"
#include <atomic>
#include <vector>

struct Subscriber_Entry
{
  Subscriber_var subscriber;
  bool unsubscribed {};
};

class Publisher_impl::Worker : public ACE_Task_Base
{
public:
  explicit Worker (Publisher_impl *owner);
  ~Worker () override = default;
  Worker (Worker const &) = delete;
  Worker &operator= (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker &operator= (Worker &&) = delete;

  void addSubscriber (Subscriber_ptr subscriber);
  int svc () override;
  void terminate ();
private:
  std::atomic<bool> terminated {};
  std::vector<Subscriber_Entry> subscribers;
  TAO_SYNCH_MUTEX mutex;
  // Borrowed from Publisher_impl, which joins this worker before destruction.
  Publisher_impl * const owner_;
};

Publisher_impl::Worker::Worker (Publisher_impl *owner)
  : owner_ (owner)
{
}

void Publisher_impl::Worker::addSubscriber (Subscriber_ptr subscriber)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex);
  subscribers.push_back (Subscriber_Entry {});
  Subscriber_Entry &entry = subscribers.back ();
  entry.subscriber = Subscriber::_duplicate (subscriber);
}

int Publisher_impl::Worker::svc ()
{
  double data {};
  bool do_shutdown {};
  unsigned long iteration {};
  ACE_Time_Value tv;
  tv.set (0.01);
  while (!this->terminated.load ())
    {
      data += 0.01;
      ++iteration;
      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex, 0);
        do_shutdown = !this->subscribers.empty ();
        for (Subscriber_Entry &entry : this->subscribers)
          {
            if (!entry.unsubscribed)
              {
                do_shutdown = false;
                try
                  {
                    if (!CORBA::is_nil (entry.subscriber.in ()))
                      entry.subscriber->onData (data);
                    else
                      entry.unsubscribed = true;
                  }
                catch (...)
                  {
                    entry.unsubscribed = true;
                  }
              }
          }
      }
      if (iteration % 200 == 0)
        {
          ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex, 0);
          for (Subscriber_Entry &entry : this->subscribers)
            {
              if (!entry.unsubscribed)
                {
                  try
                    {
                      entry.subscriber->isAlive ();
                    }
                  catch (...)
                    {
                      entry.unsubscribed = true;
                    }
                }
            }
        }
      if (do_shutdown)
        this->owner_->shutdown ();
      else
        ACE_OS::sleep (tv);
    }
  return 0;
}

void Publisher_impl::Worker::terminate ()
{
  this->terminated.store (true);
}

Publisher_impl::Publisher_impl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , worker_ (std::make_unique<Worker> (this))
{
  this->worker_->activate ();
}

Publisher_impl::~Publisher_impl ()
{
  this->worker_->terminate ();
  this->worker_->thr_mgr ()->wait ();
}

void
Publisher_impl::subscribe (::Subscriber_ptr subscriber)
{
  this->worker_->addSubscriber (subscriber);
}

void
Publisher_impl::shutdown ()
{
  this->orb_->shutdown (false);
  this->worker_->terminate ();
}
