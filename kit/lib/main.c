#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int bm_kit_fd;

int64_t bm_kit_measure()
{
	int64_t param;
	int res = ioctl(bm_kit_fd, 0, &param);
	if(res)
	{
		printf("ERROR: %d\n", res);
		return res;
	}
	return param;
}

int bm_kit_open(void)
{
	bm_kit_fd = open("/dev/bm_mod/io", O_RDONLY);

	if (bm_kit_fd < 0) {
		return -1;
	}

	return 0;
}

void bm_kit_close(void)
{
	close(bm_kit_fd);
}

int bm_kit_init(void)
{
	if (bm_kit_open() < 0) {
		return -1;
	}
	return 0;
}

void bm_kit_exit(void)
{
	bm_kit_close();
}