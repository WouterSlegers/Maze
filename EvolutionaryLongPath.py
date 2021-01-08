import random as rng
import numpy as np
import Maze
import concurrent.futures as future
import PathThroughMaze as ptm

x, y = 0, 0

SIZE_MAZE = 12

PRINTING_GENERATIONS = True
PRINT_INTERVAL = 10

# Evolutional parameters
GENERATIONS = 60
STARTING_AMOUNT = 120               #starting amount of randomized mazes
BEGINNING_DENSITY = 0.7             #density of the randomly initialized mazes

NUMBER_TO_NEXT_GENERATION = 12      #number of the best performing mazes taken to the next generation
MUTATED_CHILDREN = 4                #each maze taken to the new generation makes this amount of (ordinary) mutated children
MIN_MUTATIONS = 1                   #each child gets between MIN_MUTATIONS and MAX_MUTATIONS amount of mutations compared to the parent
MAX_MUTATIONS = 8
REGIONALLY_MUTATED_CHILDREN = 6     #each maze taken to the new generation makes this amount of regionally mutated children
MIN_MUTATIONS_REGIONAL = 2
MAX_MUTATIONS_REGIONAL = 6


# Weights for the scoring function
PENALTY_NO_PATH = 4                 #gets multiplied by shortest manhattan distance to end node
WEIGHT_UNREACHED = 1                #penalizes for having nodes that can not be reached
WEIGHT_DISTANCE_PATH = 1.5          #increases the score for having a longer shortest path to the end
WEIGHT_WALLS = 0.1                  #each wall of the maze adds this score


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
    if distance_to_end == maze.fields**2 + 1: #no path to end
        shortest_manhattan_distance_to_end = maze.fields*2
        for x in range(maze.fields):
            for y in range(maze.fields):
                if distance_to_fields[maze.fields - 1][maze.fields - 1] < maze.fields**2 + 1:
                    manhattan_distance_to_end = abs(x - (maze.fields - 1)) + abs(y - (maze.fields - 1)) #manhattan distance
                    if manhattan_distance_to_end < shortest_manhattan_distance_to_end:
                        shortest_manhattan_distance_to_end = manhattan_distance_to_end

        sum -= shortest_manhattan_distance_to_end * PENALTY_NO_PATH
    else:
        sum += distance_to_end * WEIGHT_DISTANCE_PATH

    for x in range(maze.fields):
        for y in range(maze.fields):
            for i in range(2):
                if maze.connections[x][y][i] == False:
                    sum += WEIGHT_WALLS

    return sum

def evaluate_maze(maze):
    distance_to_fields = np.full((maze.fields, maze.fields), maze.fields**2 + 1)
    shortest_path(maze, distance_to_fields, 0, 0, 0)
    return (maze, score_function(maze, distance_to_fields))

def create_maze_evolutionary(size, begin_density):
    instances = []
    maze = Maze.Maze(size)

    #makes the first batch of random mazes
    for dummy in range(STARTING_AMOUNT):
        maze = Maze.Maze(size)
        maze.randomize(begin_density)
        instances.append(maze)

    scored_instances = []

    with future.ProcessPoolExecutor() as executor:
        for generation in range(GENERATIONS):
            #scores all mazes of this generation
            scored_instances = []
            scored_instances = list(executor.map(evaluate_maze, instances))
            scored_instances.sort(key = lambda pair: pair[1], reverse = True)

            if PRINTING_GENERATIONS and generation % PRINT_INTERVAL == 0 :
                current_maze, score = scored_instances[0]
                print(f"Best maze of generation {generation} with score\n {score}:")
                current_maze.print(False)

            #creates the next generation
            instances = []
            for i in range(NUMBER_TO_NEXT_GENERATION):
                high_scoring_maze = scored_instances[i][0]
                instances.append(high_scoring_maze)
                for j in range(MUTATED_CHILDREN):
                    new_maze = high_scoring_maze.return_mutated(MIN_MUTATIONS, MAX_MUTATIONS)
                    instances.append(new_maze)
                for k in range(REGIONALLY_MUTATED_CHILDREN):
                    new_maze = high_scoring_maze.return_regionally_mutated(MIN_MUTATIONS_REGIONAL, MAX_MUTATIONS_REGIONAL)
                    instances.append(new_maze)


    maze = scored_instances[0][0]

    if PRINTING_GENERATIONS:
        print("The evolutionary algorithm produced:")
        maze.print(False)
        print("Now let's make sure it is fully connected...")

    #runs path-through-maze on the resulting maze to ensure it is fully connected before returning the maze
    maze_copy = maze.return_copy()
    ptm.PRINTING_STEPS = False
    steps = ptm.path_through_maze(maze, (0, 0))
    if maze == maze_copy:
        print("The evolutionary algorithm created a fully connected maze!")
    else:
        print(f"The resulting maze was not yet fully connected, we had to open up {steps} walls")
    return maze

#-------------------------------------------------------------------------------
if __name__ == "__main__":
    maze = create_maze_evolutionary(SIZE_MAZE, BEGINNING_DENSITY)
    print("The resulting maze:\n")
    maze.print(False)
