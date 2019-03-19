Summary: Hashids support for varnish
Name: vmod-hashids
Version: 0.2
Release: 1%{?dist}
License: BSD
Group: System Environment/Daemons
Source0: vmod-hashids-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Requires: varnish >= 4.1.9
BuildRequires: make
BuildRequires: varnish >= 4.1.9
BuildRequires: varnish-devel >= 4.1.9

%description
Hashids encoding and decoding support

%prep
%setup -n vmod-hashids-%{version}

%build
./configure --prefix=/usr --libdir=%{_libdir}
make

%install
make install DESTDIR=%{buildroot}
cp LICENSE %{buildroot}/usr/share/doc/%{name}/

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%{_libdir}/varnish/vmods/
%doc /usr/share/doc/%{name}/*
%{_mandir}/man?/*

%changelog
* Tue Mar 19 2019 Ole Fredrik Skudsvik <ole.fredrik.skudsvik@schibsted.com>
- 0.2.0 Small changes to memory handling and support for Varnish 6.

* Mon Mar 18 2019 Ole Fredrik Skudsvik <ole.fredrik.skudsvik@schibsted.com>
- 0.1.0 First release
