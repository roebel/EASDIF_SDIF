// $Id: easdif-defines.i,v 1.3 2014-04-10 21:34:07 roebel Exp $ -*-c-*-
//
// sdif_basics.h		16.04.2003		Diemo Schwarz
//
// define only the basic SDIF stuff for easdif swig from sdif.h
//
// $Log: not supported by cvs2svn $
// Revision 1.2  2008/03/04 02:08:39  roebel
// Define EASDIF_API to prevent swig errors.
//
// Revision 1.1  2003/04/29 16:29:09  schwarz
// renamed sdifdefines.i to easdif-defines.i
//
// Revision 1.1  2003/04/18 15:41:18  schwarz
// Don't include all definitions from sdif.h, only the ones needed by
// easdif, defined in sdifdefine.i.  -> smaller wrapper size:
// .cxx	 880k -> 130k
// .o	1000k -> 210k
// .pm	 123k -> 8k


/* SdifHard_OS.h */

#define EASDIF_API

typedef char           SdifChar;
typedef char           SdifInt1;
typedef short          SdifInt2;
typedef int            SdifInt4;
typedef unsigned char  SdifUInt1;
typedef unsigned short SdifUInt2;
typedef unsigned int   SdifUInt4;

typedef unsigned int   SdifSignature;
typedef float          SdifFloat4;
typedef double         SdifFloat8;


/* SdifGlobals.h */

typedef enum SdifDataTypeE
{
  eText	    = 0x0301,
  eChar     = 0x0301,
  eFloat4   = 0x0004,
  eFloat8   = 0x0008,
  eInt1     = 0x0101,
  eInt2     = 0x0102,
  eInt4     = 0x0104,
  eInt8     = 0x0108,
  eUInt1    = 0x0201,
  eUInt2    = 0x0202,
  eUInt4    = 0x0204,
  eUInt8    = 0x0208,
 	    
  eFloat4a  = 0x0001,	/* =  1 */    /* Backwards compatibility with old */
  eFloat4b  = 0x0020,	/* = 32 */    /* IRCAM versions < 3 of SDIF */
  eFloat8a  = 0x0002,	/* =  2 */    /* IN TEXT MODE ONLY! */
  eFloat8b  = 0x0040	/* = 64 */
} SdifDataTypeET;


/* SdifFileStruct.h */

typedef enum SdifFileModeE
{
  eUnknownFileMode,	/* 0 */
  eWriteFile,
  eReadFile,
  ePredefinedTypes,	/* 3 */
} SdifFileModeET;

