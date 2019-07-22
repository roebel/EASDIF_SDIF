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
 * @file   sdifentity.h
 * @author Fabien Tisserand
 * @date   Tue Jun 18 20:12:40 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifentity.hpp,v 1.30 2014-10-17 14:41:47 fcornu-ircam Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.29  2014/06/06 15:30:54  roebel
 * Clarified documentation or Entity::ReadNextFrame, Entity::ReadNextSelectedFrame and fixed implementation such that it works for frames not containing any matrices being located at the end of the SDIF file.
 * This fixes a bug notably in the python Entity iterator that would not retriev empty frames located at the end of an SDIF file.
 *
 * Revision 1.28  2013/11/19 15:24:31  fcornu-ircam
 * remove EASDIF_API from FRIterator which is fully inline
 *
 * Revision 1.27  2012/08/28 22:02:39  roebel
 * Renamed function arguments to avoid compiler warnings in swig python interface.
 * Enhanced documentation of ReadNextSelectedFrame()
 *
 * Revision 1.26  2012/08/10 01:03:29  roebel
 * Added new function that allows to retrieve easdif version during runtime.
 * Fixed ReadNextSelectFrame when time argument is provided and the time is the same or smaller as the first frame,
 * which would result in oscillating movements of the read frames between first and second frame for repeatedly asking to
 * read the frame with first frames time.
 *
 * Revision 1.25  2011/06/11 17:05:31  roebel
 * Moved some member functions out of class scope to avoid unnecessary inlining
 * of constructor, destructor or other costly functions.
 *
 * Revision 1.24  2010/04/16 10:39:44  roebel
 * Fixed/clarified documentation.
 *
 * Revision 1.23  2009/11/09 17:16:08  roebel
 * modify FRIterator declaration according to compiler.
 *
 * Revision 1.22  2009/10/29 16:10:23  roebel
 * Added some missing mutable and EASDIF_API keywords.
 *
 * Revision 1.21  2009/07/31 21:26:03  roebel
 * Replaced std::list<SDIFLocation> types by Directory typedef.
 *
 * Revision 1.20  2008/06/16 08:53:44  roebel
 * added missing include file string.h
 *
 * Revision 1.19  2008/05/31 22:53:13  roebel
 * Improved doc with respect to selection.
 *
 * Revision 1.18  2008/02/15 19:09:58  roebel
 * Added missing include file that defines std::find_if.
 *
 * Revision 1.17  2008/02/14 00:42:31  roebel
 * Renamed template parameter arguments that where likely to collide
 * with macro names.
 *
 * Revision 1.16  2008/01/23 12:11:23  roebel
 * Fixed typos in doc.
 *
 * Revision 1.15  2008/01/22 00:50:17  roebel
 * Added methods to ask for open state and to clear NVTs and Types.
 *
 * Revision 1.14  2008/01/12 11:58:33  roebel
 * Added '{' and '}' as forbidden characters when creating comments
 * for Matrix- and FrameType.
 *
 * Revision 1.13  2008/01/11 15:58:49  roebel
 * Added new class MatrixType, FrameType, and the realted function for
 * adding types to entity or retrieving types from an entity in terms of these
 * clases.
 *
 * Revision 1.12  2007/11/27 17:36:22  roebel
 * Replaced sdif macro by inline member function.
 *
 * Revision 1.11  2007/11/26 19:09:55  roebel
 * Fixed to avoid compiler warnings in MSVC.
 * Little problem is the export of std::containers that should be defined as export
 * which is not possible due to the given STL include files.
 * For the moment it seems these warnings are not important, because all these functions
 * are inlined as templates in the STL anyway. Has to be handled with care !!!!
 *
 * Revision 1.10  2007/10/25 22:31:20  roebel
 * Changed interface to AddNVT. AddNVT without explicit StreamID now uses
 * the streamid of the SDIFNameValueTable and no longer imposes a default value of 0.
 *
 * Revision 1.9  2007/04/30 11:31:37  roebel
 * Throw exception FrameDirError exception if  selection manipulation
 * functions or framedirectory functions are called in unsuitable states.
 * Replaced the concept of the current frame in the frame directory
 * by the concept of the last frame read which no opnger is guaranteed
 * to be selected.
 *
 * Revision 1.8  2006/04/22 11:48:09  roebel
 * Fixed left over problems from last renameing operation.
 *
 * Revision 1.7  2006/01/05 08:52:14  roebel
 * Added mutable for internal frame store (required by gcc 4.0 linux).
 *
 * Revision 1.6  2005/07/25 13:28:04  roebel
 * FRIterator now no longer a subclass of SDIFEntity. Subclasses
 * are not supported by swig! With the new design swig interface
 * works properly at least for swig 1.3.21.
 *
 * Revision 1.5  2005/06/03 18:37:37  roebel
 * Fixed docu.
 *
 * Revision 1.4  2005/06/02 22:23:27  roebel
 * Removed open set indicator which is now part of the selection sets.
 *
 * Revision 1.3  2005/05/31 19:46:55  bogaards
 * isSelected function and corrections to the selection tests
 *
 * Revision 1.2  2005/05/30 23:01:20  roebel
 * fixed error in gcc4 which  requires explicite naming of
 * derived base class functions.
 *
 * Revision 1.1  2005/05/30 21:46:18  roebel
 * Changed all include files from .h into .hpp to prevent name clash between
 * sdifmatix.h and SDIF/sdifcpp/SdifMatrix.h on MacOSX where filenames are
 * case insensitive.
 *
 * Added new class SelectionSet to handle SdifSelections in Easdif.
 * Problem is that a set can be empty meaning no selection or nothing is selected
 * because to sets with empty intersection have been applied in RestrictSelection functions.
 * The new class has a boolean isActive to distinguish these two cases.
 *
 * Revision 1.33  2005/05/30 18:16:10  bogaards
 * readnextselectedframe in iterator dereference, fixed bool for isframeselected
 *
 * Revision 1.32  2005/05/24 09:53:51  roebel
 * Changed selection management in Easdif:
 * Before EnableDirectory has been called selection
 * modification wqorks on the SDIF-Selection, after
 * enabling the directory a new high level selection mode
 * is used that can only be used to restrict the secltion by
 * forming intersections with the existing selections.
 * This mechanism exists now for stream/frame and matrix selections.
 *
 * Revision 1.31  2005/05/20 21:32:12  roebel
 * Increased consistence and documentation of SDIF frame directory
 * and SDIF frame iterator. The directory is now limited
 * to only the selected frames, streams and matrices.
 * Added new functions to restrict and reestablish
 * signature selections.
 * selections modifications are disabled whenever the
 * FrameDirectory is enabled and not yet finished.
 * Added member function to test for seekability.
 *
 * Revision 1.30  2005/05/03 16:31:48  roebel
 * Fixed documentation.
 *
 * Revision 1.29  2005/05/03 16:23:26  roebel
 * Added 2 new functions to handle selections in a more specific manner.
 *
 * Revision 1.28  2004/10/07 14:48:11  roebel
 * Replaced calls to resize(0) by clear() and
 * test using size() by !empty() to improve efficiency.
 *
 * Revision 1.27  2004/09/10 14:45:28  roebel
 * Hopefully fixed the private access problems with CodeWarrior.
 * Added access functions to current location FrameDirectory and directory
 * iterator.
 *
 * Revision 1.26  2004/09/10 10:58:21  roebel
 * Fixed signed unsigned compiler warnings
 *
 * Revision 1.25  2004/09/10 09:20:52  roebel
 * Extend frame directory to contain the matrix signatures for each frame.
 * No longer needs to re read the frame to decide whether frame is selected.
 *
 * Revision 1.24  2004/09/09 19:36:52  roebel
 * Made some members of the iterator private.
 *
 * Revision 1.23  2004/09/09 19:17:38  roebel
 * Version 1.0.0beta:
 * First complete version of iterator access when reading files. Frame-Iterators use the
 * internal Frame Directory that each Entity will generate and update on the fly
 * to minimize disk access during positioning.
 *
 * Revision 1.22  2004/09/08 09:15:57  roebel
 * Improved efficiency of FrameDirectory by means of
 * preventing the need to  search the complete directoy for each frame read.
 * FrameDir is now a list and there exists a current read position pointer into that list
 * that indicates where to search for insert positions.
 *
 * Revision 1.21  2004/08/25 09:22:11  roebel
 * SDIFEntity::Rewind(time) no longer private.
 *
 * Revision 1.20  2004/07/28 13:03:27  roebel
 * Fixed position type to be SdiffPosT
 *
 * Revision 1.19  2004/07/27 17:40:19  roebel
 * Changed include directive to use user path and not system path for sdif.h
 *
 * Revision 1.18  2004/07/21 13:20:24  roebel
 * Added support to hold a frameDirectory in the entity and
 * the possibility to read frames from a given time position.
 *
 * Revision 1.17  2004/07/13 15:01:42  roebel
 * Removed unused variable mNbFrames.
 *
 * Revision 1.16  2004/05/04 12:53:35  roebel
 * Fixed documentation.
 *
 * Revision 1.15  2004/02/11 19:28:17  roebel
 * Added inline function to test state of entity. Added function to access NVTs that indicates existance of name.
 *
 * Revision 1.14  2004/01/19 15:49:55  bogaards
 * Added Rewind and ReadNextSelectedFrame methods to Entity
 *
 * Revision 1.13  2003/08/06 18:00:33  roebel
 * Fixed documentation
 *
 * Revision 1.12  2003/07/18 21:19:36  roebel
 * Made WriteNVTs private. It is for internal use only.
 *
 * Revision 1.11  2003/07/18 20:39:35  roebel
 * Improved documentation
 *
 * Revision 1.10  2003/05/24 00:27:38  roebel
 * Internal representation of types now using std::string.
 * Parameters for type handling routines completely moved
 * to std::string
 *
 * SDIFEntity::SetTypeString no longer appends but sets
 * the internal string. In fact there was now way to reset
 * the type string before.
 *
 * Revision 1.9  2003/05/19 13:59:40  roebel
 * swig rename moved to swig  interface desription.
 *
 * Revision 1.8  2003/05/01 18:59:12  roebel
 * Added missing class keyword for friend declaration.
 *
 * Revision 1.7  2003/04/29 15:54:04  schwarz
 * Use SWIG_RENAME_EASDIF to control class renaming.
 *
 * Revision 1.6  2003/04/29 15:41:30  schwarz
 * Changed all names View* to Print* and *Info to *Header for consistency
 * with SDIF library.
 *
 * Revision 1.5  2003/04/18 17:43:22  schwarz
 * eof() const method
 *
 * Revision 1.4  2003/04/18 16:44:00  schwarz
 * Small changes to make easdif swiggable:
 * - name change for swig-generated classes
 * - eof() returns bool, not bool&
 * - Matrix::Set takes int/float, not int&/float&
 *
 * Revision 1.3  2003/04/08 17:26:04  roebel
 * Updated doc.
 *
 * Revision 1.2  2003/04/06 16:31:08  roebel
 * Added license info
 *
 * Revision 1.1  2003/03/03 19:00:16  roebel
 * Moved src directory to new name easdif
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
 * Revision 1.2  2002/07/12 10:33:56  ftissera
 * *** empty log message ***
 *
 * Revision 1.1  2002/06/18 18:44:14  ftissera
 * Project for new SDIF API
 *  ----------------------------------------------------------------------
 * 
 * 
 */

#ifndef SDIFENTITY_H_
#define SDIFENTITY_H_ 1

#include <string.h>
#include <string>
#include <list>
#include <set>
#include <algorithm>
#include <functional>
#include "sdif.h"

#include "easdif_exports.hpp"
#include "easdif/sdif_frame.hpp"
#include "easdif/sdifnamevaluetable.hpp"

/* for initialise SDIF library and install exception*/
#include "easdif/sdifinit.hpp"

namespace Easdif {

  struct SDIFLocation;
  class  SDIFEntity;

  /** 
   * \ingroup initialization 
   * \brief return EaSDIF Version string
   * 
   */
  EASDIF_API
  const char* Version();

  /**
   * \defgroup  directory SDIFEntity - Directory 
   * \brief The frame directory
   *
   * The sdif directory assembles the frame information about a file that 
   * has been opend for reading. It is created on the fly while reading the file
   * if the function Easdif::SDIFEntity::EnableFrameDir() has been called.
   * This function can be called only right after opening the file to prevent
   * any missing frames. The content of the directory will be updated
   * when the file is read. It  contains a struct Easdif::SDIFLocation entry
   * for all the frames and matrices that have been selected when
   * the directoy has been enabled.  The directory contains the information
   * found in the frame header as well as
   * the list of selected matrices in the frame using the SDIF selection 
   * that is valid  at the time when EnableFrameDir() has ben called.
   * 
   * The internal directory can be obtained  by means of 
   * Easdif::SDIFEntity::GetDirectory(). The use of the frame iterators 
   * Easdif::SDIFEntity::iterator and Easdif::SDIFEntity::const_iterator
   * rely on the directory, so you need to enable the directory if you want to 
   * use the iterators.
   *
   * The use of selection requires some attention. As long as the
   * FrameDirectory is not enabled the Selection functions \ref selection
   * work on the low level selection that has been initialized by the 
   * file name based  selection. 
   * After the framedirectory has been enabled only the following functions 
   * are allowed:
   * Easdif::SDIFEntity::RestrictMatrixSelection(const SelectionSet<SdifSignature>& sigs), and 
   * Easdif::SDIFEntity::RestrictFrameSelection(const SelectionSet<SdifSignature>& sigs)
   * Easdif::SDIFEntity::RestrictStreamSelection(const SelectionSet<SdifSignature>& sigs)
   *    Easdif::SDIFEntity::ReestablishStreamSelection(),
   *   Easdif::SDIFEntity::ReestablishFrameSelection(),
   * Easdif::SDIFEntity::ReestablishMatrixSelection().
   *
   * The restrict function create a selection that contains the intersection
   * of the existing selection and the set of elements that is given as 
   * argument. A result of such a restriction may be an empty set, which is signaled either
   * as a set containing  
   * The reestablish functions will reestablish the selection
   * that has ben active when the frame directory has been initiialized.
   */

  /**
   * \brief A class describing the SDIF selections
   * \ingroup selection
   *
   * This class contains a set of selected entities and a boolean. The boolean  is necessary due to
   * the fact that a selection resulting from the SDIFEntity::RestrictMatrixSelection()
   * SDIFEntity::RestrictFrameSelection(), or SDIFEntity::RestrictStramSelection() calls
   * may be an empty selection, while an empty selection in SDIF means nothing
   * is selected.
   *
   * The SelectionSet is derived from a std::set and all methods of the std::set<TYPE> can be applied.
   */
  template<class TYPE>
  class   EASDIF_API SelectionSet : public std::set<TYPE> {
    bool mlActive;
    bool mlOpen;
  public:

    /** 
     *  \brief default constructor 
     * \ingroup selection
     * 
     *  creates an empty inactive selection
     *
     * @return 
     */ 
    SelectionSet()  :  mlActive(false), mlOpen(false)   { }

    /** 
     * \brief create a selection set 
     * \ingroup selection
     *
     *  if the given list of elements is not empty the selection will be enabled;
     * it is not possible to create a active SelectionSet that has no elements 
     * selected (an empty set). Use clear(false) after creation for this.
     *
     * @param inset   set to copy elements from
     * @param _open   if given and true this signals all the elements
     *  after the last element will be included in the set.
     * 
     * @return 
     */
    SelectionSet(std::set<TYPE> &inset,bool _open=false)
      :  std::set<TYPE>(inset),mlActive(false),mlOpen(false)  { 
      if (!std::set<TYPE>::empty())
        mlActive=true;
    }

    /** 
     * \brief Test whether a selection has been established
     * 
     * If the selection has been established (function returns true)
     * the entities in the set are used to filter 
     * the sdif data. The data returned by ReadNextSelectedFrame
     * will contain only those elements that match the values in the selected set. 
     * If no selection has been established the set is not used to filter data.
     *
     * The test for an empty selection (which will not pass any data is)
     \code
     SdifSelection<TYPE> sel; 
     sel.isActive() && sel.empty()
     \endcode
     *
     * @return indicates whether selection set is used to filter data 
     */
    bool isActive() const {return mlActive;}
    
    /** 
     * \brief test for open set
     * \ingroup selection
     *
     * test for open selection , openselections are only sensible for 
     * streamid selection where the < operator makes sense.
     *
     * @return true if the selection is an open set which means all elements
     * after the last element will be included
     */
    bool isOpen() const {return mlOpen;}


    /** 
     * \brief make an open selection
     * \ingroup selection
     *
     * an open selection can be used that all elements after the last element in the selection
     * are selected (makes only snese for stream ids);
     */
    void setOpen() {mlOpen =true;}

	/** 
     * \brief test whether element is selected
     * \ingroup selection
     *
     * test whether element is selected
     *
     * @return true if the element is found in the set or the selection is not active, or, for an open selection, the element
     * is larger than the last selected element
     */
    bool isSelected(const TYPE inType) const {
      return (!isActive() || std::set<TYPE>::find(inType) != std::set<TYPE>::end() || (!std::set<TYPE>::empty() && isOpen() && inType > *--std::set<TYPE>::end()));
    }

    /** 
     * \brief add an element into the selection set
     * \ingroup selection
     *
     * the element will be added into the selection an the selection will be 
     * automatically enabled
     * 
     * @param __x element to select
     * 
     * @return  pair containing iterator to new element and boolean indicating whether element
     *         has been added.
     */
    std::pair<typename std::set<TYPE>::iterator,bool> 
    insert(const TYPE& __x) { 
      mlActive = true;
      return std::set<TYPE>::insert(__x);
    }

    /** 
     * \brief add an element into the selection set
     * \ingroup selection
     *
     * the element will be added into the selection an the selection will be 
     * automatically enabled
     * 
     * @param __position iterator giving a hint where the element has to be inserted
     * @param __x element to select
     * 
     * @return  set iterator to new element 
     *        
     */
    typename std::set<TYPE>::iterator
    insert(typename std::set<TYPE>::iterator __position, const TYPE& __x) {
      mlActive = true;
      return std::set<TYPE>::insert(__position,__x);
    }

    /** 
     *  \brief add an element into the selection set
     *  \ingroup selection
     *
     *  Add range of elements into the set and enable the set.
     *
     * @param __first first element to add into the set.
     * @param __last  after the end element of the arange to add  into the set.
     *
     */
    template <class _InputIterator>
    void insert(_InputIterator __first, _InputIterator __last) {
      mlActive = true;
      std::set<TYPE>::insert(__first,__last);
    }

    /** 
     * \brief clear the selection
     * \ingroup selection
     * 
     * \param deactivate if true the selection is cleared and deactivated signaling
     * that no selection is performed. If deactivate is given and false
     * the set is cleared but the selection remains active such that no element is selected.
     *
     *  clear all elements from the set and disable the selection
     */
    void clear(bool deactivate=true)      { 
      mlActive = !deactivate;
      std::set<TYPE>::clear();
      mlOpen=false;
    }
  };

  /** 
   * \ingroup description
   * \brief matrix type descriptor
   * 
   */
  struct EASDIF_API  MatrixType {
    SdifSignature mSig;
    std::vector<std::string> mvColumnNames;
    MatrixType() :mSig(eEmptySignature) {}
    MatrixType(SdifSignature sig) :mSig(sig) {}
    ~MatrixType();

    /** 
     * \ingroup description
     * \brief add new column to matrix type 
     * 
     * @param column_name 
     */
    void AddColumn(const char *column_name) {
      
      if(!column_name || !*column_name) 
        throw  TypeError(eError,"Error in MatrixType::AddColumn:: empty column",0,eUnknown,0,0);

      const char *ic = column_name;
      const char *ie = column_name+strlen(column_name);
      while(ic != ie) {
        if(*ic == ' ' ||*ic == ';' || *ic == ','|| *ic == '}'|| *ic == '{')
          throw TypeError(eError,"Error in MatrixType::AddColumn:: invalid character in column_name",0,eUnknown,0,0);
        ++ic;
      }
      mvColumnNames.push_back(column_name);
    }

    /** 
     * \ingroup description
     * \brief set signature
     * 
     * @param sig 
     */
    void SetSignature(SdifSignature sig) {
      mSig = sig;
    }

    /** 
     * \ingroup description
     * \brief reset matrix type 
     * 
     */
    void Clear() {
      mSig = eEmptySignature;
      mvColumnNames.clear();
    }

    /** 
     * \ingroup description
     * \brief get number of columns in the matrix type
     * 
     * @return number of columns in current frame type (does not include columns in predefined matrix type 
     * of the same signature)
     */
    int GetNbCols() const {return mvColumnNames.size();};

    /** 
     * \ingroup description
     * \brief get signature related to MatrixType
     * @return signature
     */
    SdifSignature GetSignature() const { return mSig;}
 };

  /** 
   * \ingroup description
   * \brief frame type descriptor
   * 
   */
  struct EASDIF_API FrameType {
    SdifSignature mSig;
    std::vector<std::string> mvMatrixNames;
    std::vector<MatrixType>  mvMatrixTypes;

    FrameType(SdifSignature sig) :mSig(sig) {}
    FrameType()                  : mSig(eEmptySignature) {}
    ~FrameType();

    struct equal_mat : std::equal_to<SdifSignature> {
      typedef MatrixType second_argument_type;
#if 1
      typedef MatrixType first_argument_type;
      bool operator()(const MatrixType&mat, const MatrixType&mat2) const{
        return std::equal_to<SdifSignature>::operator()(mat.GetSignature(),mat2.GetSignature());
      }
#else
      typedef SdifSignature first_argument_type;
      bool operator()(const MatrixType&mat, SdifSignature mat2) const{
        return std::equal_to<SdifSignature>::operator()(mat.GetSignature(),mat2);
      }
#endif
    };
    /**
     * \ingroup description
     * \brief add matrix type to frame type
     * 
     * @param mat 
     * @param matrix_name 
     */
    void AddMatrixType(const MatrixType& mat, const char *matrix_name) {

      if(mat.mSig == eEmptySignature) 
        throw  TypeError(eError,"Error in FrameType::AddMatrixType::uninitialized matrix ",0,eUnknown,0,0);
      
      if(!matrix_name || !*matrix_name) 
        throw  TypeError(eError,"Error in FrameType::AddMatrixType:: empty column",0,eUnknown,0,0);

      if(std::find_if(mvMatrixTypes.begin(),mvMatrixTypes.end(),std::bind2nd(equal_mat(),mat)) != mvMatrixTypes.end())
        throw  TypeError(eError,"Error in FrameType::AddMatrixType:: matrix type exists.",0,eUnknown,0,0);

      const char *ic = matrix_name;
      const char *ie = matrix_name+strlen(matrix_name);
      while(ic != ie) {
        if(*ic == ' ' ||*ic == ';' || *ic == ','|| *ic == '}'|| *ic == '{')
          throw TypeError(eError,"Error in FrameType::AddMatrixType:: invalid character in matrix_name",0,eUnknown,0,0);
        ++ic;
      }
      mvMatrixNames.push_back(matrix_name);
      mvMatrixTypes.push_back(mat);
    }

    /** 
     * \ingroup description
     * \brief set signature
     * 
     * @param sig 
     */
    void SetSignature(SdifSignature sig) {
      mSig = sig;
    }

    /** 
     * \ingroup description
     * \brief reset frame type 
     * 
     */
    void Clear() {
      mSig = eEmptySignature;     
      mvMatrixTypes.clear();
      mvMatrixNames.clear();
    }

    /** 
     * \ingroup description
     * \brief get number of matrices in the frame type
     * 
     * @return number of matrices in current frame type (does not include matrices in predefined frame type 
     * of the same signature)
     */
    int GetNbMatrices() const {return mvMatrixTypes.size();};

    /** 
     * \ingroup description
     * \brief get signature related to FrameType
     * @return signature
     */
    SdifSignature GetSignature() const { return mSig;}
  };


  /** 
   * \brief SDIF Frame Directory tpye describing the contents of an SDIF File
   * \ingroup directory
   * 
   */
  typedef std::list<SDIFLocation> Directory;

  /**
   * \brief  A class that hold information about a single frame  in an SDIF File
   * \ingroup directory
   *  
   * The SDIFLocation class describes the frame in the file. 
   * It does not take into account
   * any selection.
   *
   */
  struct EASDIF_API SDIFLocation {

    /// file position for this location
    SdiffPosT        mPos;   
    /// the related frame header 
    SdifFrameHeaderS mFrameHdr; 
    /// First matrix signature contained in the frame or eEmptySignature 
    SdifSignature    mMatrixSig0;   
    /// Second matrix signature contained in the frame or eEmptySignature 
    SdifSignature    mMatrixSig1;
    /// Third matrix signature contained in the frame or eEmptySignature 
    SdifSignature    mMatrixSig2;
    
    /// fourth and more matrix signatures contained in the frame  
    std::vector<SdifSignature> mMatrixN;

    /// Default constructor
    SDIFLocation():mPos(-1){      
      mMatrixSig0 = mMatrixSig1 = mMatrixSig2 =
        mFrameHdr.Signature = eEmptySignature;
      mFrameHdr.Size      = 0;
      mFrameHdr.NbMatrix  = 0;
      mFrameHdr.NumID     = 0;
      mFrameHdr.Time      = -1.;
    }

    /// constructor
    SDIFLocation(SdifUInt4 _pos,SdifUInt4 _id, SdifFloat8 _time, 
                 SdifSignature _sig,  SdifUInt4  _nmatrix   )
      :mPos(_pos)  {
      mMatrixSig0 = mMatrixSig1 = mMatrixSig2 = eEmptySignature;
      mFrameHdr.Signature =   _sig;
      mFrameHdr.Size      = 0;
      mFrameHdr.NbMatrix  = _nmatrix;
      mFrameHdr.NumID     = _id;
      mFrameHdr.Time      = _time;
      if(_nmatrix >3)
        mMatrixN.resize(_nmatrix-3,eEmptySignature);
    }

    /// Get StreamID of current location
    SdifUInt4      LocStreamID()  const { return mFrameHdr.NumID;}
    /// Get Number of Matrices of current frame, this is the number of matrices in the file independent of any high or low level selection
    SdifUInt4      LocNbMatrix()  const { return mFrameHdr.NbMatrix;}
    /// Get Time of current location
    SdifFloat8     LocTime()      const { return mFrameHdr.Time; }
    /// Get FrameSignature of current location
    SdifSignature  LocSignature() const { return mFrameHdr.Signature;}
    /// Get File Position of current location
    SdiffPosT      LocPos()       const { return mPos;}
    /** 
     * \brief  Get Matrix Signature of matrix(ind) of current location
     * 
     * @param ind  index of matrix to get signature for
     * 
     * @return matrix signature or eEmptySignature if matrix is not available
     */
    SdifSignature  LocMSignature(SdifUInt4 ind)     const { 
      if(ind >= mFrameHdr.NbMatrix)
        return eEmptySignature;
      switch(ind) {
      case 0 : return mMatrixSig0; break;
      case 1 : return mMatrixSig1; break;
      case 2 : return mMatrixSig2; break;
      default: break;
      }
      return mMatrixN[ind-3];
    }

    /** 
     * \brief  test existance of  Matrix with given signature
     * 
     * @param in  signature to look for
     * 
     * @return true if found
     */
    bool  LocMatrixExists(SdifSignature inSig)     const { 
      int nn = mFrameHdr.NbMatrix;
      if(!nn)
        return false;
      if(nn && mMatrixSig0 == inSig)
        return true;
      if(nn>1 && mMatrixSig1 == inSig)
        return true;
      if(nn>2 && mMatrixSig2 == inSig)
        return true;
      nn -= 2;
      while(--nn)
        if(mMatrixN[nn] == inSig)
          return true;
      return false;
    }

    /** 
     * \brief  test existance of  Matrix with given signature
     * 
     * @param in  string containing the signature to look for
     * 
     * @return true if found
     */
    bool  LocMatrixExists(const std::string &inStr)     const { 
      SdifSignature sig = SdifStringToSignature(const_cast<char*>(inStr.c_str()));
      return LocMatrixExists(sig);
    }

    /** 
     *  \brief  Set Matrix Signature of matrix(ind) of current location
     * 
     * @param ind  index of matrix to set signature for
     * @param _sig matrix signature to use
     */
    void SetMSignature(SdifUInt4 ind,SdifSignature _sig ){
      if( ind >= mFrameHdr.NbMatrix){
        std::cerr << " SDIFLocation:: SetMSignature ind out of bounds "<< ind << "\n";
        return ;
      }
      switch(ind) {
      case 0 : mMatrixSig0 =_sig; break;
      case 1 : mMatrixSig1 =_sig; break;
      case 2 : mMatrixSig2 =_sig; break;
      default : mMatrixN[ind-3] =_sig; break;
      }
    }
  };

namespace {
  // inversion of const tag for elem_iterator classes
  template<int EASDIF_SC_CONST_FLAG>
  struct swap_const {
    enum {INVERTED_CONST_FLAG=1};
  };
  
  template<>
  struct swap_const<1> {
    enum {INVERTED_CONST_FLAG=0};
  };

  template<int EASDIF_SC_CONST_FLAG>
  struct Base_Iterator {
    typedef  Directory::iterator basic_iterator;
  };

  template<>
  struct Base_Iterator<1> {
    typedef Directory::const_iterator basic_iterator;
  };

  template<int EASDIF_SC_CONST_FLAG>
  struct IteratorTypes {
    typedef Easdif::SDIFFrame    value_type;
    typedef Easdif::SDIFFrame   *pointer;
    typedef Easdif::SDIFFrame&   reference;    
  };

  template<>
  struct IteratorTypes<1> {
    typedef const Easdif::SDIFFrame   value_type;
    typedef const Easdif::SDIFFrame  *pointer;
    typedef const Easdif::SDIFFrame&  reference;    
  };
}

  // forward declaration
  template<int EASDIF_FR_CONST_FLAG>
  class FRIterator;

  /** 
   * @brief class holding all information concerned with a single sdif file.
   *
   * SDIFEntity is composed of different methods that allow the handling of an 
   * sdif-file. 
   * 
   */
  class EASDIF_API SDIFEntity
  {
    friend class SDIFFrame;
  
public:
  /// The parts of a selection that can be cleared independently
  enum SelectionPartsE {
    eSP_Stream,     /** <  Selection on Stream   */
    eSP_Frame,      /** <  Selection on frames   */
    eSP_Matrix,     /** <  Selection on matrix   */
    eSP_Time,       /** <  Selection on time   */
    eSP_Row,        /** <  Selection on rows   */
    eSP_Column,     /** <  Selection on columns   */
    eSP_All         /** <  all selections  */
  };

private:

/** 
 * contain the SDIFNameValueTable of the entity
 */
  std::vector<SDIFNameValueTable> mv_NVT;
  
  SdifFileT* efile;
  std::string mDescription;
  
  SdifUInt4 mSize;
  SdiffPosT mFirstFramePos;	// file position after reading the header
  
  bool mEof;
  bool mEofSeen;

  int mOpen;
  size_t generalHeader;
  size_t asciiChunks;

  mutable Directory::iterator mNextDirPos;  
  // iterator to last high level selected frame that was read 
  mutable Directory::iterator mLastReadPos;  
  bool isFrameDirEnabled;

  const SDIFLocation endLoc;
  
  // initial  matrix selection list 
  // before EnableFrameDir has been given this list
  // contains the selection that was part of the filename
  // after EnableFrameDir has been called 
  // this selection list contains the selection state
  // at the moment when the FrameDirectory has been enabled
  // used only as backup storage
  SelectionSet<SdifSignature> msMatrixSelection; 

  // true if matrix signature selection list has been read form the SDifFile
  bool          mlMatrixSelectionRead; 

  // initial frame selection list 
  // before EnableFrameDir has been given this list
  // contains the selection that was part of the filename
  // after EnableFrameDir has been called 
  // this selection list contains the selection state
  // at the moment when the FrameDirectory has been enabled
  // used only as backup storage
  SelectionSet<SdifSignature> msFrameSelection; 
  // true if frame signature selection list has been read form the SDifFile
  bool          mlFrameSelectionRead;  

  // initial stream selection list 
  // before EnableFrameDir has been given this list
  // contains the selection that was part of the filename
  // after EnableFrameDir has been called 
  // this selection list contains the selection state
  // at the moment when the FrameDirectory has been enabled
  // used only as backup storage
  SelectionSet<unsigned int>  msStreamSelection;
  // true if stream selection list has been read form the SDifFile
  bool          mlStreamSelectionRead; 

  // high level stream selection
  // contains the additonam selection that has been
  // applied after the frame directory has been enabled
  SelectionSet <unsigned int>  msHighLevelStreamSelection;
  // high level matrix selection
  // contains the additonam selection that has been
  // applied after the frame directory has been enabled
  SelectionSet <SdifSignature> msHighLevelMatrixSelection;
  // high level frame selection
  // contains the additonam selection that has been
  // applied after the frame directory has been enabled
  SelectionSet <SdifSignature> msHighLevelFrameSelection;

public: 


  /// \ingroup directory
  /// sdifentity iterator type
  typedef FRIterator<0>       iterator;
  /// \ingroup directory
  /// sdifentity iterator type
  typedef FRIterator<1> const_iterator;      
  friend class FRIterator<0>;
  friend class FRIterator<1>;

  /// the Frame Directory of the current file.
  mutable Directory           mFrameDirectory;


  /// Default constructor
  SDIFEntity();

  ~SDIFEntity();

  /** 
   * \brief begin iterator 
   * \ingroup directory
   * 
   * @return const_iterator to first selected frame of entity
   */
  const_iterator
  begin () const;

  /** 
   * \brief begin iterator 
   * \ingroup directory
   * 
   * @return iterator to first selected frame of entity
   */
  iterator
  begin ();

  /** 
   * \brief end iterator 
   * \ingroup directory
   * 
   * @return const_iterator to first selected frame of entity
   */
  const_iterator
  end () const;

  /** 
   * \brief SDIFEntity::iterator to end of file 
   * \ingroup directory
   * 
   * @return iterator to first selected frame of entity
   */
  iterator
  end ();

  /** 
   * \brief Get Current Location
   * \ingroup directory
   * @return Reference to SDIFLocation at current file position
   */
  const SDIFLocation&
  currLoc() const {
    if(!isFrameDirEnabled || mFrameDirectory.empty() || mLastReadPos ==  mFrameDirectory.end())
      return endLoc;
    return *mLastReadPos;
  }

  /** 
   * \brief Get SDIFEntity::const_iterator  pointing to last file position
   * \ingroup directory
   * 
   * Note, that the selection may have changed
   * in the meantime such that the last position is no longer selected.
   * You may test this by means of the method IsSelected()
   * 
   * @return A const iterator referencing the file position that contained the
   *      last high level selected matrix.
   */  
  const_iterator
  lastRead() const;

  /** 
   * \brief Get SDIFEntity::iterator pointing to current file position
   * \ingroup directory
   *
   * Note, that the selection may have changed
   * in the meantime such that the last position is no longer selected.
   * You may test this by means of the method IsSelected()
   * 
   * @return An iterator referencing the file position that contained the
   *      last high level selected matrix. 
   */  
  iterator
  lastRead();

  /** 
   * \brief get Easdif::Directory of current SDIF File
   * \ingroup directory
   * 
   * @return reference to Easdif::Directory of current file as has been read so far. 
   */
  const Directory&
  GetDirectory() const {
    return mFrameDirectory;
  }

/*************************************************************************/
/* Description type */
/**
* \defgroup  description SDIFEntity - Description type
*/

/** 
 * \ingroup description
 * \brief write type info
 *
 * add the description type to the frame "1TYP". (for internal use when
 * opening the file )
 */
 private:
    bool WriteTypes();
    //int AddDescriptionType();

 public:

    /** 
     * \ingroup description
     *
     * \brief add new frame type  (or extend existing)  frame type for the  entity
     *
     * ATTENTION: AddFrameType(const std::string&,const std::string&) is a deprecated function,
     * use  AddFrameType(const FrameType&) instead.
     *
     * the description is added into the local file type descrption for the entity.
     * It is written  to the file when the entity is opened for wrinting. Therefore, 
     * type descriptors  have to be added to the entity before the file is opened.
     *
     * AddFrameType() must be used after any new matrix types have been added using
     * Easdif::SDIFEntity::AddMatrixType()
     *
     * @param frametype string that identifies the  new frame type,
     *  the identifier will use  at most 4 characters
     * @param matrix_description string that defines the matrix elements that may be part  
     * of the frame. Each matrix is descriped by a non empty test of two elements
     * separated by a single space. The desriptions are separated by semicolon and no semicolon
     * should be present at the end of the matrix_description.
     *
     * Example: 
     * 
     *  entity.AddFrameType("1NEW", "1NEW NewMatrix; 1FQ0 New1FQ0");
     *
     */
    bool AddFrameType(const std::string& frametype, 
		      const std::string& matrix_description);

    /** 
     * \ingroup description
     *
     * \brief add new frame type  or extend existing  frame type  of the entity
     *
     * the description is added into the local file type descrption for the entity.
     * It is written  to the file when the entity is opened for wrinting. Therefore, 
     * type descriptors  have to be added to the entity before the file is opened.
     *
     * AddFrameType() must be used after any new matrixtypes have been 
     * established by means of Easdif::SDIFEntity::AddMatrixType().
     *
     * @param frametype frametype to include into type description
     *
     */
    bool AddFrameType(const FrameType& frameType);

    /** 
     * \ingroup description
     * 
     * \brief define a new (or extend an existing) matrix type for  the entity.
     *
     * ATTENTION: AddMatrixType(const std::string&,const std::string&) is a deprecated function,
     * use  AddMatrixType(const MatrixType&) instead.
     * 
     * The type description is added into the local type description and written into the entity after
     * opening it for writing. Therefore, type descriptors  have to be added
     * to the entity before the file is opened.
     *
     * AddMatrixType() must be used before Easdif::SDIFEntity::AddFrameType()
     *
     * @param matrixtype string for define a new matrix type 
     * or redefined a matrix type 
     * @param colnames string to defined the differents parameters of 
     * the matrix type
     *
     * Example: 
     * 
     *  entity.AddMatrixType("1NEW", "amplitude, phase");
     *
     */
    bool AddMatrixType(const std::string& matrixtype, 
		       const std::string& colnames);


    /** 
     * \ingroup description
     * 
     * \brief define a new (or extend an existing) matrix type for  the entity.
     *
     * The type description is added into the local type description and written into the entity after
     * opening it for writing. Therefore, type descriptors  have to be added
     * to the entity before the file is opened.
     *
     * AddMatrixType() must be used before Easdif::SDIFEntity::AddFrameType()
     *
     * @param matrixtype string for define a new matrix type 
     * or redefined a matrix type 
     *
     */
    bool AddMatrixType(const MatrixType& matrixtype);
   
    /** 
     * \ingroup description
     * Print the SdifStringT* which have the types definitions of the frames
     * and matrix
     */
    int PrintTypes();

    /** 
     * \ingroup description
     * \brief get vector of FrameType objects that describe the type string of the file
     * 
     * @param frametypes io vector
     */    
    void GetTypes(std::vector<FrameType>& frametypes) const ;
    /** 
     * \ingroup description
     * \brief get vector of MatrixType objects that describe the type string of the file
     * 
     * @param matrixtypes io vector
     */    
    void GetTypes(std::vector<MatrixType>& matrixtypes) const ;

    /** 
     * \ingroup description
     * return a string  containing the user defined types of the
     * file that is stored in the frame "1TYP".
     *
     *  \return Type string
     */
    const std::string& GetTypeString() const;
    
    /** 
     *   \ingroup description
     *  clear all description types
     */
    void clearTypes() {
      mDescription.clear();
    }

    /** 
     * \ingroup description
     * Set the user defined frame types in the frame "1TYP" for the current file to
     * the argument string 
     * 
     * \param TypeString
     * \return  true if success
     */
    bool SetTypeString(const std::string& TypeString);

/*************************************************************************/
/* Change the selection */
/**
 * \defgroup selection SDIFEntity - Selection
 * \brief functions related to handle the sdif data selections  
 */

 /** 
  *\brief replace selection for current file
  * \ingroup selection
  * Replace current selection by new one given in argument.
  * The selection specification may contain all the parts of a filename
  * based selection after the selection indicator :: .
  *
  * This function is only allowed if  EnableFrameDir() has not yet been called
  *
  * @param selection  selection string that will be parsed to replace the selection
  *
  * \return  true if success
  */
  bool ChangeSelection(const std::string& selection);

  /** 
   * \brief merge selection to the selection that is currently active
   * \ingroup selection
   *
   * This function is only allowed if EnableFrameDir() has not been called
   *
   * @param selection selection string to merge
   * \return  true if success
   */ 
  bool MergeSelection(const std::string& selection);

  /** 
   * \brief clear part or all of active selection
   * \ingroup selection
   *
   * This function is only allowed if EnableFrameDir() has not been called
   *
   * @param part the enum indicating what part of the selection to clear.
   * \return  true if success
   */
  bool ClearSelection(Easdif::SDIFEntity::SelectionPartsE part);

  /** 
   * \ingroup selection
   * \brief get the vector of selected matrix signatures that exist for the current file
   * 
   * @param out vector to be filled with existing selection, empty result
   *        indicates no selection, a set containing a single Signature of value
   *        eEmptySignature signals that no matrices are selected, which means
   *        that only empty frames are read.
   * 
   * @return true if file and returned selection is valid.
   */
  bool GetMatrixSelection(SelectionSet<SdifSignature>& out) const;

  /** 
   * \ingroup selection
   * \brief test whether a given matrix signature is part of the current selection
   * 
   * @param sig signature to test
   * 
   * @return true if the given matrix  signature is selected (or if no matrix selection exists at all)
   */
  bool TestMatrixSelection(SdifSignature sig) const;



  /** 
   * \ingroup selection
   * \brief get the vector of selected frame signatures that exist for the current file
   * 
   * @param out vector to be filled with existing selection, empty result
   *        indicates no selection, a set containing a single Signature of value
   *        eEmptySignature signals that no frames are selected.
   * 
   * @return true if file and returned selection is valid.
   */
  bool GetFrameSelection(SelectionSet<SdifSignature>& out) const;

  /** 
   * \ingroup selection
   * \brief test whether a given frame signature is part of the current selection
   * 
   * @param sig signature to test
   * 
   * @return true if the frame signature is selected (or if no frame selection exists at all)
   */
  bool TestFrameSelection(SdifSignature sig) const;

  /** 
   * \ingroup selection
   * \brief get the set of selected streamid's that exist for the current file
   * 
   * @param out vector to be filled with existing selection, empty result
   *        indicates no selection, an empty selection that results of an restriction
   *        of two incompatible sets will result in a set with a single stream
   *        of id SDIFEntity::kEmptyStreamSelection
   *    
   * @return true if file and returned selection is valid.
   */
  bool GetStreamSelection(SelectionSet<unsigned int>& out) const;

  /** 
   * \ingroup selection
   * \brief test whether a given streamid is part of the current selection
   * 
   * @param streamid to test
   * 
   * @return true if the given streamid is selected (or if no matrix selection exists at all)
   */
  bool TestStreamSelection(unsigned int streamid) const;


private:


  // test whether aFrame matches a highlevel selection
  bool isFrameHLSelected(unsigned int streamid, SdifSignature sig){
    bool streammatch = msHighLevelStreamSelection.isSelected(streamid);
    
    bool framematch = msHighLevelFrameSelection.isSelected(sig);
    
    return streammatch && framematch;
  }

  /** 
   * \ingroup selection
   * \brief   create a selection list containing the given signatures 
   * 
   * @param listsel   A selection list pointer in which the selection list shall
   *                  be established
   * @param sigs      vector of signatures to use to create the selection list
   * 
   * @return  true if successful created
   */
  bool CreateSignatureSelection(SdifListT* listsel,const SelectionSet<SdifSignature>& sigs);


  
  /** 
   * \ingroup selection
   * \brief   create a sdif selection containing the given ids 
   * 
   * @param listsel   A selection  pointer obtained from the current SdifFile
   * @param streams   SelectionSet<unsigned int> to use to create the selection list
   * 
   * @return  true if successful created
   */
  bool CreateStreamSelection(SdifSelectionT *sel,
                             const SelectionSet<unsigned int>& streams);


  /** 
   * \brief create an empty StreamSelection 
   *
   * here empty is the real empty where no stream will be selected,
   * this function is only useful to have consistent behavior of 
   * RestrictStreamSelection().
   *
   * @param sel create a selection that will have no stream selected
   * 
   * @return true if successful
   */
  bool CreateEmptyStreamSelection(SdifSelectionT *sel);

  /// \ingroup selection
  /// remove the state of previous frame and matrix selections
  void ClearSelectionState() {
    if(mlStreamSelectionRead) {
      msStreamSelection.clear();
      mlStreamSelectionRead   = false;
    }
    if(mlFrameSelectionRead) {
      msFrameSelection.clear();
      mlFrameSelectionRead = false;
    }
    if(mlMatrixSelectionRead) {
      msMatrixSelection.clear();
      mlMatrixSelectionRead = false;
    }    
  }

public:

  /** 
   * \ingroup selection
   * \brief   restrict the frame signature selection 
   *
   * The frame selection is updated to contain the intersection between the existing 
   * and the given list of signatures. In case the intersection is empty
   *              a selection with signature eEmptySignature is established which should not match
   *              any valid Frame.
   *
   * As long as EnableFrameDir() has not been called this function works on
   * the low level SDIF selection, otherwise it establishes a high level selection
   * that can be savely used used with the Directory. The directory will still contain all
   * frames and matrices that are selected according to the low level selection. 
   * 
   *
   * @param sigs    set of signatures that should remain selected, this set is intersected
   *              with any existing Frame selection. 
   * 
   * @return  true if successful created
   */
  bool RestrictFrameSelection(const SelectionSet<SdifSignature>& sigs);

  /** 
   * \ingroup selection
   * \brief reestablish a previous state of frame signature selection
   *
   * This function can either be called before or after EnableFrameDir() has been called.
   * In the first case it will reestablish the frame selection that was initially given 
   * as part opf the filename.
   * In the second case it will reestablish the selection that was valid at the time
   * when EnableFrameDir() has been called.
   *
   * @return true if successful reestablished
   */
  bool ReestablishFrameSelection();


  /** 
   * \ingroup selection
   * \brief   restrict the matrix signature selection 
   *
   * The matrix selection is updated to contain the intersection between the existing 
   * and the given list of signatures. In case the intersection is empty
   *              a selection with signature eEmptySignature is established which should not match
   *              any valid matrix.
   *
   * As long as EnableFrameDir() has not been called this function works on
   * the low level SDIF selection, otherwise it establishes a high level selection
   * that can be savely used used with the Directory. The directory will still contain all
   * frames and matrices that are selected according to the low level selection. 
   * 
   * @param sigs    set of signatures that should remain selected and that will be intersected
   *                with the existing matrix selection.
   * 
   * @return  true if successful created
   */
  bool RestrictMatrixSelection(const SelectionSet<SdifSignature>& sigs);

  /** 
   * \ingroup selection
   * \brief reestablish a previous state of matrix signature selection
   * 
   * This function can either be called before or after EnableFrameDir() has been called.
   * In the first case it will reestablish the matrix selection that was initially given 
   * as part opf the filename.
   * In the second case it will reestablish the selection that was valid at the time
   * when EnableFrameDir() has been called.
   *
   * @return true if successful reestablished
   */
  bool ReestablishMatrixSelection();


  /** 
   * \ingroup selection
   * \brief   restrict the streamid selection 
   *
   * The stream selection is updated to contain the intersection between the existing 
   * and the given list of ids. In case the intersection is empty
   * a selection with signature kEmptyStreamSelection is established which should not match
   * any valid Frame.
   *
   * As long as EnableFrameDir() has not been called this function works on
   * the low level SDIF selection, otherwise it establishes a high level selection
   * that can be used with the Directory;
   * 
   * @param streamid    set of signatures that should remain selected 
   * (ATTENTION OPEN STREAM SELECTIONS NOT YET PROPERLY HANDLED!!)
   * 
   * @return  true if successful created
   */
  bool RestrictStreamSelection(const SelectionSet<unsigned int>& streamid);


  /** 
   * \ingroup selection
   * \brief reestablish a previous state of stream signature selection
   * 
   * This function can either be called before or after EnableFrameDir() has been called.
   * In the first case it will reestablish the frame selection that was initially given 
   * as part opf the filename.
   * In the second case it will reestablish the selection that was valid at the time
   * when EnableFrameDir() has been called.
   *
   * @return true if successful reestablished
   */
  bool ReestablishStreamSelection();

  /*************************************************************************/
  /* Operation with a file */
  /**
   * \defgroup  file  SDIFEntity - Operations with file
   */
  
  /** 
   * \ingroup  file
   * \brief open the file of the entity in reading or writing mode
   * 
   * @param filename 
   * @param Mode can be "eReadFile" or "eWriteFile"
   *
   * @return true if opened/false if error
   */
  bool Open(const char* filename, SdifFileModeET Mode);
  
  /** 
   * \ingroup  file
   * \brief open a file in reading mode
   * @param filename 
   *               
   * @return true if opened/false if error
   */
  bool OpenRead(const char* filename);
  
  /** 
   * \ingroup  file
   * \brief open a file in reading mode without destroying any information  in the internal FrameDirectory
   * @param filename 
   *
   * In contrast to standard OpenRead the internal frame directory 
   * is kept and reused as well as the matrix and frame selections
   * that have been established either by the user in the file name
   * or by calling selection modifications before calling EnableFrameDir().
   * 
   *  Attention reusing a directory is only reasonable if the
   *       same file is.
   * 
   * If there is no directory information, either if the SDIFEntity
   * has just been  created or the file that has previously been opened
   * did not use a frame directory this call is equivalent to OpenRead()
   *               
   * @return true if opened/false if error
   */
  bool ReOpenRead(const char* filename);
  
  /** 
   * \ingroup  file
   * open a file in writing mode
   * @param filename 
   * @return true if opened/false if error
   */
  bool OpenWrite(const char* filename);
  
  /** 
   * \ingroup  file
   * \brief close a file
   *
   * All internal states (types, nvts, selections ) are reset 
   * Directory information is kept.
   *
   * @return true if closed / false if file was not opened
   */
  bool Close();
  
  /** 
   * \ingroup  file
   * \brief rewind a file to first non-ascii frame after the file header
   * @return true if positioning was successful
   */  
  bool Rewind();

  /** 
   * \ingroup  file
   * \brief skip through to te end of file
   *  This function is usefull especially for filling the frame directory
   * @return true if positioning was successful
   */  
  bool FillFrameDirAndRewind();
  
  /** 
   * \ingroup  file
   * \brief examine seekability of current file
   *
   * @return true if file can be seeked which is the case if it is not a pipe
   */    
  bool isSeekable() {if(efile)return efile->isSeekable != 0; return false; };
  
  /** 
   * \ingroup file
   * get the SdifFileT* file
   * \return SdifFile pointer related to Entity
   */
  SdifFileT* GetFile() const;
  
  /** 
   * \ingroup file
   *
   * test file state
   *
   * \return true if file is opened 
   */
  bool IsOpen() const {
    return (efile && mOpen);
  }

  /** 
   * \ingroup file
   *
   * test file state
   *
   * \return true if file is opened and not at eof
   */
  bool good() const {
    return (!eof() && mOpen && efile);
  }

/**
 * \ingroup file
 * true if file is at eof
 */
  bool eof() const {return mEof;}


  /**
   * \ingroup directory
   * \brief enable Easdif::Directory for current file
   *
   * If enabled the entity will internally keep track of the positions of all
   * frames and thereby allow to quickly reposition to a desired frame
   * by specifying the requested Frame time.
   *
   * EnableFrameDir can be called only for files opened for reading
   * and only if no data frame has yet been read. The 
   *
   * \return true if directoy can be created 
   */
  bool EnableFrameDir();

  /**
   * \ingroup directory
   * true if frame Directory is enabled
   */
  bool IsFrameDir() const {   return isFrameDirEnabled;}


  /**
   * \ingroup directory
   * dump content of Frame Directory to stderr
   */
  void PrintFrameDir() const;

private:

  /**
   * \brief add a new frame into the SDIFEntity::Directory of the current file
   * \ingroup file
   *
   * add a new time/position point to the directory
   * used only internally to maintain directory from 
   * the frame read interface.
   *
   * @param id 
   * @param sig 
   * @param time 
   * @param nbMatrix 
   * @param pos 
   * @param IPos iterator position that will always point to the
   *  directory entry that represents the  frame that is treated
   * 
   * 
   * \return returns true if the entry was newly created in the frame directory
   *     iun this case it has to be filled with the fundamentally selected 
   *     matrix signatures.
   *     If false is returned the location did exist already
   *     and no further action has to  be performed.
   */
  bool
  AddFramePos(SdifUInt4 id, SdifSignature sig, SdifFloat8 time,
              SdifUInt4 nbMatrix, SdiffPosT pos, Directory::iterator& IPos);



    bool GetLowLevelFilePos(SdiffPosT &pos) {
      pos = -1;
      if(efile) 
        pos = ftell(efile->Stream);
      return pos != -1;
    }

    bool SetLowLevelFilePos(SdiffPosT pos) {
      if(efile) 
        return 0==fseek(efile->Stream,pos, SEEK_SET);
      return false;
    }

public:
    SdifErrorT* LastError();

  /*************************************************************************/
  /* Name Value Table */
  /**
   * \defgroup  nvt  SDIFEntity - Name Value Table
   */
  
  /** 
   * \ingroup nvt
   * to see all the Name Values Tables
   */
    void PrintAllNVTs();

  /**
   * \ingroup nvt 
   * take a Name Values Table of a file. (for internal used in opening)
   * @return SDIFNameValueTable of the file
   */
private:
  SDIFNameValueTable TakeNVT();

public:
  /** 
   * \ingroup nvt
   * insert a SDIFNameValueTable in the vector of Name Value Table
   * overwrite the streamid of the given NVT with the argument provided.
   *
   * @param nvt Name Value Table to insert in the vector
   * @param StreamID StreamId of the Name Value Table 
   *
   * \return true if ok/false if error.
   */
  
  bool AddNVT(const SDIFNameValueTable &nvt, SdifUInt4 StreamID);

  /** 
   * \ingroup nvt
   * insert a SDIFNameValueTable in the vector of Name Value Table
   *
   * The streamid that will be used is the streamid of the SDIFNameValueTable.
   * 
   * @param nvt Name Value Table to insert in the vector
   *
   * \return true if ok/false if error.
   */

  bool AddNVT(const SDIFNameValueTable &nvt);


  /** 
   * \ingroup nvt
   * write the vector of Name Value Table in the file when opening a file 
   * for writing (for internal use only)
   *
   * \return true if ok/false if error.
   */
private:
  bool WriteNVTs();
public:
  
  /**
   * \ingroup nvt 
   * get the number of Name Value Tables  stored in entity
   * 
   * \return number of Name Value Tables
   */
  int GetNbNVT() const;
  
  /** 
   * \ingroup nvt
   * get the i-th Name Value Table
   */
  SDIFNameValueTable& GetNVT(unsigned int i);
  

  /** 
   * \ingroup nvt
   * clear NVT tables
   */
  void clearAllNVT() { mv_NVT.clear(); }


  /******************/
  /*************************************************************************/
  /* Read and Write */
  /**
   * \defgroup  rnwentity  SDIFEntity - Read and Write
   */

  /**
   * \ingroup rnwentity 
   * \brief read next frame from file
   *
   * read the next frame of the file
   * return the number of bytes read. This function will read the next frame and will 
   * return an unmodified frame if the next frame his not selected
   * 
   * @param frame to fill
   * 
   * @return number of data bytes read for frame header and selected matrices of selected frames
   *  returns 0 if next frame is not selected. In this case frame will be uninitialized.
   */
  int ReadNextFrame(SDIFFrame& frame);


  /**
   * \brief read next selected frame from file
   * \ingroup rnwentity
   *
   * read the next selected frame of the file
   * return the number of bytes read, sets eof if there is no next frame available
   * and throws SDIFEof if called with file already being at eof(). Will return 0 bytes read only if 
   * the last frames in the file are not selected such that eof is reached before a selected frame
   * can be read.
   * 
   * @param frame to fill
   * 
   * @return  number of data bytes read for frame header and selected matrices of selected frames
   *
   */
  int ReadNextSelectedFrame(SDIFFrame& frame);

  /**
   * \brief read previous selected frame from file
   * \ingroup rnwentity
   *
   * read the previous selected frame of the file
   * return the number of bytes read, this function will only work if the frame directory is enabled
   * 
   * @param frame to fill
   * 
   * @return  number of data bytes read for selected matrices 
   */
  int ReadPrevSelectedFrame(SDIFFrame& frame);
  
  /**
   * \brief read next selected frame from file having time equal to  or after given time
   * \ingroup rnwentity
   * read the next selected frame of the file at or after time position timePos
   * return the number of bytes read. In the case that more than a single 
   * selected frame exists at time timePos the ReadNextSelectedFrame function
   * will always return the first of the group of frames with the same
   * time. Accordingly, you should be aware of the fact that you should not
   * use  this function to parse a whole file. This function is usefull only
   * if you want to quickly find a time location. After this you should use
   *  ReadNextSelectedFrame(SDIFFrame& frame) to parse the file.
   *
   * Prior to calling this function the internal
   * FrameDirectory needs to be enabled.
   *
   * @param frame     frame to fill with data
   * @param timePos   time position to read from
   * 
   * @return  number of data bytes read for selected matrices 
   *
   * \see EnableFrameDir()
   */
  int ReadNextSelectedFrame(SDIFFrame& frame,SdifFloat8 timePos);



  /** 
   * \ingroup rnwentity
   * write the next frame of the file
   * return the size of the frame write
   */
  int WriteFrame(SDIFFrame& frame);
  
};


  /**
   * @brief bidrectional  iterator 
   *\ingroup directory
   *
   * An iterator class that will
   * iterate over all selected frames  in the SDIFEntity 
   * Note that write access to the frame pointed to by the iterator 
   * will change a frame in memory and will not yet be written into the file
   *
   * An FRIterator always indicates a selected frame in the SDIF file.
   * The frame can be obtained by means of dereferencing the iterator.  
   *
   * the frame iterator organizes the Directory as a ring of frames
   * iterating to the end in both directions (up and down) will finally result in
   * an iterator matching FRIterator::end(); 
   *
   */
  template<int EASDIF_FR_CONST_FLAG>
  class FRIterator {
    typename Base_Iterator<0>::basic_iterator mBase;
   //typename std::list<SDIFLocation>::iterator mBase;
    SDIFEntity   *mpEnt;      // internal pointer to entity that the iterator works on
    mutable SDIFFrame     mFrame;     // internal frame to hold the current frame
    bool          mlEndUP;    // if true signals no further selected frame when moving to end of file
    bool          mlEndDOWN;  // if true signals no further selected frame when moving to start of file
    mutable bool          mlFrameIsLoaded;  // true if mFrame holds the frame that the iterator is pointing too.
  private:
    
    
    friend class FRIterator<swap_const<EASDIF_FR_CONST_FLAG>::INVERTED_CONST_FLAG>;
    friend class SDIFEntity;
    typedef typename Base_Iterator<EASDIF_FR_CONST_FLAG>::basic_iterator basic_iterator;
    //typedef std::list<SDIFLocation>::iterator basic_iterator;

    /**
     *  \brief synchronize iterator position and file pointer position
     *
     *  Internal function!
     * 
     *  position the entity iterator such that it points to a selected
     *  frame ( a frame that itself is selected and
     *  contains a selected matrix) or to FRIterator::end() if no further frame 
     *  is selected
     * 
     * @param up (if true move forward in time to find the next selected frame)
     *
     */   
    void initIterator(bool up) {
      //position to next selected frame or end() if not found
      if(mBase!=
         mpEnt->mFrameDirectory.end() ||
         (!mlEndDOWN && !up)|| (!mlEndUP && up)) {

        if(!mpEnt->mFrameDirectory.empty() && (mlEndUP || mlEndDOWN)
           && mBase== mpEnt->mFrameDirectory.end()) {

          if(mlEndDOWN && up)
            {mBase = mpEnt->mFrameDirectory.begin(); mlEndDOWN=false;}
          if(mlEndUP  && !up) 
            {--mBase; mlEndUP= false;}
        }

        while(mBase!=
              mpEnt->mFrameDirectory.end()){

          if (IsSelected() )
            return;
          if(up)
            ++mBase;
          else{
            if(mBase == mpEnt->mFrameDirectory.begin() )
              break;
            else
              --mBase;                
          }
        }
      }

      if(up && !mpEnt->mEofSeen) {      
        if(mpEnt->ReadNextSelectedFrame(mFrame)){
          mBase = --mpEnt->mFrameDirectory.end();
          mlFrameIsLoaded = true;
          return;
        }
      }

      // signal not found for both directions is end of File      
      mBase = mpEnt->mFrameDirectory.end();
      if(up)
        mlEndUP   = true;
      else
        mlEndDOWN = true;
      mlFrameIsLoaded = true;
      return;
    }


    /** 
     * \brief  assign Position to an iterator
     * \ingroup directory
     * 
     * @param _ent pointer to SDIFentity
     * \param it   directory iterator to use for initialization
     */
    FRIterator &assignPos(SDIFEntity *  _ent, Directory::iterator& it) {
      mpEnt = _ent;
      mBase = it;
      mlEndUP    = (mBase == mpEnt->mFrameDirectory.end() && mpEnt->mEofSeen);
      mlEndDOWN  = false;
      return *this;
    }

    FRIterator &assignPos(const SDIFEntity *  _ent, Directory::iterator& it) {
      mpEnt = const_cast<SDIFEntity *>(_ent);
      mBase = it;
      mlEndUP    = (mBase == mpEnt->mFrameDirectory.end() && mpEnt->mEofSeen);
      mlEndDOWN  = false;
      return *this;
    }

  public:
    typedef typename basic_iterator::iterator_category  iterator_category;
    typedef SDIFFrame        value_type;
    typedef typename basic_iterator::difference_type   difference_type;
    typedef typename IteratorTypes<EASDIF_FR_CONST_FLAG>::pointer       pointer;
    typedef typename IteratorTypes<EASDIF_FR_CONST_FLAG>::reference     reference;

     
    /// \ingroup directory
    /// get the SDIFLocation for the iterators current frame position 
    const
    SDIFLocation& GetLoc() {
      if(mBase == mpEnt->mFrameDirectory.end())
        return mpEnt->endLoc;
      return *mBase;
    }
    
    /// \ingroup directory
    /// get the SDIFLocation for the iterators current frame position 
    const
    SDIFLocation& GetLoc() const {
      if(mBase == mpEnt->mFrameDirectory.end())
        return mpEnt->endLoc;
      return *mBase;
    }

       
    /// \ingroup directory
    /// constructor
    FRIterator () : mpEnt(0),mlEndUP(false),mlEndDOWN(false),mlFrameIsLoaded(false)  {};

    /// \ingroup directory
    /// copy constructor
    FRIterator (const FRIterator<EASDIF_FR_CONST_FLAG> & in) : 
      mBase(in.mBase),mpEnt(in.mpEnt), 
      mlEndUP(in.mlEndUP),mlEndDOWN(in.mlEndDOWN),mlFrameIsLoaded(false)
    {    };

    /// \ingroup directory
    /// copy constructor
    FRIterator (const FRIterator<swap_const<EASDIF_FR_CONST_FLAG>::INVERTED_CONST_FLAG> & in) : 
      mBase(in.mBase), mpEnt(in.mpEnt), 
      mlEndUP(in.mlEndUP), mlEndDOWN(in.mlEndDOWN), mlFrameIsLoaded(false)     
    {    };
	
    /** 
     * \brief  const constructor from entity pointer  and directory iterator
     * \ingroup directory
     * 
     * @param _ent pointer to SDIFentity
     * \param it   directory iterator to use for initialization
     * @param forceend  if true create iterator pointing to end of file, else
     *             create iterator pointing to current frame 
     */
    FRIterator (const SDIFEntity *  _ent, Directory::iterator& it, bool forceend) : 
      mBase(forceend  ? 
            const_cast<SDIFEntity *>(_ent)->mFrameDirectory.end() : it),
      mpEnt(const_cast<SDIFEntity *>(_ent)),mlFrameIsLoaded(false)
    {
      if(!_ent->IsFrameDir() ) {
        throw SDIFDirError(eError,
                           "Error in SDIFEntity::FRIterator:: !!! Iterator can only be used if frame directoy is enabled !!!",
                           0,eUnknown,0,0);

      }
      mlEndUP    = (forceend&& it == mpEnt->mFrameDirectory.end()) 
        || (mBase == mpEnt->mFrameDirectory.end()&&mpEnt->mEofSeen);
      mlEndDOWN  = false;
      if(!mlEndUP) {
        initIterator(true);
      }      
    }

    /** 
     * \brief  constructor from entity pointer and directory iterator
     * \ingroup directory
     * 
     * @param _ent pointer to SDIFentity
     * \param it   directory iterator to use for initialization
     * @param forceend  if true create iterator pointing to end of file, else
     *             create iterator pointing to current frame 
     */
    FRIterator (SDIFEntity *  _ent, Directory::iterator& it, bool forceend) : 
      mBase(forceend ?  _ent->mFrameDirectory.end() : it),
      mpEnt(_ent),mlFrameIsLoaded(false)
    {
      if(!_ent->IsFrameDir() ) {
        throw SDIFDirError(eError,
                           "Error in SDIFEntity::FRIterator:: !!! Iterator can only be used if frame directoy is enabled !!!",
                           0,eUnknown,0,0);

      }
      mlEndUP    = (forceend&& (it == mpEnt->mFrameDirectory.end())) 
        || (mBase == mpEnt->mFrameDirectory.end()&&mpEnt->mEofSeen);
      mlEndDOWN  = false;
      if(!mlEndUP) {
        initIterator(true);
      }      
    }


	
    ~FRIterator ( ) {};
	
    /** 
     * \brief increment iterator
     * \ingroup directory
     * 
     * when returning the iterator will point to the next selected frame 
     * the frame is read and stored internally if it is parsed for the first time.
     * Otherwise the selection operation is performed using the internal directory
     * and the frame will be read only if the iterator will be dereferenced. 
     *
     * @return iterator pointing to next selected frame 
     */
    FRIterator& operator ++() {
      mlFrameIsLoaded = false;
      mFrame.ClearData();

      if(!mlEndUP ) {
        if(mBase!=mpEnt->mFrameDirectory.end()) 
          ++mBase;

        initIterator(true);
      }    
      return *this;
    }
	
    /** 
     * \brief postfix increment iterator
     * \ingroup directory
     * 
     * when returning the iterator will point to the next selected frame 
     * the frame is read and stored internally if it is parsed for the first time.
     * Otherwise the selection operation is performed using the internal directory
     * and the frame will be read only if the iterator will be dereferenced. 
     *
     * @return iterator pointing to currently selected frame 
     */
    FRIterator operator ++(int) {
      FRIterator tmp=*this;
      this->operator++();
      return tmp;
    }
		
    
    /** 
     * \brief decrement iterator
     * \ingroup directory
     * 
     * when returning the iterator will point to the previous selected frame 
     * the frame is read and stored internally if it is parsed for the first time.
     * Otherwise the selection operation is performed using the internal directory
     * and the frame will be read only if the iterator will be dereferenced. 
     *
     * @return iterator pointing to next selected frame 
     */
    FRIterator& operator --() {
      mFrame.ClearData();
      mlFrameIsLoaded = false;
      if(!mlEndDOWN) {
        // we decrement from end of file iterator
        // here we need to read the whole file to point to the correct position
        if(mBase==
           mpEnt->mFrameDirectory.end() && !mpEnt->mEofSeen) {
          
          while(!mpEnt->eof()) {
            mpEnt->ReadNextSelectedFrame(mFrame);
          }        
        }

        if(mBase != mpEnt->mFrameDirectory.begin()){          
          --mBase;
          mlEndUP = false;
          initIterator(false);      
        }
        else{
          mBase     = mpEnt->mFrameDirectory.end();
          mlEndDOWN = true;
          mlFrameIsLoaded = true;
        }
      }
      return *this;
    }
	
    /** 
     * \brief postfix decrement iterator
     * \ingroup directory
     * 
     * when returning the iterator will point to the next selected frame 
     * the frame is not read and stored internally, so no real file movement is really
     * done. Therefore iterator decrement is even possible for pipes.
     *
     * @return iterator pointing to currently selected frame 
     */
    FRIterator operator --(int) {
      FRIterator tmp=*this;
      this->operator--();
      return tmp;
    }
	

    /** 
     * \brief dereference
     * \ingroup directory
     * 
     * at this point the frame the iterator is currently pointing 
     * is forced to be loaded from file 
     * 
     * @return reference to frame
     */
    reference operator*()  { 
      if(!mlFrameIsLoaded) {
        GotoPos();
        if(mpEnt->ReadNextSelectedFrame(mFrame) ) {
          mlFrameIsLoaded = true;            
        }
      }
      return mFrame;
    }

    /** 
     * \brief dereference
     * \ingroup directory
     * 
     * at this point the frame the iterator is currently pointing 
     * is forced to be loaded from file 
     * 
     * @return reference to frame
     */
    typename IteratorTypes<1>::reference operator*() const { 
      if(!mlFrameIsLoaded) {
        GotoPos();
        if(mpEnt->ReadNextSelectedFrame(mFrame) ) {
          mlFrameIsLoaded = true;            
        }
      }

      return mFrame;      
    }
    
    /** 
     * \brief member access
     * \ingroup directory
     * 
     * at this point the frame the iterator is currently pointing 
     * is forced to be loaded from file 
     * 
     * @return pointer to frame
     */
    typename IteratorTypes<1>::pointer operator->() const {      
      if(!mlFrameIsLoaded) {
        GotoPos();
        if(mpEnt->ReadNextSelectedFrame(mFrame) ) {
          mlFrameIsLoaded = true;            
        }
      }
      return &mFrame;
    }

    /** 
     * \brief member access
     * \ingroup directory
     * 
     * at this point the frame the iterator is currently pointing 
     * is forced to be loaded from file 
     * 
     * @return pointer to frame
     */
     pointer operator->()  {
      if(!mlFrameIsLoaded) {
        GotoPos();
        if(mpEnt->ReadNextSelectedFrame(mFrame) ) {
          mlFrameIsLoaded = true;            
        }
      }
      return &mFrame;
    }

    /** 
     * \brief check whether the iterator points to a selected frame
     *
     * Normally, all frame iterators will contain  selected frames only.
     * However, in case the selection have been changed a position 
     * may no longer be selected. In these cases you can use the present function
     * to check whether the position is still selected.
     * 
     * @return true if selected
     */
    bool IsSelected() {
      if(mBase== mpEnt->mFrameDirectory.end() ) return false;

      SdifSelectionT *sel     = mpEnt->GetFile()->Selection;

      // The frame directory is enabled so the matrix selection
      // is either in msMatrixSelection or (if restrict selection has been used)
      // in msHighLevelMatrixSelection.
      bool matrixSelection    = !mpEnt->msMatrixSelection.empty(); 
      bool hlmatrixSelection  = !mpEnt->msHighLevelMatrixSelection.empty(); 

      if(SdifFrameIsSelected(&(mBase->mFrameHdr),sel)
         && (mpEnt->msHighLevelFrameSelection.empty()
             || (mpEnt->msHighLevelFrameSelection.end()
                 != mpEnt->msHighLevelFrameSelection.find(mBase->mFrameHdr.Signature)))) {
        if(mpEnt->msHighLevelStreamSelection.empty()
           || mpEnt->msHighLevelStreamSelection.end() 
           != mpEnt->msHighLevelStreamSelection.find(mBase->mFrameHdr.NumID)){
          if(hlmatrixSelection) {
            SdifUInt4 nb = mBase->LocNbMatrix();
            for(SdifUInt4 i  = 0; i< nb;++i){
              if(mpEnt->msHighLevelMatrixSelection.end()
                 != mpEnt->msHighLevelMatrixSelection.find(mBase->LocMSignature(i)))
                return true;
            }
          }
          else if (matrixSelection){
            SdifUInt4 nb = mBase->LocNbMatrix();
            for(SdifUInt4 i  = 0; i< nb;++i){
              if(mpEnt->msMatrixSelection.end()
                 != mpEnt->msMatrixSelection.find(mBase->LocMSignature(i)))
                return true;
            }
          }
          else {
            return true;
          }
        }
      }
      return false;
    }

    /**
     *  \ingroup directory
     *  \brief seek file read pointer position to the frame the iterator point's to
     *
     *  after calling GotoPos the frame signature has been read
     *  and the next sdif read will read the frame header of the next
     *  frame. Exception is thron in case of successfull seek but
     *  error reading the next frame header.
     *
     *  \return true if position has been achieved
     */
    bool GotoPos() const  {
      SdiffPosT pos = mBase->LocPos();
      mpEnt->mNextDirPos   = mBase;
      mpEnt->mEof   = false;
      if(!mpEnt->GetFile()->isSeekable) 
        return false;
      if(-1==SdiffSetPos(mpEnt->GetFile()->Stream,&pos)) {
        return false;
      }
      
      if(mpEnt->mOpen & 2) {
        size_t SizeR = 0;
        SdifFGetSignature(mpEnt->GetFile(), &SizeR);            
        if(SizeR!=sizeof(SdifSignature)){
          char msg[512];
          sprintf(msg,"Error in  SDIFEntity::FRIterator::GotoPos()!!!error while seeking to pos pos %ld !!!\n", 
                  static_cast<long int>(pos));
          throw SDIFSeekError(eError,
                              msg,
                              0,eUnknown,__FILE__,__LINE__);
        }
      }
      
      return true;
    }

    
    /// equal  between SDIFEntity::iterator 
    template<int LOCAL_CONST_FLAG>
    FRIterator&
    operator=(const FRIterator<LOCAL_CONST_FLAG>& i) {
      mBase           = i.mBase;
      mpEnt           = i.mpEnt;
      mFrame          = i.mFrame;
      mlEndUP         = i.mlEndUP;
      mlEndDOWN       = i.mlEndDOWN;
      mlFrameIsLoaded = i.mlFrameIsLoaded;
      return *this;
    }

    FRIterator&
    operator=(const FRIterator& i) {
      mBase           = i.mBase;
      mpEnt           = i.mpEnt;
      mFrame          = i.mFrame;
      mlEndUP         = i.mlEndUP;
      mlEndDOWN       = i.mlEndDOWN;
      mlFrameIsLoaded = i.mlFrameIsLoaded;
      return *this;
    }


    /// equal comparision between SDIFEntity::iterator 
    template<int LOCAL_CONST_FLAG>
    bool operator==(const FRIterator<LOCAL_CONST_FLAG>& i)const {
      return((i.mlEndUP  ||i.mlEndDOWN) == (mlEndUP || mlEndDOWN)  && i.mBase == mBase);
    }

    /// not equal comparision between SDIFEntity::iterator 
    template<int LOCAL_CONST_FLAG>
    bool operator!=(const  FRIterator<LOCAL_CONST_FLAG>& i)const {
      return !operator==(i);
    }

  };


  // iterator creators need to be defined outside SDIFEntity 
  // because FRIterator needs to e defined 
  inline SDIFEntity::const_iterator
  SDIFEntity::begin () const {
    Directory::iterator beg = this->mFrameDirectory.begin();
    return const_iterator(this,beg,false);
  }

  inline SDIFEntity::iterator
  SDIFEntity::begin ()  { 
    Directory::iterator beg = this->mFrameDirectory.begin();
    return iterator(this,beg,false);
  }
  inline SDIFEntity::const_iterator
  SDIFEntity::end () const {
    Directory::iterator iend = this->mFrameDirectory.end();
    return const_iterator(this,iend,true);
  }
  inline SDIFEntity::iterator
  SDIFEntity::end ()  {
    Directory::iterator iend = this->mFrameDirectory.end();
    return iterator(this,iend,true);
  }

  inline SDIFEntity::const_iterator
  SDIFEntity::lastRead() const {
    SDIFEntity::const_iterator it;
    return it.assignPos(this,mLastReadPos);
  }

  inline SDIFEntity::iterator
  SDIFEntity::lastRead()  {
    SDIFEntity::iterator it;
    return it.assignPos(this,mLastReadPos);
  }

} // end of namespace Easdif

#endif


