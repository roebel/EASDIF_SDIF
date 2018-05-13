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
 * @file   sdif_frame.cpp
 * @author Fabien Tisserand
 * @date   Thu Apr 11 18:44:52 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdif_frame.cpp,v 1.7 2014-11-18 13:56:12 fcornu-ircam Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.6  2014/09/26 17:46:16  roebel
 * fixed reading empty frames if FrameDireectory is active
 *
 * Revision 1.5  2014/06/06 15:30:54  roebel
 * Clarified documentation or Entity::ReadNextFrame, Entity::ReadNextSelectedFrame and fixed implementation such that it works for frames not containing any matrices being located at the end of the SDIF file.
 * This fixes a bug notably in the python Entity iterator that would not retriev empty frames located at the end of an SDIF file.
 *
 * Revision 1.4  2008/06/20 17:07:02  roebel
 * Changed Print methods to be const.
 *
 * Revision 1.3  2007/04/30 11:33:17  roebel
 * Frame reading methods return the number of bytes read for all selected matrices.
 * Accordingly the return value can be used to decide whether anything was selected in
 * the frame.
 *
 * Revision 1.2  2006/04/22 11:48:09  roebel
 * Fixed left over problems from last renameing operation.
 *
 * Revision 1.1  2006/04/22 08:57:24  roebel
 * Renamed some files to prevent name clash of object files on macosx
 *
 * Revision 1.22  2006/02/22 14:18:06  roebel
 * Fixed SetHeader time argument from float to SdifFloat8.
 *
 * Revision 1.21  2005/12/19 11:46:11  roebel
 * Fixed argument of SetTime function to be SdifFloat8
 * instead of float!
 *
 * Revision 1.20  2005/05/30 21:43:01  roebel
 * Changed all include files from .h into .hpp to prevent name clash between
 * sdifmatix.h and SDIF/sdifcpp/SdifMatrix.h on MacOSX where filenames are
 * case insensitive.
 *
 * Revision 1.19  2005/05/30 19:32:34  bogaards
 * check whether frame is high level selected is it is already in the framedir
 *
 * Revision 1.18  2005/05/30 18:16:58  bogaards
 * return zero if frame is not selected
 *
 * Revision 1.17  2005/05/24 09:53:51  roebel
 * Changed selection management in Easdif:
 * Before EnableDirectory has been called selection
 * modification wqorks on the SDIF-Selection, after
 * enabling the directory a new high level selection mode
 * is used that can only be used to restrict the secltion by
 * forming intersections with the existing selections.
 * This mechanism exists now for stream/frame and matrix selections.
 *
 * Revision 1.16  2005/05/20 21:32:27  roebel
 * Removed remaining functions that were only there for initial tests of
 * Fabien Tisserand.
 * Changed the frame directory handling to include only the selected frames and
 * matrices.
 *
 * Revision 1.15  2004/09/10 09:20:52  roebel
 * Extend frame directory to contain the matrix signatures for each frame.
 * No longer needs to re read the frame to decide whether frame is selected.
 *
 * Revision 1.14  2004/09/09 19:17:38  roebel
 * Version 1.0.0beta:
 * First complete version of iterator access when reading files. Frame-Iterators use the
 * internal Frame Directory that each Entity will generate and update on the fly
 * to minimize disk access during positioning.
 *
 * Revision 1.13  2004/08/25 18:23:56  roebel
 * Version 0.11.0
 * removed SetNbMatrix which was confusing because people used it
 * to resize a Frame. Added a real resize function.
 * Added size(), resize() and clear() functions to
 * have more consistent interface compared to stl containers.
 *
 * Revision 1.12  2004/07/29 13:43:39  roebel
 * Use eEmptySignature to initialize signature of empty frame.
 *
 * Revision 1.11  2004/07/21 13:20:33  roebel
 * Added support to hold a frameDirectory in the entity and
 * the possibility to read frames from a given time position.
 *
 * Revision 1.10  2004/02/02 18:07:24  roebel
 * Version 0.8.1: bug fix in sdifframe establishing the possibility to
 * resize matrices within the frame.
 *
 * Revision 1.9  2003/12/05 13:53:14  ellis
 *
 * including <iostream> for the use of std::cout, cerr...
 *
 * Revision 1.8  2003/11/18 18:17:00  roebel
 * Replaced error messages by exceptions.
 *
 * Revision 1.7  2003/07/18 20:41:05  roebel
 * Added SetHeader with Signature as std::string
 *
 * Revision 1.6  2003/05/19 14:00:20  roebel
 * Include new easdif_config.h.
 *
 * Revision 1.5  2003/05/18 21:08:37  roebel
 * Added appropriate const methods.
 * For proper swig access GetMatrixWithSignature does no longer take
 * a reference as argument.
 *
 * Revision 1.4  2003/04/29 15:41:30  schwarz
 * Changed all names View* to Print* and *Info to *Header for consistency
 * with SDIF library.
 *
 * Revision 1.3  2003/04/18 16:44:00  schwarz
 * Small changes to make easdif swiggable:
 * - name change for swig-generated classes
 * - eof() returns bool, not bool&
 * - Matrix::Set takes int/float, not int&/float&
 *
 * Revision 1.2  2003/04/06 16:31:08  roebel
 * Added license info
 *
 * Revision 1.1  2003/03/03 19:00:16  roebel
 * Moved src directory to new name easdif
 *
 * Revision 1.7  2003/02/10 14:14:35  roebel
 * Fixed problem with size indicated in frameheader when the frame is
 * written more than one time withut calling cleardata().
 *
 * Revision 1.6  2002/10/10 10:49:09  roebel
 * Now using namespace Easdif.
 * Fixed handling of zero pointer arguments in initException.
 * Reading past end of file now throws an exception.
 *
 * Revision 1.5  2002/08/28 16:46:53  roebel
 * Internal reorganization and name changes.
 *
 * Revision 1.4  2002/07/12 10:18:17  ftissera
 * Read has changed for selection
 *
 * Revision 1.3  2002/06/18 14:50:19  ftissera
 * add methods with SDIFEntity for reading and writing
 *
 * Revision 1.2  2002/05/17 20:20:56  ftissera
 * Add doxygen header
 * 
 * 
 */

#include <iostream>
#include "easdif/easdif_config.hpp"
#include "easdif/sdif_frame.hpp"
#include "easdif/sdifentity.hpp"
#ifdef HAVE_SSTREAM
#include <sstream>
#else
#ifdef HAVE_STRSTREAM
#include <strstream>
#endif
#endif

namespace Easdif {

/* for reading */
int SDIFFrame::Read(SdifFileT* file, bool &eof)
{
    size_t sigBytesRead = 0;
    mFrameBytesRead = 0;
    ClearData();

    /* for selection */
    if (!(mFrameBytesRead = ReadHeader(file)))
    {
      SdifFSkipFrameData (file);
      //to have exception
      eof = (SdifFGetSignature (file, &sigBytesRead) == eEof);
      return 0;
    }

    Resize(mNbMatrix);
    mFrameBytesRead += ReadData(file);

    /* to have exception */
    
    eof = (SdifFGetSignature (file, &sigBytesRead) == eEof);
    return mFrameBytesRead;    
}


/* for reading with SDIFEntity */
//void SDIFFrame::Read(const SDIFEntity& entity)
//int SDIFFrame::Read(const SDIFEntity& entity)
int SDIFFrame::Read(SDIFEntity& entity)
{
  mFrameBytesRead  = 0;
  size_t sigBytesRead = 0;
  SdifFileT* file = entity.GetFile();

    if(entity.eof()) {
      throw Easdif::SDIFEof(eError,"Error in SDIFFrame::Read -- Eof reached",
			    file,eEof,0,0);
    }
    
    if(entity.IsFrameDir()){

      ClearData();
      if(!(mFrameBytesRead = ReadHeader(file))) {
        SdifFSkipFrameData (file);
        //to have exception
      }
      else {
        Directory::iterator it;
        bool isnew =  entity.AddFramePos(SdifFCurrID(file),SdifFCurrFrameSignature(file),
                                         SdifFCurrTime(file),SdifFCurrNbMatrix(file),
                                         file->StartChunkPos,it);

        Resize(mNbMatrix);
        if(!isnew) {
          if(entity.isFrameHLSelected(SdifFCurrID(file),SdifFCurrFrameSignature(file))){
            size_t tmp  = ReadData(entity);

            if(tmp){
              entity.mLastReadPos = it; 
            }
            mFrameBytesRead += tmp;
          }
          else {
            SdifFSkipFrameData (file);
            mFrameBytesRead = 0;
          }
        }
        else{
          SdifUInt4 nb = it->LocNbMatrix();
          int ir = 0;
          for(SdifUInt4 i=0 ; i< nb;++i){
            int ret =0;
            if((ret=mv_Matrix[ir].Read(file,&entity.msHighLevelMatrixSelection)))
              ++ir;
            mFrameBytesRead +=ret;
            it->SetMSignature(i,file->CurrMtrxH->Signature);
          }

          if(entity.isFrameHLSelected(SdifFCurrID(file),
                                      SdifFCurrFrameSignature(file))){
            Resize(ir);            
            entity.mLastReadPos = it;
          }
          else {
            ClearData();
            mFrameBytesRead = 0;
          }
        }
      }

      entity.mEof = (SdifFGetSignature (file, &sigBytesRead) == eEof);
      if(entity.mEof)
        entity.mEofSeen = true;
    }
    else
      mFrameBytesRead = Read(file, entity.mEof);

    return mFrameBytesRead;
}

/* reading the data */
int SDIFFrame::ReadData(SdifFileT* file,const std::set<SdifSignature>*hlsel)
{
    int BytesRead = 0;
    SdifUInt4 index;
    for (index = 0; index < mNbMatrix; ){
	BytesRead += mv_Matrix[index].Read(file,hlsel);
	if(mv_Matrix[index].GetSignature() == eEmptySignature)
	    mNbMatrix --;
	else
	    index++;
    }
    Resize(mNbMatrix);
    return BytesRead;
}

/* reading the data with SDIFEntity*/
int SDIFFrame::ReadData(const SDIFEntity& entity)
{

    SdifFileT* file = entity.GetFile();
    return ReadData(file,&entity.msHighLevelMatrixSelection);
}

/* reading the informations */
int SDIFFrame::ReadHeader(SdifFileT* file)
{
    int BytesRead = 0;  
    //size_t BytesRead = 0;
    //int eof = 0;

    BytesRead += SdifFReadFrameHeader(file);
    /* for selection*/
    if (!SdifFCurrFrameIsSelected (file))
      {
        //SdifFSkipFrameData (file);
        //eof = SdifFGetSignature (file, &bytesread) == eEof;
        return 0;
      }
    mTime    = SdifFCurrTime(file);
    mSig      = SdifFCurrFrameSignature(file);
    mStreamID = SdifFCurrID(file);
    mNbMatrix  = SdifFCurrNbMatrix(file);

    return BytesRead;
}


/* reading the informations with SDIFEntity */
int SDIFFrame::ReadHeader(const SDIFEntity& entity)
{
    SdifFileT* file = entity.GetFile();
    return ReadHeader(file);
}


/* for writing */
int SDIFFrame::Write(SdifFileT* file)
{        
    SdifUInt4 index;
    SdifUInt4 lsize=GetSize();
    
    SdifUInt4 _size    = WriteHeader(file);
    for (index = 0; index < mNbMatrix; index++) {
      _size += mv_Matrix[index].Write(file);     
    }

    // Signature and size entry do not count for frame size
    if(_size != lsize + 8){
      throw Easdif::SDIFFrameHeaderSizeError(eError,"Error in SDIFFrame::Write -- FrameSize  does not match written size",file,eBadHeader,0,0);
    }
    return _size; 
}

/* for writing with SDIFEntity */
int SDIFFrame::Write(const SDIFEntity& entity)
{ 
    return Write(entity.GetFile());    
}

/* writing informations */
int SDIFFrame::WriteHeader(SdifFileT* file)
{
  SdifFSetCurrFrameHeader(file, mSig, GetSize(),
			  mNbMatrix, mStreamID, mTime); 
  return SdifFWriteFrameHeader(file);     
}

/* writing informations with SDIFEntity*/
int SDIFFrame::WriteHeader(const SDIFEntity& entity)
{
   return WriteHeader(entity.GetFile());
}

/* to see */
void SDIFFrame::PrintHeader() const
{
    std::cout << "\nFrame signature : " << SdifSignatureToString(mSig)
	      << std::endl;
    std::cout << "StreamID: " << mStreamID;
    std::cout << " Time: " << mTime;
    std::cout << " number of Matrix in current Frame : " << mNbMatrix << std::endl; 
}

void SDIFFrame::Print() const
{
    SdifUInt4 index;
    PrintHeader();
    for (index = 0; index < mNbMatrix; index++)
	mv_Matrix[index].Print();		  
}

/* to Set */
void SDIFFrame::SetHeader(SdifSignature sig, SdifUInt4 streamID, SdifFloat8 time)//, SdifUInt4 nbMatrix)
{
    mSig = sig;
    mStreamID = streamID;
    mTime = time; //and  mNbMatrix = nbMatrix;
}

void SDIFFrame::SetHeader(const std::string& sig, SdifUInt4 streamID, SdifFloat8 time)//, SdifUInt4 nbMatrix)
{
    mSig = SdifStringToSignature(const_cast<char*>(sig.c_str()));
    mStreamID = streamID;
    mTime = time; //and  mNbMatrix = nbMatrix;
}

//void SDIFFrame::SetNbMatrix(SdifUInt4 nbMatrix)
//{
//    mNbMatrix = nbMatrix;
//}

void SDIFFrame::SetSignature(SdifSignature sig)
{
    mSig = sig;
}

void SDIFFrame::SetStreamID(SdifUInt4 streamID)
{
    mStreamID = streamID;
}

void SDIFFrame::SetSignature(const std::string& signature)
{
    mSig = SdifStringToSignature(const_cast<char*>(signature.c_str()));
}

void SDIFFrame::SetTime(SdifFloat8 time)
{
    mTime = time;
}

/* to Get */
SDIFMatrix& SDIFFrame::GetMatrix(unsigned int index)   
  throw(SDIFMatrixNotAvailable)
{
    // Check index
    if (index > (mv_Matrix.size()-1))
    {
      const char *cmsg = "GetMatrix(unsigned int):: Matrix index  out of range! ";
#ifdef HAVE_SSTREAM
      std::ostringstream msg;
      msg << "GetMatrix(unsigned int):: Matrix index " << index << " out of range! ";
      cmsg = msg.str().c_str();
#else
#ifdef HAVE_STRSTREAM
      std::ostrstream msg;
      msg << "GetMatrix(unsigned int):: Matrix index " << index << " out of range! ";
      cmsg = msg.str();
#endif
#endif
      throw Easdif::SDIFMatrixNotAvailable(eError, cmsg, 0, eNotFound, 
					   __FILE__, __LINE__);
    }
    return mv_Matrix[index];
}

/* to Get */
const SDIFMatrix& SDIFFrame::GetMatrix(unsigned int index) const  
  throw(SDIFMatrixNotAvailable)

{
    // Check index
    if (index > (mv_Matrix.size()-1))
    {
      const char *cmsg = "GetMatrix(unsigned int) const:: Matrix index  out of range! ";
#ifdef HAVE_SSTREAM
      std::ostringstream msg;
      msg << "GetMatrix(unsigned int) const:: Matrix index " << index << " out of range! ";
      cmsg = msg.str().c_str();
#else
#ifdef HAVE_STRSTREAM
      std::ostrstream msg;
      msg << "GetMatrix(unsigned int) const:: Matrix index " << index << " out of range! ";
      cmsg = msg.str();
#endif
#endif

      throw Easdif::SDIFMatrixNotAvailable(eError, cmsg, 0, eNotFound, 
					   __FILE__, __LINE__);
    }
    return mv_Matrix[index];
}



SdifUInt4 SDIFFrame::GetSize() const
{
  SdifUInt4       size=SdifSizeOfFrameHeader();

  for(unsigned int i=0;i<GetNbMatrix();++i){
    size += GetMatrix(i).GetSize();
  }
  return size;

}

/* clean */
void SDIFFrame::ClearData()
{
    mStreamID = 0;
    mNbMatrix = 0;
    mSig      = eEmptySignature;
    mTime     = -1.;
    mv_Matrix.clear();
}

/* add a matrix */
int SDIFFrame::AddMatrix(const SDIFMatrix& aMatrix)
{
    mv_Matrix.insert(mv_Matrix.end(), aMatrix);
    mNbMatrix++;
    return mNbMatrix;
}

#if 0
int SDIFFrame::AddMatrixSelected(SdifFileT* file, const SDIFMatrix& aMatrix)
{
    SdifSignature sig, sigm;
    sig = GetMatrixSelection(file);
    sigm = aMatrix.GetSignature();
    if(sig != 0)
    {
	if(sig == sigm)
	{
	    return AddMatrix(aMatrix);
	}
	else
	    return 0;
    }
    else/* no selection on matrix : all matrix are selected*/
	return AddMatrix(aMatrix);
}
#endif

/* resize */
void SDIFFrame::Resize(int nbMatrix)
{
  mNbMatrix = nbMatrix;
  mv_Matrix.resize(mNbMatrix);    
}


bool SDIFFrame::MatrixExists(const SdifSignature& sig) const
{ 
    bool test = false;
    SdifUInt4 index = 0;    

    while (!test && (index < mNbMatrix))
    {
	test = (mv_Matrix[index].GetSignature() == sig);
	++index;
    }
    return test;
}

bool SDIFFrame::MatrixExists(const std::string& signature) const
{
    SdifSignature sig = SdifStringToSignature(const_cast<char*>
					      (signature.c_str()));
    return MatrixExists(sig);
}


SDIFMatrix& SDIFFrame::GetMatrixWithSig(const SdifSignature sig)
  throw(SDIFMatrixNotAvailable)
{
    bool test = false;    
    SdifUInt4 index = 0;

    while (!test && (index < mNbMatrix))
    {
	test = (mv_Matrix[index].GetSignature() == sig);
	++index;
    }

    if (test)
	return mv_Matrix[index-1];
    else
    {
      std::string msg = "GetMatrixWithSig(const SdifSignature sig) :: Matrix signature ";
      msg += SdifSignatureToString(sig);
      msg += " not contained in Frame !";
      throw Easdif::SDIFMatrixNotAvailable(eError, msg.c_str(), 0, eNotFound, 
					   __FILE__, __LINE__);
    }
}

const SDIFMatrix& SDIFFrame::GetMatrixWithSig(const SdifSignature sig) const
  throw(SDIFMatrixNotAvailable)
{
    bool test = false;    
    SdifUInt4 index = 0;

    while (!test && (index < mNbMatrix))
    {
	test = (mv_Matrix[index].GetSignature() == sig);
	++index;
    }

    if (test)
	return mv_Matrix[index-1];
    else
    {
      std::string msg = "GetMatrixWithSig(const SdifSignature sig) const:: Matrix signature ";
      msg += SdifSignatureToString(sig);
      msg += " not contained in Frame !";
      throw Easdif::SDIFMatrixNotAvailable(eError, msg.c_str(), 0, eNotFound, 
					   __FILE__, __LINE__);
    }
}



SDIFMatrix& SDIFFrame::GetMatrix(const std::string& signature)
  throw(SDIFMatrixNotAvailable)
{   
    /*
      bool test = false;    
      SdifUInt4 index = 0;
    */

    SdifSignature sig = SdifStringToSignature(const_cast<char*>
					      (signature.c_str()));
    /* with GetMatrixWithSig(sig) */
    try {
      return GetMatrixWithSig(sig);
    }
    catch (Easdif::SDIFMatrixNotAvailable) {
      std::string msg = "GetMatrix(const std::string) :: Matrix signature "+ signature + " not contained in Frame !";
      throw Easdif::SDIFMatrixNotAvailable(eError, msg.c_str(), 0, eNotFound, 
					   __FILE__, __LINE__);      
    }
}


const SDIFMatrix& SDIFFrame::GetMatrix(const std::string& signature) const
  throw(SDIFMatrixNotAvailable)
{   
    /*
      bool test = false;    
      SdifUInt4 index = 0;
    */

    SdifSignature sig = SdifStringToSignature(signature.c_str());
    /* with GetMatrixWithSig(sig) */
    try {
      return GetMatrixWithSig(sig);
    }
    catch (Easdif::SDIFMatrixNotAvailable) {
      std::string msg = "GetMatrix(const std::string) const:: Matrix signature "+ signature + " not contained in Frame !";
      throw Easdif::SDIFMatrixNotAvailable(eError, msg.c_str(), 0, eNotFound, 
					   __FILE__, __LINE__);      
    }


}

#if 0
SdifSignature SDIFFrame::GetMatrixSelection(SdifFileT* file) const
{
    SdifListT* listsel;
    SdifSignature sig = 0;
    listsel = file->Selection->matrix;
    SdifListInitLoop (listsel);
    while (SdifListIsNext (listsel))
    {
	sig = ((SdifSelectElementT *) SdifListGetNext 
	       (listsel))->value.signature;
    }
    return sig;
}

SdifSignature SDIFFrame::GetMatrixSelection(const SDIFEntity& entity) const
{
    return GetMatrixSelection(entity.GetFile());
}
#endif

} // end of namespace Easdif

