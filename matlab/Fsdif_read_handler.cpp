/**
 * @file   Fsdif_read_handler.cpp
 * @author Axel Roebel
 * @date   Sat Jan 19 15:05:41 2008
 * 
 * @brief  handle read file io in matlab
 * 
 *
 * $Revision: 1.17 $   last changed on $Date: 2011-09-27 11:27:35 $
 *
 *                                    Copyright (c) 2008 by IRCAM
 * 
 */

#include <mex.h>
#include <cstdlib>
#include <stdint.h>
#include "easdif/easdif.h"

#include "fieldnames.hpp"

#define MAKE_STRING_FROM_MACRO(MM) MAKE_STRING_FROM_MACRO_HELP(MM) 
#define MAKE_STRING_FROM_MACRO_HELP(MM) #MM



// global list for all easdif file pointers
typedef std::list<std::pair<Easdif::SDIFEntity *,Easdif::SDIFEntity::const_iterator> > EASDList;
EASDList pList;

// mexAtExit cleanup function 
void cleanup() {
  EASDList::iterator it = pList.begin();
  EASDList::iterator ite = pList.end();
  while(it!=ite){
    delete it->first;
    it->first = 0;
    ++it;
  }
  pList.clear();  
  //  Easdif::EasdifEnd();
}
void cleanupAndEEnd() {
  cleanup();
  Easdif::EasdifEnd();
}

// validate file pointer
bool CheckList(Easdif::SDIFEntity *p, EASDList::iterator&it) {
  it = pList.begin();
  EASDList::iterator ite = pList.end();
  while(it!=ite){
    if(p==it->first){      
      return true;
    }
    ++it;
  }
  return false;
}

char errMess [PATH_MAX+50];

mxClassID
getClassFromString(const mxArray *pp){
  if (mxGetClassID(pp) == mxCHAR_CLASS ) {
    char buf [100];
    buf[0] = '0';
    mxGetString(pp, buf, 100);
    std::string class_str = buf;
    if(class_str == "double")
      return mxDOUBLE_CLASS;
    else if(class_str == "single" || class_str == "float")
      return mxSINGLE_CLASS;
    else if(class_str == "INT8")
      return mxINT8_CLASS;
    else if(class_str == "INT16")
      return mxINT16_CLASS;
    else if(class_str == "INT32")
      return mxINT32_CLASS;
    else if(class_str == "INT64")
      return mxINT64_CLASS;
    else if(class_str == "UINT8")
      return mxUINT8_CLASS;
    else if(class_str == "UINT16")
      return mxUINT16_CLASS;
    else if(class_str == "UINT32")
      return mxUINT32_CLASS;
    else if(class_str == "UINT64")
      return mxINT64_CLASS;
    else {
      sprintf(errMess,"Fsdif_read_handler :: unsupported output class selected !");
      mexErrMsgTxt(errMess);              
    }    
  }
  return mxUNKNOWN_CLASS;
}

inline
int
nbSelectedMatricesInFrame(Easdif::SDIFEntity*p, 
                          Easdif::SDIFEntity::const_iterator& it) {
  
  int nbAllMatrix = it.GetLoc().LocNbMatrix();
  int nbMatrix    = 0;
  
  for(int im=0;im!=nbAllMatrix;++im){          
    //mexPrintf("matrix sig  %s ", SdifSignatureToString(it.GetLoc().LocMSignature(im)));
    if(p->TestMatrixSelection(it.GetLoc().LocMSignature(im))){
      //      mexPrintf("selected \n");
      ++nbMatrix;
    }
    //    else
    //        mexPrintf("not selected \n");
  }          
  return nbMatrix;
}

// create frame struct
// either only meta data if nFields == 4
// or meat and matrix data if nFields == 5
void
createFrame(Easdif::SDIFEntity*p, Easdif::SDIFEntity::const_iterator& it, int ii, 
            mxArray *out,int nFields, const char **dfields, mxClassID classid) {
  if(nFields ==4 ) {
    int nbMatrix = nbSelectedMatricesInFrame(p, it);
    int nbAllMatrix =  it.GetLoc().LocNbMatrix();
    mxArray *ftypsig    = mxCreateNumericMatrix(1,4,mxDOUBLE_CLASS,mxREAL);
    mxArray *ftypstrid  = mxCreateNumericMatrix(1,1,mxDOUBLE_CLASS,mxREAL);
    mxArray *ftyptime   = mxCreateNumericMatrix(1,1,mxDOUBLE_CLASS,mxREAL);
    mxArray *ftypmsig   = mxCreateNumericMatrix(nbMatrix,4,mxDOUBLE_CLASS,mxREAL);
    
    mxSetField(out,ii,dfields[0], ftypsig);      
    mxSetField(out,ii,dfields[1], ftypstrid);      
    mxSetField(out,ii,dfields[2], ftyptime);      
    mxSetField(out,ii,dfields[3], ftypmsig);      
    
    double* pd= reinterpret_cast<double*>( mxGetData(ftypsig));
    char* sigstr = SdifSignatureToString(it.GetLoc().LocSignature());
    
    *pd     = sigstr[0];
    *(pd+1) = sigstr[1];
    *(pd+2) = sigstr[2];
    *(pd+3) = sigstr[3];
    
    *reinterpret_cast<double*>( mxGetData(ftypstrid)) = it.GetLoc().LocStreamID();
    *reinterpret_cast<double*>( mxGetData(ftyptime))  = it.GetLoc().LocTime();
    
    pd= reinterpret_cast<double*>( mxGetData(ftypmsig));
    for(int im=0;im!=nbAllMatrix;++im){          
      if(p->TestMatrixSelection(it.GetLoc().LocMSignature(im))){
        sigstr = SdifSignatureToString(it.GetLoc().LocMSignature(im));
        *pd              = sigstr[0];
        *(pd+nbMatrix)   = sigstr[1];
        *(pd+2*nbMatrix) = sigstr[2];
        *(pd+3*nbMatrix) = sigstr[3];
        ++pd;        
      }          
    }
  }
  else if(nFields ==5){
    Easdif::SDIFFrame frame;
    frame = *it;
    int nbMatrix = frame.GetNbMatrix();
    
    mxArray *ftypsig    = mxCreateNumericMatrix(1,4,mxDOUBLE_CLASS,mxREAL);
    mxArray *ftypstrid  = mxCreateNumericMatrix(1,1,mxDOUBLE_CLASS,mxREAL);
    mxArray *ftyptime   = mxCreateNumericMatrix(1,1,mxDOUBLE_CLASS,mxREAL);
    mxArray *ftypmsig   = mxCreateNumericMatrix(nbMatrix,4,mxDOUBLE_CLASS,mxREAL);
    std::vector<char> names(8*nbMatrix,0);
    std::vector<char*> ptr(nbMatrix);

    for(int im=0;im!=nbMatrix;++im){
      char* sigstr = SdifSignatureToString(frame.GetMatrix(im).GetSignature());
      ptr[im] = &names[im*8];
      ptr[im][0] = 'M';
      ptr[im][1] = 'D';
      ptr[im][2] = '_';
      ptr[im][3] = sigstr[0];
      ptr[im][4] = sigstr[1];
      ptr[im][5] = sigstr[2];
      ptr[im][6] = sigstr[3];
    }
    
    mxArray *ftypmdata  = mxCreateStructMatrix(1,1,nbMatrix,const_cast<const char**>(&ptr[0]));
    
    mxSetField(out,ii,dfields[0], ftypsig);      
    mxSetField(out,ii,dfields[1], ftypstrid);      
    mxSetField(out,ii,dfields[2], ftyptime);      
    mxSetField(out,ii,dfields[3], ftypmsig);      
    mxSetField(out,ii,dfields[4], ftypmdata);      
   
    double* pd= reinterpret_cast<double*>(mxGetData(ftypsig));
    char* sigstr = SdifSignatureToString(frame.GetSignature());
    
    *pd     = sigstr[0];
    *(pd+1) = sigstr[1];
    *(pd+2) = sigstr[2];
    *(pd+3) = sigstr[3];
    
    *reinterpret_cast<double*>( mxGetData(ftypstrid)) = frame.GetStreamID();
    *reinterpret_cast<double*>( mxGetData(ftyptime))  = frame.GetTime();
    
    pd= reinterpret_cast<double*>( mxGetData(ftypmsig));
    for(int im=0;im!=nbMatrix;++im){          
      Easdif::SDIFMatrix &mat = frame.GetMatrix(im);
      sigstr = SdifSignatureToString(mat.GetSignature());
      *pd              = sigstr[0];
      *(pd+nbMatrix)   = sigstr[1];
      *(pd+2*nbMatrix) = sigstr[2];
      *(pd+3*nbMatrix) = sigstr[3];
      ++pd;        
      mxArray *data = 0;

      if(mat.GetNbRows()) {
        if(classid == mxUNKNOWN_CLASS) {
          switch(mat.GetType()) {
          case eFloat4:
          case eFloat4a:
          case eFloat4b:
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxSINGLE_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<float*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case eFloat8:
          case eFloat8a:
          case eFloat8b:
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxDOUBLE_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<double*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;          
          case eInt1 :
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxINT8_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<unsigned char *> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case eInt2 :
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxINT16_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifInt2*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case eInt4 :
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxINT32_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifInt4*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case eUInt1 :
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxUINT8_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifUInt1*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case eUInt2 :
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxUINT16_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifUInt2*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case eUInt4 :
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxUINT32_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifUInt4*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case eText:{
            std::string str;
            mat.Get(str);
            data = mxCreateString(str.c_str());
            break;
          }
          default:
            char tmperr[256];
            sprintf(tmperr,"Unsupported data type %x",mat.GetType());
            mexErrMsgTxt(tmperr);
          } 
        }
        else {
          data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(), classid, mxREAL);
          switch(classid) {
          case mxDOUBLE_CLASS:
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<double*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case mxSINGLE_CLASS:
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<float*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;          
          case mxINT8_CLASS :
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxINT8_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifInt1 *> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case  mxINT16_CLASS :
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifInt2*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case mxINT32_CLASS :
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifInt4*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case  mxUINT8_CLASS :
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifUInt1*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case mxUINT16_CLASS:
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifUInt2*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case mxUINT32_CLASS :
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifUInt4*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          default:
            char tmperr[256];
            sprintf(tmperr,"Unsupported data type %x",mat.GetType());
            mexErrMsgTxt(tmperr);
          }         
        }
      }
      if(data)
        mxSetField(ftypmdata,0,ptr[im],data);      
    }
  }
}


void
createMData(Easdif::SDIFEntity::const_iterator& it, int& ii, mxArray *mxData,
            mxClassID classid,
            mxArray *mxHead, mxArray *mxFrame, mxArray *mxMatrix, mxArray* mxDims) {
  if(mxData){
    Easdif::SDIFFrame frame = *it;
    char *sigstr;
    int totNum         = mxGetNumberOfElements(mxData);
    SdifSignature frameSig = it.GetLoc().LocSignature();
    int nbMatrix       =frame.GetNbMatrix();
    double ftime       = it.GetLoc().LocTime();
    SdifUInt4 streamId = it.GetLoc().LocStreamID();
    double *pd = 0;

    for (int im= 0;im != nbMatrix; ++im) {
      Easdif::SDIFMatrix &mat = frame.GetMatrix(im);
      if(mxDims){
        pd= reinterpret_cast<double*>( mxGetData(mxDims)) + ii + im;        
        *pd            = mat.GetNbRows();
        *(pd+totNum)   = mat.GetNbCols();
      }
      if(mxHead){
        pd= reinterpret_cast<double*>( mxGetData(mxHead)) + ii + im;        
        *pd            = ftime;
        *(pd+totNum)   = streamId;
      }
      if(mxFrame){
        pd= reinterpret_cast<double*>( mxGetData(mxFrame)) + ii + im;
        sigstr = SdifSignatureToString(frameSig);
        *pd            = sigstr[0];
        *(pd+totNum)   = sigstr[1];
        *(pd+2*totNum) = sigstr[2];
        *(pd+3*totNum) = sigstr[3];
      }

      if(mxMatrix){
        pd= reinterpret_cast<double*>( mxGetData(mxMatrix)) + ii + im;
        sigstr = SdifSignatureToString(mat.GetSignature());
        *pd            = sigstr[0];
        *(pd+totNum)   = sigstr[1];
        *(pd+2*totNum) = sigstr[2];
        *(pd+3*totNum) = sigstr[3];
      }

      mxArray *data = 0;
      if(mat.GetNbRows()) {
        if(classid == mxUNKNOWN_CLASS) {
          switch(mat.GetType()) {
          case eFloat4:
          case eFloat4a:
          case eFloat4b:
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxSINGLE_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<float*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case eFloat8:
          case eFloat8a:
          case eFloat8b:
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxDOUBLE_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<double*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
        
          case eInt1 :
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxINT8_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<unsigned char *> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case eInt2 :
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxINT16_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifInt2*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case eInt4 :
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxINT32_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifInt4*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case eUInt1 :
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxUINT8_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifUInt1*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case eUInt2 :
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxUINT16_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifUInt2*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case eUInt4 :
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxUINT32_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifUInt4*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case eText:{
            std::string str;
            mat.Get(str);
            data = mxCreateString(str.c_str());
            break;
          }
          default:
            char tmperr[256];
            sprintf(tmperr,"Unsupported data type %x",mat.GetType());
            mexErrMsgTxt(tmperr);
          }
        }
        else {
          data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(), classid, mxREAL);
          switch(classid) {
          case mxDOUBLE_CLASS:
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<double*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case mxSINGLE_CLASS:
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<float*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;          
          case mxINT8_CLASS :
            data = mxCreateNumericMatrix(mat.GetNbRows(),mat.GetNbCols(),mxINT8_CLASS,mxREAL);
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifInt1 *> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case  mxINT16_CLASS :
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifInt2*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case mxINT32_CLASS :
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifInt4*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case  mxUINT8_CLASS :
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifUInt1*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case mxUINT16_CLASS:
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifUInt2*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          case mxUINT32_CLASS :
            for(int ic=0;ic!=mat.GetNbCols();++ic)
              mat.GetCol(reinterpret_cast<SdifUInt4*> (mxGetData(data))+ic*mat.GetNbRows(),ic);
            break;
          default:
            char tmperr[256];
            sprintf(tmperr,"Unsupported data type %x",mat.GetType());
            mexErrMsgTxt(tmperr);
          }         
        }
      }
      if(data)
        mxSetCell(mxData,ii+im,data);      
    }
      
  }
  ii += it.GetLoc().LocNbMatrix();
}

#ifdef GCC_HAS_VISIBILITY
__attribute__ ((visibility("default")))
#endif
void 
mexFunction (int nlhs, mxArray *plhs [], int nrhs, const mxArray *prhs [])
{
  char command[128];

  if(Easdif::EasdifInit()) {
    mexAtExit(cleanupAndEEnd);
  }

  if(nrhs < 1 || nrhs > 4)
    mexErrMsgTxt ("Fsdif_read_handler::Wrong number of input arguments (at least 1, at most 4 )");

  if(!mxIsChar(prhs[0]) ||  (mxGetString(prhs[0], command, 128) != 0) ){
     mexErrMsgTxt ("Fsdif_read_handler::no command at first argument");   
  }
  
  if(!strcmp(command,"issdif")){
    char	 filename [PATH_MAX];
    if(nrhs != 2)
      mexErrMsgTxt ("Fsdif_read_handler::Wrong number of input arguments need filename");
    if (!mxIsChar (prhs [1]))
      mexErrMsgTxt ("Fsdif_read_handler::Wrong type of input argument: want char");
    
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
  else if(!strcmp(command,"open")){

    char	 filename [PATH_MAX];
    
    // test parameters
    if(nrhs != 2)
      mexErrMsgTxt ("Fsdif_read_handler::Wrong number of input arguments need filename");

    if(nlhs > 3)
      mexErrMsgTxt ("Fsdif_read_handler::Wrong number of output arguments (at most 3 )");
        
    if ((mxGetString(prhs[1], filename, PATH_MAX) != 0))
      mexErrMsgTxt("Fsdif_read_handler::Could not convert your 1 argument (filename) into string data.");
    
    // get a free slot in sdif file pointer list
    Easdif::SDIFEntity *p = 0;
    EASDList::iterator it = pList.begin();
    EASDList::iterator ite = pList.end();
    while(it!=ite){
      if(! it->first->IsOpen()){
        p = it->first;
        break;
      }
      ++it;
    }
    if(it==ite){
      p = new Easdif::SDIFEntity();
      if(p){
        pList.push_back(std::pair<Easdif::SDIFEntity*,Easdif::SDIFEntity::const_iterator>(p,Easdif::SDIFEntity::const_iterator()));
        it = --pList.end();
      }
      else {
        mexErrMsgTxt("Fsdif_read_handler :: Failed allocating Easdif file");      
      }
    }

    // open the file
    try{
      if(!it->first->OpenRead(filename)){
        sprintf(errMess,"Fsdif_read_handler :: Cannot open file %s",filename);
        mexErrMsgTxt(errMess);      
      }      
    }
    catch(const Easdif::SDIFException&ex ) {
      sprintf(errMess,"Fsdif_read_handler :: Cannot open file %s\nsystem message is :: %s",filename,ex.what());
      mexErrMsgTxt(errMess);      
    }

    // enable frame dir to be able to work with selections
    it->first->EnableFrameDir();
    it->second = it->first->begin();

    // create file output argument
#if SIZEOF_VOID_P  == 4
    plhs[0] = mxCreateNumericMatrix(1,1,mxUINT32_CLASS,mxREAL);
    *reinterpret_cast<Easdif::SDIFEntity **>(mxGetData(plhs[0])) = p;
#else
    plhs[0] = mxCreateNumericMatrix(1,1,mxUINT64_CLASS,mxREAL);
    *reinterpret_cast<Easdif::SDIFEntity **>(mxGetData(plhs[0])) = p;
#endif
      
    if(nlhs > 1) {
      const char *fields[3];
      fields[0] = headNVT_fieldString; 
      fields[1] = headIDS_fieldString; 
      fields[2] = headTYP_fieldString; 
      plhs[1] = mxCreateStructMatrix(1,1,3,fields);
      
      // NVTs
      {
        int numNVTs =  p->GetNbNVT();
        
        int sumNVTEntries = 0;
        for(int ii=0;ii!=numNVTs;++ii){      
          sumNVTEntries += p->GetNVT(ii).size();
        }
        
        mxArray *NVT  = mxCreateCellMatrix(std::max(0,sumNVTEntries + numNVTs - 1), 2);
        if(NVT ==0 ){
          mexErrMsgTxt("Fsdif_read_handler :: Error allocating NVT Cell aray");          
        }

        int offvalue = std::max(0,sumNVTEntries + numNVTs - 1);
        int inval=0;
        for(int invt=0; invt != numNVTs ; ++invt){
          Easdif::SDIFNameValueTable &currNvt = p->GetNVT(invt);
          std::map<std::string,std::string>::const_iterator it = currNvt.begin();
          std::map<std::string,std::string>::const_iterator ite = currNvt.end();
          while(it != ite){
            mxSetCell(NVT,inval,mxCreateString((*it).first.c_str()));
            mxSetCell(NVT,inval+offvalue,mxCreateString((*it).second.c_str()));
            ++it;
            ++inval;
          }
          if(invt != numNVTs - 1){
            mxSetCell(NVT,inval,mxCreateNumericMatrix(0,0,mxUINT32_CLASS,mxREAL));
            mxSetCell(NVT,inval+offvalue,mxCreateNumericMatrix(0,0,mxUINT32_CLASS,mxREAL));
          }
        }
        mxSetField(plhs[1],0,fields[0],NVT);
      }
      
      //      IDS
      {
        int numstreamid;
        SdifStringT *string;
        char idnumasstring[30];
        SdifFileT *input = p->GetFile();       
        
        string = SdifStringNew();
        
        if((numstreamid=SdifStreamIDTableGetNbData(input->StreamIDsTable)) > 0){
          unsigned int iID;
          SdifHashNT* pID;
          SdifStreamIDT *sd;
          
          for(iID=0; iID<input->StreamIDsTable->SIDHT->HashSize; iID++)
            for (pID = input->StreamIDsTable->SIDHT->Table[iID]; pID; pID = pID->Next) {          
              SdifStringAppend(string,"IDS ");
              sd = ((SdifStreamIDT * )(pID->Data));
              sprintf(idnumasstring,"%d ",sd->NumID);
              SdifStringAppend(string,idnumasstring);
              SdifStringAppend(string,sd->Source);
              SdifStringAppend(string,":");
              SdifStringAppend(string,sd->TreeWay);
              SdifStringAppend(string,"\n");          
            }
        }
        
        if (string->SizeW)  
          mxSetField(plhs[1],0,fields[1], mxCreateString(string->str));          
        else
          mxSetField(plhs[1],0,fields[1], mxCreateString(""));          
        
        SdifStringFree(string);
        
      }
      
      //TYP
      {
        const char *tfields[2];
        tfields[0] = headMatrixTypeD_fieldString; 
        tfields[1] = headFrameTypeD_fieldString; 
        mxArray *typ = mxCreateStructMatrix(1,1,2,tfields);
        mxSetField(plhs[1],0,fields[2], typ);      

        // matrix types
        {
          const char *mfields[2];
          mfields[0] = mtdMatrixSig_fieldString; 
          mfields[1] = mtdColName_fieldString; 
          std::vector<Easdif::MatrixType> matrixtypes;
          p->GetTypes(matrixtypes);
          
          mxArray *mtyp = mxCreateStructMatrix(matrixtypes.size(),1,2,mfields);
          mxSetField(typ,0,tfields[0], mtyp);      
          
          for(unsigned int ii=0;ii!=matrixtypes.size();++ii){
            Easdif::MatrixType &mat = matrixtypes[ii];
          
            mxArray *mtypsig = mxCreateNumericMatrix(1,4,mxDOUBLE_CLASS,mxREAL);
            mxArray *mtypcol = mxCreateCellMatrix(1,mat.mvColumnNames.size());
            mxSetField(mtyp,ii,mfields[0], mtypsig);      
            mxSetField(mtyp,ii,mfields[1], mtypcol);      
            
            double* pd= reinterpret_cast<double*>( mxGetData(mtypsig));
            char* sigstr = SdifSignatureToString(mat.GetSignature());
            *pd     = sigstr[0];
            *(pd+1) = sigstr[1];
            *(pd+2) = sigstr[2];
            *(pd+3) = sigstr[3];
            
            for(unsigned int ic=0;ic!=mat.mvColumnNames.size();++ic){            
              mxSetCell(mtypcol,ic,mxCreateString(mat.mvColumnNames[ic].c_str()));        
            }
          }
        }
        
        // frame types
        {
          
          const char *ffields[3];
          ffields[0] = ftdFrameSig_fieldString; 
          ffields[1] = ftdMatrixSig_fieldString; 
          ffields[2] = ftdMatrixName_fieldString; 
          std::vector<Easdif::FrameType> frametypes;
          p->GetTypes(frametypes);
          mxArray *ftyp = mxCreateStructMatrix(frametypes.size(),1,3,ffields);
          mxSetField(typ,0,tfields[1], ftyp);      
        
          for(unsigned int ii=0;ii!=frametypes.size();++ii){
            Easdif::FrameType &frm = frametypes[ii];
            
            mxArray *ftypsig  = mxCreateNumericMatrix(1,4,mxDOUBLE_CLASS,mxREAL);
            mxArray *ftypmsig = mxCreateNumericMatrix(frm.mvMatrixTypes.size(),4,mxDOUBLE_CLASS,mxREAL);
            mxArray *ftypmnam = mxCreateCellMatrix(1,frm.mvMatrixNames.size());
            
            mxSetField(ftyp,ii,ffields[0], ftypsig);      
            mxSetField(ftyp,ii,ffields[1], ftypmsig);      
            mxSetField(ftyp,ii,ffields[2], ftypmnam);      
            
            double* pd= reinterpret_cast<double*>( mxGetData(ftypsig));
            char* sigstr = SdifSignatureToString(frm.GetSignature());
            
            *pd     = sigstr[0];
            *(pd+1) = sigstr[1];
            *(pd+2) = sigstr[2];
            *(pd+3) = sigstr[3];
            
            int off   = frm.mvMatrixNames.size();
            pd= reinterpret_cast<double*>( mxGetData(ftypmsig));
            for(unsigned int im=0;im!=frm.mvMatrixTypes.size();++im){          
              sigstr = SdifSignatureToString(frm.mvMatrixTypes[im].GetSignature());
              *pd         = sigstr[0];
              *(pd+off)   = sigstr[1];
              *(pd+2*off) = sigstr[2];
              *(pd+3*off) = sigstr[3];
              ++pd;        
              mxSetCell(ftypmnam,im,mxCreateString(frm.mvMatrixNames[im].c_str()));        
            }
          }
        }
      }

      if(nlhs > 2) {
        // create frame directory output
        const char *dfields[4];
        dfields[0] = frameFrameSig_fieldString; 
        dfields[1] = frameStreamID_fieldString; 
        dfields[2] = frameTime_fieldString; 
        dfields[3] = frameMatrixSig_fieldString; 
        
        Easdif::SDIFEntity::const_iterator it = p->begin();
        Easdif::SDIFEntity::const_iterator ite = p->end();
        // establish directory
        while(it !=ite)
          ++it;
        p->Rewind();
        const Easdif::Directory & dir= p->GetDirectory();
       
        it      = p->begin();
        plhs[2] = mxCreateStructMatrix(dir.size(),1,4,dfields);
        for(int ii=0; it !=ite; ++it,++ii) {
          createFrame(p, it, ii, plhs[2], 4, dfields, mxUNKNOWN_CLASS);
        }
      }
    }
  }
  // close file
  else  if(!strcmp(command,"close")){
    // check arguments
    if(nrhs != 2)
      mexErrMsgTxt ("Fsdif_read_handler::Wrong number of input arguments, need to have filepointer");

    if(nlhs > 1)
      mexErrMsgTxt ("Fsdif_read_handler::Wrong number of output arguments (at most 1 )");
  
    if(mxGetNumberOfElements(prhs[1]) != 1)
      mexErrMsgTxt ("Fsdif_read_handler::argument should be scalar");
      
    bool ok = false;
    if(mxIsInt32(prhs[1]) || mxIsInt64(prhs[1]) || mxIsUint32(prhs[1]) || mxIsUint64(prhs[1])){
      Easdif::SDIFEntity *p = *reinterpret_cast<Easdif::SDIFEntity **>(mxGetData(prhs[1]));    
      EASDList::iterator itl;
      // validate pointer
      if(CheckList(p,itl) && p->IsOpen()){
        ok = p->Close();
      }    
    }
    else {
      mexErrMsgTxt("Fsdif_read_handler::can only handle argument types int32/int64/uint32/uint64. Cannot close file!");
    }

    if(nlhs >= 1) {
      plhs[0] = mxCreateNumericMatrix(1,1,mxUINT32_CLASS,mxREAL);
      *reinterpret_cast<int*>(mxGetData(plhs[0])) = ok;
    }
  }
  // rewind pointer to frist frame
  else if (!strcmp(command,"rewind")){
    if(nrhs > 2)
      mexErrMsgTxt ("Fsdif_read_handler:: Wrong number of input arguments for rewind: give only file handle\n");
    if(mxIsInt32(prhs[1]) || mxIsInt64(prhs[1]) || mxIsUint32(prhs[1]) || mxIsUint64(prhs[1])){
      Easdif::SDIFEntity *p = *reinterpret_cast<Easdif::SDIFEntity **>(mxGetData(prhs[1]));    
      EASDList::iterator itl;
      // validate pointer
      if(CheckList(p,itl) && p->IsOpen()){
        itl->second = itl->first->begin();
      }    
    } 
  }
  // read file
  else  if(!strcmp(command,"read")){

    // check arguments
    if(nrhs < 2 || nrhs > 4)
      mexErrMsgTxt ("Fsdif_read_handler::Wrong number of input arguments:\n supported are  filepointer, optional selectionmask or frame count and an optional class specification for returned data\n");

    if(nlhs != 1)
      mexErrMsgTxt ("Fsdif_read_handler::Wrong number of output arguments (at most 1 )");
  
    if(mxGetNumberOfElements(prhs[1]) != 1)
      mexErrMsgTxt ("Fsdif_read_handler::argument should be scalar");
  
    if(mxIsInt32(prhs[1]) || mxIsInt64(prhs[1]) || mxIsUint32(prhs[1]) || mxIsUint64(prhs[1])){
      Easdif::SDIFEntity *p = *reinterpret_cast<Easdif::SDIFEntity **>(mxGetData(prhs[1]));    
      if(p==0 && !pList.empty())
        p = pList.front().first;

      EASDList::iterator itl;

      // validate pointer
      if(CheckList(p,itl) && p->IsOpen()){
        Easdif::SDIFEntity::const_iterator ite = p->end();        
        if(nrhs == 2 || (mxIsNumeric(prhs[2]) && mxGetNumberOfElements(prhs[2]) == 1) ){
          mxClassID classid = mxUNKNOWN_CLASS;
          if(nrhs == 4) {
            classid = getClassFromString(prhs[3]);
          }
          int maxNum = nrhs > 2 ? static_cast<int>(mxGetScalar(prhs[2])): 1;

          if (maxNum > 0) {
            if(itl->second != ite && !itl->second.IsSelected())
              ++(itl->second);
            
            if(itl->second != ite){
              const char *dfields[5];
              dfields[0] = frameFrameSig_fieldString; 
              dfields[1] = frameStreamID_fieldString; 
              dfields[2] = frameTime_fieldString; 
              dfields[3] = frameMatrixSig_fieldString; 
              dfields[4] = frameData_fieldString; 

              
              plhs[0] = mxCreateStructMatrix(maxNum, 1, 5, dfields);
              while(maxNum && itl->second != ite) {
                createFrame(p, itl->second, 0, plhs[0],5, dfields, classid);
                ++(itl->second);
                --maxNum;
              }
            }
            else {
              plhs[0] = mxCreateNumericMatrix(0,0, 
                                              classid == mxUNKNOWN_CLASS ? mxDOUBLE_CLASS: classid, 
                                              mxREAL);
            }
          }
          else {
            plhs[0] = mxCreateNumericMatrix(0,0, 
                                            classid == mxUNKNOWN_CLASS ? mxDOUBLE_CLASS: classid, 
                                            mxREAL);
          }
        }
        else if (nrhs >= 3){
          std::vector<double> timeSel;
          double startTime = 0;
          double endTime   = -1;
          unsigned int start_id = 0;
          unsigned int end_id   = static_cast<unsigned int>(-1);
          Easdif::SDIFEntity::const_iterator it_stored = itl->second;

          mxClassID classid = mxUNKNOWN_CLASS;
          if(nrhs == 4) {
            classid = getClassFromString(prhs[3]);
          }
          // remove all high level selections 
          // == ( temporary selections working above the file name selections)
          p->ReestablishStreamSelection();
          p->ReestablishFrameSelection();
          p->ReestablishMatrixSelection();
            
          // establish temporary selection 
          if(mxIsStruct(prhs[2])){
            int numStructs = mxGetNumberOfElements(prhs[2]);
            char sigstr[5] = {0};
            
            for(int ii=0; ii!= numStructs; ++ii){
              const mxArray* fsig      = mxGetField(prhs[2],ii,frameFrameSig_fieldString);      
              const mxArray* streamid  = mxGetField(prhs[2],ii,frameStreamID_fieldString);      
              const mxArray* ftime     = mxGetField(prhs[2],ii,frameTime_fieldString);      
              const mxArray* msig      = mxGetField(prhs[2],ii,frameMatrixSig_fieldString);      

              const mxArray* streamid_range  = mxGetField(prhs[2],ii,frameStreamIDRange_fieldString);      
              const mxArray* ftime_range     = mxGetField(prhs[2],ii,frameTimeRange_fieldString);      
        
              if(fsig){
                if( !mxIsNumeric(fsig) || !mxIsDouble(fsig) || mxIsComplex(fsig) || mxGetN(fsig) != 4 ){
                  itl->second = it_stored;
                  sprintf(errMess,"Fsdif_read_handler :: frame signature selection .%s  has to be matrix of doubles with 4 columns !",frameFrameSig_fieldString);
                  mexErrMsgTxt(errMess);
                }
                int numSel = mxGetM(fsig);
                
                double *pd = reinterpret_cast<double*>(mxGetData(fsig));
                Easdif::SelectionSet<SdifSignature>  frameSel;
                for(int isel = 0; isel != numSel; ++isel){
                  sigstr[0]= static_cast<char>(*(pd + 0*numSel + isel)); 
                  sigstr[1]= static_cast<char>(*(pd + 1*numSel + isel)); 
                  sigstr[2]= static_cast<char>(*(pd + 2*numSel + isel)); 
                  sigstr[3]= static_cast<char>(*(pd + 3*numSel + isel)); 
  
                  frameSel.insert(SdifStringToSignature(sigstr));    
                }
                p->RestrictFrameSelection(frameSel);
              }

              if(msig){
                if( !mxIsNumeric(msig) || !mxIsDouble(msig) || mxIsComplex(msig) || mxGetN(msig) != 4 ){
                  itl->second = it_stored;
                  sprintf(errMess,"Fsdif_read_handler :: matrix signature selection .%s  has to be matrix of doubles with 4 columns !",frameMatrixSig_fieldString);
                  mexErrMsgTxt(errMess);
                }
                int numSel = mxGetM(msig);
                
                double *pd = reinterpret_cast<double*>(mxGetData(msig));
                Easdif::SelectionSet<SdifSignature>  matrixSel;
                for(int isel = 0; isel != numSel; ++isel){
                  sigstr[0]= static_cast<char>(*(pd + 0*numSel + isel)); 
                  sigstr[1]= static_cast<char>(*(pd + 1*numSel + isel)); 
                  sigstr[2]= static_cast<char>(*(pd + 2*numSel + isel)); 
                  sigstr[3]= static_cast<char>(*(pd + 3*numSel + isel)); 
                  matrixSel.insert(SdifStringToSignature(sigstr));    
                }
                p->RestrictMatrixSelection(matrixSel);
              }

              if(streamid){
                if(!mxIsNumeric(streamid) || mxIsComplex(streamid)){
                  itl->second = it_stored;
                  sprintf(errMess,"Fsdif_read_handler :: streamid selection .%s field has to be a real matrix !",frameStreamID_fieldString);
                  mexErrMsgTxt(errMess);
                }

                int numSel = mxGetNumberOfElements(streamid);
                if(numSel>0){
                  std::vector<unsigned int> tempStr(numSel);
                  Easdif::SelectionSet<unsigned int>  streamIDSel;
                  
                  readData(streamid,&tempStr[0]);
                  streamIDSel.insert(tempStr.begin(),tempStr.end());
                  p->RestrictStreamSelection(streamIDSel);
                }                
              }
              if(streamid_range){
                int numSel = mxGetNumberOfElements(streamid_range);
                if(numStructs >1 && numSel  ){
                  itl->second = it_stored;
                  sprintf(errMess,"Fsdif_read_handler :: more than a single selection with streamid range range  is not supported !");
                  mexErrMsgTxt(errMess);
                }
                if(!mxIsNumeric(streamid_range) || mxIsComplex(streamid_range) || numSel != 2){
                  itl->second = it_stored;
                  sprintf(errMess,"Fsdif_read_handler :: streamidrange selection .%s field has to be a real matrix with exactly 2 elements !",frameStreamIDRange_fieldString);
                  mexErrMsgTxt(errMess);
                }

                if(numSel){
                  unsigned int tmpStr[2];
                  readData(streamid_range,tmpStr);
                  if(start_id < tmpStr[0])
                    start_id = tmpStr[0];
                  if(end_id > tmpStr[1])
                    end_id = tmpStr[1];
                }
              }

              if(ftime && ftime_range){
                itl->second = it_stored;
                sprintf(errMess,"Fsdif_read_handler :: cannot handle time and time range selection at the same time !");
                mexErrMsgTxt(errMess);
              }
                
              if(ftime) {
                int numSel = mxGetNumberOfElements(ftime);
                if( !mxIsNumeric(ftime) || !mxIsDouble(ftime)|| mxIsComplex(ftime) ){
                  itl->second = it_stored;
                  sprintf(errMess,"Fsdif_read_handler :: time selection .%s field has to be a real matrix  !",frameTime_fieldString);
                  mexErrMsgTxt(errMess);
                }

                if(numSel>0){     
                  int oldSize = timeSel.size();
                  timeSel.resize(numSel+timeSel.size());
                  readData(ftime,&timeSel[oldSize]); 
                }                
              }
              if(ftime_range) {                
                int numSel = mxGetNumberOfElements(ftime_range);
                if(numSel){
                  if(numStructs >1 && numSel){
                    itl->second = it_stored;
                    sprintf(errMess,"Fsdif_read_handler :: more than a single selection with time range  is not supported !");
                    mexErrMsgTxt(errMess);
                  }
                  if( !mxIsNumeric(ftime_range) || !mxIsDouble(ftime_range)|| mxIsComplex(ftime_range) 
                      || numSel != 2 ){
                    itl->second = it_stored;
                    sprintf(errMess,"Fsdif_read_handler :: time selection .%s field has to be a real matrix with no more than two elements !",frameTimeRange_fieldString);
                    mexErrMsgTxt(errMess);
                  }
                  
                  double *pd = reinterpret_cast<double*>(mxGetData(ftime_range));
                  startTime = *pd;
                  endTime   = *(pd+1);
                }                
              }    
            }
          }
          if(!timeSel.empty()) {
            std::sort(timeSel.begin(),timeSel.end());
            startTime = timeSel.front();
            endTime   = timeSel.back();
          }

          itl->second = itl->first->begin();
          int numFR = 0;
          if(startTime<=endTime){
            if(timeSel.empty()){
              while(itl->second != ite){
                double locTime = itl->second.GetLoc().LocTime();
                unsigned int locStreamID = itl->second.GetLoc().LocStreamID();
                if(start_id <= locStreamID && end_id >=locStreamID&&
                   locTime >=  startTime && locTime <= endTime)
                  ++numFR;
                ++(itl->second);
              }
            }
            else{
              std::sort(timeSel.begin(),timeSel.end());
              std::vector<double>::const_iterator tit   = timeSel.begin();
              std::vector<double>::const_iterator tit_e = timeSel.end();
              while(tit != tit_e && itl->second != ite){
                double locTime = itl->second.GetLoc().LocTime();
                unsigned int locStreamID = itl->second.GetLoc().LocStreamID();
                while(tit != tit_e && *tit <locTime) ++tit;
                if(tit != tit_e && *tit == locTime
                   && start_id <= locStreamID && end_id >=locStreamID)
                  ++numFR;
                ++(itl->second);
              }
            }
          }
          else {
            while(itl->second != ite){
              unsigned int locStreamID = itl->second.GetLoc().LocStreamID();
              if(start_id <= locStreamID && end_id >= locStreamID)
                ++numFR;
              ++(itl->second);
            }
          }
          itl->second = itl->first->begin();

          if(itl->second != ite){
            const char *dfields[5];
            dfields[0] = frameFrameSig_fieldString; 
            dfields[1] = frameStreamID_fieldString; 
            dfields[2] = frameTime_fieldString; 
            dfields[3] = frameMatrixSig_fieldString; 
            dfields[4] = frameData_fieldString; 

            plhs[0] = mxCreateStructMatrix(numFR,1,5,dfields);
            if(startTime<=endTime){
              if(timeSel.empty()){
                for(int ir=0;itl->second != ite && numFR > 0; ++(itl->second)){
                  double locTime = itl->second.GetLoc().LocTime();
                  unsigned int locStreamID = itl->second.GetLoc().LocStreamID();
                  if(locTime >=  startTime && locTime <= endTime
                     && start_id <= locStreamID && end_id >=locStreamID){
                    createFrame(p, itl->second, ir, plhs[0],5, dfields, classid);
                    ++ir;
                    --numFR;
                  }
                }
              }
              else{
                std::vector<double>::const_iterator tit   = timeSel.begin();
                std::vector<double>::const_iterator tit_e = timeSel.end();

                for(int ir=0;tit!=tit_e && itl->second != ite && numFR > 0; ++(itl->second)){
                  double locTime        = itl->second.GetLoc().LocTime();
                  unsigned int locStreamID = itl->second.GetLoc().LocStreamID();
                  while(tit != tit_e && *tit <locTime) ++tit;
                  if(tit != tit_e && *tit == locTime
                     && start_id <= locStreamID && end_id >=locStreamID){
                    createFrame(p, itl->second, ir, plhs[0],5, dfields, classid);
                    ++ir;
                    --numFR;
                  }                    
                }
              }
            }
            // no time selection
            else{
              for(int ir=0;itl->second != ite  && numFR > 0; ++(itl->second),++ir){
                unsigned int locStreamID = itl->second.GetLoc().LocStreamID();
                if ( start_id <= locStreamID && end_id >=locStreamID) {
                  createFrame(p, itl->second, ir, plhs[0],5, dfields, classid);
                  --numFR;
                }
              }
            }
          }
          else {
            plhs[0] = mxCreateNumericMatrix(0,0,
                                            classid == mxUNKNOWN_CLASS ? mxDOUBLE_CLASS : classid,
                                            mxREAL);
          }
         itl->second = it_stored;
        }
      }
      else
        mexErrMsgTxt("Fsdif_read_handler::invalid file pointer");      
    }
    else {
      mexErrMsgTxt("Fsdif_read_handler::can only filepointer argument types int32/int64/uint32/uint64!");
    }
  }
  // load complete file in flat non sructured format
  else  if(!strcmp(command,"loadfile")){
    
    if(nrhs < 2 || nrhs > 3)
      mexErrMsgTxt ("Fsdif_read_handler::Wrong number of input arguments, need to have filname and eventually a output data type specification");

    if(nlhs > 5)
      mexErrMsgTxt ("Fsdif_read_handler::Wrong number of output arguments (at most 4 )");

    mxClassID classid = mxUNKNOWN_CLASS;
    if(nrhs == 3) {
      classid = getClassFromString(prhs[2]);
    }

    if(mxIsInt32(prhs[1]) || mxIsInt64(prhs[1]) || mxIsUint32(prhs[1]) || mxIsUint64(prhs[1])){
      Easdif::SDIFEntity *p = *reinterpret_cast<Easdif::SDIFEntity **>(mxGetData(prhs[1]));    
      if(p==0 && !pList.empty())
        p = pList.front().first;      

      EASDList::iterator itl;

      // validate pointer
      if(CheckList(p,itl) && p->IsOpen()){
        Easdif::SDIFEntity::const_iterator ite = p->end();               
        
        itl->second = itl->first->begin();
        int numMat= 0;
        while(itl->second != ite){
          numMat += itl->second.GetLoc().LocNbMatrix();
          ++(itl->second);
        }
        itl->second = itl->first->begin();
        
        mxArray * mxData = 0;
        mxArray * mxHead = 0;
        mxArray * mxFrame = 0;
        mxArray * mxMatrix = 0;
        mxArray * mxDims  = 0;
        mxData = plhs[0] = mxCreateCellMatrix(numMat,1);
        if(nlhs > 1 )
          mxHead = plhs[1] = mxCreateNumericMatrix(numMat,2,mxDOUBLE_CLASS,mxREAL);
        if(nlhs > 2)
          mxFrame = plhs[2] = mxCreateNumericMatrix(numMat,4,mxDOUBLE_CLASS,mxREAL);
        if(nlhs > 3 )
          mxMatrix = plhs[3] = mxCreateNumericMatrix(numMat,4,mxDOUBLE_CLASS,mxREAL);
        if(nlhs > 4 )
          mxDims = plhs[4] = mxCreateNumericMatrix(numMat,2,mxDOUBLE_CLASS,mxREAL);
        
        for(int iMat=0; itl->second != ite; ++(itl->second)){
          createMData(itl->second, iMat, mxData, classid, mxHead, mxFrame, mxMatrix, mxDims);
        }
      }
      else
        mexErrMsgTxt("Fsdif_read_handler::invalid file pointer");      
    }
    else {
      mexErrMsgTxt("Fsdif_read_handler::can only filepointer argument types int32/int64/uint32/uint64!");
    }
  }
  else if(!strcmp(command,"version")){
    mexPrintf("Fsdif_read_handler: Version %s\n",MAKE_STRING_FROM_MACRO(EASDIF_MATLAB_VERSION));
    mexPrintf("Easdif Version %s\n", EASDIF_VERSION_STRING);
  }  
  else if(!strcmp(command,"help")){
    mexPrintf("Fsdif_read_handler: Version %s\n",MAKE_STRING_FROM_MACRO(EASDIF_MATLAB_VERSION));
    mexPrintf("issdif    : check sdiffile\n");
    mexPrintf("open      : open file \n");
    mexPrintf("read      : read from file \n");
    mexPrintf("close     : close file\n");
    mexPrintf("clear     : close all files and release memory\n");
    mexPrintf("rewind    : rewind file\n");
    mexPrintf("loadfile  : load complete file\n");
    mexPrintf("version   : display version number\n");
    mexPrintf("help      : this short help\n");
  }
  else{  
    mexErrMsgTxt ("Fsdif_read_handler:: unknown command");
  }
  return;
}
