## Genetic Algorithm for Triangle Image Creation
This program is designed to use a genetic algorithm to create an image from a source image from colored trianagles.  


# How to Run
Program takes 5-parameters: 
- image name: name of image without .png extension. Program only takes png images. Images to input will be placed in ./Images directory (this tells program where to look).
- populaton size: size of population to conduct nutural selection from
- number of triangles number of trianagles within each individual of the population
- probability of crossover: the probability of crossover for parent1
- mutation probability: the probability of mutation occuring

To execute genetic algorithm open either a terminal and command prompt and run the following command:

WINDOWS
```
<path of python.exe> .\GeneticImageCreator.py <image name> <population size> <number of triangles per individual> <probability of crossover> <mutation probability>
```

LINUX
```
$ python .\GeneticImageCreator.py <image name> <population size> <number of triangles per individual> <probability of crossover> <mutation probability>
```

On output, you should see see a directory created under ./Images called:
```
"<Image name>_pop<population size>_genelength<number of triangles>_mut<mutation probability>_cross<corssover probability>"
```

# References
I used https://github.com/DING-PENG/image-approx as reference to create my genetic algorithm. I mainly used their fitness function. 