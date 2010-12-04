#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <time.h>

#define OUTPUT_BUFFER_SIZE      4096

char pcmout[OUTPUT_BUFFER_SIZE];

int decode() {
  /*
    Declare Ogg Vorbis file structure
   */
  OggVorbis_File vf;
  int eof = 0;
  int current_section;


  /*
    Initialize Ogg Vorbis file structure
    using the ov_open_callbacks() function
   */
  if(ov_open_callbacks(stdin, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0) {
    fprintf(stderr,"Input does not appear to be an Ogg bitstream.\n");
    exit(1);
  }

  /*
    Get the file information using ov_info
   */
  {
    char **ptr=ov_comment(&vf,-1)->user_comments;
    vorbis_info *vi=ov_info(&vf,-1);
    while(*ptr){
      fprintf(stderr,"%s\n",*ptr);
      ++ptr;
    }
    fprintf(stderr,"\nBitstream is %d channel, %ldHz\n",vi->channels,vi->rate);
    fprintf(stderr,"\nDecoded length: %ld samples\n",
            (long)ov_pcm_total(&vf,-1));
    fprintf(stderr,"Encoded by: %s\n\n",ov_comment(&vf,-1)->vendor);
  }

  /*
    Read the data from the file and write the data to PCM
   */
  while(!eof){
    long ret=ov_read(&vf,pcmout,sizeof(pcmout),0,2,1,&current_section);
    if (ret == 0) {
      /* EOF */
      eof=1;
      fprintf(stderr, "End of File\n");
    } else if (ret < 0) {
      /* error in the stream.  Not a problem, just reporting it in
	 case we (the app) cares.  In this case, we don't. */
      fprintf(stderr, "Error in the stream");
    } else {
      /* we don't bother dealing with sample rate changes, etc, but
	 you'll have to*/
      fwrite(pcmout,1,ret,stdout);
    }
  }

  /*
    finished with decoding. Clear all the allocated memory
   */
  ov_clear(&vf);
  
  fprintf(stderr,"Done.\n");
  return(0);
}
