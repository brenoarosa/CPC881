import pygmo as pg
import numpy as np

prob = pg.problem(pg.cec2014(prob_id=5, dim=10))
algo = pg.algorithm(pg.cmaes(gen=100))
archi = pg.archipelago(8, algo=algo, prob=prob, pop_size=20)
archi.evolve(1000)
archi.wait()
res = [isl.get_population().champion_f for isl in archi]
res = np.array(res)
print(f"Problem {5}: {res.mean()}")
