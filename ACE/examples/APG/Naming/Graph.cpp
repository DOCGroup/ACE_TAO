#include <array>

#include "ace/Log_Msg.h"
#include "Graph.h"

void Graph::graph (char const *filename, Graphable_Element_List &data)
{
  data.sort ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Save graph to %C\n"), filename));

  std::array<std::array<char, 10>, 10> graph {};
  for (int n = 0; n < 10; ++n)
    {
      for (int j = 0; j < 10; ++j)
        {
          graph[n][j] = ' ';
        }
    }

  std::array<int, 10> times {};
  int k {};
  for (Graphable_Element const &element : data)
    {
      times[k] = element.when ();

      int const temp = static_cast<int> (element.temp () - 80.0f);

      for (int j = 0; j <= temp; ++j)
        {
          graph[k][j] = '#';
        }
      ++k;
    }

  for (int m = 0; m < 10; ++m)
    {
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("%d  "), times[m]));

      for (int j = 0; j < 10; ++j)
        {
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("%c"), graph[m][j]));
        }
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("\n")));
    }
}
