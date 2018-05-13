// $Id: easdif-python.i,v 1.20 2017-03-11 09:15:07 roebel Exp $ -*-c-*-
//
// easdif-python.i		30.04.2003		Patrice Tisserand
//
// Interface file for swig, defining the callable easdif functions
//
// $Log: not supported by cvs2svn $
// Revision 1.19  2014/08/26 14:38:24  roebel
// Added special exception specification for OpenRead to be able to catch the SDIFBadHeader exception.
//
// Revision 1.18  2014/06/19 20:48:52  roebel
// Properly handle exception in OpenRead.
//
// Revision 1.17  2014/05/23 10:33:15  roebel
// Changed so that ReadNext..Frame methods properly throw EOF exception.
//
// Revision 1.16  2014/05/21 23:55:21  roebel
// Fixed FrameType constructor to accept python strings with exactly 4 chars.
// Added __str__ method to FrameType and MatrixType structs to be able to print them.
//
// Revision 1.15  2014/04/11 17:33:31  roebel
// Support reading with int8 numpy type.
//
// Revision 1.14  2014/04/10 21:34:07  roebel
// More robust include file organisation.
// Extended types of vector that are supported to read from matrices and vectors.
//
// Revision 1.13  2012/09/02 01:19:03  roebel
// Added support for using numpy  arrays to Set/Get columns and rows from
// matrices. For large matrices this can increase efficiency when accessing
// matrix data by factor ~1000!
//
// Revision 1.12  2012/08/28 22:08:14  roebel
// Fixed next iterator for sdifentity.
//
// Revision 1.11  2012/08/19 18:28:57  roebel
// Added copy method to Frame.
//
// Revision 1.10  2012/01/05 11:11:21  roebel
// Added documentation and fixed memory leak.
//
// Revision 1.9  2012/01/05 01:18:07  roebel
// Make Entity and Frame iterable and take car that iteration is efficient by means of
// returning pointers to Frames and matrices.
//
// Revision 1.8  2012/01/04 12:19:09  roebel
// Create derived class to render Frame and Entity into python iterable.
//
// Revision 1.7  2006/12/10 17:37:30  roebel
// SelectionSet now renamed to SelectionSetI to prevent
// name clash in swig.
//
// Revision 1.6  2006/12/08 18:02:20  roebel
// Extended python support for SelectionSet.
//
// Revision 1.5  2006/11/26 20:35:41  roebel
// Unified by means of ../easdif-common-init.i
//
// Revision 1.4  2006/11/26 16:15:08  roebel
// Improved platform support by means of using automake more consistently.
// Added new example script.
// Reorganized to work with swig 1.3.31!
//
// Revision 1.3  2003/06/05 11:15:09  roebel
// Moved stlmap conditional to easdif-common.i.in
//
// Revision 1.2  2003/05/28 16:57:03  roebel
// Made configure switching map support according to the swig version installed.
//
// Revision 1.1  2003/04/30 11:45:48  tisseran
// Added swig python stuff
// Started autoconfiscation of swig
//
// include common init


%include ../easdif-common-init.i
%feature("autodoc", "1");

// include typemaps
%include sdiftypemap-python.i


// define only the needed basic SDIF stuff from sdif.h
%include ../easdif-defines.i

%exception OpenRead {
        try {
                $action
        } catch(const SDIFBadHeader& e) {
                PyErr_SetString(PyExc_EOFError,e.getmessage().c_str());
                return 0;
        } catch(const SDIFEof& e) {
                PyErr_SetString(PyExc_EOFError,e.getmessage().c_str());
                return 0;
        } catch(const SDIFException& e) {
                PyErr_SetString(PyExc_IOError,e.getmessage().c_str());
                return 0;
        } catch(const std::exception& e) {
                PyErr_SetString(PyExc_IOError, e.what());
                return 0;
        } catch(...) {
                SWIG_exception(SWIG_UnknownError,"Unknown exception");
        }
}

%exception ReadNextFrame {
        try {
                $action
        } catch(const SDIFEof& e) {
                PyErr_SetString(PyExc_EOFError,e.getmessage().c_str());
                return 0;
        } catch(const SDIFException& e) {
                PyErr_SetString(PyExc_IOError,e.getmessage().c_str());
                return 0;
        } catch(const std::exception& e) {
                PyErr_SetString(PyExc_IOError, e.what());
                return 0;
        } catch(...) {
                SWIG_exception(SWIG_UnknownError,"Unknown exception");
        }
}

%exception ReadNextSelectedFrame {
        try {
                $action
        } catch(const SDIFEof& e) {
                PyErr_SetString(PyExc_EOFError,e.getmessage().c_str());
                return 0;
        } catch(const SDIFException& e) {
                PyErr_SetString(PyExc_IOError,e.getmessage().c_str());
                return 0;
        } catch(const std::exception& e) {
                PyErr_SetString(PyExc_IOError, e.what());
                return 0;
        } catch(...) {
                SWIG_exception(SWIG_UnknownError,"Unknown exception");
        }
}

%exception ReadPrevSelectedFrame {
        try {
                $action
        } catch(const SDIFEof& e) {
                PyErr_SetString(PyExc_EOFError,e.getmessage().c_str());
                return 0;
        } catch(const SDIFException& e) {
                PyErr_SetString(PyExc_IOError,e.getmessage().c_str());
                return 0;
        } catch(const std::exception& e) {
                PyErr_SetString(PyExc_IOError, e.what());
                return 0;
        } catch(...) {
                SWIG_exception(SWIG_UnknownError,"Unknown exception");
        }
}

%exception OpenRead {
        try {
                $action
        } catch(const SDIFException& e) {
                PyErr_SetString(PyExc_IOError,e.getmessage().c_str());
                return 0;
        } catch(const std::exception& e) {
                PyErr_SetString(PyExc_IOError, e.what());
                return 0;
        } catch(...) {
                SWIG_exception(SWIG_UnknownError,"Unknown exception");
        }
}



%define NEXT_INTERFACE_SUPPORT(NEXT_FUN_NAME)
%{
#include "easdif/easdif.h"

  
// proxy class for frame that provides Frame iterator with storage location for iterator position
struct
FrameIt  {
  const Easdif::SDIFFrame *mpOriFrame;
  int mIterPos;
  FrameIt(const Easdif::SDIFFrame& in ) : mpOriFrame(&in), mIterPos(0) {}
  ~FrameIt() {}

  const Easdif::SDIFMatrix&  
  NEXT_FUN_NAME() {
    if(mIterPos < static_cast<int>(mpOriFrame->GetNbMatrix()))
      return mpOriFrame->GetMatrix(mIterPos++);
    throw std::runtime_error("StopIteration");
  }
};

%}


// include common module directive
%include ../easdif-common.i


// include typemap for std::string from SWIG library
%include std_set.i

// provide access to std::set as eaSDIF.vector
namespace std {
   %template(ISet) set<unsigned int>;
};


%template(SelectionSetI) Easdif::SelectionSet<unsigned int>;


// make entity iterable
// but attention next method does not provide a reference
// but a pointer to a new Frame so we cannot modify the entity by means of the 
// frames in a loop

%exception Easdif::SDIFEntity::NEXT_FUN_NAME() {
  try {
    $action
      }
  catch(const Easdif::SDIFEof& e) {
      PyErr_SetNone(PyExc_StopIteration);
      return NULL;
  }
 }

%newobject Easdif::SDIFEntity::NEXT_FUN_NAME;

%extend Easdif::SDIFEntity{
  Easdif::SDIFEntity&
  __iter__() {
    // ensure that the complete file will be read
    $self->Rewind();
    return *$self;
  }

   // prefer to work with pointers in python
   Easdif::SDIFFrame*
   NEXT_FUN_NAME() {
     //allocate new frame
     Easdif::SDIFFrame *localFrame = new(Easdif::SDIFFrame);
     try {
       // force to run into eof exception if unselected frames are located at the end of the file
       while(!$self->ReadNextSelectedFrame(*localFrame)){};
     } catch(...) {
       delete localFrame;
       throw;
     }
     return localFrame;
   }
 }

// make entity iterable
// but attention next method does not provide a reference
// but a pointer to a new Matrix.  This avoids problems with the missing reference counting
// that would arise if teh original frame disappears before the matrix
%exception Easdif::SDIFFrame::NEXT_FUN_NAME() {
  try {
    $action
      }
  catch(const std::runtime_error& e) {
      PyErr_SetNone(PyExc_StopIteration);
      return NULL;
  }
 }

%newobject Easdif::SDIFFrame::NEXT_FUN_NAME;
%newobject Easdif::SDIFFrame::copy;
%extend Easdif::SDIFFrame {
  Easdif::SDIFFrame*
  __iter__() {
    return reinterpret_cast<Easdif::SDIFFrame*>(new FrameIt(*$self));
  }
  // prefer to work with pointers in python
  Easdif::SDIFMatrix*
  NEXT_FUN_NAME() {
    FrameIt * it = reinterpret_cast<FrameIt*>($self);
    Easdif::SDIFMatrix *pMat = new(Easdif::SDIFMatrix);
    try {
      *pMat = it->NEXT_FUN_NAME();
    }
    catch(...) {
      delete  pMat;
      throw;
    }
    return pMat; 
  }

  Easdif::SDIFFrame*
  copy() {
    return new Easdif::SDIFFrame(*$self);
  }
}

%enddef

#ifdef SWIG_PYTHON_3
   NEXT_INTERFACE_SUPPORT(__next__)
#else
   NEXT_INTERFACE_SUPPORT(next)
#endif


%extend Easdif::FrameType{
    PyObject* __str__() {
      std::string str;
      str = " 1FTD ";
      str += SdifSignatureToString($self->mSig);
      str += "\n\t{\n\t ";
      for(int ii=0; ii< $self->mvMatrixTypes.size(); ++ii) {
        str += SdifSignatureToString($self->mvMatrixTypes[ii].GetSignature());
        str += "\t";
        str += $self->mvMatrixNames[ii];
        str += ";\n\t ";
      }
      str += "}\n";
      
      return PyString_FromString(str.c_str());
    }
 }

%extend Easdif::MatrixType{
    PyObject* __str__() {
      std::string str;
      str = " 1MTD ";
      str += SdifSignatureToString($self->mSig);
      str += "\t{";
      int nb = $self->GetNbCols();
      for(int ii=0; ii< nb; ++ii) {
        str += $self->mvColumnNames[ii];
        if (ii < nb - 1)
          str += ",";
      }
      str += "}\n";
      
      return PyString_FromString(str.c_str());
    }
 }



#ifdef USE_NUMPY

%exception GetColA {
  $action
  if( result == 0) {
    PyErr_SetString(PyExc_IndexError,"GetColA::output array does not match column size"); 
    return 0;
  }
}
%exception SetColA {
  $action
  if( result == 0) {
    PyErr_SetString(PyExc_IndexError,"SetColA::input array does not match column size"); 
    return 0;
  }
}
%exception GetRowA {
  $action
  if( result == 0) {
    PyErr_SetString(PyExc_IndexError,"GetRowA::output array does not match row size"); 
    return 0;
  }
}
%exception SetRowA {
  $action
  if( result == 0) {
    PyErr_SetString(PyExc_IndexError,"SetRowA::input array does not match row size"); 
    return 0;
  }
}




%extend Easdif::SDIFMatrix {


%apply (double* INPLACE_ARRAY1, int DIM1) {(double *outarr, int outarrsize)}
%apply (double* IN_ARRAY1, int DIM1) {(const double *inarr, int inarrsize)}
%apply (float* INPLACE_ARRAY1, int DIM1) {(float *outarr, int outarrsize)}
%apply (float* IN_ARRAY1, int DIM1) {(const float *inarr, int inarrsize)}
%apply (int* INPLACE_ARRAY1, int DIM1) {(int *outarr, int outarrsize)}
%apply (int* IN_ARRAY1, int DIM1) {(const int *inarr, int inarrsize)}
%apply (short* INPLACE_ARRAY1, int DIM1) {(short *outarr, int outarrsize)}
%apply (short* IN_ARRAY1, int DIM1) {(const short *inarr, int inarrsize)}
%apply (char* INPLACE_ARRAY1, int DIM1) {(char *outarr, int outarrsize)}
%apply (char* IN_ARRAY1, int DIM1) {(char *inarr, int inarrsize)}
%apply (signed char* INPLACE_ARRAY1, int DIM1) {(signed char *outarr, int outarrsize)}
%apply (signed char* IN_ARRAY1, int DIM1) {(signed char *inarr, int inarrsize)}
%apply (unsigned char* INPLACE_ARRAY1, int DIM1) {(unsigned char *outarr, int outarrsize)}
%apply (unsigned char* IN_ARRAY1, int DIM1) {(const unsigned char *inarr, int inarrsize)}
%apply (unsigned short* INPLACE_ARRAY1, int DIM1) {(unsigned short *outarr, int outarrsize)}
%apply (unsigned short* IN_ARRAY1, int DIM1) {(const unsigned short *inarr, int inarrsize)}
%apply (unsigned int* INPLACE_ARRAY1, int DIM1) {(unsigned int *outarr, int outarrsize)}
%apply (unsigned int* IN_ARRAY1, int DIM1) {(const unsigned int *inarr, int inarrsize)}

#define MakeGetColA(type) int \
    GetColA(type * outarr, int outarrsize, int col) { \
    if ($self->GetNbRows() != outarrsize ) {\
      return 0; \
    }\
    $self->GetCol(outarr, col); \
    return 1; \
  }
  int  GetColA(signed char * outarr, int outarrsize, int col) { 
    if ($self->GetNbRows() != outarrsize ) {
      return 0;
    }
    $self->GetCol(reinterpret_cast<char*> (outarr), col); 
    return 1; 
  }

   MakeGetColA(double);
   MakeGetColA(float);
   MakeGetColA(unsigned int);
   MakeGetColA(unsigned short);
   MakeGetColA(unsigned char);
   MakeGetColA(int);
   MakeGetColA(short);
   MakeGetColA(char);

#define MakeGetRowA(type) int \
    GetRowA(type * outarr, int outarrsize, int col) { \
    if ($self->GetNbCols() != outarrsize ) {\
      return 0; \
    }\
    $self->GetRow(outarr, col); \
    return 1; \
  }

 int  GetRowA(signed char * outarr, int outarrsize, int col) { 
    if ($self->GetNbRows() != outarrsize ) {
      return 0; 
    }
    $self->GetRow(reinterpret_cast<char*> (outarr), col); 
    return 1; 
  }

   MakeGetRowA(double);
   MakeGetRowA(float);
   MakeGetRowA(unsigned int);
   MakeGetRowA(unsigned short);
   MakeGetRowA(unsigned char);
   MakeGetRowA(int);
   MakeGetRowA(short);
   MakeGetRowA(char);

#define MakeSetColA(type) int \
    SetColA(const type * inarr, int inarrsize, int col) { \
    if ($self->GetNbRows() != inarrsize ) {\
      return 0; \
    }\
    $self->SetCol(inarr, col);                  \
    return 1; \
  }

   MakeSetColA(double);
   MakeSetColA(float);
   MakeSetColA(unsigned int);
   MakeSetColA(unsigned short);
   MakeSetColA(unsigned char);
   MakeSetColA(int);
   MakeSetColA(short);
   MakeSetColA(char);

   int SetColA(const signed char * outarr, int outarrsize, int col) { 
    if ($self->GetNbRows() != outarrsize ) {
      return 0;
    }
    $self->SetCol(reinterpret_cast<const char*> (outarr), col); 
    return 1; 
  }

#define MakeSetRowA(type) int \
    SetRowA(const type * inarr, int inarrsize, int col) { \
    if ($self->GetNbCols() != inarrsize ) {\
      return 0; \
    }\
    $self->SetRow(inarr, col); \
    return 1; \
  }


   MakeSetRowA(double);
   MakeSetRowA(float);
   MakeSetRowA(unsigned int);
   MakeSetRowA(unsigned short);
   MakeSetRowA(unsigned char);
   MakeSetRowA(int);
   MakeSetRowA(short);
   MakeSetRowA(char);

  int  SetRowA(const signed char * outarr, int outarrsize, int col) { 
    if ($self->GetNbRows() != outarrsize ) {
      return 0;
    }
    $self->SetRow(reinterpret_cast<const char*> (outarr), col); 
    return 1; 
  }


  PyObject*                                         
    GetMatrixData() { 
    int ssrows = $self->GetNbRows();
    int sscols = $self->GetNbCols();
    int type = 0;
    int internal_type = $self->GetType();
    if (internal_type == eFloat8)
      type =NPY_DOUBLE;
    else if (internal_type == eFloat4)
      type =NPY_FLOAT32;
    else if (internal_type == eInt8)
      type =NPY_INT64;
    else if (internal_type == eUInt8)
      type =NPY_UINT64;
    else if (internal_type == eInt4)
      type =NPY_INT32;
    else if (internal_type == eUInt4)
      type =NPY_UINT32;
    else if (internal_type == eInt2)
      type =NPY_INT16;
    else if (internal_type == eUInt2)
      type =NPY_UINT16;

    npy_intp ss[2];
    ss[0] = ssrows;
    ss[1] = sscols;
    PyArrayObject *pa = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNew(2, ss, type));
    if (!pa) { 
      PyErr_SetString(PyExc_RuntimeError, "cannot create umpy array for matrix content\n");
      return 0;
    }
    // or fortran style column continuous which is the format used in SDIF
    PyArray_ENABLEFLAGS(pa, NPY_ARRAY_F_CONTIGUOUS);
    PyArray_CLEARFLAGS(pa, NPY_ARRAY_C_CONTIGUOUS);
    if (ssrows * sscols) {
      void *pp = PyArray_DATA(pa);
      memcpy(pp, $self->GetData(), ssrows*sscols*$self->GetElementSize());
    }
    return reinterpret_cast<PyObject*>(pa);
  }

 int  GetRowA(signed char * outarr, int outarrsize, int col) { 
    if ($self->GetNbRows() != outarrsize ) {
      return 0; 
    }
    $self->GetRow(reinterpret_cast<char*> (outarr), col); 
    return 1; 
  }

   MakeGetRowA(double);
   MakeGetRowA(float);
   MakeGetRowA(unsigned int);
   MakeGetRowA(unsigned short);
   MakeGetRowA(unsigned char);
   MakeGetRowA(int);
   MakeGetRowA(short);
   MakeGetRowA(char);

}
#endif
