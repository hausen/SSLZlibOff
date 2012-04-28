# sslzliboff #

This is a Python extension written in C to disable Zlib Compression in
OpenSSL, which saves lots of memory per connection.

The original idea is due to Paul Querna [1]. Its Python implementation
uses the ctypes module to import some C functions from the OpenSSL C
library, which are then directly called from Python. In some versions
of OpenSSL, for instance in the one distributed with DD-WRT v24-sp2,
calling those exported functions directly from Python causes the
interpreter to exit with a Bus Error or a Segmentation Fault, whereas
if those functions are called from a C program, no error occurs.

This extension implements a function in C with bindings for Python that
does the task of disabling the Zlib compression directly, avoiding
segfaults.

## Building and Installing ##

The usual headers required for compiling Python extensions and for
compiling C programs that use the OpenSSL must be installed (generally
they come in the form of packages named pythonX.Y-dev and
libssl-dev/openssl-dev). After making sure that those headers are
installed, compile the extension:

    $ python setup.py build

If the C compiler complains about an implicit declaration of the function
`SSL_COMP_get_compression_methods`, then there is a missing function
declaration in the OpenSSL headers. If this warning appears, edit `setup.py`,
set `BROKEN_OPENSSL_INCLUDES` to `True`, remove the `build` directory and
recompile.

The extension can be installed system-wide via

    $ python setup.py install

or the sslzliboff.so file can be placed in the same directory of the
Python program that will use it.

## Usage ##

A safer way of disabling the Zlib compression in OpenSSL can be achieved by
calling the disableSSLCompression function below, which is a slight
improvement on Paul Querna's original idea. Notice that the original
method of disabling the compression is a fallback, in the case that
sslzliboff is not found.

    def disableSSLCompression(self):
      try:
        import sslzliboff
        sslzliboff.disableZlib()
      except Exception:
        try:
          import ctypes
          import glob
          openssl = ctypes.CDLL(None, ctypes.RTLD_GLOBAL)
          try:
            f = openssl.SSL_COMP_get_compression_methods
          except AttributeError:
            ssllib = sorted(glob.glob("/usr/lib/libssl.so.*"))[0]
            openssl = ctypes.CDLL(ssllib, ctypes.RTLD_GLOBAL)

          openssl.SSL_COMP_get_compression_methods.restype = ctypes.c_void_p
          openssl.sk_zero.argtypes = [ctypes.c_void_p]
          openssl.sk_zero(openssl.SSL_COMP_get_compression_methods())
        except Exception as e:
          log.msg('disableSSLCompression: Failed:')
          log.msg(e)

## License and Copyright ##

sslzliboff is Copyright 2012, Rodrigo de Alencar Hausen <https://github.com/hausen>

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

## REFERENCES ##

[1] QUERNA, Paul. OpenSSL memory use in Node.js. <http://journal.paul.querna.org/articles/2011/04/05/openssl-memory-use/> Last access: 2012-04-28
