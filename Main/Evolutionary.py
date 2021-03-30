import random as rng
import numpy as np
import Maze

x, y = 0, 0

SIZE_MAZE = 12

PRINTING_GENERATIONS = True
PRINT_INTERVAL = 5

# Evolutional parameters
GENERATIONS = 30
STARTING_AMOUNT = 80               #starting amount of randomized mazes
BEGIN_DENSITY = 0.7
NUMBER_TO_NEXT_GENERATION = 12      #number of the best performing mazes taken to the next generation
NUMBER_OF_CHILDREN = 10             #each maze taken to the new generation makes this amount of mutated children
MIN_MUTATIONS = 1                  #each child gets between MIN_MUTATIONS and MAX_MUTATIONS amount of mutations compared to the parent
MAX_MUTATIONS = 8


# Weights for the scoring function.
WEIGHT_DISTANCE = 4
WEIGHT_UNREACHED = 1
WEIGHT_WALLS = 0.1

def distance_to_end(maze):
    searched = []
    to_search = [(0, 0)]
    shortest_distance = (maze.fields - 1) * 2
    reached_fields = 0

    while len(to_search) > 0:
        x, y = to_search.pop(0)
        reached_fields += 1

        distance = abs(x - (maze.fields - 1)) + abs(y - (maze.fields - 1)) #manhattan distance
        if distance < shortest_distance:
            shortest_distance = distance

        connected_neighbours = maze.get_connected_neighbours(x, y, True)
        for neighbour in connected_neighbours:
            if(neighbour not in searched and neighbour not in to_search):
                to_search.append(neighbour)

        searched.append((x, y))

    unreached_fields = maze.fields**2 - reached_fields
    return shortest_distance, unreached_fields

def score_function(maze):
    sum = 0

    distance, unreached_fields = distance_to_end(maze)
    sum -= distance * WEIGHT_DISTANCE
    sum -= unreached_fields * WEIGHT_UNREACHED

    for x in range(maze.fields):
        for y in range(maze.fields):
            for i in range(2):
                if maze.connections[x][y][i] == False:
                    sum += WEIGHT_WALLS
    return sum


def create_maze_evolutionary(size, begin_density):
    instances = []
    maze = Maze.Maze(size)

    for dummy in range(150):
        maze = Maze.Maze(size)
        maze.randomize(begin_density)
        instances.append(maze)

    scored_instances = []

    for generation in range(GENERATIONS):
        scored_instances = []
        for maze in instances:
            scored_instances.append((maze, score_function(maze)))

        scored_instances.sort(key = lambda pair: pair[1], reverse = True)

        if PRINTING_GENERATIONS and generation % PRINT_INTERVAL == 0 :
            current_maze, score = scored_instances[0]
            print("Best maze of generation {} with \n distance to end: {}, unreached fields: {} and \nscore {}".format(generation, *distance_to_end(current_maze), score))
            current_maze.print(False)

        instances = []
        for i in range(NUMBER_TO_NEXT_GENERATION):
            high_scoring_maze = scored_instances[i][0]
            instances.append(high_scoring_maze)
            for j in range(NUMBER_OF_CHILDREN):
                new_maze = high_scoring_maze.return_mutated(MIN_MUTATIONS, MAX_MUTATIONS)
                instances.append(new_maze)

    return scored_instances[0][0]

#-------------------------------------------------------------------------------
if __name__ == "__main__":
    maze = create_maze_evolutionary(SIZE_MAZE, BEGIN_DENSITY)
    print("The resulting maze:\n")
    maze.print(False)
    # input("Press any button to quit.")
