FOLDERS= latex sample_data v2
COLOR1='\e[1;32m'
COLOR2='\e[1;34m'
NC='\e[0m' # No Color

all: clean
	@for i in $(FOLDERS); do ./draw "Make $$i" $(COLOR1); cd $$i; make $@; cd ..; done;
clean: clean_files
	@for i in $(FOLDERS); do ./draw "Clean $$i" $(COLOR1); cd $$i; make $@; cd ..; done;
clean_files:
	@for i in $(FOLDERS); do ./draw "Clean $$i" $(COLOR1); cd $$i; make $@; cd ..; done;

.PHONY: all clean clean_files
