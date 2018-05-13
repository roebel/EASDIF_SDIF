/*
 * This file is part of the IRCAM EASDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 2002-2007 by IRCAM-Centre Georges Pompidou, Paris, France.
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
 * @file   sdifnamevaluetable.h
 * @author Fabien Tisserand
 * @date   Tue Jun 18 19:38:54 2002
 * 
 * @brief  
 *
 * 
 *
 * 
 * $Id: sdifnamevaluetable.hpp,v 1.4 2008-01-22 00:52:25 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.3  2007/11/26 19:09:18  roebel
 * Fixed to avoid compiler warnings in MSVC.
 * Little problem is the export of std::containers that should be defined as export
 * which is not possible due to the given STL include files.
 * For the moment it seems these warnings are not important, because all these functions
 * are inlined as templates in the STL anyway. Has to be handled with care !!!!
 *
 * Revision 1.2  2007/10/25 22:31:08  roebel
 * Fixed constructor to initialize streamid to Signature NVID
 * which translates in a very large value that will no longer conflict with cnmat library
 * when 1NVT Frames and data frames stay on the same streamid.
 *
 * Revision 1.1  2005/05/30 21:43:00  roebel
 * Changed all include files from .h into .hpp to prevent name clash between
 * sdifmatix.h and SDIF/sdifcpp/SdifMatrix.h on MacOSX where filenames are
 * case insensitive.
 *
 * Revision 1.9  2004/07/27 17:40:19  roebel
 * Changed include directive to use user path and not system path for sdif.h
 *
 * Revision 1.8  2004/02/11 19:28:17  roebel
 * Added inline function to test state of entity. Added function to access NVTs that indicates existance of name.
 *
 * Revision 1.7  2003/05/22 21:23:58  roebel
 * SDIFNameValueTable now derived from std::map which makes handling more conform
 * to standard and handling in swig easier.
 *
 * Revision 1.6  2003/05/19 13:59:40  roebel
 * swig rename moved to swig  interface desription.
 *
 * Revision 1.5  2003/04/29 15:54:08  schwarz
 * Use SWIG_RENAME_EASDIF to control class renaming.
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
 * Revision 1.5  2002/11/28 21:16:56  roebel
 * Back to simple map which sorts alphabetically. In any case
 * in SDIF the name value tables are mixed arbitrarily.
 *
 * Revision 1.4  2002/11/27 20:10:39  roebel
 * Fixed NVT to keep ordering as defined by the sequence of additions to the table.
 * Added begin/end iterator.
 * Added global functions to get name and value from SDIFNameValueTable iterators.
 * Removed member that redundantly indicated size of map.
 *
 * Revision 1.3  2002/10/10 10:49:09  roebel
 * Now using namespace Easdif.
 * Fixed handling of zero pointer arguments in initException.
 * Reading past end of file now throws an exception.
 *
 * Revision 1.2  2002/08/28 16:46:53  roebel
 * Internal reorganization and name changes.
 *
 * Revision 1.1  2002/06/18 17:56:06  ftissera
 * Project for new SDIF API
 * 
 * 
 */

#ifndef SDIFNAMEVALUETABLE_H_
#define SDIFNAMEVALUETABLE_H_ 1

#include <string>
#include <vector>
#include <map>
#include <iterator>
#include "sdif.h"

#include "easdif_exports.hpp"

namespace Easdif {

  /** 
   * @brief class which can be associated with a Name Value Table
   *
   * SDIFNameValueTable is composed of different methods which permits to
   * manipulate a Name Value Table.
   *
   * It is derived from std::map<std::string,std::string> and provides
   * all methods of that class
   */
  class EASDIF_API SDIFNameValueTable : public std::map<std::string,std::string>
  {
  private:

    SdifUInt4 mStreamID;
    typedef std::map<std::string,std::string> NVTMap;

  public:

    /* construct a NameValueTable, use StreamID with value NVID as default */
    SDIFNameValueTable() : mStreamID( _SdifNVTStreamID )  {};
  
    ~SDIFNameValueTable() {};

    /** 
     *  \brief iterator types for iterating over the internal map
     * 
     */
    typedef NVTMap::const_iterator const_iterator;
    typedef NVTMap::iterator iterator;

    /** 
     *  \brief add a Name Value in the map
     * 
     * @param name 
     * @param value 
     * 
     * @return  the number of Name Values in the map
     */
    int AddNameValue(const std::string& name, const std::string& value);


    /*************************************************************************/
    /*
    // FUNCTION GROUP:	set the members
    */
  
    /**  
     * @brief set the streamID
     * 
     * @param streamid 
     * 
     * @return 
     */
    SdifUInt4 SetStreamID(const SdifUInt4& streamid);

    /*************************************************************************/
    /*
    // FUNCTION GROUP:	to see
    */
    /** 
     * @brief print a Name Value
     * 
     * 
     * @param name 
     */
    void PrintNameValue(const std::string& name)const;

    /** 
     * @brief print a Name Value Table
     */
    void PrintNameValueTable() const;


    /*************************************************************************/
    /*
    // FUNCTION GROUP:	get the members
    */
    /** 
     * @brief get the number of Name Value
     */
    int GetNbNameValue();

    /** 
     * @brief get the StreamID
     */
    SdifUInt4 GetStreamID();

    /** 
     * @brief get the Value
     * @param name string
     * @return value string
     */
    std::string GetValue(const std::string& name) const;

    /** 
     * @brief get the Value
     * @param name  string
     * @param value result string
     * @return true if name was found false if not. 
     */
    bool GetValue(const std::string& name,std::string& value ) const;


  };
} // end of namespace Easdif

#endif

