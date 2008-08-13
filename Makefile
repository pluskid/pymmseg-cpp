SOURCES = bin data *.py mmseg-cpp/{*.h,*.cpp,Makefile} README Makefile
BINARIES = *.pyc mmseg-cpp/mmseg.so

release-source:
	(cd .. && tar czf pymmseg-cpp-$(VERSION).tar.gz $(SOURCES:%=pymmseg/%))

build-binary:
	python -c 'import compileall as c; c.compile_dir(".", 1)'
	(cd mmseg-cpp && make)

release-binary: build-binary
	(cd .. && tar czf pymmseg-cpp-linux-i386-$(VERSION).tar.gz \
		$(SOURCES:%=pymmseg/%) $(BINARIES:%=pymmseg/%))
