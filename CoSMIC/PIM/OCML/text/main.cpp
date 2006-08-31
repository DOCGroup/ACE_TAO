#include "ocml_console_application.hpp"
#include <iostream>
#include <fstream>
#include <common/exception.hpp>

#include <ace/Log_Msg.h>

int main(int argc, char* argv[])
{
  ACE_LOG_MSG->priority_mask(LM_SHUTDOWN, ACE_Log_Msg::PROCESS);

  try {
    switch (argc)
      {
      case 1:
        {
          OCML_Console_Application app(std::cin);
          app.run();
          break;
        }

      case 2:
        {
          std::ifstream f(argv[1]);
          if (!f)
            {
              std::cerr << "File not found: " << argv[1] << std::endl;
              return 1;
            }
          OCML_Console_Application app(f);
          app.run();
          break;
        }

      default:
        std::cerr << "Too many arguments" << std::endl;
        return 1;
      }
  }
  catch (common::Exception& e)
  {
    e.print();
  }
  catch (OCML::Null_Value_Exception&)
  {
    std::cerr << "Null value exception" << std::endl;
  }
  catch (OCML::Value_Type_Mismatch&)
  {
    std::cerr << "Value type mismatch" << std::endl;
  }
  catch (...)
  {
    std::cerr << "Unknown exception" << std::endl;
    throw;
  }

  return 0;
}
