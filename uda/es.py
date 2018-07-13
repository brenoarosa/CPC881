"""
Evolutionary Strategy
"""

import numpy as np

class ES(object):

    def __init__(self, memory=True):
        self.sigma = None
        self.memory = memory


    def init_sigma(self, population):
        self.sigma = np.random.uniform(low=0, high=1, size=(len(population), population.problem.get_nx()))


    def mutate(self, population_x, population_sigma, tal_constant=1., sigma_eps=0.01):
        """
        Uncorrelated Mutation with n Steps.
        """

        population_size = population_x.shape[0]
        nx = population_x.shape[1]

        tal = tal_constant * 1 / np.sqrt(2 * np.sqrt(population_size))
        tal_line = tal_constant * 1 / np.sqrt(2 * population_size)

        sigma = population_sigma * \
                    np.exp(np.random.normal(loc=0, scale=tal, size=(population_size, nx)) + \
                           np.random.normal(loc=0, scale=tal_line, size=(population_size, 1)))
        sigma[sigma < sigma_eps] = sigma_eps

        x = population_x + np.random.normal(loc=0, scale=sigma)
        return (x, sigma)


    def recombine(self, parents_x, parents_sigma):
        """
        Local Recombination.

        Discrete is used for the object part.
        Intermediate is used for the strategy part.
        """

        # shuffle parents
        n_parents = parents_x.shape[0]
        nx = parents_x.shape[1]

        idx_shuffled = np.random.permutation(n_parents)

        parents_x = parents_x[idx_shuffled, :]
        parents_sigma = parents_sigma[idx_shuffled, :]

        # form pairs of parents
        parents_x = parents_x.reshape(-1, 2, nx)
        parents_sigma = parents_sigma.reshape(-1, 2, nx)


        x_pattern = np.zeros(parents_x.shape).astype(bool)
        x_pattern[:, 0, :] = np.random.randint(low=0, high=2, size=x_pattern[:, 0, :].shape).astype(bool)
        x_pattern[:, 1, :] = ~x_pattern[:, 0, :]
        x_pattern = x_pattern.astype("uint8")

        x = (parents_x * x_pattern).sum(axis=1)
        sigma = parents_sigma.mean(axis=1)
        return (x, sigma)


    def parent_selection(self, population, offspring_rate=5):
        """
        Parent Selection.

        Sample uniformly (2 * offspring_rate * population_size) individuals.
        This will result in the creation of a offspring population with
        offspring_rate * population_size) individuals.
        """

        n_parents = 2 * offspring_rate * len(population)
        select_idx = np.random.choice(len(population), size=n_parents, replace=True)

        x = population.get_x()[select_idx, :]
        sigma = self.sigma[select_idx, :]
        return (x, sigma)


    def natural_selection(self, x, sigma, fitness, population_size):
        """
        Comma Selection.
        """

        # get indices of best (population_size) individuals
        idx = np.argpartition(fitness[:, 0], population_size)[:population_size]
        x = x[idx]
        sigma = sigma[idx]
        fitness = fitness[idx]
        return (x, sigma, fitness)


    def evolve(self, population):
        if (self.sigma is None) or not self.memory:
            self.init_sigma(population)

        parents_x, parents_sigma = self.parent_selection(population)
        offspring_x, offspring_sigma = self.recombine(parents_x, parents_sigma)
        offspring_x, offspring_sigma = self.mutate(offspring_x, offspring_sigma)
        offspring_f = np.array([population.problem.fitness(offspring_x[i, :]) for i in range(offspring_x.shape[0])])

        x, sigma, fitness = self.natural_selection(offspring_x, offspring_sigma, offspring_f, len(population))

        self.sigma = sigma
        for i in range(len(population)):
            population.set_xf(i, x[i, :], fitness[i])

        return population
