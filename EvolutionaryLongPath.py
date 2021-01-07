import random as rng
import numpy as np
import Maze

x, y = 0, 0

SIZE_MAZE = 12

PRINTING_GENERATIONS = True
PRINT_INTERVAL = 50

# Evolutional parameters
GENERATIONS = 500
STARTING_AMOUNT = 120               #starting amount of randomized mazes
BEGINNING_DENSITY = 0.7             #density of the randomly initialized mazes
NUMBER_TO_NEXT_GENERATION = 12      #number of the best performing mazes taken to the next generation
NUMBER_OF_CHILDREN = 10             #each maze taken to the new generation makes this amount of mutated children
MIN_MUTATIONS = 1                   #each child gets between MIN_MUTATIONS and MAX_MUTATIONS amount of mutations compared to the parent
MAX_MUTATIONS = 8

# Weights for the scoring function
PENALTY_NO_PATH = 10
WEIGHT_UNREACHED = 1
WEIGHT_DISTANCE_PATH = 1
WEIGHT_WALLS = 0.1

def shortest_path(maze, distance_to_fields, x, y, length):
    distance_to_fields[x][y] = length

    connected_neighbours = maze.get_connected_neighbours(x, y, True)

    for neighbour in connected_neighbours:
        if distance_to_fields[neighbour[0]][neighbour[1]] > length + 1:
            shortest_path(maze, distance_to_fields, *neighbour, length + 1)


def score_function(maze, distance_to_fields):
    sum = 0

    for x in range(maze.fields):
        for y in range(maze.fields):
            distance_to_node = distance_to_fields[x][y]
            if distance_to_node == maze.fields**2 + 1:
                sum -= WEIGHT_UNREACHED

    distance_to_end = distance_to_fields[maze.fields - 1][maze.fields - 1]
    if distance_to_end == maze.fields**2 + 1:
        sum -= PENALTY_NO_PATH
    else:
        sum += distance_to_end * WEIGHT_DISTANCE_PATH

    for x in range(maze.fields):
        for y in range(maze.fields):
            for i in range(2):
                if maze.connections[x][y][i] == False:
                    sum += WEIGHT_WALLS

    return sum


def create_maze_evolutionary(size, begin_density):
    instances = []
    maze = Maze.Maze(size)
    distance_to_fields_cache = np.full((maze.fields, maze.fields), maze.fields**2 + 1)

    for dummy in range(STARTING_AMOUNT):
        maze = Maze.Maze(size)
        maze.randomize(begin_density)
        instances.append(maze)

    scored_instances = []

    for generation in range(GENERATIONS):
        scored_instances = []
        for maze in instances:
            distance_to_fields_cache.fill(maze.fields**2 + 1)
            shortest_path(maze, distance_to_fields_cache, 0, 0, 0)
            scored_instances.append((maze, score_function(maze, distance_to_fields_cache)))

        scored_instances.sort(key = lambda pair: pair[1], reverse = True)


        if PRINTING_GENERATIONS and generation % PRINT_INTERVAL == 0 :
            current_maze, score = scored_instances[0]
            print(f"Best maze of generation {generation} with score\n {score}:")
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
    maze = create_maze_evolutionary(SIZE_MAZE, BEGINNING_DENSITY)
    print("The resulting maze:\n")
    maze.print(False)
