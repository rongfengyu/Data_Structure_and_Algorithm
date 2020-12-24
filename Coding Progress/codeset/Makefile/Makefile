PROJECTDIR=.

include $(PROJECTDIR)/Makefile.inc

SUBS=src bin #authlib tool


.PHONY: all clean install uninstall

all:
	@for sub in $(SUBS);\
		do make -C $$sub || exit 1; \
	done
		
clean:
	@for sub in $(SUBS);\
		do make clean -C $$sub || exit 1; \
	done
					
include Makefile.ins

