#ifndef _TEST_HPP_
#define _TEST_HPP_

#include <string>
#include <iostream>

class Test
{
  public:
    Test(const char* begin_msg, const char* end_msg)
      : begin_msg_(begin_msg), end_msg_(end_msg)
    {
    }

    virtual ~Test() { }

    void operator()()
    {
      std::cout << "=========================================\n";
      std::cout << begin_msg_ << std::endl;
      execute();
      std::cout << end_msg_ << std::endl;
      print_results();
      std::cout << "=========================================\n";
    }

  protected:
    virtual void execute() = 0;
    virtual void print_results() = 0;

  private:
    std::string begin_msg_;
    std::string end_msg_;
};

std::string bool_as_string(bool value);

void write_bool_str(bool value);

#include "test.ipp"

#endif // _TEST_HPP_
