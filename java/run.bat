@echo off
chcp 65001 > nul
echo ========================================================
echo   AVVIO GESTIONE SARTORIA DIGITALE V2 (JAVA ANT GUI)
echo ========================================================
echo.

set ANT_CMD="C:\Program Files\Apache NetBeans\extide\ant\bin\ant.bat"

if exist %ANT_CMD% (
    echo [*] Compilazione ed esecuzione con Apache Ant...
    call %ANT_CMD% run
) else (
    echo [*] Apache Ant non trovato nel percorso di default, esecuzione diretta dalle classi compilate...
    if not exist build\classes mkdir build\classes
    javac -encoding UTF-8 -d build\classes src\gestionesartoria\model\*.java src\gestionesartoria\service\*.java src\gestionesartoria\gui\dnd\*.java src\gestionesartoria\gui\*.java
    java -cp build\classes gestionesartoria.gui.FinestraPrincipale
)

pause
