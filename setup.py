#!/usr/bin/python
BROKEN_OPENSSL_INCLUDES = False

from distutils.core import setup, Extension
import sys

defines = []

if BROKEN_OPENSSL_INCLUDES:
    defines = [ ( 'BROKEN_OPENSSL_INCLUDES', '1' ) ]

module1 = Extension('sslzliboff',
                    libraries = ['ssl'],
                    sources = ['sslzliboff.c'],
                    define_macros = defines)

setup (name = 'SSLZlibOff',
       version = '0.1',
       description = 'Disables ZLib compression in OpenSSL',
       ext_modules = [module1])
