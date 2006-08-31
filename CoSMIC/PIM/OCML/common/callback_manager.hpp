#ifndef CALLBACK_MANAGER_HPP
#define CALLBACK_MANAGER_HPP

#include <list>

namespace common
{

  template <typename LISTENER, typename OBJECT, typename METHOD>
  class listeners
  {
  private:
    typedef std::list<LISTENER*> listeners_type;

  public:
    typedef typename listeners_type::iterator index;

  public:
    listeners(const OBJECT* object, METHOD method)
      : object_(object), method_(method)
    {
    }

    index register_listener(LISTENER* l)
    {
      listeners_.push_front(l);
      return listeners_.begin();
    }

    void unregister_listener(index i)
    {
      listeners_.erase(i);
    }

    void call()
    {
      for (index i = listeners_.begin(); i != listeners_.end(); ++i)
      {
        ((**i).*method_)(object_);
      }
    }

  private:
    const OBJECT* object_;
    METHOD method_;
    listeners_type listeners_;
  };

} // namespace common

#endif // CALLBACK_MANAGER_HPP
