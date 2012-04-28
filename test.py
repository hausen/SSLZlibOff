#!/usr/bin/python
# Tests if sslzliboff has been built and installed correctly.

try:
    import sslzliboff
    sslzliboff.disableZlib()
    print("OK. sslzliboff loaded correctly.")
except Exception as e:
    print("ERROR: sslzliboff not loaded. This shouldn't have happened.")

