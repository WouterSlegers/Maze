import random as rng
import numpy as np
import time
import Maze


x, y = 0, 0
PRINTING_STEPS = True

#Taxing memory more
#wildly unpractical for purpose, expensive, just fun
#inspired by AI learning to play games

NUMBER_OF_INSTANCES = 1000


def score_function(maze):
    #flood fill + kleinste manhattan
    #nodes met 0 of 4 muren heul erg
    pass

def create_maze_evolutionary(size):

    instances = []
    for dummy in range(NUMBER_OF_INSTANCES):
        maze = Maze.Maze(size)
        maze.randomize()
        instances.append(maze)
