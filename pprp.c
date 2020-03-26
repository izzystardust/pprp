#include <stdio.h>
#include <string.h>
#include <MagickCore/MagickCore.h>

#include "pakon.h"

int main(int argc, char **argv) {
	ExceptionInfo *exception;
	Image *image_raw;
	ImageInfo *image_info;

	struct pakon_raw pr;
	struct pakon_raw_error *pre;

	if (argc != 3) {
		// should porbably check 
	}

	char *raw_file_path = argv[1];
	pre = load_pakon_raw(&pr, raw_file_path);

	if (pre) {
		fprintf(stderr, "Error: %s\n", pre->msg);
		return -1;
	}

	MagickCoreGenesis(*argv, MagickTrue);
	exception = AcquireExceptionInfo();
	image_info = CloneImageInfo((ImageInfo *) NULL);

	image_raw = ConstituteImage(pr.columns, pr.rows, "RGB", LongPixel, pr.pixels, exception);
	if (exception->severity != UndefinedException) {
		CatchException(exception);
	}
	image_raw->gamma = 2.2;
	image_raw->depth = 16;
	strcpy(image_info->filename, "out.png");
	strcpy(image_info->magick, "png");
	image_info->file = fopen("out.png", "w+b");
	WriteImage(image_info, image_raw, exception);



	return 0;
}
