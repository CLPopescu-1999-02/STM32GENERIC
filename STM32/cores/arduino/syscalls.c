/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/**
  * \file syscalls_sam3.c
  *
  * Implementation of newlib syscall.
  *
  */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/


#include "syscalls.h"

#include <stdio.h>
#include <stdarg.h>
#if defined (  __GNUC__  ) /* GCC CS3 */
  #include <sys/types.h>
  #include <sys/stat.h>
#endif

#include "Arduino.h"
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/errno.h>

// Helper macro to mark unused parameters and prevent compiler warnings.
// Appends _UNUSED to the variable name to prevent accidentally using them.
#ifdef __GNUC__
#define UNUSED_PARAM(x) x ## _UNUSED __attribute__((__unused__))
#else
#define UNUSED_PARAM(x) x ## _UNUSED
#endif

/*----------------------------------------------------------------------------
 *        Exported variables
 *----------------------------------------------------------------------------*/

#undef errno
extern int errno ;
extern int  _end ;

static unsigned char *heap_brk = NULL;
static unsigned char *heap_end = NULL;

void setHeap(unsigned char *start, unsigned char *end) {
    heap_brk = start;
    heap_end = end;
}

caddr_t _sbrk ( int incr )
{
   caddr_t prev_heap;

  if ( heap_brk == NULL )
  {
      heap_brk = (unsigned char *)&_end ;
  }
  prev_heap = (caddr_t)heap_brk;

  if (heap_end != NULL && (heap_brk + incr) > heap_end) {
      return (caddr_t)-1;
  }
  
  heap_brk += incr ;

  return prev_heap ;
}

int _open(char *path, int flags, ...)
{
	/* Pretend like we always fail */
    UNUSED(path);
    UNUSED(flags);
	return -1;
}

__attribute__((weak))
int _close( UNUSED_PARAM(int file) )
{
  return -1 ;
}

int _link(char *old, char *new)
{
    UNUSED(old);
    UNUSED(new);	
	errno = EMLINK;
	return -1;
}

int _unlink(char *name)
{
    UNUSED(name);
	return -1;
}

int _times(struct tms *buf)
{
    UNUSED(buf);
	return -1;
}


__attribute__((weak))
int _fstat( UNUSED_PARAM(int file), struct stat *st )
{
  st->st_mode = S_IFCHR ;

  return 0 ;
}

__attribute__((weak))
int _isatty( UNUSED_PARAM(int file) )
{
  return 1 ;
}

__attribute__((weak))
int _lseek( UNUSED_PARAM(int file), UNUSED_PARAM(int ptr), UNUSED_PARAM(int dir) )
{
  return 0 ;
}

__attribute__((weak))
int _read(UNUSED_PARAM(int file), UNUSED_PARAM(char *ptr), UNUSED_PARAM(int len) )
{
  return 0 ;
}
/*
extern int _write( UNUSED_PARAM(int file), UNUSED_PARAM(char *ptr), int len )
{
  int iIndex = 0;

  for ( iIndex=0 ; iIndex < len ; iIndex++) {
    //TODO write to Serial
  }
  return iIndex ;
}
*/
void _exit(UNUSED_PARAM(int status))
{
//  printf( "Exiting with status %d.\n", status ) ;
  for ( ; ; ) ;
}

__attribute__((weak))
int _kill( UNUSED_PARAM(int pid), UNUSED_PARAM(int sig) )
{
  errno = EINVAL;	
  return -1;
}

int _getpid ( void )
{
  return -1 ;
}
