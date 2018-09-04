echo Starte CMake...
cmake ..

echo Starte Kompiliervorgang...
make

echo Installiere...
sudo make install

echo Aktualisiere Konfiguration
sudo ldconfig

echo Fertig!
