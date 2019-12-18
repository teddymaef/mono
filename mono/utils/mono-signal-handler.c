#include "mono-signal-handler.h"

#include <signal.h>
#include <string.h>
#include "glib.h"

struct mono_sigpair
{
	int signo;
	const char* signame;
};

struct mono_sigpair mono_signames[] =
{
	{SIGABRT, "SIGABRT"},
	{SIGKILL, "SIGKILL"},
	{SIGTRAP, "SIGTRAP"},
	{SIGSYS, "SIGSYS"},
	{SIGSEGV, "SIGSEGV"},
	{SIGQUIT, "SIGQUIT"},
	{SIGFPE, "SIGFPE"},
	{SIGILL, "SIGILL"},
	{SIGBUS, "SIGBUS"}
};

struct mono_sigpair *sigpair_buf = NULL;
int sigpair_buflen;

void
mono_load_signames ()
{
	if (sigpair_buf)
		return;
#if !defined (__linux__)

	sigpair_buflen = sizeof (sys_signame) / sizeof (sys_signame [0]);
	sigpair_buf = (struct mono_sigpair *) g_malloc (sigpair_buflen * sizeof (struct mono_sigpair));
	struct mono_sigpair *cur = sigpair_buf;
	for (int i = 0; i < sigpair_buflen; ++i)
	{
		cur->signo = i;
		cur->signame = sys_signame [i];
		cur++;
	}

#else // no sys_signame[]

	sigpair_buflen = sizeof (mono_signames) / sizeof (mono_signames [0]);
	sigpair_buf = mono_signames;

#endif
}

const char *
mono_get_signame (int signo)
{
	const char *result = "UNKNOWN";
	struct mono_sigpair *cur = sigpair_buf;
	for (int i = 0; i < sigpair_buflen; ++i)
	{
		if (cur->signo == signo)
		{
			result = cur->signame;
			break;
		}
		cur++;
	}
	return result;
}
