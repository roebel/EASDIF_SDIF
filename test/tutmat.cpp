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

    size_t taille;

    SdifFileT *file;
    std::string m_nvt;
    char* signature;
    /*for writing*/
    size_t generalHeaderw;
    size_t asciiChunksw;
    SdifFileT *filew;

    SdifNameValueT* NV;
    SdifNameValuesLT* NVlist;
    SdifNameValueTableT* NVtable;
    const char* value;
    const char* StreamID;
    
    std::string stri, strin;
    
    SdifGenInit("");
    /*for the reading*/
    file = SdifFOpen (argv[1], eReadFile);
    /* BEGIN: SHOULD BE CHANGED */
    generalHeader = SdifFReadGeneralHeader(file);
    asciiChunks = SdifFReadAllASCIIChunks(file);
    
    int z = SdifFNameValueNum(file);
    std::cout << " nombre de NVT : " << z <<std::endl;
    /* END */

    /******************for the writing*/
    filew = SdifFOpen ("ecr.sdif", eWriteFile);
    /* BEGIN: SHOULD BE CHANGED */
    generalHeaderw = SdifFWriteGeneralHeader(filew);

/*
filew->NameValues = SdifFNameValueList(file);
*/     

    //  NVtable =  SdifCreateNameValueTable (SdifUInt4 StreamID,
      //  file->NameValues->HashSize,
      //  SdifUInt4 NumTable)

    //    NVtable =  SdifCreateNameValueTable (1,
    //   file->NameValues->HashSize,
    //   1);   


//NVlist = SdifNameValuesLNewTable( SdifFNameValueList(file), 1);
    //   NVlist = SdifFNameValueList(file); 
//  SdifNameValuesLSetCurrNVT(NVlist, 1);

    /**** for NVTs ****/
    //SdifNameValuesLNewTable(file->NameValues, 1);

    // SdifNameValuesLPut( file->NameValues, "Name Table", "General");
    // NV = SdifNameValuesLPutCurrNVT   (file->NameValues, "NAME",  "VALUE");


//filew->NameValues = SdifFNameValueList(file);
//SdifNameValuesLPutCurrNVT   (SdifFNameValueList(file), "namety",  "valueyt");
//SdifNameValuesLNewTable( SdifFNameValueList(file), 1);
  
//  cout << "hashsize" << file->NameValues->HashSize;

// filew->NameValues   = SdifFNameValueList(file);

   
 asciiChunksw = SdifFWriteAllASCIIChunks(filew);

NVlist = filew->NameValues;
SdifNameValuesLNewTable( NVlist, 12);
// SdifNameValuesLSetCurrNVT(SdifFNameValueList(file), 1);
//NV = SdifNameValuesLGet(SdifFNameValueList(file), "StreamID");

/*
SdifNameValueT*
SdifCreateNameValue(const char *Name,  const char *Value)
*/
 stri = "StreamID";
 strin = "valuen";
 StreamID = const_cast<char*>(stri.c_str());
 StreamID = "streamidmodif";
 value = const_cast<char*>(strin.c_str());
SdifNameValuesLPutCurrNVT   (NVlist, StreamID,  value);
SdifNameValuesLPutCurrNVT   (NVlist, "DATE",  "la_date");


NV =  SdifNameValuesLGet(SdifFNameValueList(file), "Date");

//NV =  SdifNameValuesLGet(SdifFNameValueList(file), file->NameValue->NVTList[3]);
 std::cout << "la namevalue " <<NV->Name << NV->Value << std::endl;

// std::cout << "le numero 3 dans la liste : " << file->NameValues->NVTList->Head<<std::endl ;
//SdifNameValueT* SdifNameValuesLGet(SdifNameValuesLT *NameValuesL, char *Name)

    //NV = SdifNameValuesLGet(file->NameValues, "General");



    
 asciiChunksw = SdifFWriteAllASCIIChunks(filew);

    // taille = SdifFWriteAllNameValueNVT(filew);
    /*signature of the file*/
    // sign_file = SdifFCurrSignature (file);
    /* END */
    //int y = SdifFNameValueLCurrNVTtoSdifString(file, m_nvt);
    // std::cout << m_nvt << std::endl;

    while (!eof  &&  SdifFLastError(file) == NULL)
    {

	SDIFFrame frame;
	/*******  Reading the frame  *******/
	frame.Read(file);
	/* Print to stdout the content of the frame */
	frame.View();

	/*******  Creating a frame  *******/
	SDIFFrame frameToWrite;	   
	for (unsigned int i=0 ; i < frame.GetNbMatrix() ; i++)
	{
	    /*take the matrix number "i" and put it in tmpMatrix */
	    SDIFMatrix tmpMatrix = frame.GetMatrix(i);		


	    /*if we want to see the type of the matrix :*/
	    std::cout << "view the matrix type  before transformation" << std::endl;
	    std::cout << tmpMatrix.GetType() <<std::endl;

	    /*if we want to see the signature of the matrix :*/
	    std::cout << "view the matrix string signature  before transformation" << std::endl;
	    std::cout << tmpMatrix.GetStringSignature() <<std::endl;
;

	    /*if we want to see the matrix :*/
	    std::cout << "view the matrix before transformation" << std::endl;
	    tmpMatrix.View();
	       		  
	    /*add a matrix in the matrix vector of the frame*/
	    frameToWrite.AddMatrix(tmpMatrix);
	    /*how access to the data : an example, if we want to multiply with 2 the av last column of a matrix  matrix :*/
	    double dou;
	    int ncols = tmpMatrix.GetNbCols();
	    for(int i = 0 ; i < tmpMatrix.GetNbRows() ; i++)
	    {
		//dou = tmpMatrix.GetDouble(i, ncols-1);
//#else
		tmpMatrix.Get(i, ncols-2,dou);
//#endif
		tmpMatrix.Set(i, ncols-2,2.*dou);
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
     



