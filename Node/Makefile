COLLISION = ./src/Collision/
SOURCE = ./src/source/
NODEMOD = ./src/nodemodules/
OBJECTS = collision.o CubicBezierCurve.o CubicBezierSegment.o GameObject.o MiniSnake.o Point.o Vector.o
INC = -I./lib -I./lib/boost/tr1 -I./lib/nodejs/src -I./lib/nodejs/deps/uv/include -I./lib/nodejs/deps/uv/src -I./lib/nodejs/deps/v8/include -I./lib/nodejs/deps/v8/src

all : main

clean :
	rm -rf *.o *.exe

main : $(OBJECTS)
	g++ -o main $(OBJECTS)
	
main.o : $(SOURCE)main.cpp
	g++ $(INC) -c $(SOURCE)main.cpp
	
collision.o : $(SOURCE)collision.cpp
	g++ $(INC) -c $(SOURCE)collision.cpp
	
CubicBezierCurve.o : $(SOURCE)CubicBezierCurve.cpp
	g++ $(INC) -c $(SOURCE)CubicBezierCurve.cpp
	
CubicBezierSegment.o : $(SOURCE)CubicBezierSegment.cpp
	g++ $(INC) -c $(SOURCE)CubicBezierSegment.cpp
	
GameObject.o : $(SOURCE)GameObject.cpp
	g++ $(INC) -c $(SOURCE)GameObject.cpp
	
MiniSnake.o : $(SOURCE)MiniSnake.cpp
	g++ $(INC) -c $(SOURCE)MiniSnake.cpp
	
Point.o : $(SOURCE)Point.cpp
	g++ $(INC) -c $(SOURCE)Point.cpp
	
Snake.o : $(SOURCE)Snake.cpp
	g++ $(INC) -c $(SOURCE)Snake.cpp
	
Vector.o :$(SOURCE)Vector.cpp
	g++ $(INC) -c $(SOURCE)Vector.cpp
	
GridController.o :$(SOURCE)GridController.cpp
	g++ $(INC) -c $(SOURCE)GridController.cpp
	
pointTest : $(SOURCE)main.cc $(SOURCE)Point.cpp
	g++ -g -o pointTest $(SOURCE)main.cc $(SOURCE)Point.cpp