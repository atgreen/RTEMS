/*
 *  $Id$
 */

#include <assert.h>
#include <errno.h>
#include <signal.h>

#include <rtems/system.h>
#include <rtems/score/thread.h>

/*
 *  Currently only 20 signals numbered 1-20 are defined
 */

#define SIGNAL_ALL_MASK  0x000fffff

#define signo_to_mask( _sig ) (1 << ((_sig) - 1))

#define is_valid_signo( _sig ) \
  ((signo_to_mask(_sig) & SIGNAL_ALL_MASK) != 0 )

/*
 *  3.3.2 Send a Signal to a Process, P1003.1b-1993, p. 68
 *
 *  NOTE: Behavior of kill() depends on _POSIX_SAVED_IDS.
 */

int kill(
  pid_t pid,
  int   sig
)
{
  /*
   *  Only supported for the "calling process" (i.e. this node).
   */
 
  assert( pid == getpid() );

  /* SIGABRT comes from abort via assert */
  if ( sig == SIGABRT ) {
    exit( 1 );
  }
  return POSIX_NOT_IMPLEMENTED();
}

/*
 *  3.3.3 Manipulate Signal Sets, P1003.1b-1993, p. 69
 */

int sigemptyset(
  sigset_t   *set
)
{
  assert( set );  /* no word from posix, solaris returns EFAULT */

  *set = 0;
  return 0;
}

/*
 *  3.3.3 Manipulate Signal Sets, P1003.1b-1993, p. 69
 */

int sigfillset(
  sigset_t   *set
)
{
  assert( set );

  *set = SIGNAL_ALL_MASK;
  return 0;
}

/*
 *  3.3.3 Manipulate Signal Sets, P1003.1b-1993, p. 69
 */

int sigaddset(
  sigset_t   *set,
  int         signo
)
{
  assert( set );

  if ( !is_valid_signo(signo) ) {
    errno = EINVAL;
    return -1;
  }

  *set |= signo_to_mask(signo);
  return 0;
}

/*
 *  3.3.3 Manipulate Signal Sets, P1003.1b-1993, p. 69
 */

int sigdelset(
  sigset_t   *set,
  int         signo
)
{
  assert( set );
 
  if ( !is_valid_signo(signo) ) {
    errno = EINVAL;
    return -1;
  }
 
  *set &= ~signo_to_mask(signo);
  return 0;
}

/*
 *  3.3.3 Manipulate Signal Sets, P1003.1b-1993, p. 69
 */

int sigismember(
  const sigset_t   *set,
  int               signo
)
{
  assert( set );
 
  if ( !is_valid_signo(signo) ) {
    errno = EINVAL;
    return -1;
  }
 
  if ( *set & signo_to_mask(signo) )
    return 1;

  return 0;
}

/*
 *  3.3.4 Examine and Change Signal Action, P1003.1b-1993, p. 70
 */

int sigaction(
  int                     sig,
  const struct sigaction *act,
  struct sigaction       *oact
)
{
  return POSIX_NOT_IMPLEMENTED();
}

/*
 *  3.3.5 Examine and Change Blocked Signals, P1003.1b-1993, p. 73
 *
 *  NOTE: P1003.1c/D10, p. 37 adds pthread_sigmask().
 */

int sigprocmask(
  int               how,
  const sigset_t   *set,
  sigset_t         *oset
)
{
  return POSIX_NOT_IMPLEMENTED();
}

/*
 *  3.3.5 Examine and Change Blocked Signals, P1003.1b-1993, p. 73
 *
 *  NOTE: P1003.1c/D10, p. 37 adds pthread_sigmask().
 */

int pthread_sigmask(
  int               how,
  const sigset_t   *set,
  sigset_t         *oset
)
{
  return POSIX_NOT_IMPLEMENTED();
}

/*
 *  3.3.6 Examine Pending Signals, P1003.1b-1993, p. 75
 */

int sigpending(
  sigset_t  *set
)
{
  return POSIX_NOT_IMPLEMENTED();
}

/*
 *  3.3.7 Wait for a Signal, P1003.1b-1993, p. 75
 */

int sigsuspend(
  const sigset_t  *sigmask
)
{
  return POSIX_NOT_IMPLEMENTED();
}

/*
 *  3.3.8 Synchronously Accept a Signal, P1003.1b-1993, p. 76
 *
 *  NOTE: P1003.1c/D10, p. 39 adds sigwait().
 */

int sigwaitinfo(
  const sigset_t  *set,
  siginfo_t       *info
)
{
  return POSIX_NOT_IMPLEMENTED();
}

/*
 *  3.3.8 Synchronously Accept a Signal, P1003.1b-1993, p. 76
 *
 *  NOTE: P1003.1c/D10, p. 39 adds sigwait().
 */

int sigtimedwait(
  const sigset_t         *set,
  siginfo_t              *info,
  const struct timespec  *timeout
)
{
  return POSIX_NOT_IMPLEMENTED();
}

/*
 *  3.3.8 Synchronously Accept a Signal, P1003.1b-1993, p. 76
 *
 *  NOTE: P1003.1c/D10, p. 39 adds sigwait().
 */

int sigwait(
  const sigset_t  *set,
  int             *sig
)
{
  return POSIX_NOT_IMPLEMENTED();
}

/*
 *  3.3.9 Queue a Signal to a Process, P1003.1b-1993, p. 78
 */

int sigqueue(
  pid_t               pid,
  int                 signo,
  const union sigval  value
)
{
  return POSIX_NOT_IMPLEMENTED();
}

/*
 *  3.3.10 Send a Signal to a Thread, P1003.1c/D10, p. 43
 */

int pthread_kill(
  pthread_t   thread,
  int         sig
)
{
  return POSIX_NOT_IMPLEMENTED();
}

/*
 *  3.4.1 Schedule Alarm, P1003.1b-1993, p. 79
 */

unsigned int alarm(
  unsigned int seconds
)
{
  return POSIX_NOT_IMPLEMENTED();
}

/*
 *  3.4.2 Suspend Process Execution, P1003.1b-1993, p. 81
 */

int pause( void )
{
  return POSIX_NOT_IMPLEMENTED();
}
