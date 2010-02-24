Summary: MPC - The Makefile, Project and Workspace Creator.
Name: MPC
Version: VERSION
Release: 0
License: Open Software License
Group: Software Development
BuildRoot: /tmp/mpc
Source: %{name}-%{version}.tar.bz2
URL: http://www.ociweb.com/products/mpc
Vendor: Object Computing, Inc.
Packager: PACKAGER
buildarch: noarch
%description
MPC (Make, Project and workspace Creator) is an open source tool, developed
by OCI and written in Perl. It is designed to generate a variety of tool
specific project files from a common baseline.
%defattr(-,root,root)
%pre
%prep
%setup
%build
%install
tar cf - * | (cd $RPM_BUILD_ROOT && tar xf -)
%post
ln -sf FINALDIR/mwc.pl /usr/bin
ln -sf FINALDIR/mpc.pl /usr/bin
%postun
rm -f /usr/bin/mwc.pl /usr/bin/mpc.pl
%files
FINALDIR
