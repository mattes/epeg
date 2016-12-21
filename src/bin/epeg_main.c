#include "epeg_main.h"
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

static int verbose_flag = 0;
static int thumb_width = 0; // < 0 means % of input
static int thumb_height = 0; // < 0 means % of input
static int crop_top = 0; // Pixels to crop from the top
static int crop_bottom = 0; // Pixels to crop from the bottom
static int crop_left = 0; // Pixels to crop from the left
static int crop_right = 0; // Pixels to crop from the right
static int max_dimension = 0; // > 0 means we reduce max(w,h) to max_dimension, with aspect preserved
static int outbound_flag = 0; // Ensure specified dimensions will be covered
static int crop_flag = 0; // Crop thumbnail after scaling
static int thumb_quality = 85; // Quality value from 1 to 100
static char *thumb_comment = NULL;
static struct option long_options[] =
{
    {"verbose",  no_argument,       0, 'v'},
    {"width",    required_argument, 0, 'w'},
    {"height",   required_argument, 0, 'h'},
    {"max",      required_argument, 0, 'm'},
    {"outbound", no_argument,       0, 'o'},
    {"crop",     no_argument,       0, 'r'},
    {"quality",  required_argument, 0, 'q'},
    {"comment",  required_argument, 0, 'c'},
    {0, 0, 0, 0}
};

void 
usage(const char *myname)
{
    printf("Usage: %s [options] input.jpg thumb.jpg\n"
	   " -v,  --verbose\n"
	   " -w,  --width=<width>[%%]   set thumbnail width [%% of input]\n"
	   " -h,  --height=<height>[%%] set thumbnail height [%% of input]\n"
	   " -m,  --max=<maximum>       reduce max(w,h) to maximum, with aspect preserved\n"
	   " -o,  --outbound            cover at least the specified size (no upscaling or cropping)\n"
	   " -r,  --crop                crop the resulting thumbnail (to be used with --outbound)\n"
	   " -c,  --comment=<comment>   put a comment in thumbnail\n"
	   " -q,  --quality=<quality>   set thumbnail quality (1-100)\n", myname);
    exit(0);
}

int
main(int argc, char **argv)
{
   Epeg_Image *im;
   int c;
   int option_index = 0;
   char *input_file = NULL, *output_file = NULL;
   char *p;

   while ((c = getopt_long(argc, argv, "w:h:vorc:m:q:", long_options, &option_index)) != -1) {
       switch (c) {
       case 0:
	   usage(argv[0]);
	   break;
       case 'v':
	   verbose_flag = 1;
	   break;
       case 'w':
	   thumb_width = strtol(optarg, NULL, 10);
	   if (thumb_width == 0) {
	       fprintf(stderr, "setting thumb_width to a default minimum of 64\n");
	       thumb_width = 64;
	   } else {
	       // If optarg="NUMBER%" store -NUMBER (see below)
	       if ((p = strstr(optarg, "%"))) thumb_width = -thumb_width;
	   }
	   if (verbose_flag) printf("thumb_width = %d\n", thumb_width);
	   break;
       case 'h':
	   thumb_height = strtol(optarg, NULL, 10);
	   if (thumb_height == 0) {
	       fprintf(stderr, "setting thumb_height to a default minimum of 64\n");
	       thumb_height = 64;
	   } else {
	       // If optarg="NUMBER%" store -NUMBER (see below)
	       if ((p = strstr(optarg, "%"))) thumb_height = -thumb_height;
	   }
	   if (verbose_flag) printf("thumb_height = %d\n", thumb_height);
	   break;
       case 'm':
	   max_dimension = strtol(optarg, NULL, 10);
	   if (verbose_flag) printf("max_dimension = %d\n", max_dimension);
	   break;
       case 'q':
	   thumb_quality = strtol(optarg, NULL, 10);
	   if (thumb_quality < 1 || thumb_quality > 100) {
	       fprintf(stderr, "setting thumb_quality to default of 85\n");
	       thumb_quality = 85;
	   }
	   if (verbose_flag) printf("thumb_quality = %d\n", thumb_quality);
	   break;
       case 'o':
           outbound_flag = 1;
           break;
       case 'r':
           crop_flag = 1;
           break;
       case 'c':
	   thumb_comment = strdup(optarg);
	   if (verbose_flag) printf("thumb_comment = %s\n", thumb_comment);
	   break;
       case '?':
	   usage(argv[0]);
	   break;
       default:
	   abort();
       }
   }

   if (optind < argc) {
       if (optind < (argc-1)) {
	   input_file = argv[optind++];
	   if (verbose_flag) printf("input_file = %s\n", input_file);
	   output_file = argv[optind];
	   if (verbose_flag) printf("output_file = %s\n", output_file);
       } else {
	   usage(argv[0]);
       }
   }

   if (!input_file || !output_file) usage(argv[0]);

   if (!thumb_comment) thumb_comment = "";

   im = epeg_file_open(input_file);
   if (!im) {
       fprintf(stderr, "%s: cannot open %s\n", argv[0], input_file);
       exit(-1);
   }
    
   {
       const char *com;
       Epeg_Thumbnail_Info info;
       int w, h;
       int scaled_w, scaled_h;
       
       com = epeg_comment_get(im);
       if (verbose_flag) if (com) printf("Comment: %s\n", com);
       epeg_thumbnail_comments_get(im, &info);
       if (info.mimetype) {
	   if (verbose_flag) printf("Thumb Mimetype: %s\n", info.mimetype);
	   if (verbose_flag) if (info.uri) printf("Thumb URI: %s\n", info.uri);
	   if (verbose_flag) printf("Thumb Mtime: %llu\n", info.mtime);
	   if (verbose_flag) printf("Thumb Width: %i\n", info.w);
	   if (verbose_flag) printf("Thumb Height: %i\n", info.h);
       }
       epeg_size_get(im, &w, &h);
       if (verbose_flag) printf("Image size: %ix%i\n", w, h);

       if (thumb_width < 0) {
	   // This means we want %thumb_width of w
	   thumb_width = w * (-thumb_width) / 100;
       }
       if (thumb_height < 0) {
	   // This means we want %thumb_height of h
	   thumb_height = h * (-thumb_height) / 100;
       }

       if (max_dimension > 0) {
	   if (w > h ^ outbound_flag) {
	       thumb_width = max_dimension;
	       thumb_height = max_dimension * h / w;
	   } else {
	       thumb_height = max_dimension;
	       thumb_width = max_dimension * w / h;
	   }
	   if (outbound_flag && crop_flag) {
	       crop_top = (thumb_height - max_dimension) / 2;
	       crop_bottom = (thumb_height - max_dimension - crop_top);
	       crop_left = (thumb_width - max_dimension) / 2;
	       crop_right = (thumb_width - max_dimension - crop_left);
	   }
       } else if (outbound_flag) {
            scaled_w = thumb_height * w / h;
            scaled_h = thumb_width * h / w;
            if(scaled_w > thumb_width) {
                if(crop_flag) {
                    crop_left = (scaled_w - thumb_width) / 2;
                    crop_right = scaled_w - thumb_width - crop_left;
                }
                thumb_width = scaled_w;
            }
            if(scaled_h > thumb_height) {
                if(crop_flag) {
                    crop_top = (scaled_h - thumb_height) / 2;
                    crop_bottom = scaled_h - thumb_height - crop_top;
                }
                thumb_height = scaled_h;
            }
       }
   }
   
   if (verbose_flag) printf("Thumb size: %dx%d\n", thumb_width, thumb_height);
   if (verbose_flag) printf("Crop (TxBxLxR): %dx%dx%dx%d\n", crop_top, crop_bottom, crop_left, crop_right);
   epeg_decode_size_set(im, thumb_width, thumb_height);
   epeg_quality_set               (im, thumb_quality);
   epeg_thumbnail_comments_enable (im, 1);
   epeg_comment_set               (im, thumb_comment);
   epeg_crop_set                  (im, crop_top, crop_bottom, crop_left, crop_right);
   epeg_file_output_set           (im, output_file);
   epeg_encode                    (im);
   epeg_close                     (im);
   return 0;
}
