DEMANGLE_SUPPORT=1 EMCC_DEBUG=1 emcc -g main.cpp notes.cpp -s ASSERTIONS=2 -s WASM=1 -s USE_SDL=2 -s USE_SDL_MIXER=2 -s USE_SDL_TTF=2 -o tetris.html -DEMCXX  --preload-file "./font.ttf"



