CXX=/home/ivpc/emsdk/upstream/emscripten/em++
CXXRUN=/home/ivpc/emsdk/upstream/emscripten/emrun
#CXX 	:= /sgoinfre/ide-abre/emsdk/upstream/emscripten/em++
#CXXRUN	:= /sgoinfre/ide-abre/emsdk/upstream/emscripten/emrun

CPP	:=	g++

INC := -I_inc -I_inc/imgui -I_inc/ECS -I_inc/components -I_inc/systems

IMGUI		:= _inc/imgui/backends/imgui_impl_sdl3.cpp  _inc/imgui/backends/imgui_impl_opengl3.cpp _inc/imgui/imgui*.cpp
CPPFILES 	:= $(IMGUI) $(wildcard *.cpp _src/*.cpp _src/*/*.cpp)

CPPFLAGS	:= -lGL -lGLEW -lSDL3


all: desktop

web: $(CPPFILES)
	$(CXX) $(INC) -s USE_SDL=3 -s FULL_ES3 -std=c++23 $(CPPFILES) -o web/index.html --preload-file res

desktop: $(CPPFILES)
	$(CPP) $(INC) -std=c++23 $(CPPFILES) -o desktop/game $(CPPFLAGS)

run_web: web
	python3 -m http.server

run_desktop: desktop
	./desktop/game

clean:
	rm -f web/*
	rm -f desktop/*
