#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sdif.h>
#include "sdifframe.h"
#include "sdifentity.h"
//#include "sdifnamevaluetable.h"

/***********/


/************/
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


    readentity.OpenRead(argv[1]);

    file = readentity.GetFile();
    std::cout<< "selection frame : ";
/***pour obtenir la selection********/
    liste = file->Selection->matrix;
//    liste = file->Selection->frame;
    sig = 0;
/**************************/
     SdifListInitLoop (liste);
       while (SdifListIsNext (liste))
      {
	//int g;


	  /* pour obtenir la signature de la frame selectionnee*/
	  sig = ((SdifSelectElementT *) SdifListGetNext (liste))->value.signature;


	  sel = SdifSignatureToString(sig);
	  std::cout<< "Signature fi:"<<std::endl;
	  std::cout<<sel<<std::endl;;


	     }

    y = readentity.GetNbNVT();
    if( y != 0)
    {
	for (i = 0 ; i <= y ; i++)
	{
	    entity.AddNVT(readentity.GetNVT(i),3);
	}	
    }
    
    entity.SetSdifString(readentity.GetSdifString());
    

  entity.OpenWrite("hop.sdif");

/*********************************************************************/

 while (!eof  &&  SdifFLastError(readentity.GetFile()) == NULL)
 {
     i = frame.Read(readentity);
     if (i == 0)
     {
	 //eof = SdifFGetSignature(file, &bytesread) == eEof;
	 continue;
     }       
/***deselection***/
/****/

     for (unsigned int i=0 ; i < frame.GetNbMatrix() ; i++)
     {
	 /*take the matrix number "i" and put it in tmpMatrix */
	 //SDIFMatrix tmpMatrix = frame.GetMatrix(i);
	 SDIFMatrix tmpMatrix = frame.GetMatrixIfSelected(readentity.GetFile(), i);

	 /*add a matrix in the matrix vector of the frame*/
	 frameTowrite.AddMatrix(tmpMatrix);
	 //frameTowrite.AddMatrixSelected(readentity.GetFile(), tmpMatrix);
     }
	
     frameTowrite.SetStreamID(frame.GetStreamID());
     frameTowrite.SetTime(frame.GetTime());
     frameTowrite.SetSignature(frame.GetSignature());

     //frameTowrite.Write(entity.GetFile());
     frameTowrite.Write(entity);
     //frameTowrite.View();
     
     eof = SdifFGetSignature(readentity.GetFile(), &bytesread) == eEof;

     frameTowrite.ClearData();
     frame.ClearData();

 }


    if (SdifFLastError(readentity.GetFile())) 
 exit (1);

/*********************************************************************/

    SdifGenKill();
    return 0;
}
   
