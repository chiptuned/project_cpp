OUTPUT = C++AvancéTPprojet
all: sujet

sujet: dijkstra_projet.tex directed_graph.pdf ratp_graph.pdf
	pdflatex -jobname $(OUTPUT) -shell-escape $^
	pdflatex -jobname $(OUTPUT) -shell-escape $^
	mv $(OUTPUT).pdf ../

directed_graph.pdf: directed_graph.svg
	inkscape directed_graph.svg --export-pdf=directed_graph.pdf

ratp_graph.pdf: ratp_graph.svg
	inkscape ratp_graph.svg --export-pdf=ratp_graph.pdf

clean_files :
	rm -f dijkstra_projet.synctex.gz *.pdf

clean : clean_files
	rm -f ../$(OUTPUT).pdf
