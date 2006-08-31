#include <fstream>
#include <algorithm>
#include <iterator>

#include <iostream>

#include "../SvcConfExporter.hpp"

int main()
{
  // Read the values from file
  std::string values;
  std::ifstream values_file("values.xml");
  values_file.unsetf(std::ios_base::skipws);
  std::copy(std::istream_iterator<char>(values_file),
            std::istream_iterator<char>(),
            std::back_inserter(values));

  SvcConfExporter exporter;
  // Call the function and print out to the std out.
  std::cout << exporter.generate_svc_conf(values.c_str())
            << std::endl;
  
  return 0;
}
