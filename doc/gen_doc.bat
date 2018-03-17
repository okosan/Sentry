@echo off
cls

echo "Preparing to run Doxygen"
set PATH=%PATH%;C:\DEVSDK\Graphviz\graphviz-2.38\release\bin\

mkdir doxygen_docs

"C:/Program Files\doxygen/bin/doxygen.exe" Doxyfile

echo "DONE!"
pause

