/****************************************************************************
sslzliboff.c - A Python extension written in C to disable Zlib Compression in
OpenSSL.

This file is Copyright 2012, Rodrigo de Alencar Hausen.

This program is free software: you can redistribute it and/or modify it under
the terms of the  GNU  Affero General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,  but  WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see: <http://www.gnu.org/licenses/>

******************************************************************************/

#include <Python.h>
#include <openssl/ssl.h>
#include <openssl/safestack.h>

const char *sslzliboff_version = "0.1";

#ifdef BROKEN_OPENSSL_INCLUDES
STACK_OF(SSL_COMP) *SSL_COMP_get_compression_methods(void);
#endif

static PyObject *sslzliboff_disableZlib(PyObject *self, PyObject *args)
{
    sk_SSL_COMP_zero(SSL_COMP_get_compression_methods());

    return Py_BuildValue("i", 0);
}


/*
 * Bind Python function names to our C functions
 */
static PyMethodDef sslzliboff_methods[] = {
	{"disableZlib", sslzliboff_disableZlib, METH_VARARGS},
	{NULL, NULL}
};

PyMODINIT_FUNC initsslzliboff(void)
{
	PyObject *m;
	m = Py_InitModule("sslzliboff", sslzliboff_methods);
	if (m == NULL)
		PyErr_SetString(PyExc_Exception, "Could not initialize sslzliboff");
}
