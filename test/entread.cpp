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

    SdifGenInit("");

    SDIFEntity readentity;
    SDIFEntity entity;
    SdifFileT* file;
    char* sel;
    SdifSignature sig;
    SdifListT *liste;

     SDIFFrame frame;
     SDIFFrame frameTowrite;

    int i = 0;
    int y = 0;


/*
    SDIFNameValueTable nm;
    nm.AddNameValue("hop", "la");
    nm.AddNameValue("dat","ta");
    nm.AddNameValue("na","bel");

    SDIFNameValueTable nma;
    nma.AddNameValue("hopa", "lfa");
    nma.AddNameValue("dgat","tsa");
    nma.AddNameValue("nga","bdel");

    entity.AddNVT(nma, 3);
    entity.AddNVT(nm, 3);
*/
/****************************************/
    // entity.AddNVT(readentity.GetNVT(2),4);
/***************************************/

    readentity.OpenRead(argv[1]);

    file = readentity.GetFile();


/*************/
    // std::cout<< file->Selection << std::endl;
    y = readentity.GetNbNVT();
    if( y != 0)
    {
	for (i = 0 ; i <= y ; i++)
	{
	    entity.AddNVT(readentity.GetNVT(i),3);
	}	
    }
    
    entity.SetSdifString(readentity.GetSdifString());
    

//  entity.AddMatrixType("1TRC", "windoxsid, wondowsSize, wind");   
//  entity.AddFrameType("1POC", "1PIC la_matrix_PIC; APER du_sonchat");

  // entity.SetSdifString(readentity.GetSdifString());
  entity.OpenWrite("hop.sdif");

//entity.GetColumnIndex(SdifStringToSignature("1TRC"), "yzIndexncv");

/*********************************************************************/

// while (!eof  &&  SdifFLastError(readentity.GetFile()) == NULL)
// {
     // SDIFFrame frame;
     // SDIFFrame frameTowrite;
//while ( !eof && SdifFLastError(readentity.GetFile()) == NULL)
  i =1;

  while( i )
 {
     i = readentity.ReadNextFrame(frame);
/*    
 i = frame.Read(readentity);
     if (i == 0)
     {
	 //eof = SdifFGetSignature(file, &bytesread) == eEof;
	 continue;
     }
*/       
/***deselection***/
/****/

     for (unsigned int i=0 ; i < frame.GetNbMatrix() ; i++)
     {
	 /*take the matrix number "i" and put it in tmpMatrix */
	 SDIFMatrix tmpMatrix = frame.GetMatrix(i);
	 
	 /*add a matrix in the matrix vector of the frame*/
	 frameTowrite.AddMatrixSelected(readentity.GetFile(), tmpMatrix);
     }
	
     frameTowrite.SetStreamID(frame.GetStreamID());
     frameTowrite.SetTime(frame.GetTime());
     frameTowrite.SetSignature(frame.GetSignature());
     frameTowrite.Write(entity);


     frame.View();

 }

  std::cout << "valeur de i :" << i << std::endl;

   if (SdifFLastError(readentity.GetFile())) 
 exit (1);

/*********************************************************************/

    SdifGenKill();
    return 0;
}
   
