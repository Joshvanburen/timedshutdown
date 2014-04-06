#Josh Van Buren
#Make file for wxwidgets example

#Variables
GCC = g++
SOURCE = timedShutdown.cpp
CSOURCE = timedShutdown.cpp
LIBS = -mthreads -lz -lrpcrt4 -loleaut32 -lole32 -luuid -lwinspool -lwinmm -lshell32 -lcomctl32 -lcomdlg32 -ladvapi32 -lwsock32 -lgdi32 -ltag
WXOPTS1 = -I/usr/local/lib/wx/include/msw-unicode-static-2.9 -I/usr/local/include/wx-2.9 -D_LARGEFILE_SOURCE=unknown -D__WXMSW__ -mthreads
WXOPTS2 = -D_LARGEFILE_SOURCE=unknown -D__WXMSW__ -mthreads -L/c/wx/mswrelease/lib -L/usr/local/lib  /usr/local/lib/libwx_mswu_xrc-2.9.a /usr/local/lib/libwx_mswu_html-2.9.a /usr/local/lib/libwx_mswu_qa-2.9.a /usr/local/lib/libwx_mswu_adv-2.9.a /usr/local/lib/libwx_mswu_core-2.9.a /usr/local/lib/libwx_baseu_xml-2.9.a /usr/local/lib/libwx_baseu_net-2.9.a /usr/local/lib/libwx_baseu-2.9.a -lexpat -lwxregexu-2.9 -lwxtiff-2.9 -lwxjpeg-2.9 -lwxpng-2.9 -lz -lrpcrt4 -loleaut32 -lole32 -luuid -lwinspool -lwinmm -lshell32 -lcomctl32 -lcomdlg32 -ladvapi32 -lwsock32 -lgdi32 -static
OBJS = timedShutdown.o resource.o
PNAME = timedShutdown.exe
WINDRES = windres


#Compile command
timedShutdown.exe : $(SOURCE) 
	$(WINDRES) timedShutdown.rc -O coff -o resource.o -I/usr/local/lib/wx/include/msw-unicode-static-2.9 -I/usr/local/include/wx-2.9
	$(GCC) -c $(CSOURCE) $(WXOPTS1) -static 
	$(GCC) -o $(PNAME) $(OBJS) $(WXOPTS2) $(LIBS) -s -static -mwindows
	
#Cleans up
.PHONY clean :
	rm *.o *.exe -f  