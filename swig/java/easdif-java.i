// $Id: easdif-java.i,v 1.4 2006-11-27 16:52:53 roebel Exp $ -*-c-*-
//
// easdif-java.i		05.05.2003		Patrice Tisserand
//
// Interface file for swig, defining the callable easdif functions
//
// $Log: not supported by cvs2svn $
// Revision 1.3  2006/11/26 20:34:44  roebel
// Use automatic directory names.
//
// Revision 1.2  2005/07/25 13:29:24  roebel
// Adapted for new include file names - h->hpp.
//
// Revision 1.1  2003/05/05 16:05:32  tisseran
// Added swig java binding.
// TODO: change configure and java/Makefile.am for jni include path
//
//


// generate java module named 'eaSDIF'
%module eaSDIF


// swig + java doesn't like a lot method overloaded only by const
%ignore Easdif::SDIFNameValueTable::begin() const;
%ignore Easdif::SDIFNameValueTable::end() const;

// const char* and std::string are the same
%ignore Easdif::EasdifInit(const char*);

%ignore  Easdif::SDIFEntity::begin() const;
%ignore  Easdif::SDIFEntity::end() const;
%ignore  Easdif::SDIFEntity::current() const;
// we know how to convert SdifSignature to std::string ...
%ignore Easdif::SDIFFrame::SetSignature(SdifSignature);
%ignore Easdif::SDIFFrame::SetHeader(SdifSignature,SdifUInt4 , SdifFloat8);
%ignore Easdif::SDIFFrame::GetMatrix(unsigned int) const;
%ignore Easdif::SDIFFrame::GetMatrix(const std::string&) const;
%ignore Easdif::SDIFFrame::begin() const;
%ignore Easdif::SDIFFrame::end() const;

%ignore Easdif::SDIFMatrix::SetSignature(SdifSignature);
%ignore Easdif::SDIFMatrix::Init(SdifSignature, int, int, SdifDataTypeET);
%ignore Easdif::SDIFMatrix::SDIFMatrix(SdifSignature);
%ignore Easdif::SDIFMatrix::SDIFMatrix(SdifSignature,int);
%ignore Easdif::SDIFMatrix::SDIFMatrix(SdifSignature,int,int);

%ignore Easdif::SDIFLocation::LocMatrixExists(SdifSignature) const;

// include typemaps
%include sdiftypemap-java.i

// include common interface files
%include ../easdif-defines.i
%include ../easdif-common.i

// java module need to know EasdifInit function
%include "../../easdif/sdifinit.hpp"
