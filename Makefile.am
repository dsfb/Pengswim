install-data-local:
	test -d data \
	&& install -d -m 0755 ${pkgdatadir}/images \
	&& install -d -m 0755 ${pkgdatadir}/sounds \
	&& install -m 0644 data/images/*.png ${pkgdatadir}/images \
	&& install -m 0644 data/sounds/*.{it,wav} ${pkgdatadir}/sounds

dist-hook:
	test -d data \
	&& mkdir ${distdir}/data \
	&& mkdir ${distdir}/data/images \
	&& mkdir ${distdir}/data/sounds \
	&& cp -p data/images/* ${distdir}/data/images \
	&& cp -p data/sounds/* ${distdir}/data/sounds

SUBDIRS = src
