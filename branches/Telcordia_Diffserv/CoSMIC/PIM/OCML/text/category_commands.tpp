template <typename InputIterator>
void List_Command::list(const char* type,
                        InputIterator begin, InputIterator end)
{
  std::cout << type << ": " << std::endl;

  if (begin == end)
    std::cout << "\tNo " << type << std::endl;

  for (; begin != end; ++begin)
    std::cout << '\t' << (*begin)->name() << std::endl;
}
