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
    std::cout<< "selection frame : ";
/***pour obtenir la selection********/
//    liste = file->Selection->matrix;
    liste = file->Selection->frame;
    //sig = SdifStringToSignature("1TRC");
//sig = SdifSignatureConst('E','L','O','R');
//SdifInitSelect;

/************************/
/*initialise*/
//SdifInitSelection (file->Selection, argv[1], 12);/*12 pour le int namelen*/
//file->Selection = SdifCreateSelection();
/**************************/
  SdifListInitLoop (liste);
     while (SdifListIsNext (liste))
      {
	//int g;


	  /* pour obtenir la signature de la frame selectionnee*/
	  //sig = ((SdifSelectElementT *) SdifListGetNext (liste))->value.signature;


//g = ((SdifSelectElementT *) SdifListGetNext (liste))->value.signature;
	      //((SdifSelectElementT *) SdifListGetNext (liste))->value.signature = sig;
//((SdifSelectElementT *) SdifListGetNext (file->Selection->frame))->value.signature = sig;

	//((SdifSelectElementT *) SdifListSetNext (file->Selection->frame, sig));

std::cout<< "ds while :"<<std::endl;
/******/
 if (liste->Curr == NULL)
    {
std::cout<< "ds getHead :"<<std::endl;
        SdifListGetHead(liste);
    }
 //  if (liste->Curr != liste->Tail)
 //  {
std::cout<< "ds liste->curr :"<<std::endl;
	      liste->Curr= liste->Curr->Next;
	      //((SdifSelectElementT *)  liste->Curr->Data)->value.signature = sig;
	      sig = ((SdifSelectElementT*)  liste->Curr->Data)->value.signature;
sel = SdifSignatureToString(sig);
 std::cout<< "Signature fi:"<<std::endl;
 std::cout<<sel;

 //  }
 /*
   else
   {
   std::cout<< "ds else :"<<std::endl;
   break;
   }
 */
/*****/

sel = SdifSignatureToString(sig);
 std::cout<< "Signature :"<<std::endl;
 std::cout<<sel;
	   }
/*   
     if (SdifListIsNext(liste))
     cout<< file->Selection->frame->value.signature;
*/
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

 while (!eof  &&  SdifFLastError(readentity.GetFile()) == NULL)
 {
     // SDIFFrame frame;
     //   SDIFFrame frameTowrite;
     // readentity.ReadNextFrame(frame);
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
	 SDIFMatrix tmpMatrix = frame.GetMatrix(i);
	 
	 /*add a matrix in the matrix vector of the frame*/
	 frameTowrite.AddMatrix(tmpMatrix);
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
   
