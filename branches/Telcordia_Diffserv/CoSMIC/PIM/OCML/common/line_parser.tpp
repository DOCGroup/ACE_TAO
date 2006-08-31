/* -*- C++ -*- */

/**
 * @file LineParser.tpp
 *
 * $Id$
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#include <algorithm>

namespace common
{
  namespace
  {
    /// Used to check if the given word is composed only by alpha chars.
    inline bool is_pure_alpha_word(const std::string& word);
  }
}

template <typename OutputIterator>
common::line_parser<OutputIterator>::
line_parser(const line_parser<OutputIterator>& lp)
    : iter_(lp.iter_),
      escape_(false)
{
}
  
template <typename OutputIterator>
common::line_parser<OutputIterator>::line_parser(OutputIterator iter)
  :iter_(iter),
   escape_(false),
   word_()
{
}

template <typename OutputIterator>
common::line_parser<OutputIterator>::~line_parser()
{
  if (word_.size())
    {
      // At the end if there is still character(s) in word copy to output.
      *iter_ = word_;
      ++iter_;
      word_ = "";
    }
  if (escape_)
    {
      throw open_escape();
    }
}
  
template <typename OutputIterator>
void common::line_parser<OutputIterator>::operator()(char ch)
{
  if (escape_)
  {
    switch (ch)
    {
      case 'n':
        word_ += '\n';
        break;
      default:
        // If in escape mode add the char.
        word_ += ch;
    }
    escape_ = false;
  }
  else if (ch == '\\')
  {
    // If the char is '\' enter into the escape mode.
    escape_ = true;
  }
  else if (isspace(ch))
  {
    // If the char is space, copy the word to the output.
    if (word_.size())
    {
      *iter_ = word_;
      ++iter_;
      word_ = "";
    }
  }
  else
  {
    // Otherwise just add the next character to the word.
    word_ += ch;
  }
}

template <typename InputIterator, typename OutputIterator> void
common::parse_line(InputIterator begin, InputIterator end, OutputIterator to)
{
  std::for_each(begin, end, line_parser<OutputIterator>(to));
}
