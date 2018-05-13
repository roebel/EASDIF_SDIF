/**
 * @file   Fsdif_access_all_matrix.cpp
 * @author Axel Roebel
 * @date   Tue Jan 22 21:33:35 2008
 * 
 * @brief  simplify access to common matrix in frame data
 * 
 * 
 *
 * $Revision: 1.1 $   last changed on $Date: 2008-01-23 12:12:01 $
 *
 *                                    Copyright (c) 2008 by IRCAM
 *  
 */


#include <mex.h>
#include <cstdlib>
#include <stdint.h>

#include "fieldnames.hpp"

char errMess[512] = {0};

template<class REAL>
inline
void
copyElements(REAL* pout,int offout,
             REAL* pin, int N) {
  
  REAL *pine = pine + N;
  while(pin != pine){
    *pout = *pin++;
    pout += offout;
  }
}

void 
mexFunction (int nlhs, mxArray *plhs [], int nrhs, const mxArray *prhs [])
{

  if( nrhs  != nlhs +1)
    mexErrMsgTxt ("Fsdif_access_all_matrix:: number of input arguments have to match number of output arguments plus 1");

  for(int ii=1;ii!=nrhs;++ii){
    if(!mxIsChar(prhs[ii]) ){
      mexErrMsgTxt ("Fsdif_access_all_matrix:: all but the first arguments have to be strings indicating field names in the struct given by the first argument");   
    }
  }
  
  int numEl     = mxGetNumberOfElements(prhs[0]);
  int dataField = mxGetFieldNumber(prhs[0],frameData_fieldString);
  if(dataField <= 0){
    sprintf(errMess,"no %s field present in struct \n",frameData_fieldString);
    mexErrMsgTxt("no data field present in struct \n");
  }
  int maxStoreLen= 512; 
  char *store = malloc(maxStoreLen);
  for( ii=1;ii!=nrhs;++ii){
    
    if(mxGetNumberOfElements(prhs[ii]) + 1 > maxStoreLen){
      maxStoreLen= mxGetNumberOfElements(mxGetNumberOfElements(prhs[ii])) + 1;
      store = reinterpret_cast<char*>(realloc(store, maxStoreLen));
    }
    if(mxGetString(prhs[ii],store,maxStoreLen) )
      mexErrMsgTxt("Fsdif_access_all_matrix :: Failed to store NVT Name");
    
    int maxM = 0;
    int maxN = 0;
    int nTarget = 0;
    int targetID=  mxUNKNOWN_CLASS;
    for(int iel = 0; iel != numEl; ++iel) {
      mxArray *pData = mxGetFieldByNumber(prhs[0],iel,dataField);
      int targetField = mxGetFieldNumber(pData,store);
      if(targetField>=0){        
        mxArray pTarget = mxGetFieldByNumber(pData,0,targetField);
        if(targetID == mxUNKNOWN_CLASS)
          targetID = mxGetClassID(pTarget);
        else
          if (targetID != mxGetClassID(pTarget))
            mexErrMsgTxt("Don't support accessing matrices with varying class type");

        maxM = std::max(maxM, mxGetM(pTarget));
        maxN = std::max(maxN, mxGetN(pTarget));
        ++nTarget;
      }
    }
    if(std::min(maxM,maxN) == 1) {
      plhs[ii-1] = mxCreateNumericMatrix(maxM==1?nTarget:maxM,maxM==1?maxN:nTarget,targetID,mxREAL);      
      void *pout = mxGetData(plhs[ii-1]);
      int outouf = 1;
      if(maxM == 1) 
        outoff = numEl;

      for(int iel=0;iel !=numEl; ++iel) {
        mxArray *pData = mxGetFieldByNumber(prhs[0],iel,dataField);
        int targetField = mxGetFieldNumber(pData,store);
        if(targetField>=0){        
          mxArray pTarget = mxGetFieldByNumber(pData,0,targetField);
          N = mxGetN(pTarget);
          switch (targetID){
          case mxDOUBLE_CLASS:
            copyElements(reinterpret_cast<double*>(pout)+iel,numEl,
                         reinterpret_cast<double*>(mxGetData(pTarget)),N);
            break;
          case mxSINGLE_CLASS:
            copyElements(reinterpret_cast<float*>(pout)+iel,numEl,
                         reinterpret_cast<float*>(mxGetData(pTarget)),N);
            break;
          case mxINT32_CLASS:
            copyElements(reinterpret_cast<int32_t*>(pout)+iel,numEl,
                         reinterpret_cast<int32_t*>(mxGetData(pTarget)),N);
            break;
          case mxUINT32_CLASS:
            copyElements(reinterpret_cast<uint32_t*>(pout)+iel,numEl,
                         reinterpret_cast<uint32_t*>(mxGetData(pTarget)),N);
            break;
          default:              
          }
          
        }
      }
    }
    else if(std::min(maxM,maxN) > 1) {
      int dims[3];
      dims[0] = maxM;
      dims[1] = maxN;
      dims[2] = numEl;
      
      plhs[ii-1] = mxCreateNumericArray(3,dims,targetID,mxREAL);      
      void *pout = mxGetData(plhs[ii-1]);
      int outouf = 1;
      if(maxM == 1) 
        outoff = numEl;

      for(int iel=0;iel !=numEl; ++iel) {
        mxArray *pData = mxGetFieldByNumber(prhs[0],iel,dataField);
        int targetField = mxGetFieldNumber(pData,store);
        if(targetField>=0){        
          mxArray pTarget = mxGetFieldByNumber(pData,0,targetField);
          N = mxGetN(pTarget);
          switch (targetID){
          case mxDOUBLE_CLASS:
            copyMatrix(reinterpret_cast<double*>(pout)+iel,numEl,
                       reinterpret_cast<double*>(mxGetData(pTarget)),N);
            break;
          case mxSINGLE_CLASS:
            copyMatrix(reinterpret_cast<float*>(pout)+iel,numEl,
                       reinterpret_cast<float*>(mxGetData(pTarget)),N);
            break;
          case mxINT32_CLASS:
            copyMatrix(reinterpret_cast<int32_t*>(pout)+iel,numEl,
                       reinterpret_cast<int32_t*>(mxGetData(pTarget)),N);
            break;
          case mxUINT32_CLASS:
            copyMatrix(reinterpret_cast<uint32_t*>(pout)+iel,numEl,
                       reinterpret_cast<uint32_t*>(mxGetData(pTarget)),N);
            break;
          default:              
          }
          
        }
      }

    }
    else{
      plhs[ii-1] = mxCreateNumericMatrix(0,0,mxDOUBLE_CLASS,mxREAL);  
    }
  }
}
