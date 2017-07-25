// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#ifndef chore_h
#define chore_h
#include <mono.h>

using mono::display::MonoIcon;

struct Chore
{
  MonoIcon const * icon;
  uint32_t unixLastDone;
  Chore (MonoIcon const *);
};

#endif // chore_h
