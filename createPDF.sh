cat tex/tex_header.tex $@ tex/tex_footer.tex > tex/temp.tex
pdflatex -synctex=1 -interation=nonstopmode -output-directory=tex -aux_directory=tex --shell-escape tex/temp.tex > tex/log 2>&1
cp tex/temp.pdf preview.pdf
echo "Finished: preview.pdf" 
