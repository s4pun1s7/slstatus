/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "../slstatus.h"
#include "../util.h"

const char *
run_command(const char *cmd)
{
	char *p;
	FILE *fp;
	int status;

	if (!(fp = popen(cmd, "r"))) {
		warn("popen '%s':", cmd);
		return NULL;
	}

	p = fgets(buf, sizeof(buf) - 1, fp);
	status = pclose(fp);
	/*
	 * ECHILD is expected when SIGCHLD is SIG_IGN / SA_NOCLDWAIT (dwm).
	 * Keep the output we already read instead of reporting n/a.
	 */
	if (status < 0 && errno != ECHILD) {
		warn("pclose '%s':", cmd);
		return NULL;
	}
	if (!p)
		return NULL;

	if ((p = strrchr(buf, '\n')))
		p[0] = '\0';

	return buf[0] ? buf : NULL;
}
