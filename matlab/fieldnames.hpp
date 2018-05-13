/**
 * @file   fieldnames.hpp
 * @author Axel Roebel
 * @date   Mon Jan 21 23:43:44 2008
 * 
 * @brief  field names for matlab structs and accessor functions for variably types mxArrays
 * 
 * 
 */

#ifndef FIELDNAMES_HPP
#define FIELDNAMES_HPP

  // cast and copy data 
  template<class REALIN, class REAL>
  inline
  void copyData(const void *p, REAL* d ,int n) {
    const REALIN *pd  = reinterpret_cast<const REALIN *>(p);
    const REALIN *pde = pd+n;
    while(pd!=pde)
      *d++ = static_cast<REAL>(*pd++);
  }
  
  
  // transfer data from a matrix array int a memory array
  template<class DATA>
  inline
  DATA* readData(const mxArray* toRead, DATA*d) {
    
    int n = mxGetNumberOfElements(toRead);
    if(mxIsDouble(toRead)){
      copyData<double,DATA>(mxGetData(toRead),d,n);
    }
    else if(mxIsSingle(toRead)){
      copyData<float,DATA>(mxGetData(toRead),d,n);
    }
    else if(mxIsUint8(toRead)){
      copyData<SdifUInt1,DATA>(mxGetData(toRead),d,n);
    }
    else if(mxIsInt8(toRead)){
      copyData<SdifInt1,DATA>(mxGetData(toRead),d,n);
    }
    else if(mxIsUint16(toRead)){
      copyData<SdifUInt2,DATA>(mxGetData(toRead),d,n);
    }
    else if(mxIsInt16(toRead)){
      copyData<SdifInt2,DATA>(mxGetData(toRead),d,n);
    }
    else if(mxIsUint32(toRead)){
      copyData<SdifUInt4,DATA>(mxGetData(toRead),d,n);
    }
    else if(mxIsInt32(toRead)){
      copyData<SdifInt4,DATA>(mxGetData(toRead),d,n);
    }
    else {
      mexErrMsgTxt("Fsdif_write_handler :: cannot write frame, unsupported data type");
    }
    return d;
  }

  // frame description struct
  const char *frameFrameSig_fieldString  = "fsig";
  const char *frameStreamID_fieldString  = "stream";
  const char *frameTime_fieldString      = "time";
  const char *frameMatrixSig_fieldString = "msig";
  const char *frameData_fieldString      = "data";

  // only for selection 
  const char *frameStreamIDRange_fieldString  = "streamRange";
  const char *frameTimeRange_fieldString      = "timeRange";
  
  // header description struct
  const char *headNVT_fieldString =  "NVT";
  const char *headTYP_fieldString =  "TYP";
  const char *headIDS_fieldString =  "IDS";
  
  // TYPE  sub struct
  const char *headMatrixTypeD_fieldString =  "MTD";
  const char *headFrameTypeD_fieldString  =  "FTD";

  // Matrix type definitions
  const char* mtdMatrixSig_fieldString    = "msig"; 
  const char* mtdColName_fieldString      = "cname"; 

  // Frame type definitions
  const char* ftdFrameSig_fieldString     = "fsig"; 
  const char* ftdMatrixSig_fieldString    = "msig";
  const char* ftdMatrixName_fieldString   = "mname"; 

#endif
