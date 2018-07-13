"""
Evolutionary Programming
"""

import numpy as np

class EP(object):

    def __init__(self, memory=True):
        self.sigma = None
        self.memory = memory


    def init_sigma(self, population):
        self.sigma = 6 * np.ones((len(population), population.problem.get_nx()))


    def mutate(self, population, alfa=.2, sigma_eps=0.03):
        """
        Gaussian Mutation - sigma first.
        """

        # one gaussian variable per individual
        sigma = self.sigma * np.random.normal(loc=1, scale=alfa, size=len(population)).reshape(-1, 1)
        sigma[sigma < sigma_eps] = sigma_eps

        x = population.get_x() + np.random.normal(loc=0, scale=sigma, size=(len(population), population.problem.get_nx()))
        return (x, sigma)


    def natural_selection(self, x, sigma, fitness, q=10):
        """
        Tournament Competition.
        """
        wins = np.zeros(len(x))

        for i in range(len(x)):
            # select opponents without selecting itself
            possible_opponents = [idx for idx in range(len(x)) if idx != i]
            opponents =  np.random.choice(possible_opponents, replace=False, size=q)

            # count how many wins individual have in its group
            wins[i] = (fitness[i] < fitness[opponents]).sum()

        half_population = len(x) // 2
        most_wins = wins.argsort()[-half_population:]

        x = x[most_wins]
        sigma = sigma[most_wins]
        fitness = fitness[most_wins]

        return (x, sigma, fitness)


    def evolve(self, population):
        if (self.sigma is None) or not self.memory:
            self.init_sigma(population)

        offspring_x, offspring_sigma = self.mutate(population)
        offspring_f = np.array([population.problem.fitness(offspring_x[i, :]) for i in range(offspring_x.shape[0])])

        x = np.vstack((population.get_x(), offspring_x))
        sigma = np.vstack((self.sigma, offspring_sigma))
        fitness = np.vstack((population.get_f(), offspring_f))

        x, sigma, fitness = self.natural_selection(x, sigma, fitness)

        self.sigma = sigma
        for i in range(len(population)):
            population.set_xf(i, x[i, :], fitness[i])

        return population
