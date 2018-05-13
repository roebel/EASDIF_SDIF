/**
 * @file   Fsdif_write_handler.cpp
 * @author Axel Roebel
 * @date   Sat Jan 19 15:05:41 2008
 * 
 * @brief  handle write file io in matlab
 * 
 *
 * $Revision: 1.8 $   last changed on $Date: 2011-03-28 23:03:18 $
 *
 *                                 Copyright (c) 2008 by IRCAM
 */

#include <mex.h>
#include <cstdlib>
#include <stdint.h>
#include "easdif/easdif.h"

#include "fieldnames.hpp"

#define MAKE_STRING_FROM_MACRO(MM) MAKE_STRING_FROM_MACRO_HELP(MM) 
#define MAKE_STRING_FROM_MACRO_HELP(MM) #MM

// global list for all easdif file pointers
typedef std::list<Easdif::SDIFEntity * > EASDList;
EASDList pList;

// mexAtExit cleanup routine
void cleanup() {
  EASDList::iterator it = pList.begin();
  EASDList::iterator ite = pList.end();
  while(it!=ite){
    delete *it;
    ++it;
  }
  pList.clear();  
}

void cleanupAndEEnd() {
  cleanup();
  Easdif::EasdifEnd();
}

// validate pointer with list
bool CheckList(Easdif::SDIFEntity *p) {
  EASDList::iterator it = pList.begin();
  EASDList::iterator ite = pList.end();
  while(it!=ite){
    if(p==*it){      
      return true;
    }
    ++it;
  }
  return false;
}


char errMess [512];

int
writeData(Easdif::SDIFEntity *p, const mxArray *toWrite) {

  int nFrames = mxGetNumberOfElements(toWrite);
  Easdif::SDIFFrame frame;
  for(int ii=0; ii!=nFrames; ++ii) {
    frame.clear();
    char sig[4];
    SdifInt4 streamid;
    double time;

    const mxArray* msig      = mxGetField(toWrite,ii,frameFrameSig_fieldString);      
    if(!msig || !mxIsNumeric(msig) || mxIsComplex(msig) || mxGetNumberOfElements(msig) != 4 ){
      sprintf(errMess,"Fsdif_write_handler :: cannot write frame data :: frame.%s field has to be matrix with 4 elements !",frameFrameSig_fieldString);
      mexErrMsgTxt(errMess);
    }
    frame.SetSignature(SdifStringToSignature(readData(msig,sig)));    

    const mxArray* mstreamid = mxGetField(toWrite,ii,frameStreamID_fieldString);      
    if(!mstreamid || !mxIsNumeric(mstreamid) || mxIsComplex(mstreamid)||  mxGetNumberOfElements(mstreamid) !=1 ){
      sprintf(errMess,"Fsdif_write_handler :: cannot write frame data :: frame.%s field has to be a scalar matrix !",frameStreamID_fieldString);
      mexErrMsgTxt(errMess);
    }
    frame.SetStreamID(*readData(mstreamid,&streamid));    

    const mxArray* mtime     = mxGetField(toWrite,ii,frameTime_fieldString);      
    if(!mtime || !mxIsNumeric(mtime) || mxIsComplex(mtime)||  mxGetNumberOfElements(mtime) !=1 ){
      sprintf(errMess,"Fsdif_write_handler :: cannot write frame data :: frame.%s field has to be a scalar matrix !",frameTime_fieldString);
      mexErrMsgTxt(errMess);
    }
    frame.SetTime(*readData(mtime,&time));    

    const mxArray* mdata= mxGetField(toWrite,ii,frameData_fieldString);      
    if(!mdata){
      sprintf(errMess,"Fsdif_write_handler :: cannot write frame data :: frame.%s field has to exist !",frameData_fieldString);
      mexErrMsgTxt(errMess);
    }
    int nMatrix = mxGetNumberOfFields(mdata);
    for (int im = 0; im != nMatrix; ++ im) {
      Easdif::SDIFMatrix mat;
      const char* msig = mxGetFieldNameByNumber(mdata,im);
      if(!msig||strlen(msig) != 7){
        sprintf(errMess,"Fsdif_write_handler :: can not use data field name <%s> to derive matrix signature\n field name has to be of size 7 starting with MD_",msig);
        mexErrMsgTxt(errMess);
      }

      const mxArray* data =  mxGetFieldByNumber(mdata,0,im);
      if(!data || mxIsComplex(data) || !(mxIsNumeric(data) || mxIsChar(data))){
        mexErrMsgTxt("Fsdif_write_handler :: can only write real valued numeric or char data");
      }
      
      if(mxIsChar(data)){
        mat.Init(SdifStringToSignature(msig+3), mxGetNumberOfElements(data), 1, eText);        
        char* str = reinterpret_cast<char*>(calloc(mxGetNumberOfElements(data)+1,1));        
        mxGetString(data,str,mxGetNumberOfElements(data)+1);
        mat.SetCol(str,0);
      }
      else if(mxIsDouble(data)){
        mat.Init(SdifStringToSignature(msig+3), mxGetM(data), mxGetN(data), eFloat8);        
        for(int ic=0;ic!=mat.GetNbCols();++ic)
          mat.SetCol(reinterpret_cast<double*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
      }
      else      if(mxIsSingle(data)){
        mat.Init(SdifStringToSignature(msig+3), mxGetM(data), mxGetN(data), eFloat4);        
        for(int ic=0;ic!=mat.GetNbCols();++ic)
          mat.SetCol(reinterpret_cast<float*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
      }
      else if(mxIsInt8(data)){       
        mat.Init(SdifStringToSignature(msig+3), mxGetM(data), mxGetN(data), eInt1);        
        for(int ic=0;ic!=mat.GetNbCols();++ic)
          mat.SetCol(reinterpret_cast<unsigned char *> (mxGetData(data))+ic*mat.GetNbRows(),ic);
      }
      else if(mxIsInt16(data)){       
        mat.Init(SdifStringToSignature(msig+3), mxGetM(data), mxGetN(data), eInt2);        
        for(int ic=0;ic!=mat.GetNbCols();++ic)
          mat.SetCol(reinterpret_cast<SdifInt2*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
      }
      else if(mxIsInt32(data)){       
        mat.Init(SdifStringToSignature(msig+3), mxGetM(data), mxGetN(data), eInt4);        
        for(int ic=0;ic!=mat.GetNbCols();++ic)
          mat.SetCol(reinterpret_cast<SdifInt4*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
      }
      else if(mxIsUint8(data)){       
        mat.Init(SdifStringToSignature(msig+3), mxGetM(data), mxGetN(data), eUInt1);        
        for(int ic=0;ic!=mat.GetNbCols();++ic)
          mat.SetCol(reinterpret_cast<SdifUInt1*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
      }
      else if(mxIsUint16(data)){       
        mat.Init(SdifStringToSignature(msig+3), mxGetM(data), mxGetN(data), eUInt2);        
        for(int ic=0;ic!=mat.GetNbCols();++ic)
          mat.SetCol(reinterpret_cast<SdifUInt2*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
      }
      else if(mxIsUint32(data)){       
        mat.Init(SdifStringToSignature(msig+3), mxGetM(data), mxGetN(data), eUInt4);        
        for(int ic=0;ic!=mat.GetNbCols();++ic)
          mat.SetCol(reinterpret_cast<SdifUInt4*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
      }
      else {
        char tmperr[256];
        sprintf(tmperr,"Fsdif_write_handler :: Unsupported data type %x",mat.GetType());
        mexErrMsgTxt(tmperr);
      }
      frame.AddMatrix(mat);
    }
    p->WriteFrame(frame);
  }
  return nFrames;
}

void addNVTs(Easdif::SDIFEntity* p, const mxArray *pNVT) {
  int maxStoreLen = 512;
  char *store =reinterpret_cast<char*>(malloc(maxStoreLen));
  std::string stringsName;
  if(mxGetNumberOfElements(pNVT)){
    if(!mxIsCell(pNVT) || mxGetN(pNVT) != 2)
      mexErrMsgTxt("Fsdif_write_handler :: NVT needs to be given as Nx2 cell array of strings");
    Easdif::SDIFNameValueTable nvt;
    int nrows = mxGetM(pNVT);
    for(int ir = 0; ir != nrows; ++ ir) {      
      if(!mxGetCell(pNVT,ir) || !mxGetCell(pNVT,ir+nrows) || !mxIsChar(mxGetCell(pNVT,ir))){
        if(nvt.GetNbNameValue()){
          p->AddNVT(nvt,_SdifNVTStreamID);
          nvt.clear();
        }
      }
      else {
        if(mxGetNumberOfElements(mxGetCell(pNVT,ir)) + 1 > maxStoreLen){
          maxStoreLen= mxGetNumberOfElements(mxGetCell(pNVT,ir)) + 1;
          store = reinterpret_cast<char*>(realloc(store, maxStoreLen));
        }
        if(mxGetString(mxGetCell(pNVT,ir),store,maxStoreLen) )
          mexErrMsgTxt("Fsdif_write_handler :: Failed to store NVT Name");
        
        stringsName =  store;
        
        if(mxGetNumberOfElements(mxGetCell(pNVT,ir+nrows)) + 1  > maxStoreLen){
          maxStoreLen = mxGetNumberOfElements(mxGetCell(pNVT,ir+nrows)) + 1;
          store = reinterpret_cast<char*>(realloc(store, maxStoreLen));
        }
        if(mxGetString(mxGetCell(pNVT,ir+nrows),store,maxStoreLen) )
          mexErrMsgTxt("Fsdif_write_handler :: Failed to store NVT Name");
        
        nvt.AddNameValue(stringsName,store);
      }
    }
    if(nvt.GetNbNameValue()){
      p->AddNVT(nvt);    
    }
  }
}

void addTYPs(Easdif::SDIFEntity* p, const mxArray *pTYP) {

  int maxStoreLen = 512;
  char *store =reinterpret_cast<char*>(malloc(maxStoreLen));

  int field = mxGetFieldNumber(pTYP,headMatrixTypeD_fieldString);
  char sigstr [4];
  // Add Matrix Types
  if(field >= 0){
    mxArray *pMTD = mxGetFieldByNumber(pTYP,0,field);
    int numMTD = mxGetNumberOfElements(pMTD);
    int sigfield = mxGetFieldNumber(pMTD,mtdMatrixSig_fieldString);
    int colfield = mxGetFieldNumber(pMTD,mtdColName_fieldString);
    if(sigfield>=0 && colfield >=0){
      Easdif::MatrixType mtd;
     
      for(int ii=0; ii!= numMTD;++ii){
        mtd.Clear();
        const mxArray* msig      = mxGetFieldByNumber(pMTD,ii,sigfield);      
        if(!msig || !mxIsNumeric(msig) || mxIsComplex(msig) || mxGetNumberOfElements(msig) != 4 )
          mexErrMsgTxt("Fsdif_write_handler :: cannot write MTD Type :: msig entries have to be matrices with 4 elements !");
        mtd.SetSignature(SdifStringToSignature(readData(msig,sigstr)));    
        const mxArray* mcol      = mxGetFieldByNumber(pMTD,ii,colfield);      
        if(!mcol|| (!mxIsCell(mcol) && !mxIsChar(mcol)))
          mexErrMsgTxt("Fsdif_write_handler :: cname has to be cellarrays o strings or individual strings if the matrix contains a single colmun");
        if(mxIsCell(mcol)){
          int nel = mxGetNumberOfElements(mcol);
          for(int ir = 0; ir != nel; ++ ir) {
            mxArray *pCell = mxGetCell(mcol,ir) ;
            if(!pCell || !mxIsChar(pCell)){
              mexErrMsgTxt("Fsdif_write_handler :: cname has to be cellarrays of strings or individual strings if the matrix contains a single colmun");

            }
            if(mxGetNumberOfElements(pCell) + 1  > maxStoreLen){
              maxStoreLen= mxGetNumberOfElements(pCell) + 1;
              store = reinterpret_cast<char*>(realloc(store, maxStoreLen));
            }
            if(mxGetString(pCell,store,maxStoreLen) )
              mexErrMsgTxt("Fsdif_write_handler :: Failed to read TYP cname");

            mtd.AddColumn(store);
          }
        }
        else {
          if(mxGetNumberOfElements(mcol)  + 1 > maxStoreLen){
            maxStoreLen= mxGetNumberOfElements(mcol) + 1;
            store = reinterpret_cast<char*>(realloc(store, maxStoreLen));
          }
          if(mxGetString(mcol,store,maxStoreLen) )
            mexErrMsgTxt("Fsdif_write_handler :: Failed to read TYP  cname");

          mtd.AddColumn(store);
        }
        p->AddMatrixType(mtd);
      }
    }
    else   {
      sprintf(errMess,"Fsdif_write_handler :: MTD type description fields \"%s\" or \"%s\" are missing",mtdMatrixSig_fieldString,mtdColName_fieldString);
      mexErrMsgTxt(errMess);
    }
  }
  field = mxGetFieldNumber(pTYP,headFrameTypeD_fieldString);

  // Add Frame Types
  if(field >= 0){
    mxArray *pFTD = mxGetFieldByNumber(pTYP,0,field);
    int numFTD = mxGetNumberOfElements(pFTD);
    int sigfield   = mxGetFieldNumber(pFTD,ftdFrameSig_fieldString);
    int msigfield  = mxGetFieldNumber(pFTD,ftdMatrixSig_fieldString);
    int mnamefield = mxGetFieldNumber(pFTD,ftdMatrixName_fieldString);
  
    if(sigfield>=0 && mnamefield >=0 && msigfield >=0){
      Easdif::FrameType ftd;
      
      for(int ii=0; ii!= numFTD;++ii){
        ftd.Clear();
        
        const mxArray* fsig      = mxGetFieldByNumber(pFTD,ii,sigfield);      
        if(!fsig || !mxIsNumeric(fsig) || mxIsComplex(fsig) || mxGetNumberOfElements(fsig) != 4 )
          mexErrMsgTxt("cannot write FTD Type :: sig entries have to be matrices with 4 elements !");
        ftd.SetSignature(SdifStringToSignature(readData(fsig,sigstr)));    

        const mxArray* msig  = mxGetFieldByNumber(pFTD,ii,msigfield);      
        const mxArray* mname = mxGetFieldByNumber(pFTD,ii,mnamefield);      
        if(!msig|| !mxIsNumeric(msig) || !mxIsDouble(msig) )
          mexErrMsgTxt("Fsdif_write_handler :: matrixsig has to be a numeric array of type double");
        if(mxGetN(msig) != 4 )
          mexErrMsgTxt("Fsdif_write_handler :: matrixsig has to be a nx4 array");
        if(!mname || ! mxIsCell(mname) || (mxGetM(mname) !=1 && mxGetN(mname) != 1 ))
          mexErrMsgTxt("Fsdif_write_handler :: matrixname  has to be a 1-d cell array of strings");
        double *pd = reinterpret_cast<double*>(mxGetData(msig));
        int nbMTyp = mxGetM(msig);
        for(int im = 0; im != nbMTyp; ++ im){
          Easdif::MatrixType mtd;
          sigstr[0]= static_cast<char>(*(pd + 0*nbMTyp + im)); 
          sigstr[1]= static_cast<char>(*(pd + 1*nbMTyp + im)); 
          sigstr[2]= static_cast<char>(*(pd + 2*nbMTyp + im)); 
          sigstr[3]= static_cast<char>(*(pd + 3*nbMTyp + im)); 
          mxArray *pCell = mxGetCell(mname,im);
          if(pCell) {
            if(mxGetNumberOfElements(pCell)  + 1 > maxStoreLen){
              maxStoreLen= mxGetNumberOfElements(mxGetCell(mname,im)) + 1;
              store = reinterpret_cast<char*>(realloc(store, maxStoreLen));
            }
            if(mxGetString(mxGetCell(mname,im),store,maxStoreLen) )
              mexErrMsgTxt("Fsdif_write_handler :: Failed to store matrixName of FTD cell array has to contain only strings");
          }
          else
            mexErrMsgTxt("Fsdif_write_handler :: Failed to store matrixName in FTD cell array has to contain  strings of size larger than 0.");

          ftd.AddMatrixType(Easdif::MatrixType(SdifStringToSignature(sigstr)),store);
        }          

        p->AddFrameType(ftd);
      }
    }
    else {
      sprintf(errMess,"Fsdif_write_handler :: FTD type description fields \"%s\" or \"%s\" or \"%s\" are missing",ftdFrameSig_fieldString,ftdMatrixSig_fieldString,ftdMatrixName_fieldString);
      mexErrMsgTxt(errMess);
    }
  }
}
void addIDSs(Easdif::SDIFEntity* p, const mxArray *pIDS) {
  mexWarnMsgTxt("Fsdif_write_handler :: Writing of IDS String not yet supported - skipped");
}

#ifdef GCC_HAS_VISIBILITY
__attribute__ ((visibility("default")))
#endif
void 
mexFunction (int nlhs, mxArray *plhs [], int nrhs, const mxArray *prhs [])
{
  char command[128];

  // establish cleanup
  if(Easdif::EasdifInit()) {
    mexAtExit(cleanupAndEEnd);
  }

  if(nrhs < 1 || nrhs > 3)
    mexErrMsgTxt ("Fsdif_write_handler ::Wrong number of input arguments (at least 1, at most 3 )");

  if(!mxIsChar(prhs[0]) ||  (mxGetString(prhs[0], command, 128) != 0) ){
     mexErrMsgTxt ("Fsdif_write_handler ::no command at first argument");   
  }
  
  // test sdif file
  if(!strcmp(command,"issdif")){
    char	 filename [PATH_MAX];
    if(nrhs != 2)
      mexErrMsgTxt ("Fsdif_write_handler ::Wrong number of input arguments need filename");
    if (!mxIsChar (prhs [1]))
      mexErrMsgTxt ("Fsdif_write_handler ::Wrong type of input argument: want char");
    
    mxGetString (prhs [1], filename, PATH_MAX);
    if(nlhs>0) {
      plhs[0] = mxCreateDoubleMatrix (1, 1, mxREAL);
      if(SdifCheckFileFormat(filename))
        *mxGetPr(plhs[0]) = 1.;	
      else
        *mxGetPr(plhs[0]) = 0.;
    }
  }  
  else if(!strcmp(command,"clear")){
    cleanup();
  }  
  // open file for writing
  else if(!strcmp(command,"open")){

    char	 filename [PATH_MAX];

    // check arguments
    if(nrhs < 2 || nrhs > 3)
      mexErrMsgTxt ("Fsdif_write_handler ::Wrong number of input arguments need filename");

    if(nlhs > 1)
      mexErrMsgTxt ("Fsdif_write_handler ::Wrong number of output arguments (at most 1 )");
        
    if ((mxGetString(prhs[1], filename, PATH_MAX) != 0))
      mexErrMsgTxt("Fsdif_write_handler :: Could not convert your 1 argument (filename) into string data.");
    
    // determine unused slot in global list
    Easdif::SDIFEntity *p = 0;
    EASDList::iterator it = pList.begin();
    EASDList::iterator ite = pList.end();
    while(it!=ite){
      if(!(*it)->IsOpen()){
        p = *it;
        break;
      }
      ++it;
    }
    if(it==ite){
      p = new Easdif::SDIFEntity();
      if(p){
        pList.push_back(p);
      }
    }

    if(!p) {
      mexErrMsgTxt("Fsdif_write_handler :: Failed allocating Easdif file");      
    }

    try{
      // Add header data
      if (( nrhs == 3) && mxIsStruct(prhs[2])){
        // NVTs
        {
          int NVTField = mxGetFieldNumber(prhs[2], headNVT_fieldString);
          if(NVTField >= 0) {
            mxArray *NVT = mxGetFieldByNumber(prhs[2],0,NVTField);
            addNVTs(p,NVT);
          }          
        }
        // TYPs
        {
          int TYPField = mxGetFieldNumber(prhs[2], headTYP_fieldString);
          if(TYPField >= 0) {
            mxArray *TYP = mxGetFieldByNumber(prhs[2],0,TYPField);
            addTYPs(p,TYP);              
          }          
        }
        
        // IDS, not yet supported
        {
          int IDSField = mxGetFieldNumber(prhs[2], headIDS_fieldString);
          if(IDSField >= 0) {
            mxArray *IDS = mxGetFieldByNumber(prhs[2],0,IDSField);
            if(mxGetNumberOfElements(IDS)> 0 )
              addIDSs(p,IDS);              
          }          
        }
      }

      // open file and write header data
      if(!p->OpenWrite(filename)){
        char errMess [PATH_MAX+50];
        sprintf(errMess,"Fsdif_write_handler :: Cannot open file %s",filename);
        mexErrMsgTxt(errMess);      
      }      
    }
    catch(const Easdif::SDIFException&ex ) {
      char errMess [PATH_MAX+50];
      sprintf(errMess,"Fsdif_write_handler :: Cannot open file %s\nsystem message is :: %s",filename,ex.what());
      mexErrMsgTxt(errMess);      
    }

    // prepare output variable
#if SIZEOF_VOID_P  == 4
    plhs[0] = mxCreateNumericMatrix(1,1,mxUINT32_CLASS,mxREAL);
    *reinterpret_cast<Easdif::SDIFEntity **>(mxGetData(plhs[0])) = p;
#else
    plhs[0] = mxCreateNumericMatrix(1,1,mxUINT64_CLASS,mxREAL);
    *reinterpret_cast<Easdif::SDIFEntity **>(mxGetData(plhs[0])) = p;
#endif    
  }
  // close a file
  else  if(!strcmp(command,"close")){
    // check arguments
    if(nrhs != 2)
      mexErrMsgTxt ("Fsdif_write_handler :: Wrong number of input arguments, need to have filepointer");

    if(nlhs > 1)
      mexErrMsgTxt ("Fsdif_write_handler :: Wrong number of output arguments (at most 1 )");
  
    if(mxGetNumberOfElements(prhs[1]) != 1)
      mexErrMsgTxt ("Fsdif_write_handler :: argument should be scalar");
    
    // validate argument
    bool ok = false;
    if(mxIsInt32(prhs[1]) || mxIsInt64(prhs[1]) || mxIsUint32(prhs[1]) || mxIsUint64(prhs[1])){
      Easdif::SDIFEntity *p = *reinterpret_cast<Easdif::SDIFEntity **>(mxGetData(prhs[1]));    
      if(CheckList(p) &&p->IsOpen()){
        ok = p->Close();
      }    
    }
    else {
      mexErrMsgTxt("Fsdif_write_handler :: can only handle argument types int32/int64/uint32/uint64. Cannot close file!");
    }

    if(nlhs >= 1) {
      plhs[0] = mxCreateNumericMatrix(1,1,mxUINT32_CLASS,mxREAL);
      *reinterpret_cast<int*>(mxGetData(plhs[0])) = ok;
    }
  }
  // write file
  else  if(!strcmp(command,"write")){
    // check arguments
    if(nrhs !=3)
      mexErrMsgTxt ("Fsdif_write_handler :: Wrong number of input arguments, need to have filepointer and data data write");

    if(nlhs > 1)
      mexErrMsgTxt ("Fsdif_write_handler :: Wrong number of output arguments (at most 1)");

    // check file pointer type
    int written = 0 ;
    if(mxIsInt32(prhs[1]) || mxIsInt64(prhs[1]) || mxIsUint32(prhs[1]) || mxIsUint64(prhs[1])){
      Easdif::SDIFEntity *p = *reinterpret_cast<Easdif::SDIFEntity **>(mxGetData(prhs[1]));    
      // validate file pointer
      if(CheckList(p) && p->IsOpen()){
        // write frames
        written = writeData(p, prhs[2]);
      }
      else
        mexErrMsgTxt("Fsdif_write_handler :: Winvalid file pointer");      
    }
    else {
      mexErrMsgTxt("Fsdif_write_handler :: Wcan only filepointer argument types int32/int64/uint32/uint64!");
    }

    if(nlhs == 1){
      plhs[0] = mxCreateNumericMatrix(1,1,mxDOUBLE_CLASS,mxREAL);      
      *reinterpret_cast<double*>(mxGetData(plhs[0])) = written;
    }
  }
  else if(!strcmp(command,"version")){
    mexPrintf("Fsdif_write_handler: Version %s\n",MAKE_STRING_FROM_MACRO(EASDIF_MATLAB_VERSION));
    mexPrintf("Easdif Version %s\n", EASDIF_VERSION_STRING);
  }  
  else if(!strcmp(command,"help")){
    mexPrintf("Fsdif_write_handler: Version %s\n",MAKE_STRING_FROM_MACRO(EASDIF_MATLAB_VERSION));
    mexPrintf("issdif    : check sdiffile\n");
    mexPrintf("open      : open file \n");
    mexPrintf("write      : read from file \n");
    mexPrintf("close     : close file\n");
    mexPrintf("clear     : close all files and release memory\n");
    mexPrintf("version   : display version number\n");
    mexPrintf("help      : this short help\n");
  }
  else{  
    mexErrMsgTxt ("Fsdif_write_handler:: unknown command");
  }

  return;
}
