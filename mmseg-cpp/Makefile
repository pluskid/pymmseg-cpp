OBJS = algor.o dict.o memory.o mmseg.o
CPPFLAGS = -O2 -Wall

mmseg.so: $(OBJS)
	$(CXX) -shared -o mmseg.so $(OBJS)

.cpp.o:
	$(CXX) $(CPPFLAGS) -c $<

clean:
	@-$(RM) *.o *.so
