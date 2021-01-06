import random as rng
import numpy as np
import time
import Maze


x, y = 0, 0
visited = []

PRINTING_STEPS = True

SIZE_MAZE = 20
CHANCE_STRAIGHT = 0.1 #Additional chance to go straight.


def create_maze_walker(size):
    maze = Maze.Maze(size)
    if (not self_avoiding_walk(maze, (0, 0), -1)):
        print("Something may have gone wrong...")
    return maze

def self_avoiding_walk(maze, starting_point, direction):
    visited.append(starting_point)

    if PRINTING_STEPS:
        if (direction >= 0):
            maze.set_field(*starting_point, 'v')
        else:
            maze.set_field(*starting_point, 'N')
            maze.print(True)

    if len(visited) >= maze.fields**2:
        return True

    neighbours, possible_paths = maze.list_neighbours(*starting_point, False)

    if possible_paths > 0:
        if direction >= 0:
            if rng.random() < CHANCE_STRAIGHT:
                neighbour = neighbours[direction]
                if neighbour != None and neighbour not in visited:
                    maze.connect_neighbours(*starting_point, *neighbour)
                    return self_avoiding_walk(maze, neighbour, direction)

        order = list(range(4))
        rng.shuffle(order)
        for dir in order:
            neighbour = neighbours[dir]
            if neighbour != None and neighbour not in visited:
                direction = dir
                maze.connect_neighbours(*starting_point, *neighbour)
                return self_avoiding_walk(maze, neighbour, direction)

    if PRINTING_STEPS:
        maze.set_field(*starting_point, 'e')
        maze.print(True)

    rng.shuffle(visited)
    for node in visited:
        disconnected_neighbours = maze.get_connected_neighbours(*node, False)
        for neighbour in disconnected_neighbours:
            if neighbour not in visited:
                maze.connect_neighbours(*node, *neighbour)
                return self_avoiding_walk(maze, neighbour, -1)

#-------------------------------------------------------------------------------
if __name__ == "__main__":
    maze = create_maze_walker(SIZE_MAZE)
    print("The resulting maze:\n")
    maze.print(False)
