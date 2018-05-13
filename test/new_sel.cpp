#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sdif.h>
#include "sdifframe.h"

int main(int argc, char** argv)
{
    size_t  bytesread = 0;
    int     eof       = 0;  /* End-of-file flag */
    
    /*for reading*/
    size_t generalHeader;
    size_t asciiChunks;
    SdifFileT *file;

    /*for writing*/
    size_t generalHeaderw;
    size_t asciiChunksw;
    SdifFileT *filew;

    SdifSignature sign_file;
    
    SdifGenInit("");
    /*for the reading*/
    // file = SdifFOpen (argv[1], eReadFile);
    //    file = SdifFOpen ("lic.sdif::@1.7+0.05", eReadFile);
    //file = SdifFOpen ("lic.sdif", eReadFile);
    file = SdifFOpen ("ecro.sdif::#2", eReadFile);

    /* BEGIN: SHOULD BE CHANGED */
    generalHeader = SdifFReadGeneralHeader(file);
    asciiChunks = SdifFReadAllASCIIChunks(file);
    /* END */

    /*for the writing*/
    filew = SdifFOpen ("ecr.sdif", eWriteFile);
    /* BEGIN: SHOULD BE CHANGED */
    generalHeaderw = SdifFWriteGeneralHeader(filew);
    asciiChunksw = SdifFWriteAllASCIIChunks(filew);

    /*signature of the file*/
    sign_file = SdifFCurrSignature (file);
    /* END */

    while (!eof  &&  SdifFLastError(file) == NULL)
    {
	int i =0;
	SDIFFrame frame;
	/*******  Reading the frame  *******/
	i = frame.Read(file);
	if (i == 0)
	{
	    //eof = SdifFGetSignature(file, &bytesread) == eEof;
	    continue;
	}
	/* Print to stdout the content of the frame */
	
//frame.View();

	/*******  Creating a frame  *******/
	SDIFFrame frameToWrite;	   
	for (unsigned int i=0 ; i < frame.GetNbMatrix() ; i++)
	{
	    /*take the matrix number "i" and put it in tmpMatrix */
	    SDIFMatrix tmpMatrix = frame.GetMatrix(i);		
	    /*if we want to see the matrix :*/
	    std::cout << "view the matrix before transformation" << std::endl;
	    tmpMatrix.View();
	       		  
	    /*add a matrix in the matrix vector of the frame*/
	    frameToWrite.AddMatrix(tmpMatrix);
	    /*how access to the data : an example, if we want to multiply with 2 the last column of a matrix  matrix :*/
	    double dou;
	    int ncols = tmpMatrix.GetNbCols();
	    for(int i = 0 ; i < tmpMatrix.GetNbRows() ; i++)
	    {
		//dou = tmpMatrix.GetDouble(i, ncols-1);
//#else
		tmpMatrix.Get(i, ncols-1,dou);
//#endif
		tmpMatrix.Set(i, ncols-1,2.*dou);
	    }
	    std::cout << "view the matrix after transformation" << std::endl;
	    tmpMatrix.View();
	    /*end of example*/
	}
	/*set the header of the frame*/
	//if(frame.GetTime() > 1.72)
	    //    frameToWrite.SetStreamID(frame.GetStreamID()+1); 

//	else   
	    frameToWrite.SetStreamID(frame.GetStreamID());

	frameToWrite.SetTime(frame.GetTime());
	frameToWrite.SetSignature(frame.GetSignature());

	/*if we want to  set all the informations :
	  frameToWrite.SetInfo(Sig, StreamID, Time, NbMatrix);
	*/
	/*write the header and the data*/
	frameToWrite.Write(filew);
	/* Print to stdout the content of the frame */
	frameToWrite.View();
	/*if we want to clear the data of the frame :*/
	frameToWrite.ClearData();
	/* read next signature */
	eof = SdifFGetSignature(file, &bytesread) == eEof;	
    }

    if (SdifFLastError(file))   /* Check for errors */
	exit (1);

    SdifFClose(file);
    SdifFClose(filew);
    
    SdifGenKill();
    return 0;
}
     



