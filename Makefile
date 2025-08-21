#CXX=/home/ivpc/emsdk/upstream/emscripten/em++
#CXXRUN=/home/ivpc/emsdk/upstream/emscripten/emrun
CXX 	:= /sgoinfre/ide-abre/emsdk/upstream/emscripten/em++
CXXRUN	:= /sgoinfre/ide-abre/emsdk/upstream/emscripten/emrun


INC := -I_inc -I_inc/ECS -I_inc/components
CPPFILES := $(wildcard *.cpp _src/*.cpp _src/*/*.cpp)

all: $(CPPFILES)
	$(CXX) $(INC) -s USE_SDL=3 -s FULL_ES3 -std=c++23 $(CPPFILES) -o web/index.html --preload-file res

run: all
	python3 -m http.server

clean:
	rm -f web/*
