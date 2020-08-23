call script\cmake_mingw.bat ..\HexTerrainPrototype_GL -DURHO3D_64BIT=1 -DURHO3D_LUAJIT=1 -DURHO3D_OPENGL=1 -DURHO3D_HOME=C:\Users\verte\Projects\Urho3D_GL -DCMAKE_BUILD_TYPE=RelWithDebInfo
cd ..\HexTerrainPrototype_GL
mingw32-make.exe
cd ..\HexTerrainProtype
