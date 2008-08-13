SOURCES = bin data *.py mmseg-cpp/{*.h,*.cpp,Makefile} README Makefile
BINARIES = *.pyc mmseg-cpp/mmseg.so

release-source:
	(cd .. && tar czf pymmseg-cpp-src-$(VERSION).tar.gz $(SOURCES:%=pymmseg/%))

build:
	python -c 'import compileall as c; c.compile_dir(".", 1)'
	(cd mmseg-cpp && make)

release-binary: build
	(cd .. && tar czf pymmseg-cpp-linux-i386-$(VERSION).tar.gz \
		$(SOURCES:%=pymmseg/%) $(BINARIES:%=pymmseg/%))

release: release-source release-binary

