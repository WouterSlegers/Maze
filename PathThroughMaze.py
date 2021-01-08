import random as rng
import numpy as np
import time
import Maze

x, y = 0, 0
searched = []
to_search = []
dead_ends = []
steps = 0

PRINTING_STEPS = True

SIZE_MAZE = 14
BEGINNING_DENSITY = 0.8

def search_path_maze(maze, field):
    to_search.append(field)
    while len(to_search) > 0:
        x, y = to_search.pop(0)
        maze.set_field(x, y, 'c')

        connected_neighbours = maze.get_connected_neighbours(x, y, True)
        for new_node in connected_neighbours:
            if(new_node not in searched and new_node not in to_search):
                to_search.append(new_node)

        searched.append((x, y))

        if (len(connected_neighbours) <= 1):
            dead_ends.append((x, y))
            maze.set_field(x, y, 'd')
        else:
            maze.set_field(x, y, 's')
    return False

def create_maze_ptrm(size, begin_density):
    maze = Maze.Maze(size)
    maze.randomize(begin_density)

    steps = path_through_maze(maze, (0, 0))
    print(f"We're done after {steps} steps!")
    return maze

def path_through_maze(maze, starting_point):
    if PRINTING_STEPS:
        print("Searching from ", starting_point, " in the following maze")
        maze.print(True)

    search_path_maze(maze, starting_point)
    if len(searched) >= maze.fields**2:
        return 0

    dead_ends.sort(reverse=True, key=lambda node: abs(node[0] - node[1]))

    for dead_end in dead_ends:
        x, y = dead_end
        index = dead_ends.index(dead_end)

        disconnected_neighbours = maze.get_connected_neighbours(x, y, False)
        rng.shuffle(disconnected_neighbours)
        for neighbour in disconnected_neighbours:
            if neighbour not in searched:
                maze.connect_neighbours(*dead_end, *neighbour)
                maze.set_field(x, y, 'O')
                dead_ends.pop(index)
                return 1 + path_through_maze(maze, neighbour)

    #If there are no suitable dead ends:
    rng.shuffle(searched)
    for (x, y) in searched:
        disconnected_neighbours = maze.get_connected_neighbours(x, y, False)
        for neighbour in disconnected_neighbours:
            if neighbour not in searched:
                maze.connect_neighbours(x, y, *neighbour)
                maze.set_field(x, y, 'O')
                return 1 + path_through_maze(maze, neighbour)

def return_steps():
    return steps
#-------------------------------------------------------------------------------
if __name__ == "__main__":
    maze = create_maze_ptrm(SIZE_MAZE, BEGINNING_DENSITY)
    print("The resulting maze:\n")
    maze.print(False)

# Replacing
# maze.randomize(begin_density)
# in create_maze_ptrm
# with the row of maze.connections that follow
# gives the situation without suitable dead ends described in the report
# maze.connections[0][0][1] = True
# maze.connections[0][1][0] = True
# maze.connections[1][1][0] = True
# maze.connections[2][0][1] = True
# maze.connections[1][0][0] = True
# maze.connections[0][1][1] = True
# maze.connections[0][2][1] = True
# maze.connections[0][3][0] = True
# maze.connections[1][3][0] = True
# maze.connections[2][3][0] = True
# maze.connections[2][2][0] = True
# maze.connections[2][2][1] = True
# maze.connections[3][2][1] = True
