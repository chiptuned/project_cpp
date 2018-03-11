all: sujet

sujet: dijkstra_projet.tex directed_graph.pdf ratp_graph.pdf
	pdflatex -shell-escape $^

directed_graph.pdf: directed_graph.svg
	inkscape directed_graph.svg --export-pdf=directed_graph.pdf

ratp_graph.pdf: ratp_graph.svg
	inkscape ratp_graph.svg --export-pdf=ratp_graph.pdf
