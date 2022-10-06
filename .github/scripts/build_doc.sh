#!/bin/bash -eu

latexmk -xelatex -synctex=1 -interaction=nonstopmode -file-line-error \
	-outdir=latex_build -auxdir=latex_build "$1"
mv "latex_build/$(basename "${1%.tex}.pdf")" ./
