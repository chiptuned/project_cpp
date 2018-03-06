all: sujet

sujet: dijkstra_projet.tex directed_graph.pdf
	pdflatex -shell-escape $^

directed_graph.pdf: directed_graph.svg
	inkscape directed_graph.svg --export-pdf=directed_graph.pdf
