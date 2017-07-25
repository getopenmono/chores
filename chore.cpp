// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt
#include "chore.hpp"

Chore::Chore (MonoIcon const * icon_)
:
  icon(icon_),
  unixLastDone(0)
{}
