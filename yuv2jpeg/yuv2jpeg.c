/*
* use libjpeg lib to convert YV12 to jpeg file
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <fcntl.h>
#include <jpeglib.h>
#include <setjmp.h>
#include <string.h>

int jpeg_enc_yv12(unsigned char* buffer, int width, int height, int quality, char* filename)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *outfile = NULL;
    int ret = TRUE;
    if(buffer == NULL || width <=0 || height <=0|| filename == NULL)
        return FALSE;
    if ((outfile = fopen(filename, "wb")) == NULL) 
    {  
        return FALSE;
    }    
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = width; 
    cinfo.image_height = height;
    cinfo.input_components = 3;        
    cinfo.in_color_space = JCS_YCbCr;
    jpeg_set_defaults(&cinfo);

#if JPEG_LIB_VERSION >= 70

 cinfo.do_fancy_downsampling = FALSE;
 cinfo.dct_method = JDCT_FASTEST;
 cinfo.smoothing_factor = 0;

#endif
    jpeg_set_quality(&cinfo, quality, TRUE);
    cinfo.raw_data_in = TRUE;  
    
    {
        JSAMPARRAY pp[3];
        JSAMPROW *rpY = (JSAMPROW*)malloc(sizeof(JSAMPROW) * height);
        JSAMPROW *rpU = (JSAMPROW*)malloc(sizeof(JSAMPROW) * height);
        JSAMPROW *rpV = (JSAMPROW*)malloc(sizeof(JSAMPROW) * height);
        pp[0] = rpY;
        /*pp[1] = rpU;*/
        /*pp[2] = rpV;*/
        pp[1] = rpV;
        pp[2] = rpU;
        int k;
        if(rpY == NULL && rpU == NULL && rpV == NULL)
        {
            ret = FALSE;
            goto exit;
        }
        cinfo.raw_data_in = TRUE;                  // supply downsampled data
        cinfo.comp_info[0].h_samp_factor = 2;
        cinfo.comp_info[0].v_samp_factor = 2;
        cinfo.comp_info[1].h_samp_factor = 1;
        cinfo.comp_info[1].v_samp_factor = 1;
        cinfo.comp_info[2].h_samp_factor = 1;
        cinfo.comp_info[2].v_samp_factor = 1;
        cinfo.dct_method = JDCT_FASTEST;
        jpeg_start_compress(&cinfo, TRUE);

        int i,j;
        for (j = 0; j < cinfo.image_height; j += 16) {
            for (i = 0; i < 16; i++) {
                rpY[i] = buffer + width * (i + j);
                if (i%2 == 0) {
                    rpU[i/2] = buffer + width * height + width / 2 * ((i + j) / 2);
                    rpV[i/2] = buffer + width * height + width * height / 4 + width / 2 * ((i + j) / 2);
                }
            }
            jpeg_write_raw_data(&cinfo, pp, 16);
        }

        jpeg_finish_compress(&cinfo);
        free(rpY);
        free(rpU);
        free(rpV);
    }
exit:
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
    return ret;
}


void
usage (char *pname) {
	fprintf (stderr,
		"Usage: %s [-v]  [-i #] [-w #] [-h #] \n",
		pname);
	exit (1);
}



int main(int argc, char ** argv)
{
	int c, fd,  input = -1;
	char * input_file="capture1.yuv";
	char outfile_name[20] = {0};
	int width=320, height =240;
	unsigned int size;
	int qualite = 100;
	 unsigned char *buffer;
	 int verbose=0;
	
	/* parse arguments
	 */
	while ((c = getopt (argc, argv, "vi:w:h:")) != EOF) {
		switch (c) {
			case 'i':
				input_file = optarg;
				break;
			case 'w':
				width = strtoul(optarg, NULL, 10);
				break;
			case 'h':
				height = strtoul(optarg, NULL, 10);
				break;
			case 'v':
				verbose++;
				break;
			default:
				usage (argv[0]);
				break;
		}
	}
	
	size = width* height *3/2;
	
   	buffer = malloc(width*height*3/2);
	 if (!buffer ){
		printf("can't alloc buffer \n");
		exit(-1);
	}
 	fd = open (input_file, O_RDWR);
	if (fd < 0 ){
		printf("can't open file to read/write\n");
		exit(-1);
	}
	
	if (read(fd, buffer, size) != size){
		printf("read file error\n");
		exit(-1);
	}
	
	strcat(outfile_name, strtok(input_file,"."));
	strcat(outfile_name, ".jpg");
	printf("outfile name : %s\n", outfile_name);
    
	jpeg_enc_yv12(buffer, width, height, qualite, outfile_name);
	free(buffer);
	close(fd);
	return 0;
    
}
