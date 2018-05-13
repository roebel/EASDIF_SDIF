#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sdif.h>
#include "sdifframe.h"
#include "sdifentity.h"
//#include "sdifnamevaluetable.h"



int main(int argc, char** argv)
{  
    size_t  bytesread = 0;
    int     eof       = 0;  /* End-of-file flag */
    std::string stri, strin;
    SdifSignature sign;

/**************************************/
/**************************************/
/*
  int i;

  SdifUInt4    NbMatrix = 3;
  SdifUInt4    NumID = 0;
  SdifFloat8   Time = 0.0;

  SdifFloat4   TabValue[] = {1,2,3,4,5,6,7,8,9,10};
  SdifFloat8   TabValue2[] = {4,8,10,39, 72,99,101,201};
  SdifFloat4  *pTabValue;
  size_t       SizeFrameW;
  size_t       SizeMatrixW;

  char        *sentence;
*/

/**************************************/
/**************************************/

    SdifGenInit("");

    SDIFEntity readentity;
    SDIFEntity entity;


    SDIFFrame frame;
    SDIFFrame frameTowrite;

    int i = 0;
/****************************************/

    SDIFNameValueTable nm;
    nm.AddNameValue("hop", "la");
    nm.AddNameValue("dat","ta");
    nm.AddNameValue("na","bel");
    
    // entity.PutNVT(file);

    SDIFNameValueTable nma;
    nma.AddNameValue("hopa", "lfa");
    nma.AddNameValue("dgat","tsa");
    nma.AddNameValue("nga","bdel");

    SDIFNameValueTable nmo;
    nmo.AddNameValue("et", "hop!");
    nmo.AddNameValue("ddfgat","tsdfsdfsdfa");
    nmo.AddNameValue("vector","method");

    // entity.AddNVT(readentity.GetNVT(2),4);

    entity.AddNVT(nm, 3);
    entity.AddNVT(nma, 2);
    entity.AddNVT(nmo, 3);

/***************************************/

    readentity.OpenRead(argv[1]);
    std::cout << " readentity.ViewString(); " << std::endl;

    //  entity.AddNVT(readentity.GetNVT(2),4);

      readentity.ViewString();
    std::cout << " readentity.ViewString(); " << std::endl;
    
 entity.SetSdifString(readentity.GetSdifString());

 // entity.AddMatrixType("APER", "windoxsid, wondowsSize, wind");

/* must be used before AddFrameType */
 entity.AddMatrixType("APER", "windoxsid, wondowsSize, wind, ows, kjsd, jhr, jh");


  entity.AddMatrixType("1FQ0", "windoxsid, wondowsSize, wind, kfdf, hop, we");   
  

//entity.AddFrameType("1POC", "1PIC la_matrix_PIC; APER du_sonchat");
entity.AddFrameType("1POC", "1PIC la_matrix_PIC; APER");


  // entity.SetSdifString(readentity.GetSdifString());
entity.OpenWrite("testpr.sdif");

//entity.GetColumnIndex(SdifStringToSignature("1TRC"), "yzIndexncv");

/*********************************************************************/

 while (!eof  &&  SdifFLastError(readentity.GetFile()) == NULL)
 {
     //  SDIFFrame frame;
     //   SDIFFrame frameTowrite;
     //readentity.ReadNextFrame(frame);
       frame.Read(readentity);

      // i = frame.MatrixExists(SdifStringToSignature("1FQ0"));
      //  i = frame.MatrixExists("1FQ0");
      //    if (frame.MatrixExists("1FQ0"))
      //    {
        

//   std::cout << " matrix exist : " << i << std::endl;


	   for (unsigned int i=0 ; i < frame.GetNbMatrix() ; i++)
      {
	    /*take the matrix number "i" and put it in tmpMatrix */
           SDIFMatrix tmpMatrix = frame.GetMatrix(i);
		
	   // SDIFMatrix tmpMatrix = frame.GetMatrix("1FQ0");
      //   sign = SdifStringToSignature("1FQ0");
      //   SDIFMatrix tmpMatrix = frame.GetMatrixWithSig(sign);

	    /*add a matrix in the matrix vector of the frame*/
	    frameTowrite.AddMatrix(tmpMatrix);
	    }
	

//if (frame.MatrixExists("1FQ0"))
	    //         {
	frameTowrite.SetStreamID(frame.GetStreamID());

	//std::cout << "streamId de la frame : " << frame.GetStreamID() << std::endl;
	frameTowrite.SetTime(frame.GetTime());
	frameTowrite.SetSignature(frame.GetSignature());

	//frameTowrite.Write(entity.GetFile());
	frameTowrite.Write(entity);
//	entity.WriteNextFrame(frameTowrite);
     //  entity.WriteNextFrame(frame);
	//  eof = SdifFGetSignature(readentity.GetFile(), &frame.mFrameBytesRead);

	//  }
      //   else{

	      //bytesread += SdifFSkipMatrixData(readentity.GetFile());
	    //  SdifFSkipMatrixData(readentity.GetFile());
	    //    SdifFSkipFrameData(readentity.GetFile());
	    //  }

	eof = SdifFGetSignature(readentity.GetFile(), &bytesread) == eEof;
//eof = SdifFGetSignature(readentity.GetFile(), &bytesread) == eEof;
	//eof = SdifFGetSignature(readentity.GetFile(), NULL) == eEof;

 frameTowrite.ClearData();
	 frame.ClearData();

 }


    if (SdifFLastError(readentity.GetFile())) 
 exit (1);

/*********************************************************************/

    SdifGenKill();
    return 0;
}
   
