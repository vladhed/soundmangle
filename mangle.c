#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<math.h>

void main(int argc, char **argv)
{
	char *ifilename = NULL;
	char *ofilename = NULL;
	FILE *ifp,*ofp;
	char hdr[45];
	char *dummy;
	short *ibuf,*obuf;
	unsigned long riffsize, fmtsize, datasize, nwords, delay, ndelay;
	

	if(argc != 4) 
	{
		printf("%s: <input wav> <output wav> <delay msec>\n",argv[0]);
		exit(-1);
	}

	ifilename = argv[1];
	ofilename = argv[2];
	delay = strtoul(argv[3],&dummy,10) * 16;

	if((ifp = fopen(ifilename,"r")) == NULL)
	{
		perror(ifilename);
		exit(-1);
	}
	/* read the RIFF header and keep it for the output file */
	fread(hdr,44,1,ifp);
	
	/* For a WAV file, the data starts at byte 44
	 * The actual amount of data is an unsigned long at byte 40
         * according to http://soundfile.sapp.org/doc/WaveFormat/
	 */
	fseek(ifp,40,SEEK_SET);
	fread(&datasize,4,1,ifp);

	// data is signed short, so 2 bytes per sample
	nwords = datasize/2;

	// datasize is the size in bytes
	ibuf = (short *)malloc(datasize);
	obuf = (short *)malloc(datasize);

	fread(ibuf,datasize,1,ifp);
	fclose(ifp);


	// okay, now we process the sounds
	// each sample is 62.5 usec, 16 per msec
	for(int i=0;i<nwords;i++)
	{
		// copy input buffer to output buffer, but with
		// a delay, that's modulated by a sine function
		ndelay = delay * sin((double)i/(delay))/10;
		// or, uncomment this line for a simple delay
		//ndelay = delay;

		// make sure we don't run off the end of the buffer
		if(i+ndelay > nwords)
		{
			obuf[i]=ibuf[i];
		} else {
			// mix delay with original signal?
			obuf[i]=(ibuf[i]/2 + ibuf[i+ndelay]/2);
			// or just the delayed signal
			//obuf[i]=ibuf[i+ndelay];
		}
	}

	// write the result to out.wav file
	if((ofp = fopen(ofilename,"w+")) == NULL)
	{
		perror(ofilename);
		exit(-1);
	}

	fwrite(hdr,44,1,ofp);
	fwrite(obuf,datasize,1,ofp);
	
	fclose(ofp);
	
}
