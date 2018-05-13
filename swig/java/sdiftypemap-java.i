// $Id: sdiftypemap-java.i,v 1.2 2006-11-26 16:16:27 roebel Exp $ -*-c-*-
//
// sdiftypemap.i		17.04.2003		Diemo Schwarz
//
// typemaps for SWIG to map SdifSignature to strings and back
//
// $Log: not supported by cvs2svn $
// Revision 1.1  2003/05/05 16:05:32  tisseran
// Added swig java binding.
// TODO: change configure and java/Makefile.am for jni include path
//
// Revision 1.1  2003/04/18 17:45:23  schwarz
// Renamed sdifsignature.i to sdiftypemap.i, because all types are in there.
// Include typemap for std::string from SWIG library
// Added typemap freearg SdifStringT *
//
// Revision 1.2  2003/04/18 14:30:10  schwarz
// Added typemap for string to SdifSignature conversion (input arg).
//
// Revision 1.1  2003/04/17 12:19:56  schwarz
// Perl output mapping for SdifSignature works
// (by comparison with _wrap_SdifSignatureToString).


// include typemap for std::string from SWIG library
%include std_string.i


//
// typemaps for SWIG to map SdifSignature to strings and back
//
/* %typemap(java, in) SdifSignature  */
/* { */
/*     if (PyString_Check($input)) */
/*     { */
/* 	$1 = SdifStringToSignature(PyString_AsString($input)); */
/*     } */
/*     else */
/*     { */
/* 	PyErr_SetString(PyExc_TypeError, "not a String"); */
/* 	return NULL; */
/*     } */
/* } */

%typemap(in) SdifSignature
{
    if ($input)
    {
	const char* pstr = (const char*)jenv->GetStringUTFChars($input, 0);
	if (!pstr)
	    return $null;
	$1 = SdifStringToSignature(const_cast<char*>(pstr));
    }
}

%typemap(out) SdifSignature
{
    $result = jenv->NewStringUTF(SdifSignatureToString($1));
}

%typemap(jni) SdifSignature "jstring"
%typemap(jtype) SdifSignature "String"
%typemap(jstype) SdifSignature "String"

//
// typemaps for SWIG to map SdifStringT to strings and back
//

/* // using an SdifStringT as input argument: convert from python string */
/* %typemap(in) SdifStringT * */
/* { */
/*     SdifStringT *str = SdifStringNew();		// memory leak... */
/*     SdifStringAppend(str, PyString_AsString($input)); */
/*     $1 = str; */
/* } */

/* // using an SdifStringT as return value: convert to python string */
/* %typemap(out) SdifStringT * */
/* {  */
/*     $result = PyString_FromString($1->str); */
/* } */

/* %typemap (python, freearg) SdifStringT * */
/* { */
/*     SdifStringFree($1); */
/* } */
