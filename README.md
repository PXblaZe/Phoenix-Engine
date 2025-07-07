# Phoenix Blaze

## To compile use following command:
```bash
    g++ -std=c++23 -fdiagnostics-color=always -Wno-conversion-null -Wno-pointer-arith -Wpedantic -Ofast -march=native -frename-registers -funroll-loops -g ${file} libs/* -o ${fileDirname}/${fileBasenameNoExtension} -I${workspaceFolder}/include -lGL -lGLU -lglfw -lX11 -lXrandr -fopenmp -lpthread -lXi -lGLEW -lglut -lassimp $(pkg-config --cflags --libs glib-2.0 gio-2.0)
```
