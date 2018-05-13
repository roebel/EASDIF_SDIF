// $Id: sdiftypemap-python.i,v 1.9 2017-03-16 19:46:25 roebel Exp $ -*-c-*-
//
// sdiftypemap-python.i		30.04.2003		Patrice Tisserand
//
// typemaps for SWIG to map SdifSignature to strings and back
//
// $Log: not supported by cvs2svn $
// Revision 1.8  2017/03/15 10:54:34  roebel
// Summary: Version 1.4.25 - Python 3 fix
//
// Fixed typemap for converting strings into sdif signatures.
//
// Revision 1.7  2015/12/03 18:03:01  roebel
// Added support for python 3.
//
// Revision 1.6  2014/05/21 23:55:21  roebel
// Fixed FrameType constructor to accept python strings with exactly 4 chars.
// Added __str__ method to FrameType and MatrixType structs to be able to print them.
//
// Revision 1.5  2006/11/26 20:35:41  roebel
// Unified by means of ../easdif-common-init.i
//
// Revision 1.4  2006/11/26 16:39:30  roebel
// Renamed representation of std::vector<double> from eaSDIF.vectord
// into eaSDIF.Vector
//
// Revision 1.3  2006/11/26 16:15:08  roebel
// Improved platform support by means of using automake more consistently.
// Added new example script.
// Reorganized to work with swig 1.3.31!
//
// Revision 1.2  2003/05/18 21:10:55  roebel
// Added proper handling of int to typemap.
//
// Revision 1.1  2003/04/30 11:45:48  tisseran
// Added swig python stuff
// Started autoconfiscation of swig
//


//
// typemaps for SWIG to map SdifSignature to strings and back
//
%typemap(in) SdifSignature 
{
%#if PY_MAJOR_VERSION < 3  
  if (PyString_Check($input) && (PyString_Size($input) == 4))
    {
	$1 = SdifStringToSignature(PyString_AsString($input));
    }
  else if (PyInt_Check($input))
    {
	$1 = PyInt_AS_LONG($input);
    }
%#else  
  if (PyUnicode_Check($input) )
    {
      Py_ssize_t len = 0;
      char *ascii_string = PyUnicode_AsUTF8AndSize($input, &len);

      if (ascii_string==0) {
        /* exception is set in conversion function */
        return NULL;
      }
      if(len ==4) 
	$1 = SdifStringToSignature(ascii_string);
      else{
        PyErr_SetString(PyExc_TypeError, "Signature argument has to be an integer or a 4 char ASCII string");
	return NULL;
      }
    }
    else if (PyInt_Check($input))
    {
	$1 = PyLong_AS_LONG($input);
    }
%#endif
  
    else  
    {
      PyErr_SetString(PyExc_TypeError, "Signature argument has to be an integer or a 4 char ASCII string");
	return NULL;
    }
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_INTEGER) SdifSignature {
  $1 = (PyString_Check($input)  
         || PyInt_Check($input)) ? 1 : 0;
}

%typemap(out) SdifSignature 
{
    $result = PyString_FromString(SdifSignatureToString($1));
}


//
// typemaps for SWIG to map SdifStringT to strings and back
//

// using an SdifStringT as input argument: convert from python string
%typemap(in) SdifStringT *
{
    SdifStringT *str = SdifStringNew();		// memory leak...
    SdifStringAppend(str, PyString_AsString($input));
    $1 = str;
}

// using an SdifStringT as return value: convert to python string
%typemap(out) SdifStringT *
{ 
    $result = PyString_FromString($1->str);
}

%typemap (freearg) SdifStringT *
{
    SdifStringFree($1);
}
