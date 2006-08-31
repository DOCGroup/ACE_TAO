#include "definitions_test.hpp"
#include "test.hpp"

#include <iostream>
#include <memory>

class Definition_Test: public Test
{
  public:
    Definition_Test()
      : Test("Creating definitions...", "Definitions created.")
    {
    }

  protected:
    virtual void execute()
    {
      category_.reset(create_category());
    }

    virtual void print_results()
    {
      std::cout << "Short info: " << str_info(category_.get()) << std::endl;
      std::cout << "Full info: " << std::endl; write_info(category_.get());
    }

  private:
    std::auto_ptr<OCML::Option_Category> category_;
};

int main(int, char*[])
{
  Definition_Test()();

  return 0;
}
