#include <CoreFoundation/CoreFoundation.h>

#include "libvproc_public.h"
#include "libvproc_private.h"

#include <launch.h>

void
print_jobs(launch_data_t j, const char *key __attribute__((unused)), void *context __attribute__((unused)))
{
	static size_t depth = 0;
	launch_data_t lo = launch_data_dict_lookup(j, LAUNCH_JOBKEY_LABEL);
	launch_data_t pido = launch_data_dict_lookup(j, LAUNCH_JOBKEY_PID);
	launch_data_t stato = launch_data_dict_lookup(j, LAUNCH_JOBKEY_LASTEXITSTATUS);
	const char *label = launch_data_get_string(lo);
	size_t i;

	if (pido) {
		fprintf(stdout, "%lld\t-\t", launch_data_get_integer(pido));
	} else if (stato) {
		int wstatus = (int)launch_data_get_integer(stato);
		if (WIFEXITED(wstatus)) {
			fprintf(stdout, "-\t%d\t", WEXITSTATUS(wstatus));
		} else if (WIFSIGNALED(wstatus)) {
			fprintf(stdout, "-\t-%d\t", WTERMSIG(wstatus));
		} else {
			fprintf(stdout, "-\t???\t");
		}
	} else {
		fprintf(stdout, "-\t-\t");
	}
	for (i = 0; i < depth; i++)
		fprintf(stdout, "\t");

	fprintf(stdout, "%s\n", label);
}

int main (int argc, const char * argv[]) {
	launch_data_t resp;
    if (vproc_swap_complex(NULL, VPROC_GSK_ALLJOBS, NULL, &resp) == NULL)
    {
        launch_data_dict_iterate(resp, print_jobs, NULL);
        launch_data_free(resp);

        return 0;
    }
    else
    {
        return 1;
    }
}
