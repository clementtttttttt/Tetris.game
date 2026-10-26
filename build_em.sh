 emcc main.cpp mixer.cpp notes.cpp sound_effects.cpp theme.cpp st_maingame.cpp st_titlescreen.cpp -s WASM=1 -s USE_SDL=2 -s USE_SDL_MIXER=2 -s USE_SDL_TTF=2 -sINITIAL_HEAP=0KB -sINITIAL_MEMORY=32MB -o tetris.html -DEMCXX -O3  --preload-file "./font.ttf"   --shell-file=./template.html



