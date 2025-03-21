# Version: 1.0
# #     author="David Schaurecker",
# #     author_email="david.schaurecker@gmail.com"
# #     description="A Python wrapper for the C++ battery CID arbitrage simulation.",

from .simulation import Simulation
from .data import Data
from .results import Results

__all__ = ["Simulation", "Data", "Results"]

__doc__ = """
bite
(Battery Intraday Trading Engine)
======

A Python wrapper for the C++ battery CID arbitrage simulation.

Classes:
    Simulation: Core simulation class to run and manage simulations.
    Data: Data class to manage input data for simulations.
    Results: Results class to manage simulation results.
"""