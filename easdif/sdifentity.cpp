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
 * @file   sdifentity.cpp
 * @author Fabien Tisserand
 * @date   Tue Jun 18 20:12:24 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifentity.cpp,v 1.48 2012-08-28 22:01:48 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.47  2012/08/10 01:03:29  roebel
 * Added new function that allows to retrieve easdif version during runtime.
 * Fixed ReadNextSelectFrame when time argument is provided and the time is the same or smaller as the first frame,
 * which would result in oscillating movements of the read frames between first and second frame for repeatedly asking to
 * read the frame with first frames time.
 *
 * Revision 1.46  2011/06/11 17:05:31  roebel
 * Moved some member functions out of class scope to avoid unnecessary inlining
 * of constructor, destructor or other costly functions.
 *
 * Revision 1.45  2010/05/08 19:35:55  roebel
 * Fixed SDIFEntity::GetStreamSelection to properly establish the selected streams.
 *
 * Revision 1.44  2009/07/31 21:25:47  roebel
 * Replaced std::list<SDIFLocation> types by Directory typedef.
 *
 * Revision 1.43  2008/05/31 22:54:15  roebel
 * Fixed TestFrameSelection and TestMatrixSelection in case no high-level selection is active.
 *
 * Revision 1.42  2008/01/22 00:49:25  roebel
 * Clear NVTs and Types when closing the file.
 *
 * Revision 1.41  2008/01/11 15:58:38  roebel
 * Added new class MatrixType, FrameType, and the realted function for
 * adding types to entity or retrieving types from an entity in terms of these
 * clases.
 *
 * Revision 1.40  2007/11/27 17:36:29  roebel
 * Replaced sdif macro by inline member function.
 *
 * Revision 1.39  2007/11/26 19:10:08  roebel
 * Fixed to avoid compiler warnings in MSVC.
 * Little problem is the export of std::containers that should be defined as export
 * which is not possible due to the given STL include files.
 * For the moment it seems these warnings are not important, because all these functions
 * are inlined as templates in the STL anyway. Has to be handled with care !!!!
 *
 * Revision 1.38  2007/10/25 22:31:37  roebel
 * Changed interface to AddNVT. AddNVT without explicit StreamID now uses
 * the streamid of the SDIFNameValueTable and no longer imposes a default value of 0.
 *
 * Revision 1.37  2007/04/30 11:31:47  roebel
 * Throw exception FrameDirError exception if  selection manipulation
 * functions or framedirectory functions are called in unsuitable states.
 * Replaced the concept of the current frame in the frame directory
 * by the concept of the last frame read which no opnger is guaranteed
 * to be selected.
 *
 * Revision 1.36  2006/10/06 10:08:13  roebel
 * Completed output of PrintFrameDir.
 * Fixed test of direction in ReadNextSelectedFrame(SDIFFrame& frame, SdifFloat8 time)
 *
 * Revision 1.35  2005/06/06 13:54:49  roebel
 * Changed mEof for files opened for writing to false.
 *
 * Revision 1.34  2005/06/02 22:22:49  roebel
 * Fixed RestrictSelection functions for the case that alow level
 * selection exists and a high level selection has to be created.
 *
 * Revision 1.33  2005/05/31 19:46:55  bogaards
 * isSelected function and corrections to the selection tests
 *
 * Revision 1.32  2005/05/30 22:05:31  roebel
 * Added missing include file for good old gcc-2
 *
 * Revision 1.31  2005/05/30 21:46:09  roebel
 * Changed all include files from .h into .hpp to prevent name clash between
 * sdifmatix.h and SDIF/sdifcpp/SdifMatrix.h on MacOSX where filenames are
 * case insensitive.
 *
 * Added new class SelectionSet to handle SdifSelections in Easdif.
 * Problem is that a set can be empty meaning no selection or nothing is selected
 * because to sets with empty intersection have been applied in RestrictSelection functions.
 * The new class has a boolean isActive to distinguish these two cases.
 *
 * Revision 1.30  2005/05/30 18:14:00  bogaards
 * fixed highlevel selection for non-existing base selection
 *
 * Revision 1.29  2005/05/24 13:12:28  roebel
 * Added mising return statements and fixed minor compiler warnings.
 *
 * Revision 1.28  2005/05/24 09:53:51  roebel
 * Changed selection management in Easdif:
 * Before EnableDirectory has been called selection
 * modification wqorks on the SDIF-Selection, after
 * enabling the directory a new high level selection mode
 * is used that can only be used to restrict the secltion by
 * forming intersections with the existing selections.
 * This mechanism exists now for stream/frame and matrix selections.
 *
 * Revision 1.27  2005/05/20 21:32:04  roebel
 * Increased consistence and documentation of SDIF frame directory
 * and SDIF frame iterator. The directory is now limited
 * to only the selected frames, streams and matrices.
 * Added new functions to restrict and reestablish
 * signature selections.
 * selections modifications are disabled whenever the
 * FrameDirectory is enabled and not yet finished.
 * Added member function to test for seekability.
 *
 * Revision 1.26  2005/05/03 16:23:34  roebel
 * Added 2 new functions to handle selections in a more specific manner.
 *
 * Revision 1.25  2005/02/04 12:31:43  roebel
 * Fixed return value of SDIFEntity::WriteFrame
 *
 * Revision 1.24  2004/10/07 14:48:11  roebel
 * Replaced calls to resize(0) by clear() and
 * test using size() by !empty() to improve efficiency.
 *
 * Revision 1.23  2004/09/10 09:20:52  roebel
 * Extend frame directory to contain the matrix signatures for each frame.
 * No longer needs to re read the frame to decide whether frame is selected.
 *
 * Revision 1.22  2004/09/09 19:17:37  roebel
 * Version 1.0.0beta:
 * First complete version of iterator access when reading files. Frame-Iterators use the
 * internal Frame Directory that each Entity will generate and update on the fly
 * to minimize disk access during positioning.
 *
 * Revision 1.21  2004/09/08 09:14:46  roebel
 * Improved efficiency of FrameDirectory by means of
 * preventing the need to  search the complete directoy for each frame read.
 *
 * Revision 1.20  2004/07/28 13:03:27  roebel
 * Fixed position type to be SdiffPosT
 *
 * Revision 1.19  2004/07/27 18:58:06  roebel
 * Removed remaining debug message.
 *
 * Revision 1.18  2004/07/21 13:20:19  roebel
 * Added support to hold a frameDirectory in the entity and
 * the possibility to read frames from a given time position.
 *
 * Revision 1.17  2004/02/13 11:34:48  roebel
 * Reset eof to false after rewinding file
 *
 * Revision 1.16  2004/02/11 19:28:17  roebel
 * Added inline function to test state of entity. Added function to access NVTs that indicates existance of name.
 *
 * Revision 1.15  2004/01/24 18:51:19  roebel
 * Fixed rewind for files opened for writing.
 * Improved error checking when doing rewind.
 *
 * Revision 1.14  2004/01/19 15:49:55  bogaards
 * Added Rewind and ReadNextSelectedFrame methods to Entity
 *
 * Revision 1.13  2003/12/05 13:53:14  ellis
 *
 * including <iostream> for the use of std::cout, cerr...
 *
 * Revision 1.12  2003/11/18 18:28:00  roebel
 * removed coments, changed exceptions to use new exception interface.
 *
 * Revision 1.11  2003/05/24 00:27:21  roebel
 * Internal representation of types now using std::string.
 * Parameters for type handling routines completely moved
 * to std::string
 *
 * SDIFEntity::SetTypeString no longer appends but sets
 * the internal string. In fact there was now way to reset
 * the type string before.
 *
 * Revision 1.10  2003/05/22 21:23:58  roebel
 * SDIFNameValueTable now derived from std::map which makes handling more conform
 * to standard and handling in swig easier.
 *
 * Revision 1.9  2003/05/22 17:57:53  roebel
 * Removed redundant initilization of variable
 *
 * Revision 1.8  2003/05/21 20:36:23  roebel
 * Protect for reopening the same entity. The existing file will be properly closed
 * and the entity reinitialized.
 *
 * Revision 1.7  2003/05/19 13:58:03  roebel
 * Include new easdif_config.h.
 *
 * Revision 1.6  2003/05/01 18:58:43  roebel
 * eof now true for uninitialized entity. eof is properly reset
 * when opening for reading.
 *
 * Revision 1.5  2003/04/29 15:41:30  schwarz
 * Changed all names View* to Print* and *Info to *Header for consistency
 * with SDIF library.
 *
 * Revision 1.4  2003/04/18 17:43:22  schwarz
 * eof() const method
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
 * Revision 1.8  2003/02/07 18:37:58  roebel
 * Empty sdif files (only header information) will be correctly handled
 * now.
 *
 * Revision 1.7  2002/11/27 20:13:04  roebel
 * Removed member that redundantly indicated size of NVT vector.
 * Make use of improved SDIFNameValueTable interface.
 *
 * Revision 1.6  2002/10/30 15:27:32  roebel
 * Changed return type from int to bool.
 * Changed error checking in openroutines.
 *
 * Revision 1.5  2002/10/10 10:49:09  roebel
 * Now using namespace Easdif.
 * Fixed handling of zero pointer arguments in initException.
 * Reading past end of file now throws an exception.
 *
 * Revision 1.4  2002/10/03 11:26:56  tisseran
 * Check if efile is not null before trying to close it.
 * Initialize efile to 0 by sdifentity::sdifentity()
 *
 * Revision 1.3  2002/08/28 16:46:53  roebel
 * Internal reorganization and name changes.
 *
 * Revision 1.2  2002/07/12 10:34:08  ftissera
 * *** empty log message ***
 *
 * Revision 1.1  2002/06/18 18:43:08  ftissera
 * Project for new SDIF API
 * 
 * 
 */

#include <iostream>
#include <algorithm>
#include "easdif/easdif_config.hpp"
#include "easdif/sdifentity.hpp"
#include "easdif/easdif_version.hpp"

// isx indicated as  exported but isn't
int SdifParseSelection (SdifSelectionT *sel, const char *str);
void SdifInitIntMask (SdifSelectIntMaskP mask);
void SdifSelectGetIntMask (SdifListP list, SdifSelectIntMaskP mask);


namespace Easdif {

  const char* Version() {
    return EASDIF_VERSION_STRING;
  }

  MatrixType::~MatrixType() {}
  FrameType::~FrameType() {}

  SDIFEntity::SDIFEntity(): efile(0), mSize(0), mEof(true), mEofSeen(false),
                            mOpen(0), generalHeader(0), asciiChunks(0), 
                            isFrameDirEnabled(false), 
                            endLoc(SdifUInt4(-1), 0, -1., eEmptySignature,  0),
                            mlMatrixSelectionRead(false),mlFrameSelectionRead(false),
                            mlStreamSelectionRead(false)
  {
    mFirstFramePos = 0;
  };

  SDIFEntity::~SDIFEntity()
  {	    
    Close();
  };



/* to open a file in mode Read  */
bool SDIFEntity::OpenRead(const char* filename)
{
    isFrameDirEnabled = false;
    mFrameDirectory.clear();
    ClearSelectionState();
    bool ret =ReOpenRead(filename);
    // store original file selection for later use 
    GetMatrixSelection(msMatrixSelection);
    mlMatrixSelectionRead =true;
    GetFrameSelection(msFrameSelection);
    mlFrameSelectionRead =true;
    GetStreamSelection(msStreamSelection);
    mlStreamSelectionRead=true;
    return ret;
}


/* to reopen a file in mode Read  */
bool SDIFEntity::ReOpenRead(const char* filename)
{
    int n;
    // close file in case it was already open
    Close();      
    efile = SdifFOpen (filename, eReadFile);

    if(!efile)
      return false;
      
    generalHeader = SdifFReadGeneralHeader(efile);
    if(!generalHeader){
      Close();
      return false;
    }

    asciiChunks = SdifFReadAllASCIIChunks(efile);

    /* to put matrix and frame's types in the SdifString : mDescription */
    SdifStringT *_tmp = SdifStringNew();
    SdifFAllMatrixTypeToSdifString(efile, _tmp);
    SdifFAllFrameTypeToSdifString(efile, _tmp);
    mDescription = _tmp->str;

    n = SdifFNameValueNum(efile);
 
    if( n != 0)
    {
	/* used in the loop for getting the next NVT  */	
	SdifNameValuesLT* NVlist;
	NVlist = efile->NameValues;

	for (int i = 1 ; i <= n ; i++)
	{	 
	  SdifNameValuesLSetCurrNVT(NVlist, i);
	  AddNVT(TakeNVT(), efile->NameValues->CurrNVT->StreamID );
	}
    }  

    // empty sdif file
    if(feof(efile->Stream))
      mEofSeen = mEof = true;
    else
      mEofSeen = mEof = false;

    mOpen = 2;

    GetLowLevelFilePos(mFirstFramePos);
    mFirstFramePos -= 4;	// subtract the first 4 read chars of the already read signature

    // reestablish selection state
    if(mlMatrixSelectionRead)
      ReestablishMatrixSelection();
    if(mlFrameSelectionRead)
      ReestablishFrameSelection();
    if(mlStreamSelectionRead)
      ReestablishStreamSelection();

    return true;
}

// enable Framne directory
bool SDIFEntity::EnableFrameDir() {
  // directory only makes sense for files that are open for reading
  if(mOpen!=2)
    return false;

  if(!isFrameDirEnabled){
    SdiffPosT pos;
    GetLowLevelFilePos(pos);
    // this will be true for pipes or if no
    // frame has yet been read from the file
    if(pos == mFirstFramePos+4){
      isFrameDirEnabled = true;    
      mNextDirPos  = mFrameDirectory.end();
      mLastReadPos = mFrameDirectory.end();
      if(mlFrameSelectionRead)
        msFrameSelection.clear();
      mlFrameSelectionRead   = true;
      GetFrameSelection(msFrameSelection);
      if(mlMatrixSelectionRead)
        msMatrixSelection.clear();
      mlMatrixSelectionRead   = true;
      GetMatrixSelection(msMatrixSelection);
      if(mlStreamSelectionRead) {
        msStreamSelection.clear();
      }
      mlStreamSelectionRead   = true;
      GetStreamSelection(msStreamSelection);    
    }
    else
      throw Easdif::FrameDirError(eError,
                                  "EnabledFrameDir can only be called at the start of the file",GetFile(),
                                  eUnknown,
                                  __FILE__,__LINE__);
  }
  return true;
}

void SDIFEntity::PrintFrameDir() const {
  std::cerr << "Init pos "<< mFirstFramePos << "\n";
  Directory::const_iterator  start=mFrameDirectory.begin();
  Directory::const_iterator  end=mFrameDirectory.end();
  while(start != end){
    std::cerr<< "Pos "<< start->LocPos()<< " sig " << SdifSignatureToString(start->LocSignature()) <<" time "<< start->LocTime() << " stream id "<< start->LocStreamID() << "\n";
    std::cerr <<" Matrices: ";

    for(unsigned int ii=0;ii<start->LocNbMatrix();++ii)
      std::cerr <<" " <<  SdifSignatureToString(start->LocMSignature(ii));
    
    std::cerr << "\n";
    ++start;
  }
}

// Add a new location into the directory
bool
SDIFEntity::AddFramePos(SdifUInt4 id, SdifSignature sig, 
                        SdifFloat8 time,SdifUInt4 nbmat,
                        SdiffPosT pos, Directory::iterator& it) {

  if(mNextDirPos == mFrameDirectory.end()){    
    it = mFrameDirectory.insert(mFrameDirectory.end(),SDIFLocation(pos,id,time,sig,nbmat));
    mNextDirPos = mFrameDirectory.end();
    return true;
  }
  
  Directory::iterator  start=mNextDirPos;
  Directory::iterator  end  =mFrameDirectory.end();

  while(start!=end && start->LocPos() > pos) --start;
  
  while(start!=end && start->LocPos() < pos) ++start;
  // don't add duplicate entries
  if(start==end || start->LocPos() > pos){
    mNextDirPos=start;
    it = mFrameDirectory.insert(start,SDIFLocation(pos,id,time,sig,nbmat));
    return true;
  }

  it = start;
  mNextDirPos=++start;
  return false;
}

/* to open a file in mode Write  */
bool SDIFEntity::OpenWrite(const char* filename)
{
    // close file in case it was already open
    Close();
    isFrameDirEnabled = false;
    mFrameDirectory.clear();
    efile = SdifFOpen (filename, eWriteFile);

    if(!efile)
      return false;

   /* write on the file 'SDIF' and 4 bytes chunk size */
    if(!(generalHeader = SdifFWriteGeneralHeader(efile))){
      Close();
      return false;
    }

    /* to add member data (NVTs) to sdif file */
    if (mv_NVT.size() != 0)	
      WriteNVTs();

    /* to add Descriptions types to sdif file  */
    WriteTypes();
	
    asciiChunks = SdifFWriteAllASCIIChunks(efile);
    mOpen = 1; 
 
    GetLowLevelFilePos(mFirstFramePos);
    mEofSeen = mEof = false;

    return true; 
}

/* to open completely (with Header and ASCII Chunks)  a file in mode : Mode  */
bool SDIFEntity::Open(const char* filename, SdifFileModeET Mode)
{
    switch(Mode)
    {
    case eReadFile:
    {
	return OpenRead(filename);
    }
    break;
    case eWriteFile:
    {	
	return OpenWrite(filename);
    }
    break;

    /* the others cases are for administrator : 
     *case ePredefinedTypes, case eModeMask, case eParseSelection:
     */
    default:
      throw SDIFBadMode(eError,"Error in SDIFEntity::Open:: Mode specification error",
			0,eBadMode,0,0);
      break;
    }
    return false;
}

bool SDIFEntity::Rewind()
{
  SdiffPosT readpos;

  if(!isSeekable())
    return false;

  if(isFrameDirEnabled ) {
    return begin().GotoPos();
  }
  
  if(-1 == SdiffSetPos(GetFile()->Stream,&mFirstFramePos)) return false;
  if(! GetLowLevelFilePos(readpos))
    return false;

  if(readpos != mFirstFramePos)
    return false;

  mEof = false;

  if(mOpen & 2) {
    size_t SizeR = 0;
    SdifFGetSignature(GetFile(), &SizeR);

    return (SizeR > 0);
  }

  return true;
}


bool SDIFEntity::FillFrameDirAndRewind()
{
  if(!isSeekable() || ! isFrameDirEnabled)
    return false;

  iterator it=lastRead(), ite=end();
  while(it != ite)
    ++it;
  return Rewind();
}


SdifFileT* SDIFEntity::GetFile() const
{
    return efile;
}

int SDIFEntity::GetNbNVT()const
{
    return mv_NVT.size();
}

bool SDIFEntity::AddNVT(const SDIFNameValueTable& nvt, 
			       SdifUInt4 StreamId)
{
    mv_NVT.push_back(nvt);
    mv_NVT.back().SetStreamID(StreamId);

    return true;
}

bool SDIFEntity::AddNVT(const SDIFNameValueTable& nvt)
{
    mv_NVT.push_back(nvt);
    return true;
}

bool SDIFEntity::WriteNVTs()
{
    SdifNameValuesLT* NVlist;

    int nnvts = mv_NVT.size();
    for (int i = 0 ; i < nnvts ; i++)
    {
	NVlist = efile->NameValues;
	SdifNameValuesLNewTable( NVlist, mv_NVT[i].GetStreamID());
    
	//NVlist = SdifNameValuesLNewTable( NVlist, StreamID);
	// NVlist->CurrNVT->NumTable = mNbNVT + 1;
	SDIFNameValueTable::const_iterator itend= mv_NVT[i].end();
	for (SDIFNameValueTable::const_iterator p = mv_NVT[i].begin();
	     p != itend ; ++p)
	{
	    SdifNameValuesLPutCurrNVT(NVlist, 
				      p->first.c_str(), 
				      p->second.c_str());	
	}
    }
    /* for writing the Name Value Table in the file */
    //  asciiChunks = SdifFWriteAllASCIIChunks(efile);
    return true;
}

/* to get a NVT of a file */
SDIFNameValueTable SDIFEntity::TakeNVT()
{
    SdifUInt4       iNV;
    SdifHashNT     *pNV;
    SdifHashTableT *HTable;
    SDIFNameValueTable nvt;

    HTable = efile->NameValues->CurrNVT->NVHT;  
    // a loop to put the NameValues in a SDIFNameValuesTable 
    for(iNV=0; iNV<HTable->HashSize; iNV++)
    {
	for (pNV = HTable->Table[iNV]; pNV; pNV = pNV->Next)
	{

	  nvt.AddNameValue(((SdifNameValueT *)pNV->Data)->Name,
			   ((SdifNameValueT *)pNV->Data)->Value);
	}
    }

    return nvt;
}

/* to get a NVT of an entity */
SDIFNameValueTable& SDIFEntity::GetNVT(unsigned int i)
{
    /* Check the index */
    if (i > (mv_NVT.size()-1))
    {
	std::cerr << " No such NameValueTable " << std::endl;
        mv_NVT.push_back(SDIFNameValueTable());
	return mv_NVT[0];
    }
    return mv_NVT[i];
}


bool SDIFEntity::Close()
{
  if (0 != efile)
    {
      msHighLevelMatrixSelection.clear();
      msHighLevelFrameSelection.clear();
      msHighLevelStreamSelection.clear();
      clearAllNVT();
      clearTypes();
      SdifFClose(efile);
      efile=0;
      mSize=0;
      mEof = true; 
      mEofSeen = false; 
      mOpen = 0; 
      generalHeader = 0; 
      asciiChunks =0;
      return true;
    }
  return false;
}

/* temporary  */
SdifErrorT* SDIFEntity::LastError()
{
    return SdifFLastError (efile);
}


int SDIFEntity::ReadNextFrame(SDIFFrame& frame)
{
    int bytesread = 0;

    if(eof()) {
      // return -1;
      throw SDIFEof(eError,"Error in SDIFEntity::ReadNextFrame -- Eof reached",
		    efile,eEof,0,0); 
    }

    //bytesread = frame.Read(efile, eof());
    bytesread = frame.Read(*this);
    return bytesread;    

}


int SDIFEntity::ReadNextSelectedFrame(SDIFFrame& frame)
{
    int bytesread = 0;

    if(eof()) {
      // return -1;
      throw SDIFEof(eError,"Error in SDIFEntity::ReadNextFrame -- Eof reached",
		    efile,eEof,0,0); 
    }

    while(bytesread == 0 && !eof()){
      bytesread = frame.Read(*this);
    }

    return bytesread;    
}



int SDIFEntity::ReadNextSelectedFrame(SDIFFrame& frame, SdifFloat8 time)
{

    if(!isFrameDirEnabled){
      throw SDIFDirError(eError,
                         "Error in SDIFEntity::ReadNextFrame(SDIFFrame& frame, SdifFloat8 time):: !!! frame directoy not enabled  !!!",
                         0,eUnknown,0,0);
    }

    if (mLastReadPos != mFrameDirectory.end() && mLastReadPos->LocTime()== time) {
      frame = *lastRead();
      return frame.GetSize();
    }

    bool up = false;
    // attention creation of iterator changes current position
    if((mLastReadPos == mFrameDirectory.begin() && time > mLastReadPos->LocTime())
       ||(mLastReadPos == mFrameDirectory.end() 
          && !mFrameDirectory.empty() &&mFrameDirectory.back().LocTime() < time )
       ||(mLastReadPos != mFrameDirectory.end() && mLastReadPos->LocTime() < time )){
      up =true;
    }

    if(up && eof()){
      // return -1;
      throw SDIFEof(eError,"Error in SDIFEntity::ReadNextSelectedFrame -- Eof reached",
                    efile,eEof,0,0); 
    }

    // end works in both directions !!
    iterator it=lastRead(), ite=end();
    if(up) {      
      if( (it!=ite))
        ++it;
      while( (it!=ite) && it.mBase->LocTime() < time)  {
        ++it;      
      }
    }
    else {
      if( it==ite || !it.IsSelected())
        --it;
      while (it!= ite && it.mBase->LocTime() >= time) {
        --it;
      }
      // it == ite means in fact begin!!
      if(it==ite || it.mBase->LocTime() < time)
        ++it;        
    }

    if(it != ite) {
      mEof = false;
      frame = *it;
      return frame.GetSize();
    }
    else {
      frame.ClearData();
      mEof = true;
    }

    return 0;
}

int SDIFEntity::ReadPrevSelectedFrame(SDIFFrame& frame)
{

    if(!isFrameDirEnabled){
      throw SDIFDirError(eError,
                         "Error in SDIFEntity::ReadNextFrame(SDIFFrame& frame, SdifFloat8 time):: !!! frame directoy not enabled  !!!",
                         0,eUnknown,0,0);
    }
    
    if(mLastReadPos == mFrameDirectory.begin()) {
      // return -1;
      throw SDIFEof(eError,"Error in SDIFEntity::ReadNextSelectedFrame -- Bof reached",
                    efile,eEof,0,0); 
    }

    iterator it=lastRead(), ite=end();

    --it;
    if(it != ite) {
      mEof = false;
      frame = *it;
      return frame.GetSize();
    }
    else {
      frame.ClearData();
      mEof = true;
    }
    
    return 0;
}



int SDIFEntity::WriteFrame(SDIFFrame& frame)
{    
    return frame.Write(efile);
}

void SDIFEntity::PrintAllNVTs()
{
  int nnvts = mv_NVT.size();
    for (int n = 0; n < nnvts; ++n)
    {
	std::cout << std::endl <<"NameValueTable number : " << n+1 
		  << std::endl;
	mv_NVT[n].PrintNameValueTable();
    }
}

/*******************************/

bool SDIFEntity::SetTypeString(const std::string& TypeString)
{
    mDescription = TypeString;
    return true;
}

const std::string& SDIFEntity::GetTypeString() const
{
    return mDescription;
}

int SDIFEntity::PrintTypes()
{
    std::cout << std::endl <<  mDescription << std::endl;
    return 1;
}

/* for adding a frame type */
bool SDIFEntity::AddFrameType(const std::string& frametype, 
			     const std::string& matrixtype)
{
    mDescription += "1FTD "+frametype + " { " + matrixtype + ";  }\n";
    return true;
}

/* for adding a frame type */
bool SDIFEntity::AddFrameType(const FrameType& frametype) 
{
  if(frametype.GetSignature()== eEmptySignature ||
     frametype.GetNbMatrices() == 0 ) {
    throw TypeError(eError,"Error in SDIFEntity::AddFrameType::uninitialized frametype ",0,eUnknown,0,0);
  }
  mDescription += "1FTD ";
  mDescription += SdifSignatureToString(frametype.GetSignature());
  mDescription += " { ";
  for(int ii=0; ii < frametype.GetNbMatrices();++ii) {
    mDescription += SdifSignatureToString(frametype.mvMatrixTypes[ii].GetSignature());
    mDescription += " ";
    mDescription += frametype.mvMatrixNames[ii];
    mDescription += ";";
  }
  mDescription += " }\n";
  return true;
}

/* for adding a matrix type */
bool SDIFEntity::AddMatrixType(const std::string& matrixtype, 
			      const std::string& colnames)
{
    mDescription += "1MTD "+matrixtype + " { " + colnames + " }\n";
    return true;
}

/* for adding a frame type */
bool SDIFEntity::AddMatrixType(const MatrixType& matrixtype)
{
  if(matrixtype.GetSignature()== eEmptySignature ||
     matrixtype.GetNbCols() ==0 ) {
    throw TypeError(eError,"Error in SDIFEntity::AddMatrixType::uninitialized matrixtype ",0,eUnknown,0,0);
  }
  mDescription += "1MTD ";
  mDescription += SdifSignatureToString(matrixtype.GetSignature());
  mDescription += " { ";
  for(int ii=0; ii < matrixtype.GetNbCols();++ii) {
    mDescription += matrixtype.mvColumnNames[ii];
    if(ii < matrixtype.GetNbCols() - 1)
      mDescription += ", ";      
  }
  mDescription += " }\n";
  return true;
}

void SDIFEntity::GetTypes(std::vector<FrameType>& frametypes) const {
  frametypes.clear();
  std::string::size_type pos =0;
  std::string tmpStr;
  const char* str = mDescription.c_str();
  FrameType frty;
  while((pos = mDescription.find("1FTD",pos) ) !=  std::string::npos) {
    pos += 4;    
    std::string::size_type pose = mDescription.find("}",pos);
    if(pose == std::string::npos)
      throw TypeError(eError,"Error in SDIFEntity::GetTypes::uncomplete frametype ",0,eUnknown,0,0);
    std::string framestring = mDescription.substr(pos,pose+1);
    std::string::size_type startsigpos = mDescription.find_first_not_of(" \n\t",pos);
    std::string::size_type endsigpos   = mDescription.find_first_of(" {\n\t",startsigpos);
    if(endsigpos - startsigpos != 4) 
      throw TypeError(eError,"Error in SDIFEntity::GetTypes::invalid frame signature",0,eUnknown,0,0);
    frty.Clear();
    frty.SetSignature(_SdifStringToSignature(str+startsigpos));
    std::string::size_type startMatPos, endMatPos, startCommentPos, endCommentPos = endsigpos;
    while((startMatPos = mDescription.find_first_not_of("{ \t\n,;",endCommentPos)) != pose) {
      endMatPos = mDescription.find_first_of(" \n\t,;",startMatPos);
      if(endMatPos - startMatPos != 4) 
        throw TypeError(eError,"Error in SDIFEntity::GetTypes::invalid frame signature",0,eUnknown,0,0);
      startCommentPos = mDescription.find_first_not_of(" \n\t,;",endMatPos);
      endCommentPos = mDescription.find_first_of(" \n\t,;}",startCommentPos);
      if(endCommentPos - startCommentPos == 0) 
        throw TypeError(eError,"Error in SDIFEntity::GetTypes::invalid frame signature",0,eUnknown,0,0);
      tmpStr = str+startCommentPos;
      tmpStr.resize(endCommentPos - startCommentPos);
      frty.AddMatrixType(MatrixType(_SdifStringToSignature(str+startMatPos)),tmpStr.c_str());      
    }
    frametypes.push_back(frty);
  }
}

void SDIFEntity::GetTypes(std::vector<MatrixType>& matrixtypes) const {
  matrixtypes.clear();
  std::string::size_type pos = 0;
  std::string tmpStr;
  const char* str = mDescription.c_str();
  MatrixType maty;
  while((pos = mDescription.find("1MTD",pos) ) !=  std::string::npos) {
    pos += 4;
    std::string::size_type pose = mDescription.find("}",pos);
    if(pose == std::string::npos)
      throw TypeError(eError,"Error in SDIFEntity::GetTypes::uncomplete matrixtype ",0,eUnknown,0,0);
    std::string matrixstring = mDescription.substr(pos,pose+1);
    std::string::size_type startsigpos = mDescription.find_first_not_of(" \n\t",pos);
    std::string::size_type endsigpos   = mDescription.find_first_of(" {\n\t",startsigpos);
    if(endsigpos - startsigpos != 4) 
      throw TypeError(eError,"Error in SDIFEntity::GetTypes::invalid matrix signature",0,eUnknown,0,0);
    maty.Clear();
    maty.SetSignature(_SdifStringToSignature(str+startsigpos));
    std::string::size_type  startCommentPos, endCommentPos = endsigpos;
    while((startCommentPos = mDescription.find_first_not_of("{ \t\n,;",endCommentPos)) != pose) {
      endCommentPos = mDescription.find_first_of(" \n\t,;}",startCommentPos);
      if(endCommentPos - startCommentPos == 0) 
        throw TypeError(eError,"Error in SDIFEntity::GetTypes::invalid matrix signature",0,eUnknown,0,0);
      tmpStr = str+startCommentPos;
      tmpStr.resize(endCommentPos - startCommentPos);
      maty.AddColumn(tmpStr.c_str());      
    }
    matrixtypes.push_back(maty);
  }
}

/* for adding the description types when opening in mode "eWriteFile" */
bool SDIFEntity::WriteTypes() 
{
  if (mDescription.size() > 0)
  {
    SdifStringT * _tmp=SdifStringNew();
    SdifStringAppend(_tmp,mDescription.c_str());
    SdifFGetAllTypefromSdifString(efile, _tmp);
    SdifStringFree(_tmp);
    return true;
  }
  else
    return false;
}


bool SDIFEntity::ChangeSelection(const std::string& selection) 
{
  if (!efile) return false;
 
  if(isFrameDirEnabled)
    throw Easdif::FrameDirError(eError,
                                "ChangeSelection can not be called after EnableFrameDir",GetFile(),
                                eUnknown,
                                  __FILE__,__LINE__);


  const char* sel = const_cast<const char*>(selection.c_str());
  SdifReplaceSelection(sel, efile->Selection);  
  return true;
}

bool SDIFEntity::MergeSelection(const std::string& selection) 
{
  if (!efile) return false;
  if(isFrameDirEnabled)
    throw Easdif::FrameDirError(eError,
                                "ChangeSelection can not be called after EnableFrameDir",GetFile(),
                                eUnknown,
                                __FILE__,__LINE__);


  const char* sel = const_cast<const char*>(selection.c_str());
  SdifParseSelection(efile->Selection,sel);
  return true;
}

bool SDIFEntity::ClearSelection(Easdif::SDIFEntity::SelectionPartsE part)
{
  if(!efile) return false;

  if(isFrameDirEnabled)
    throw Easdif::FrameDirError(eError,
                                "ChangeSelection can not be called after EnableFrameDir",GetFile(),
                                eUnknown,
                                __FILE__,__LINE__);

  switch(part) {
  case eSP_Stream:    
    SdifInitIntMask(&efile->Selection->streammask);
    SdifMakeEmptyList(efile->Selection->stream);    
    break;
  case eSP_Frame:
    SdifMakeEmptyList(efile->Selection->frame);   
    break;
  case eSP_Matrix:
    SdifMakeEmptyList(efile->Selection->matrix);   
    break;
  case eSP_Time:
    SdifMakeEmptyList(efile->Selection->time);   
    break;
  case eSP_Row:
    SdifInitIntMask(&efile->Selection->rowmask);
    SdifMakeEmptyList(efile->Selection->row);    
    break;
  case eSP_Column:
    SdifMakeEmptyList(efile->Selection->column);    
    SdifInitIntMask(&efile->Selection->colmask);
    break;      
  case eSP_All:
  default:
    SdifMakeEmptyList(efile->Selection->stream);    
    SdifMakeEmptyList(efile->Selection->frame);   
    SdifMakeEmptyList(efile->Selection->matrix);   
    SdifMakeEmptyList(efile->Selection->time);   
    SdifMakeEmptyList(efile->Selection->column);    
    SdifMakeEmptyList(efile->Selection->row);    
    
    SdifInitIntMask(&efile->Selection->colmask);
    SdifInitIntMask(&efile->Selection->rowmask);
    SdifInitIntMask(&efile->Selection->streammask);
    
    break;
  }
  return true;
}

bool SDIFEntity::GetMatrixSelection(SelectionSet<SdifSignature> &out) const
{
  out.clear();
  if(efile ==0) return false;

  if(!isFrameDirEnabled || !msHighLevelMatrixSelection.isActive() ) {
    SdifListT* listsel;    
    SdifSignature sig = eEmptySignature;
    listsel = efile->Selection->matrix;
    SdifListInitLoop (listsel);
    while (SdifListIsNext (listsel))      {
      sig = ((SdifSelectElementT *) SdifListGetNext 
             (listsel))->value.signature;
      out.insert(sig);
    }
    // an emptySignature signals that the selection does not contain anything so it can be savely
    // removed from the set which remains active but empty
    out.erase(eEmptySignature);    
  }
  else {
    out.insert(msHighLevelMatrixSelection.begin(),msHighLevelMatrixSelection.end());
  }
  return true;
}

bool SDIFEntity::GetFrameSelection(SelectionSet<SdifSignature> &out) const
{
  out.clear();
  if(efile ==0) return false;

  if(!isFrameDirEnabled || !msHighLevelFrameSelection.isActive() ) {
    SdifListT* listsel;    
    SdifSignature sig = eEmptySignature;
    listsel = efile->Selection->frame;
    SdifListInitLoop (listsel);
    while (SdifListIsNext (listsel))      {
      sig = ((SdifSelectElementT *) SdifListGetNext 
             (listsel))->value.signature;
      out.insert(sig);
    }
    // an emptySignature signals that the selection does not contain anything so it can be savely
    // removed from the set which remains active but empty
    out.erase(eEmptySignature);
  }
  else {
    out.insert(msHighLevelFrameSelection.begin(),msHighLevelFrameSelection.end());
  }
  return true;
}

bool SDIFEntity::GetStreamSelection(SelectionSet<unsigned int> &out) const
{
  out.clear();
  if(efile ==0) return false;

  if(!isFrameDirEnabled || !msHighLevelStreamSelection.isActive() ) {    
    SdifSelectIntMaskT &mask = efile->Selection->streammask;
    if(mask.num == 0)
      return true;
    int len = mask.max+1;
    for(int ii=0;ii<len;++ii)
      if(mask.mask[ii])
        out.insert(ii);
    if( mask.openend)
      out.setOpen();
  }
  else {
    out = msHighLevelStreamSelection;
  }
  return true;
}


bool SDIFEntity::TestMatrixSelection(SdifSignature sig) const
{
  if(efile ==0) return false;

  if(!isFrameDirEnabled || !msHighLevelMatrixSelection.isActive() ) {
    SdifListT* listsel;    
    listsel = efile->Selection->matrix;
    SdifListInitLoop (listsel);
    int cnt = 0;
    while (SdifListIsNext (listsel))      {
      if(sig == ((SdifSelectElementT *) SdifListGetNext(listsel))->value.signature)
        return true;      
      ++ cnt;
    }
    if(cnt) 
      return false;
    return true;
  }

  // we have an active highlevel selection
  return msHighLevelMatrixSelection.isSelected(sig);

}

bool SDIFEntity::TestFrameSelection(SdifSignature sig) const
{
  if(efile ==0) return false;

  if(!isFrameDirEnabled || !msHighLevelFrameSelection.isActive() ) {
    SdifListT* listsel;    
    listsel = efile->Selection->frame;
    SdifListInitLoop (listsel);
    int cnt = 0;
    while (SdifListIsNext (listsel))
      {
        if(sig == ((SdifSelectElementT *) SdifListGetNext(listsel))->value.signature)
          return true;      
        ++ cnt;
      }
    if(cnt) 
      return false;
    return true;
  }

  // we have an active highlevel selection
  return msHighLevelFrameSelection.isSelected(sig);

}


bool SDIFEntity::TestStreamSelection(unsigned int streamid) const
{
  if(efile ==0) return false;

  if(!isFrameDirEnabled || !msHighLevelStreamSelection.isActive() ) {
    return SdifSelectTestIntMask(&efile->Selection->streammask,streamid)!=0;
  }

  // we have an active highlevel selection
  return ( msHighLevelStreamSelection.isSelected(streamid));

}


bool SDIFEntity::CreateSignatureSelection(SdifListT* listsel,
                                          const SelectionSet<SdifSignature>& sigs) {
  if(!SdifListIsEmpty (listsel))
    SdifMakeEmptyList(listsel);  
  std::set<SdifSignature>::const_iterator beg = sigs.begin();
  std::set<SdifSignature>::const_iterator end = sigs.end();

  while(beg!=end) {
    SdifSelectElementT *elem = SdifMalloc (SdifSelectElementT);
    elem->rangetype = sst_norange;
    elem->value.signature = *beg++;
    SdifListPutTail (listsel, elem);          
  }
  return true;
}


bool SDIFEntity::CreateStreamSelection(SdifSelectionT *sel,
                                       const SelectionSet<unsigned int>& streams) {
  
  SdifListT* listsel = sel->stream;
  if(!SdifListIsEmpty (listsel))
    SdifMakeEmptyList(listsel);  

  if(streams.isActive()) {
    if(streams.empty()) {
      return CreateEmptyStreamSelection(sel);
    }
    else {
      std::set<unsigned int>::const_iterator beg = streams.begin();
      std::set<unsigned int>::const_iterator end = streams.end();
    
      while(beg!=end) {
        SdifSelectElementT *elem = SdifMalloc (SdifSelectElementT);
        elem->rangetype = sst_norange;
        elem->value.signature = *beg++;
        SdifListPutTail (listsel, elem);          
      }
      if (sel->streammask.mask) {  
        SdifFree(sel->streammask.mask);
        SdifInitIntMask(&sel->streammask);
      }
      SdifSelectGetIntMask (listsel, &sel->streammask);
      sel->streammask.openend = streams.isOpen();
    }
  }

  return true;
}

bool SDIFEntity::CreateEmptyStreamSelection(SdifSelectionT *sel) {

  SdifListT* listsel = sel->stream;
  if(!SdifListIsEmpty (listsel))
    SdifMakeEmptyList(listsel);  

  if (sel->streammask.mask) {  
    SdifFree(sel->streammask.mask);
  }

  SdifInitIntMask(&sel->streammask);
  sel->streammask.mask = SdifCalloc(int, 2);
  sel->streammask.max  = 1;
  sel->streammask.openend  = false;
  return true;
}


bool SDIFEntity::RestrictFrameSelection(const SelectionSet<SdifSignature>& sigs) {

  if(efile ==0 ) return false;
  if(!sigs.isActive())
    return true;
  if(isFrameDirEnabled ) {
    SelectionSet<SdifSignature> tmp;
    GetFrameSelection(tmp);
    msHighLevelFrameSelection.clear(false);
    if(tmp.isActive()) {
      std::set_intersection(tmp.begin(), tmp.end(),
                            sigs.begin(),sigs.end(),
                            std::inserter(msHighLevelFrameSelection,
                                          msHighLevelFrameSelection.begin()));
    }
    else{
      msHighLevelFrameSelection.insert(sigs.begin(),sigs.end());
    }
    return true;
  } 


  SdifListT* framesel = efile->Selection->frame;
  
  // no current selection any more, can only happen if we did not apply 
  // any restriction because after restricting the List is either containing the
  // list of signatures that are active or a signature eEmptySignature to signal
  // that nothing is selected at all.
  if(SdifListIsEmpty (framesel)) {
    // there was no selection at all so we may 
    // restrict by just adding
    if(msFrameSelection.empty())
      return CreateSignatureSelection(framesel,sigs);
    // this can only happen if somebody dealed with the selections without using 
    // RestrictFrameSelection ReestablishFrameSelection, we cannot handle this correctly
    else 
      return false;
  }
  
  
  SelectionSet<SdifSignature> before,intersect;
  GetFrameSelection(before);
  set_intersection(before.begin(),before.end(),
                   sigs.begin(),sigs.end(),std::inserter(intersect,intersect.begin()));
  if(intersect.empty())
    intersect.insert(eEmptySignature);
  return CreateSignatureSelection(framesel,intersect);
}

bool SDIFEntity::ReestablishFrameSelection() 
{
  if(efile ==0) return false;
  if(isFrameDirEnabled) {
    msHighLevelFrameSelection.clear(true);
    return true;
  }

  SdifListT* framesel = efile->Selection->frame;
  return CreateSignatureSelection(framesel,msFrameSelection);  
}


bool SDIFEntity::RestrictMatrixSelection(const SelectionSet<SdifSignature>& sigs) {

  if(efile ==0) return false;
  if(!sigs.isActive())
    return true;

  if(isFrameDirEnabled ) {
    SelectionSet<SdifSignature> tmp;
    GetMatrixSelection(tmp);
    msHighLevelMatrixSelection.clear(false);
    if(tmp.isActive()) {
      std::set_intersection(tmp.begin(), tmp.end(),
                            sigs.begin(),sigs.end(),
                            std::inserter(msHighLevelMatrixSelection,
                                          msHighLevelMatrixSelection.begin()));
    }
    else{
      msHighLevelMatrixSelection.insert(sigs.begin(),sigs.end());
    }

    return true;
  } 

  SdifListT* matrixsel = efile->Selection->matrix;
  
  // no current selection any more, can only happen if we did not apply 
  // any restriction because after restricting the List is either containing the
  // list of signatures that are active or a signature eEmptySignature to signal
  // that nothing is selected at all.
  if(SdifListIsEmpty (matrixsel)) {
    // there was no selection at all so we may 
    // restrict by just adding
    if(msMatrixSelection.empty())
      return CreateSignatureSelection(matrixsel,sigs);
    // this can only happen if somebody dealed with the selections without using 
    // RestrictMatrixSelection ReestablishMatrixSelection, we cannot handle this correctly
    else 
      return false;
  }
  
  // selection is not yet empty, we need to match it with the input
  
  
  SelectionSet<SdifSignature> before,intersect;
  GetMatrixSelection(before);
  set_intersection(before.begin(),before.end(),sigs.begin(),sigs.end(),
                   std::inserter(intersect,intersect.begin()));
  if(intersect.empty())
    intersect.insert(eEmptySignature);
  return CreateSignatureSelection(matrixsel,intersect);
}

bool SDIFEntity::ReestablishMatrixSelection() 
{
  if(efile ==0) return false;
  if(isFrameDirEnabled) {
    msHighLevelMatrixSelection.clear(true);
    return true;
  }

  SdifListT* matrixsel = efile->Selection->matrix;
  return CreateSignatureSelection(matrixsel,msMatrixSelection);  
}


bool SDIFEntity::RestrictStreamSelection(const SelectionSet<unsigned int>& streamid) {

  if(efile ==0) return false;
  if(!streamid.isActive())
    return true;
  
  if(isFrameDirEnabled ) {
     SelectionSet<unsigned int> tmp;
     GetStreamSelection(tmp);
     msHighLevelStreamSelection.clear(false);
     if(tmp.isActive()) {
       std::set_intersection(tmp.begin(), tmp.end(),
                             streamid.begin(),streamid.end(),
                             std::inserter(msHighLevelStreamSelection,
                                           msHighLevelStreamSelection.begin()));
     }
     else{
       msHighLevelStreamSelection.insert(streamid.begin(),streamid.end());
     }

     return true;
  } 
  
  
  // no current selection any more, can only happen if we did not apply 
  // any restriction because after restricting the List is either containing the
  // list of signatures that are active or a signature eEmptySignature to signal
  // that nothing is selected at all.
  if(!SdifFNumStreamsSelected(efile)) {
    // there was no selection at all so we may 
    // restrict by just adding
    if(msStreamSelection.empty())
      return CreateStreamSelection(efile->Selection,streamid);
    // this can only happen if somebody dealed with the selections without using 
    // RestrictMatrixSelection ReestablishMatrixSelection, we cannot handle this correctly
    else 
      return false;
  }
  
  // selection is not yet empty, we need to match it with the input
  
  // nothing to filter
  if(streamid.empty())
    return CreateEmptyStreamSelection(efile->Selection);

  SdifSelectIntMaskT *mask = &efile->Selection->streammask;
  
  SelectionSet<unsigned int> before,intersect;
  GetStreamSelection(before);
  SelectionSet<unsigned int>::const_iterator send= streamid.end();
  // ignore the fact that streamid may be open as well !!
  // this is not yet finished!!
  if(before.isOpen())
    send=streamid.upper_bound(mask->max);
  set_intersection(before.begin(),before.end(),streamid.begin(),send,
                   std::inserter(intersect,intersect.begin()));
  intersect.insert(send,streamid.end());
  
  if(intersect.empty())
    return CreateEmptyStreamSelection(efile->Selection);
  return CreateStreamSelection(efile->Selection,intersect);
  
}

bool SDIFEntity::ReestablishStreamSelection() 
{
  if(efile ==0) return false;
  if(isFrameDirEnabled) {
    msHighLevelStreamSelection.clear(true);
    return true;
  }

  return CreateStreamSelection(efile->Selection,msStreamSelection);  
}



/*****************************************************/

} // end of namespace Easdif
