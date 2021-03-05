EXE = raytracer
SRC_DIR = ./src/
SRC = $(wildcard $(SRC_DIR)*.cpp)
OBJ = $(patsubst $(SRC_DIR)%.cpp,%.o,$(SRC))
FLAG += -lglut64
FLAG += -lglu32
FLAG += -lopengl32

all: $(EXE)

$(EXE): $(OBJ)
	g++ $^ -o $@ $(FLAG)

%.o: $(SRC_DIR)%.cpp
	g++ -c $<

.PHONY: clean
clean:
	del $(OBJ) $(EXE).exe