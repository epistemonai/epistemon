#ifndef epistemon_epistemon_h_
#define epistemon_epistemon_h_

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>

static bool epi_running = true;

typedef struct {
	void (*init)(void);
	void (*destroy)(void);
	void (*tick)(void);
} epiDesc;

epiDesc epi_main();

#ifdef EPISTEMON_ENTRY
int main(int argc, char *argv[])
{
	srand(time(NULL));
	epiDesc desc = epi_main();

	desc.init();
	while (epi_running) {
		desc.tick();
	}
	desc.destroy();

	return 0;
}
#endif

#endif
