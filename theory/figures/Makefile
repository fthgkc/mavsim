SHELL := /bin/bash

build/figures.pdf: opticalFlowSensor.svg
	@mkdir -p build; \
	echo input args: $^; \
	for svg in $^;do \
		echo svg: $$svg; \
		pdf=$$(echo "$$svg" | sed s/svg/pdf/); \
		echo pdf: $$pdf; \
		inkscape -D -z --file=$$svg --export-pdf=build/$$pdf --export-latex;\
	done; \
	cd build && pdflatex ../figures.tex

build/figures.jpg: build/figures.pdf
	@convert -density 300 $^ $@

clean:
	@rm -rf build

view: build/figures.pdf
	@evince $<
