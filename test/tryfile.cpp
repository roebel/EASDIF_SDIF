#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sdif.h>
//#include "sdifmatrixdata.h"
//#include "sdifmatrix.h"
#include "sdifframe.h"
#include "err_excep.hpp"
#include "sdiffile.h"


int main(int argc, char** argv)
{
    size_t  bytesread = 0;
    int     eof       = 0;  /* End-of-file flag */
    
    SDIFFile file;
    SDIFFile filew;

    SdifSignature sign_file;

/*********************************************/
  
    SdifGenInit("");
    if (argv[1]==NULL)    
	std::cerr << "Usage : " << argv[0] << " filename.sdif" <<std::endl;    

/*for exception(s)*/
    //    SdifSetErrorFunc(SDIFExceptionThrower);
    // try
    // {
	/*for the reading*/
	file.Open(argv[1], eReadFile);
	/* for writing*/
	filew.Open("ecr.sdif", eWriteFile);

	/*signature of the file*/
	sign_file = SdifFCurrSignature (file.file);
	/* END */	
	while (!eof  &&  SdifFLastError(filew.file) == NULL)
	{
	    SDIFFrame frame;
	    /*******  Reading the frame  *******/
	    //throw SDIFException();
/*	    
	    try 
	    {
	    frame.Read(file);
	    }
	    catch(...)
	    {
	    std::cerr << " frame.Read " << std::endl;
	    }
*/
///
	    frame.Read(file);
////
	    /* Print to stdout the content of the frame */
	    frame.View();
	   
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
	    frameToWrite.SetStreamID(frame.GetStreamID());
	    frameToWrite.SetTime(frame.GetTime());
	    frameToWrite.SetSignature(frame.GetSignature());
	    
	    /*if we want to  set all the informations :
	      frameToWrite.SetInfo(Sig, StreamID, Time, NbMatrix);
	    */
	    /*write the header and the data*/
	    frameToWrite.Write(filew);
	    //  std::cerr << " /* 1 Print to stdout the content of the frame */ " << std::endl;
	    frameToWrite.View();

	    //std::cerr << " /* 2 Print to stdout the content of the frame */ " << std::endl;
	    /*if we want to clear the data of the frame :*/
	    frameToWrite.ClearData();
	    /* read next signature */
	    eof = SdifFGetSignature(fileOR.file, &bytesread) == eEof;	
	}
	// }
/*
  catch(SDIFAllocFail& e)
  {
  std::cerr << " Catch AllocFail " << std::endl;
  std::cerr << " View error message " << std::endl;
  e.ErrorMessage();
  std::cerr << " end of catch " << std::endl;
  }

  catch(SDIFArrayPosition& e)
  {
  std::cerr << " Catch ArrayPosition " << std::endl;
  std::cerr << " View error message " << std::endl;
  e.ErrorMessage();
  std::cerr << " end of catch " << std::endl;
  }

  catch(SDIFEof& e)
  {
  std::cerr << " Catch EOF " << std::endl;
  std::cerr << " View error message " << std::endl;
  e.ErrorMessage();
  std::cerr << " end of catch " << std::endl;
  }
  catch(...)
  {
  std::cerr << " default catch " << std::endl;
  }
*/
    if (SdifFLastError(fileOR.file))   /* Check for errors */
    exit (1);

    SdifFClose(fileOR.file);
    SdifFClose(fileOW.file);
    
    SdifGenKill();
    return 0;
}
     





