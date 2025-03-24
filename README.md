# BITE
## A _Battery Intraday Trading Engine_

<h1 align="center">
<img src="docs/assets/bite_logo.png" width="300">
</h1><br>

[![Version](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://travis-ci.org/joemccann/dillinger)

A Python high-frequency intraday trading engine for simulating the rolling intrinsic strategy on the European market, solved as a dynamic program. See our paper (tbd) for details on the method and visualizations of the results.

## Table of Contents

 1. [Features](#Features)
 2. [Installation](#subheading-2)
 3. [Package](#sub-heading-3)
    - [Data Preprocessing](#sub-heading-3)
    - [Simulation](#sub-heading-3)
    - [Results Postprocessing](#sub-heading-3)
 4. [Tutorial](#tutorial)
 5. [License](#license)
 6. [Author](#author)


## Features

- Process your own raw EPEX Spot Market Data to a suitable format
- Define battery and simulation settings
- Adapt the parameters of the dynamic programming optimization
- React to every single LOB update on the exchange
- Run yearly high-frequency trading simulations in minutes/hours
- Return and visualize results and statistics

## Installation

Bite requires Python 3.8+ to run.
The package can be easily installed via

```sh
pip install bite
```




## Package

We divide our package into three major Python classes for preparing, running and visualizing the battery trading simulations. More detailed examples on how to use the package are given in [Tutorial](##tutorial)

#### Data Preprocessing

Our `Data` class allows users to read-in raw zipped LOB Data from EPEX (2020 and later), process them accordingly and save each trading day as a separate CSV file. All Data is ultimately stored in UTC timezone format.
We show and test this for German Market Data of the years 2020 and 2021, specifically using the 1h products of the continuous intraday market, but this can easily be adapted to other regions or other products.
Inputs to the parsing function simply are the `start-day` and `end-day` of the data we want to parse, plus the `path` to the zipped EPEX market data.

#### Simulation
The `Simulation` class enables users to initialize simulation instances, set parameters, load the preprocessed LOB Data into the simulation, run the simulation, and return results.
Conceptually, you first set the parameters of the simulation (battery, dynamic programming, and simulation settings), then decide which days of LOB data to feed, before subsequently running the simulation for the desired amount of time. Order book traversals and optimizations happen in C++, while pre-/post-processing and settings are done in Python. Results are returned as Pandas dataframes and can be fed into the post-processing described below.

#### Results Postprocessing

Our `Results` class, gives users some tools to visualize the final schedule, as determined by the rolling intrinsic simulation, and evaluate some key statistics. Of course, the user is encouraged to look at all simulation outputs in detail to understand the intricacies of the battery's trading behavior.

## Tutorial

We give concrete usage examples and explanations to all the classes discussed above in our [Jupyter Notebook](../blob/master/LICENSE).

To reduce data-loading times, we encourage users to follow the flow of first creating LOB data CSV files with our `Data` class, but then creating LOB data binaries with our `Simulation` class, before running any simulations. Alternatively, users can also directly pass LOB Pandas dataframes to the simulation, at the cost of additional data-loading times.

## License

MIT??

## Author

[@dschaurecker](https://github.com/dschaurecker)

with pythonic support by [@simon-hirsch](https://github.com/simon-hirsch)

[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)

   [dill]: <https://github.com/joemccann/dillinger>
   [git-repo-url]: <https://github.com/joemccann/dillinger.git>
   [john gruber]: <http://daringfireball.net>
   [df1]: <http://daringfireball.net/projects/markdown/>
   [markdown-it]: <https://github.com/markdown-it/markdown-it>
   [Ace Editor]: <http://ace.ajax.org>
   [node.js]: <http://nodejs.org>
   [Twitter Bootstrap]: <http://twitter.github.com/bootstrap/>
   [jQuery]: <http://jquery.com>
   [@tjholowaychuk]: <http://twitter.com/tjholowaychuk>
   [express]: <http://expressjs.com>
   [AngularJS]: <http://angularjs.org>
   [Gulp]: <http://gulpjs.com>

   [PlDb]: <https://github.com/joemccann/dillinger/tree/master/plugins/dropbox/README.md>
   [PlGh]: <https://github.com/joemccann/dillinger/tree/master/plugins/github/README.md>
   [PlGd]: <https://github.com/joemccann/dillinger/tree/master/plugins/googledrive/README.md>
   [PlOd]: <https://github.com/joemccann/dillinger/tree/master/plugins/onedrive/README.md>
   [PlMe]: <https://github.com/joemccann/dillinger/tree/master/plugins/medium/README.md>
   [PlGa]: <https://github.com/RahulHP/dillinger/blob/master/plugins/googleanalytics/README.md>
