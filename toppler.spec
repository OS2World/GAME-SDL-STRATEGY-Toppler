Name: toppler
Version: 0.98.1
Release: 1


URL: http://toppler.soureforge.net

License: GPL
Group: Amusements/Games

BuildRoot: %{_tmppath}/%{name}-root

Requires: SDL >= 1.2.4, SDL_mixer >= 1.2.4, zlib, libstdc++
BuildRequires: SDL-devel >= 1.2.4
AutoReqProv: no

Source0: %{name}-%{version}.tar.gz
#Patch0: %{name}-%{version}-patch0.diff.gz
#Patch1: %{name}-%{version}-patch1.diff.gz

Summary: Tower Toppler

%description 
Reimplementation of the old game (aka Nebulous). In the game you have
to climb a tower with lots of strange inhabitants that try to push you
down. Your only defence is a snowball you can throw and your skill to
avoid these beeings.

%prep

%setup -q
#%patch0 -p1
#%patch1 -p1


%build

%configure
%{__make}


%install

rm -rf $RPM_BUILD_ROOT
%makeinstall


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root)

%doc README COPYING AUTHORS NEWS ChangeLog

%{_bindir}/*
%{_datadir}/*


%changelog
* Sun Oct  6 2002 Chong Kai Xiong <descender@phreaker.net>
- Initial build.


