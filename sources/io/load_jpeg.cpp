#include "load_jpeg.hpp"
#include <bits/stdint-uintn.h>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define strcasecmp stricmp
#define strncasecmp strnicmp
#endif

#include "jerror.h"
#include "jpeglib.h"
#include <setjmp.h>

namespace rcv::io {
/*
 * ERROR HANDLING:
 *
 * The JPEG library's standard error handler (jerror.c) is divided into
 * several "methods" which you can override individually.  This lets you
 * adjust the behavior without duplicating a lot of code, which you might
 * have to update with each future release.
 *
 * Our example here shows how to override the "error_exit" method so that
 * control is returned to the library's caller when a fatal error occurs,
 * rather than calling exit() as the standard error_exit method does.
 *
 * We use C's setjmp/longjmp facility to return control.  This means that the
 * routine which calls the JPEG library must first execute a setjmp() call to
 * establish the return point.  We want the replacement error_exit to do a
 * longjmp().  But we need to make the setjmp buffer accessible to the
 * error_exit routine.  To do this, we make a private extension of the
 * standard JPEG error handler object.  (If we were using C++, we'd say we
 * were making a subclass of the regular error handler.)
 *
 * Here's the extended error handler struct:
 */

struct my_error_mgr {
	struct jpeg_error_mgr pub; /* "public" fields */

	jmp_buf setjmp_buffer; /* for return to caller */
};

typedef struct my_error_mgr *my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

METHODDEF(void)
my_error_exit(j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	my_error_ptr myerr = (my_error_ptr)cinfo->err;

	/* Always display the message. */
	/* We could postpone this until after returning, if we chose. */
	(*cinfo->err->output_message)(cinfo);

	/* Return control to the setjmp point */
	longjmp(myerr->setjmp_buffer, 1);
}

METHODDEF(int) do_read_JPEG_file(struct jpeg_decompress_struct *cinfo, const char *infilename);

/*
 * Sample routine for JPEG decompression.  We assume that the source file name
 * is passed in.  We want to return 1 on success, 0 on error.
 */

Matrix<uint8_t> loadJpeg(const std::string filename)
{
	/* This struct contains the JPEG decompression parameters and pointers to
	 * working space (which is allocated as needed by the JPEG library).
	 */
	struct jpeg_decompress_struct cinfo;

	do_read_JPEG_file(&cinfo, filename.c_str());
	return {};
}

/*
 * We call the libjpeg API from within a separate function, because modifying
 * the local non-volatile jpeg_decompress_struct instance below the setjmp()
 * return point and then accessing the instance after setjmp() returns would
 * result in undefined behavior that may potentially overwrite all or part of
 * the structure.
 */

METHODDEF(int)
do_read_JPEG_file(struct jpeg_decompress_struct *cinfo, const char *infilename)
{
	/* We use our private extension JPEG error handler.
	 * Note that this struct must live as long as the main JPEG parameter
	 * struct, to avoid dangling-pointer problems.
	 */
	struct my_error_mgr jerr;
	/* More stuff */
	FILE *infile;			  /* source file */
	JSAMPARRAY buffer = nullptr; /* Output row buffer */
	int col;
	int row_stride; /* physical row width in output buffer */

	/* In this example we want to open the input and output files before doing
	 * anything else, so that the setjmp() error recovery below can assume the
	 * files are open.
	 *
	 * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
	 * requires it in order to read/write binary files.
	 */

	if ((infile = fopen(infilename, "rb")) == NULL) {
		fprintf(stderr, "can't open %s\n", infilename);
		return 0;
	}

	/* Step 1: allocate and initialize JPEG decompression object */

	/* We set up the normal JPEG error routines, then override error_exit. */
	cinfo->err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp(jerr.setjmp_buffer)) {
		/* If we get here, the JPEG code has signaled an error.
		 * We need to clean up the JPEG object, close the input file, and return.
		 */
		jpeg_destroy_decompress(cinfo);
		fclose(infile);
		return 0;
	}

	/* Now we can initialize the JPEG decompression object. */
	jpeg_create_decompress(cinfo);

	/* Step 2: specify data source (eg, a file) */
	jpeg_stdio_src(cinfo, infile);

	/* Step 3: read file parameters with jpeg_read_header() */
	(void)jpeg_read_header(cinfo, TRUE);

	/* We can ignore the return value from jpeg_read_header since
	 *   (a) suspension is not possible with the stdio data source, and
	 *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
	 * See libjpeg.txt for more info.
	 */

	/* emit header for raw PPM format */
	// fprintf(outfile, "P6\n%d %d\n%d\n", WIDTH, HEIGHT,
	// cinfo->data_precision == 12 ? MAXJ12SAMPLE : MAXJSAMPLE);

	/* Step 4: set parameters for decompression */

	/* Step 5: Start decompressor */
	(void)jpeg_start_decompress(cinfo);

	/* We can ignore the return value since suspension is not possible
	 * with the stdio data source.
	 */

	/* We may need to do some setup of our own at this point before reading
	 * the data.  After jpeg_start_decompress() we have the correct scaled
	 * output image dimensions available, as well as the output colormap
	 * if we asked for color quantization.
	 * In this example, we need to make an output work buffer of the right size.
	 */

	/* Samples per row in output buffer */
	row_stride = cinfo->output_width * cinfo->output_components;

	/* Make a one-row-high sample array that will go away when done with image */
	// if (cinfo->data_precision == 12)
	buffer = (*cinfo->mem->alloc_sarray)((j_common_ptr)cinfo, JPOOL_IMAGE, row_stride, 1);

	/* Step 6: while (scan lines remain to be read) */
	/*           jpeg_read_scanlines(...); */

	/* Here we use the library's state variable cinfo->output_scanline as the
	 * loop counter, so that we don't have to keep track ourselves.
	 */
	// if (cinfo->data_precision == 12) {
	while (cinfo->output_scanline < cinfo->output_height) {
		/* jpeg_read_scanlines expects an array of pointers to scanlines.
		 * Here the array is only one element long, but you could ask for
		 * more than one scanline at a time if that's more convenient.
		 */
		(void)jpeg_read_scanlines(cinfo, buffer, 1);
		fwrite(buffer[0], 1, row_stride, outfile);
	}

	/* Step 7: Finish decompression */

	(void)jpeg_finish_decompress(cinfo);
	/* We can ignore the return value since suspension is not possible
	 * with the stdio data source.
	 */

	/* Step 8: Release JPEG decompression object */

	/* This is an important step since it will release a good deal of memory. */
	jpeg_destroy_decompress(cinfo);

	/* After finish_decompress, we can close the input and output files.
	 * Here we postpone it until after no more JPEG errors are possible,
	 * so as to simplify the setjmp error logic above.  (Actually, I don't
	 * think that jpeg_destroy can do an error exit, but why assume anything...)
	 */
	fclose(infile);
	fclose(outfile);

	/* At this point you may want to check to see whether any corrupt-data
	 * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
	 */

	/* And we're done! */
	return 1;
}
} // namespace rcv::io
