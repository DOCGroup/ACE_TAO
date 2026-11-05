/* -*- C++ -*- */
#ifndef THERMOMETER_H
#define THERMOMETER_H

#include <cstdlib>
#include <string>
#include <utility>

#include "ace/Log_Msg.h"

class Thermometer
{
public:
  explicit Thermometer (std::string address)
    : addr_ (std::move (address))
  { }

  float temperature ()
  {
    int const success = std::rand () % 10;
    if (success < this->threshold_)
      {
        this->threshold_ = 7;
        return -1.0;
      }

    this->threshold_ = 3;
    int const itemp = 80 + std::rand () % 10; // 80 <= t <= 90
    return static_cast<float> (itemp);
  }

  std::string const &address () const
  {
    return this->addr_;
  }

  void reset ()
  {
    this->threshold_ = 4;
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Resetting thermometer %C\n"),
                this->address ()));
  }

private:
  std::string addr_;
  int threshold_ {5};
};

#endif /* THERMOMETER_H */
