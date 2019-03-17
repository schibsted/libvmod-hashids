# vmod-hashids

This module makes hashids encoding and decoding available in VCL.
See https://hashids.org/ for details about the hashids library.

# Functions

`hashids.encode(salt, number)` returns a hashid for `number` salted with
`salt`.

`hashids.decode(salt, hashid)` returns the decoded number for `hashid`
salted with `salt`.

# Errors

```hashids.decode()``` will return negative numbers between -1 and -6 if an error occurs during decoding.

The table below shows the meaning of the returned error codes:

| Error code | Description                                                    |
|------------|----------------------------------------------------------------|
| -1         | Input hash is empty.                                           |
| -2         | Input hash is larger than max allowed size (default: 64 bytes) |
| -3         | Memory allocation error.                                       |
| -4         | No numbers was returned.                                       |
| -5         | Invalid hash / salt mismatch.                                  |
| -6         | Hashid library initialization failed.                          |

```hashids.encode()``` will return an empty string if an error occurs.

# Example

```
import hashids;

sub vcl_deliver {
  # Encodes 1234
  set resp.http.Encoded-Hash = hashids.encode("mysalt", 1234);

  # Decodes resp.http.Encoded-Hash
  set resp.http.Decoded-Hash = hashids.decode("mysalt", resp.http.Encoded-Hash);
}
```

# Installation

The source tree is based on autotools to configure the building, and
does also have the necessary bits in place to do functional unit tests
using the `varnishtest` tool.

Building requires the Varnish header files and uses pkg-config to find
the necessary paths.

Usage::

./autogen.sh ./configure

If you have installed Varnish to a non-standard directory, call
`autogen.sh` and `configure` with `PKG_CONFIG_PATH` pointing to the
appropriate path. For instance, when varnishd configure was called with
`--prefix=$PREFIX`, use

::

export
PKG\_CONFIG\_PATH=\({PREFIX}/lib/pkgconfig  export ACLOCAL_PATH=\){PREFIX}/share/aclocal

The module will inherit its prefix from Varnish, unless you specify a
different `--prefix` when running the `configure` script for this
module.

Make targets:

  - make - builds the vmod.
  - make install - installs your vmod.

## Installation directories

By default, the vmod `configure` script installs the built vmod in the
directory relevant to the prefix. The vmod installation directory can be
overridden by passing the `vmoddir` variable to `make install`.

# Common problems

  - configure: error: Need varnish.m4 – see README.rst
    
    Check whether `PKG_CONFIG_PATH` and `ACLOCAL_PATH` were set
    correctly before calling `autogen.sh` and `configure`

  - Incompatibilities with different Varnish Cache versions
    
    Make sure you build this vmod against its correspondent Varnish
    Cache version. For instance, to build against Varnish Cache 4.1,
    this vmod must be built from branch 4.1.
