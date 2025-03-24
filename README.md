# BITE

## _A Battery Intraday Trading Engine_


<img src="docs/assets/bite_logo.png" width="30">

[![Version](https://img.shields.io/github/v/tag/dschaurecker/bite?label=version&style=flat)](https://github.com/dschaurecker/bite/releases)

A Python high-frequency intraday trading engine for simulating the rolling intrinsic strategy on the European market, solved as a dynamic program. See our paper (tbd, Schaurecker et al. (2025)) for details on the method and visualizations of the results.


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

## Documentation and Tutorial

Our [Documentation](https://dschaurecker.github.io/bite/) gives a detailed overview on our package's features and will be updated continuously. We also provide a simple [Tutorial](https://github.com/dschaurecker/bite/notebooks/package_tutorial.ipynb) in form of a Jupyter Notebook, guiding users through our simulation process.

## Contribution

We are happy about all forms of feedback and would like to hear from you, if you would like to contribute as well, or are interested in the underlying C++ source code of your simulation engine.

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
