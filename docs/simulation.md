# Simulation

The `Simulation` class enables users to initialize simulation instances, set parameters, load the preprocessed LOB Data into the simulation, run the simulation, and return results.
Conceptually, you first set the parameters of the simulation (battery, dynamic programming, and simulation settings), then decide which days of LOB data to feed, before subsequently running the simulation for the desired amount of time. Order book traversals and optimizations happen in C++, while pre-/post-processing and settings are done in Python. Results are returned as Pandas dataframes and can be fed into the post-processing described below.

::: bitepy.Simulation