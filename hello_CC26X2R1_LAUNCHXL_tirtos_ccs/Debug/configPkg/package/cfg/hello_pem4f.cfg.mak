# invoke SourceDir generated makefile for hello.pem4f
hello.pem4f: .libraries,hello.pem4f
.libraries,hello.pem4f: package/cfg/hello_pem4f.xdl
	$(MAKE) -f D:\ccsWorkspace\hello_CC26X2R1_LAUNCHXL_tirtos_ccs/src/makefile.libs

clean::
	$(MAKE) -f D:\ccsWorkspace\hello_CC26X2R1_LAUNCHXL_tirtos_ccs/src/makefile.libs clean

