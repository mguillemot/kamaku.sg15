/*
 * Yggdrasil Operating System
 * $Id: sound.h,v 1.1.1.1 2006-03-31 19:01:34 root Exp $
 */

#ifndef __SOUND
#define __SOUND

#include "xbasic_types.h"
#include "xstatus.h"


/*
 * sound driver
 *
 * @author: Matthieu Guillemot <g@npng.org>
 * @date: 07/06/2005
 * @version: 1.00
 *
 *
 * Driver son & primitives de gestion des objets sonores
 *
 */


#define I_WANNA_SEE_MY_SOUNDS_BE_LOADED 1

/*** Types définis ***/

typedef struct
{
  Xuint32 *start;
  Xuint32 size;
} Sound;

/*** Gestion des objets sonores ***/

XStatus load_sound(Sound *s, char filename[]);
XStatus unload_sound(Sound *s);
Xboolean is_valid(Sound *s);


/*** Driver son ***/

XStatus init_sound();
void Set_AC97_Callback(void (*fn)());
XStatus play_sound_background(Sound *s);
XStatus stop_sound();


#endif // __SOUND
