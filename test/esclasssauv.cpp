#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sdif.h>
#include "sdifmatrixdata.h"
#include "sdifmatrix.h"
#include "sdifframe.h"

int main()//int argc, char** argv)
{
    size_t  bytesread = 0;
    int     eof       = 0;  /* End-of-file flag */
    
///////////////////*les variables ajoutes*/
    size_t generalHeader;
    size_t asciiChunks;
    SdifFileT *file;

    size_t generalHeaderw;
    size_t asciiChunksw;
    SdifFileT *filew;

    //  char* filename = NULL;
    SdifSignature sign_fich;
    
    SdifGenInit("");
    /*for the reading*/
    file = SdifFOpen ("lic.sdif", eReadFile);
    generalHeader = SdifFReadGeneralHeader(file);
    asciiChunks = SdifFReadAllASCIIChunks(file);
    
    /*for the writing*/
    filew = SdifFOpen ("ecr.sdif", eWriteFile);
    generalHeaderw = SdifFWriteGeneralHeader(filew);
    asciiChunksw = SdifFWriteAllASCIIChunks(filew);
    
    /*signature of the file*/
    sign_fich = SdifFCurrSignature (file);

    while (!eof  &&  SdifFLastError(file) == NULL)
    {
	if (!eof)
	{   	    
	    SDIFFrame frame;
	    frame.read(file);
	    frame.view();

	    SDIFFrame frameToWrite;	   
	    for (unsigned int i=0 ; i < frame.getNbMatrix() ; i++)
	    {
		SDIFMatrix tmpMatrix = frame.getMatrix(i);		
		//tmpMatrix.View();
		frameToWrite.addMatrix(tmpMatrix);
	    }
	    
	    frameToWrite.setStreamID(frame.getStreamID());
	    frameToWrite.setTime(frame.getTime());
	    frameToWrite.setSig(frame.getSig());
	    frameToWrite.setNbMatrix(frame.getNbMatrix());

/*if we want to  set all the informations :
            frameToWrite.setInfo(Sig, StreamID, Time, NbMatrix);
*/
	    //frameToWrite.writeinfo(filew);
	    //frame.Resize(filew);
	    frameToWrite.write(filew);
	    frameToWrite.view();
	    frameToWrite.reset();
	    /* read next signature */
	    eof = SdifFGetSignature(file, &bytesread) == eEof;
	}
    }

    if (SdifFLastError(file))   /* Check for errors */
	exit (1);

    SdifFClose(file);
    SdifFClose(filew);
    
    SdifGenKill();
    return 0;
}
     



