
#ifndef DIREWOLF_H
#define DIREWOLF_H 1



/*
 * Previously, we could handle only a single audio device.
 * This meant we could have only two radio channels.
 * In version 1.2, we relax this restriction and allow more audio devices.
 * Three is probably adequate for standard version.
 * Larger reasonable numbers should also be fine.
 */

#define MAX_ADEVS 3			

	
/*
 * Maximum number of radio channels.
 * Note that there could be gaps.
 * Suppose audio device 0 was in mono mode and audio device 1 was stereo.
 * The channels available would be:
 *
 *	ADevice 0:	channel 0
 *	ADevice 1:	left = 2, right = 3
 *
 * TODO1.2:  Look for any places that have
 *		for (ch=0; ch<MAX_CHANS; ch++) ...
 * and make sure they handle undefined channels correctly.
 */

#define MAX_CHANS ((MAX_ADEVS) * 2)

/*
 * Get audio device number for given channel.
 * and first channel for given device.
 */

#define ACHAN2ADEV(n) ((n)>>1)
#define ADEVFIRSTCHAN(n) ((n) * 2)

/*
 * Maximum number of modems per channel.
 * I called them "subchannels" (in the code) because 
 * it is short and unambiguous.
 * Nothing magic about the number.  Could be larger
 * but CPU demands might be overwhelming.
 */

#define MAX_SUBCHANS 9


#if __WIN32__
#include <windows.h>
#define SLEEP_SEC(n) Sleep((n)*1000)
#define SLEEP_MS(n) Sleep(n)
#else
#define SLEEP_SEC(n) sleep(n)
#define SLEEP_MS(n) usleep((n)*1000)
#endif


#if __WIN32__
#define PTW32_STATIC_LIB
#include "pthreads/pthread.h"
#else
#include <pthread.h>
#endif


/* Not sure where to put these. */

/* Prefix with DW_ because /usr/include/gps.h uses a couple of these names. */


#define DW_METERS_TO_FEET(x) ((x) == G_UNKNOWN ? G_UNKNOWN : (x) * 3.2808399)
#define DW_FEET_TO_METERS(x) ((x) == G_UNKNOWN ? G_UNKNOWN : (x) * 0.3048)
#define DW_KM_TO_MILES(x) ((x) == G_UNKNOWN ? G_UNKNOWN : (x) * 0.621371192)

#define DW_KNOTS_TO_MPH(x) ((x) == G_UNKNOWN ? G_UNKNOWN : (x) * 1.15077945)
#define DW_KNOTS_TO_METERS_PER_SEC(x) ((x) == G_UNKNOWN ? G_UNKNOWN : (x) * 0.51444444444)
#define DW_MPH_TO_KNOTS(x) ((x) == G_UNKNOWN ? G_UNKNOWN : (x) * 0.868976)
#define DW_MPH_TO_METERS_PER_SEC(x) ((x) == G_UNKNOWN ? G_UNKNOWN : (x) * 0.44704)

#define DW_MBAR_TO_INHG(x) ((x) == G_UNKNOWN ? G_UNKNOWN : (x) * 0.0295333727)




#if __WIN32__

typedef CRITICAL_SECTION dw_mutex_t;

#define dw_mutex_init(x) \
	InitializeCriticalSection (x)

/* This one waits for lock. */

#define dw_mutex_lock(x) \
	EnterCriticalSection (x) 

/* Returns non-zero if lock was obtained. */

#define dw_mutex_try_lock(x) \
	TryEnterCriticalSection (x)

#define dw_mutex_unlock(x) \
	LeaveCriticalSection (x)


#else

typedef pthread_mutex_t dw_mutex_t;

#define dw_mutex_init(x) pthread_mutex_init (x, NULL)

/* this one will wait. */

#define dw_mutex_lock(x) \
	{	\
	  int err; \
	  err = pthread_mutex_lock (x); \
	  if (err != 0) { \
	    text_color_set(DW_COLOR_ERROR); \
	    dw_printf ("INTERNAL ERROR %s %d pthread_mutex_lock returned %d", __FILE__, __LINE__, err); \
	    exit (1); \
	  } \
	}

/* This one returns true if lock successful, false if not. */
/* pthread_mutex_trylock returns 0 for success. */

#define dw_mutex_try_lock(x) \
	({	\
	  int err; \
	  err = pthread_mutex_trylock (x); \
	  if (err != 0 && err != EBUSY) { \
	    text_color_set(DW_COLOR_ERROR); \
	    dw_printf ("INTERNAL ERROR %s %d pthread_mutex_trylock returned %d", __FILE__, __LINE__, err); \
	    exit (1); \
	  } ; \
	  ! err; \
	})

#define dw_mutex_unlock(x) \
	{	\
	  int err; \
	  err = pthread_mutex_unlock (x); \
	  if (err != 0) { \
	    text_color_set(DW_COLOR_ERROR); \
	    dw_printf ("INTERNAL ERROR %s %d pthread_mutex_unlock returned %d", __FILE__, __LINE__, err); \
	    exit (1); \
	  } \
	}


#endif



#endif   /* ifndef DIREWOLF_H */