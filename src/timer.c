//
// C Implementation: timer
//
// Description: 
//
//
// Author: Tobias Glaesser <tobi.web@gmx.de>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <SDL/SDL.h>
#include "defines.h"
#include "timer.h"

unsigned int st_get_ticks(void)
{
if(st_pause_count != 0)
return SDL_GetTicks() - st_pause_ticks - SDL_GetTicks() + st_pause_count;
else
return SDL_GetTicks() - st_pause_ticks;
}

void st_pause_ticks_init(void)
{
st_pause_ticks = 0;
st_pause_count = 0;
}

void st_pause_ticks_start(void)
{
st_pause_count = SDL_GetTicks();
}

void st_pause_ticks_stop(void)
{
st_pause_ticks += SDL_GetTicks() - st_pause_count;
st_pause_count = 0;
}

void timer_init(timer_type* ptimer)
{
  ptimer->period = 0;
  ptimer->time = 0;
}

void timer_start(timer_type* ptimer, unsigned int period)
{
  ptimer->time = st_get_ticks();
  ptimer->period = period;
}

void timer_stop(timer_type* ptimer)
{
 timer_init(ptimer);
}

int timer_check(timer_type* ptimer)
{
  if((ptimer->time != 0) && (ptimer->time + ptimer->period > st_get_ticks()))
    return YES;
  else
    {
      ptimer->time = 0;
      return NO;
    }
}

int timer_started(timer_type* ptimer)
{
  if(ptimer->time != 0)
    return YES;
  else
    return NO;
}

int timer_get_left(timer_type* ptimer)
{
  return (ptimer->period - (st_get_ticks() - ptimer->time));
}

int timer_get_gone(timer_type* ptimer)
{
  return (st_get_ticks() - ptimer->time);
}
