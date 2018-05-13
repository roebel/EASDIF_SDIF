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


/**
 * @file   sdifexception.h
 * @author Axel Roebel
 * @date   Wed May 21 22:03:04 2003
 * 
 * @brief   SDIFException - SDIF Error handling via exceptions
 * 
 * 
 * 
 * $Id: sdifexception.hpp,v 1.7 2011-06-11 17:05:31 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.6  2008/07/28 16:19:57  roebel
 * Added all derived exceptions to the public API
 * of the library.
 *
 * Revision 1.5  2008/03/07 19:20:28  roebel
 * consistently define destructor of derived classes
 *
 * Revision 1.4  2008/01/11 15:58:59  roebel
 * Added new class MatrixType, FrameType, and the realted function for
 * adding types to entity or retrieving types from an entity in terms of these
 * clases.
 *
 * Revision 1.3  2007/11/26 19:09:38  roebel
 * Fixed to avoid compiler warnings in MSVC.
 * Little problem is the export of std::containers that should be defined as export
 * which is not possible due to the given STL include files.
 * For the moment it seems these warnings are not important, because all these functions
 * are inlined as templates in the STL anyway. Has to be handled with care !!!!
 *
 * Revision 1.2  2007/04/30 11:34:26  roebel
 * Added new exception to signal frame directory errors.
 *
 * Revision 1.1  2005/05/30 21:43:01  roebel
 * Changed all include files from .h into .hpp to prevent name clash between
 * sdifmatix.h and SDIF/sdifcpp/SdifMatrix.h on MacOSX where filenames are
 * case insensitive.
 *
 * Revision 1.12  2005/05/24 09:55:03  roebel
 * modified for new interface to SdifErrorFunction handlers.
 *
 * Revision 1.11  2005/05/20 21:32:20  roebel
 * New Seek error exception
 *
 * Revision 1.10  2004/09/09 19:17:38  roebel
 * Version 1.0.0beta:
 * First complete version of iterator access when reading files. Frame-Iterators use the
 * internal Frame Directory that each Entity will generate and update on the fly
 * to minimize disk access during positioning.
 *
 * Revision 1.9  2004/07/27 17:39:31  roebel
 * Changed include directive to use user path and not system path for sdif.h
 * Changed what to be of type const throw() to properly replace the underlying
 * method of the base class.
 *
 * Revision 1.8  2004/07/21 13:20:28  roebel
 * Added support to hold a frameDirectory in the entity and
 * the possibility to read frames from a given time position.
 *
 * Revision 1.7  2003/12/18 12:15:29  roebel
 * Renamed method arguments to prevent shadowing of class members
 *
 * Revision 1.6  2003/12/05 13:53:14  ellis
 *
 * including <iostream> for the use of std::cout, cerr...
 *
 * Revision 1.5  2003/11/25 10:57:05  roebel
 * Added missing namespace qualifier for exceptions.
 * Added missing exception specification to exception deconstructor.
 *
 * Revision 1.4  2003/11/18 18:18:40  roebel
 * Reorganized exception handling and interface such that
 * sdifexcpetions are now derived from standard exception and
 * sdif error tags and error numbers are treated equivalently.
 * Improved documentation.
 *
 * Revision 1.3  2003/05/21 20:40:30  roebel
 * Added documentation. Removed unused member "error".
 * Added member functions to access message and errortag.
 * 
 * 
 */


#ifndef SDIFEXCEPTION_H_
#define SDIFEXCEPTION_H_ 1

#include <iostream>
#include <string>
#include <stdexcept>
#include "sdif.h"

#include "easdif_exports.hpp"

namespace Easdif {

  /*************************************************************************/
  /* Exception Handling  */

  /**
   * \defgroup exception SDIFException - SDIF Error handling via exceptions
   *
   *
   */

  /** 
   * \ingroup exception 
   * \brief SDIFException - Basic exception from which all other Easdif exceptions derive 
   * 
   *
   * In Easdif all SDIF errors are mapped to corresponding exceptions
   * the base class is SDIFException. All other exception 
   * classes are derived from here.
   *
   */
  class EASDIF_API SDIFException : public std::exception
  {
  public:    

    /** 
     * \ingroup exception 
     *  \brief Initialize exception
     *
     *  
     */
    SDIFException(SdifErrorLevelET level,
		  const char* message,
		  SdifFileT* _sdifFile,	
		  int error, // either SdifErrorE or SdifErrorTagE
		  const char* sourceFileName,
		  int sourceFileLine);

    ~SDIFException() throw() ;

    /** 
     * \ingroup exception 
     *  \brief return message
     *
     *  The member function what returns the error message
     */
    const char *what() const throw() {      
      return mMessage.c_str();
    }


    /** 
     * \ingroup exception 
     *  \brief pretty print error message
     *
     *  The member function ErrorMessage pretty-prints the SDIF error message
     *  via  cerr.
     */
    void ErrorMessage()
    {

      std::cerr << "Exception: " << what() << std::endl;
	    
      if (mSourceFileName != "")
        std::cerr << "Source file : "
                  << mSourceFileName 
                  << "\nat line : "
                  << mSourceFileLine
                  << std::endl;
    }

    
    
    /** 
     * \ingroup exception 
     *
     * \brief get sdiffile pointer for file that causec exception
     * 
     * @return  sdiffile pointer
     */
    const SdifFileT* sdifFile() const { return mSdifFile;}


    /** 
     * \ingroup exception 
     * \brief get  error message
     * 
     * @return returns mMessage string
     */

    const std::string& getmessage() const { return mMessage;}


    /** 
     * \ingroup exception 
     * \brief get  source file name that emitted error
     * 
     * @return returns Source File name as a string
     */
    const std::string& getsourcename() const { return mSourceFileName;}

    /** 
     * \ingroup exception 
     * \brief get SDIF error tag / SdifErrorNumber
     * 
     * @return error number
     */
    int getenum() const { 
      return mError;
    }
   
  protected:
    int mSourceFileLine;
    std::string mSourceFileName;
    std::string mMessage;
    SdifFileT* mSdifFile;
    int  mError;

    SdifErrorLevelET mLevel;

  };


#define constructor(class1,class2) class1(SdifErrorLevelET level,  const char* message, \
					 SdifFileT* _sdifFile,   int error, \
                                          const char* sourceFileName, int sourceFileLine); \
  ~class1() throw();

  /****************** FILE ERRORS  *************************/

  /// \ingroup exception 
  /// \brief SDIFFileError  error during file operations
  class EASDIF_API SDIFFileError : public SDIFException
  {
  public:
    constructor(SDIFFileError,SDIFException)
      };

  /// \ingroup exception 
  /// \brief SDIFOpeningError error when opening a file 
  class EASDIF_API SDIFOpeningError : public SDIFFileError
  {
  public:
    constructor(SDIFOpeningError,SDIFFileError)
      };

  /// \ingroup exception 
  /// \brief ReadWriteOnSameFile
  class EASDIF_API SDIFReadWriteOnSameFile : public SDIFOpeningError
  {
  public:
    constructor(SDIFReadWriteOnSameFile,SDIFOpeningError)
      };

  /// \ingroup exception 
  /// \brief SDIFBadStdFile
  class EASDIF_API SDIFBadStdFile : public SDIFOpeningError
  {
  public:
    constructor(SDIFBadStdFile,SDIFOpeningError)
      };

  /// \ingroup exception 
  /// \brief SDIFBadMode bad mode when opening file
  class EASDIF_API SDIFBadMode  : public SDIFOpeningError
  {
  public:
    constructor(SDIFBadMode,SDIFOpeningError)
      };

  /// \ingroup exception 
  /// \brief SDIFHeaderError Frame/Matrix/File Header Errors 
  class EASDIF_API SDIFHeaderError : public SDIFFileError
  {
  public:
    constructor(SDIFHeaderError,SDIFFileError)
      };

  /// \ingroup exception 
  /// \brief SDIFFrameHeaderSizeError 
  class EASDIF_API SDIFFrameHeaderSizeError : public SDIFFileError
  {
  public:
    constructor(SDIFFrameHeaderSizeError,SDIFFileError)
      };

  /// \ingroup exception 
  /// \brief SDIFDirError: Indicates error when creating the SDIF Frame directory
  class EASDIF_API SDIFDirError : public SDIFFileError
  {
  public:
    constructor(SDIFDirError,SDIFFileError)
      };

  /**
   * \brief SDIFNoSeekError: Indicates error while seeking
   * \ingroup exception
   *  may happen either because given position cannot be reached or because te file  is a pipe
   */
  class EASDIF_API SDIFSeekError : public SDIFFileError
  {
  public:
    constructor(SDIFSeekError,SDIFFileError)
      };


  /// \ingroup exception 
  /// \brief SDIFBadFormatVersion
  class EASDIF_API SDIFBadFormatVersion : public SDIFHeaderError
  {
  public:
    constructor(SDIFBadFormatVersion,SDIFHeaderError)
      };

  /// \ingroup exception 
  /// \brief SDIFBadHeader
  class EASDIF_API SDIFBadHeader : public SDIFHeaderError
  {
  public:
    constructor(SDIFBadHeader,SDIFHeaderError)
      };

  /// \ingroup exception 
  /// \brief ASCII Chunk Errors 
  class EASDIF_API SDIFAsciiChunkError : public SDIFFileError
  {
  public:
    constructor(SDIFAsciiChunkError,SDIFFileError)
      };

  /// \ingroup exception 
  /// \brief SDIFReDefined
  class EASDIF_API SDIFReDefined : public SDIFAsciiChunkError
  {
  public:
    constructor(SDIFReDefined,SDIFAsciiChunkError)
      };

  /// \ingroup exception 
  /// \brief SDIFUnDefined
  class EASDIF_API SDIFUnDefined  : public SDIFAsciiChunkError
  {
  public:
    constructor(SDIFUnDefined,SDIFAsciiChunkError)
      };

  /// \ingroup exception 
  /// \brief SDIFReading error during read
  class EASDIF_API SDIFReading : public SDIFFileError
  {
  public:
    constructor(SDIFReading,SDIFFileError)
      };

  /// \ingroup exception 
  /// \brief SDIFEof: eof of sdif file reached
  class EASDIF_API SDIFEof : public SDIFReading
  {
  public:
    constructor(SDIFEof,SDIFReading)
      };

  /****************** DESCRIPTION TYPE CHECKING ************/

  /// \ingroup exception 
  /// \brief SDIFDescriptionTypeError 
  class EASDIF_API SDIFDescriptionTypeError : public SDIFException{
  public:
    constructor(SDIFDescriptionTypeError,SDIFException)
      };


  /****************** MATRIX DATA ELEMENT TYPE CHECKING ****/
  /// \ingroup exception 
  /// \brief SDIFMatrixDataError
  class EASDIF_API SDIFMatrixDataError : public SDIFException{
  public:
    constructor(SDIFMatrixDataError,SDIFException)
      };

  /// \ingroup exception 
  /// \brief SDIFNotInDataTypeUnion
  class EASDIF_API SDIFNotInDataTypeUnion  : public SDIFMatrixDataError
  {
  public:
    constructor(SDIFNotInDataTypeUnion,SDIFMatrixDataError)
      };

  /// \ingroup exception 
  /// \brief SDIFArrayPosition
  class EASDIF_API SDIFArrayPosition  : public SDIFMatrixDataError
  {
  public:
    constructor(SDIFArrayPosition,SDIFMatrixDataError)
      };

  /****************** MEMORY MANAGEMENT ********************/
  /// \ingroup exception 
  /// \brief SDIFMemoryError memory problems
  class EASDIF_API SDIFMemoryError : public SDIFException{
  public:
    constructor(SDIFMemoryError,SDIFException)
      };

  /// \ingroup exception 
  /// \brief SDIFFreeNull  mem pointer is zero
  class EASDIF_API SDIFFreeNull : public SDIFMemoryError
  {
  public:
    constructor(SDIFFreeNull,SDIFMemoryError)
      };

  /// \ingroup exception  
  /// \brief SDIFAllocFail memory allocation failed
  class EASDIF_API SDIFAllocFail : public SDIFMemoryError
  {
  public:
    constructor(SDIFAllocFail,SDIFMemoryError)
      };

  /****************** SDIF-Text Reading ********************/
  /// \ingroup exception  
  /// \brief SDIFTextReadingError  error reading text section
  class EASDIF_API SDIFTextReadingError : public SDIFException{
  public:
    constructor(SDIFTextReadingError,SDIFException)
      };

  /// \ingroup exception    
  /// \brief SDIFTokenLength
  class EASDIF_API SDIFTokenLength : public SDIFTextReadingError
  {
  public:
    constructor(SDIFTokenLength,SDIFTextReadingError)
      };

  /****************** Frame Contents ****/
  /// \ingroup exception    
  /// \brief SDIFMatrixNotAvailable requested matrix not in frame
  class EASDIF_API SDIFMatrixNotAvailable : public SDIFException{  
  public:
    constructor(SDIFMatrixNotAvailable,SDIFException)
      };

  /// \brief Error in frame directory 
  class EASDIF_API FrameDirError : public SDIFException{  
  public:
    constructor(FrameDirError,SDIFException)
      };

  /// \brief Error in construction or handling of MatrixTypes and FrameTypes 
  class EASDIF_API TypeError : public SDIFException{  
  public:
    constructor(TypeError,SDIFException)
      };


  extern "C"
  {
    EASDIF_API
    void
    ExceptionThrower(SdifErrorTagET errnum, 
                     SdifErrorLevelET errlev, 
                     char* msg, 
                     SdifFileT* file, 
                     SdifErrorT* error, 
                     char* sourcefilename, 
                     int sourcefileline) throw(SDIFException);

  }
} // end of namespace Easdif
#endif
