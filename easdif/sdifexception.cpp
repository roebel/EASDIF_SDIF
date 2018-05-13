/*
 * This file is part of the IRCAM EASDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 2002-2003 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * See file COPYING for further informations on licensing terms.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *
 * The EASDIF library provides an Easy Api for IRCAMs SDIF library. 
 *
 */


#include <iostream>
#include "easdif/easdif_config.hpp"
#include "easdif/sdifexception.hpp"

namespace Easdif {

  SDIFException::SDIFException(SdifErrorLevelET level,
                               const char* message,
                               SdifFileT* _sdifFile,	
                               int error, // either SdifErrorE or SdifErrorTagE
                               const char* sourceFileName,
                               int sourceFileLine) {
    
    mSourceFileLine = sourceFileLine;
    if(sourceFileName)
      mSourceFileName = std::string(sourceFileName);
    else
      mSourceFileName = std::string("");
    
    if(message)
      mMessage = std::string(message);
    else
      mMessage = std::string("");
    
    mLevel = level;
    mSdifFile = _sdifFile;
    mError = error;
  };

  SDIFException::~SDIFException() throw() {	
  };

#define constructor_impl(class1,class2) class1::class1(SdifErrorLevelET level,  const char* message, \
                                                       SdifFileT* _sdifFile,   int error, \
                                                       const char* sourceFileName, int sourceFileLine) \
    : class2(level,message,_sdifFile,error,sourceFileName,sourceFileLine){}

  constructor_impl(SDIFFileError,SDIFException)
  constructor_impl(SDIFOpeningError,SDIFFileError)
  constructor_impl(SDIFReadWriteOnSameFile,SDIFOpeningError)
  constructor_impl(SDIFBadStdFile,SDIFOpeningError)
  constructor_impl(SDIFBadMode,SDIFOpeningError)
  constructor_impl(SDIFHeaderError,SDIFFileError)
  constructor_impl(SDIFFrameHeaderSizeError,SDIFFileError)
  constructor_impl(SDIFDirError,SDIFFileError)
  constructor_impl(SDIFSeekError,SDIFFileError)
  constructor_impl(SDIFBadFormatVersion,SDIFHeaderError)
  constructor_impl(SDIFBadHeader,SDIFHeaderError)
  constructor_impl(SDIFAsciiChunkError,SDIFFileError)
  constructor_impl(SDIFReDefined,SDIFAsciiChunkError)
  constructor_impl(SDIFUnDefined,SDIFAsciiChunkError)
  constructor_impl(SDIFReading,SDIFFileError)
  constructor_impl(SDIFEof,SDIFReading)
  constructor_impl(SDIFDescriptionTypeError,SDIFException)
  constructor_impl(SDIFMatrixDataError,SDIFException)
  constructor_impl(SDIFNotInDataTypeUnion,SDIFMatrixDataError)
  constructor_impl(SDIFArrayPosition,SDIFMatrixDataError)
  constructor_impl(SDIFMemoryError,SDIFException)
  constructor_impl(SDIFFreeNull,SDIFMemoryError)
  constructor_impl(SDIFAllocFail,SDIFMemoryError)
  constructor_impl(SDIFTextReadingError,SDIFException)
  constructor_impl(SDIFMatrixNotAvailable,SDIFException)
  constructor_impl(SDIFTokenLength,SDIFTextReadingError)
  constructor_impl(FrameDirError,SDIFException)
  constructor_impl(TypeError,SDIFException)
  

#define destructor(class1) class1::~class1() throw() {}

  destructor(SDIFFileError)
  destructor(SDIFOpeningError)
  destructor(SDIFReadWriteOnSameFile)
  destructor(SDIFBadStdFile)
  destructor(SDIFBadMode)
  destructor(SDIFHeaderError)
  destructor(SDIFFrameHeaderSizeError)
  destructor(SDIFDirError)
  destructor(SDIFSeekError)
  destructor(SDIFBadFormatVersion)
  destructor(SDIFBadHeader)
  destructor(SDIFAsciiChunkError)
  destructor(SDIFReDefined)
  destructor(SDIFUnDefined)
  destructor(SDIFReading)
  destructor(SDIFEof)
  destructor(SDIFDescriptionTypeError)
  destructor(SDIFMatrixDataError)
  destructor(SDIFNotInDataTypeUnion)
  destructor(SDIFArrayPosition)
  destructor(SDIFMemoryError)
  destructor(SDIFFreeNull)
  destructor(SDIFAllocFail)
  destructor(SDIFTextReadingError)
  destructor(SDIFTokenLength)
  destructor(SDIFMatrixNotAvailable)
  destructor(FrameDirError)
  destructor(TypeError)

  extern "C" {
    
    void
    ExceptionThrower(SdifErrorTagET errnum, 
		     SdifErrorLevelET errlev, 
		     char* msg, 
		     SdifFileT* file, 
		     SdifErrorT* error, 
		     char* sourcefilename, 
		     int sourcefileline) throw(SDIFException)
    {   
      switch(errnum)
	{
	  /* the SdifError.c errors :*/
	case eFreeNull:
	  throw SDIFFreeNull(errlev, msg, file, errnum, 
			     sourcefilename, sourcefileline);
	  break;
	  
	case eAllocFail :
	  throw SDIFAllocFail(errlev, msg, file, errnum, 
			      sourcefilename, sourcefileline);
	  break;
	  
	case eArrayPosition :
	  throw SDIFArrayPosition(errlev, msg, file, errnum, 
				  sourcefilename, sourcefileline);
	  break;
	  
	case  eEof :	
	  throw SDIFEof(errlev, msg, file, errnum, 
			sourcefilename, sourcefileline);
	  break;

	case  eNotInDataTypeUnion :	
	  throw SDIFNotInDataTypeUnion(errlev, msg, file, errnum, 
				       sourcefilename, sourcefileline);
	  break;      

	case  eTokenLength :	
	  throw SDIFTokenLength(errlev, msg, file, errnum, 
				sourcefilename, sourcefileline);
	  break;

	/* the SdifErrMess.c errors */
	case eReDefined :
	  throw SDIFReDefined(errlev, msg, file, errnum, 
			      sourcefilename, sourcefileline);
	  break;

	case eUnDefined :
	  throw SDIFUnDefined(errlev, msg, file, errnum, 
			      sourcefilename, sourcefileline);
	  break;
	  
	case eBadHeader :
	  throw SDIFBadHeader(errlev, msg, file, errnum, 
			      sourcefilename, sourcefileline);
	  break;

	case eBadMode :
	  throw SDIFBadMode(errlev, msg, file, errnum, 
			    sourcefilename, sourcefileline);
	  break;

	case eBadStdFile :
	  throw SDIFBadStdFile(errlev, msg, file, errnum, 
			       sourcefilename, sourcefileline);
	  break;

	case eReadWriteOnSameFile :
	  throw SDIFReadWriteOnSameFile(errlev, msg, file, errnum, 
					sourcefilename, sourcefileline);
	  break;

	case eBadFormatVersion :
	  throw SDIFBadFormatVersion(errlev, msg, file, errnum, 
				     sourcefilename, sourcefileline);
	  break;

	default :	/*a revoir*/
	  throw SDIFException(errlev, msg, file, errnum, 
			      sourcefilename, sourcefileline);
	  break;
	}
    }

}
} // end of namespace Easdif
