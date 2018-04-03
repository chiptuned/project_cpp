seg=";"

all: sheets

sheets: full_list fields_to_mark
	./create_sheets full_list $(seg) fields_to_mark

full_list: names_list
	./create_full_list names_list $(seg)

.PHONY: all
