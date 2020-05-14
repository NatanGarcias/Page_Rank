# PageRank
Developed by Larry Page and Sergey Brin in the 90's, PageRank is an algorithm to rank pages on a network according to its importance relative to the others. Even tough today Google uses a different approach, their initial solution proved to be simple and effective, making it widely known in the field of Computer Science.

# Compiling
```bash
~$ make
```

## Problem Description
Given a list of pages with its respective links, determine the rank of the pages on the given network. A given page **p** is said to influence page **q** if there is a direct link from **q** -> **p**. For instance:
<p align="center">
<img src="https://raw.githubusercontent.com/NatanGarcias/Page_Rank/master/assets/sample.svg?token=ADE4HTXEYFIR4T7U2C3S2RC6YMU26">
</p>
Note that in the example above, even tough node 1 gets a lot of direct links it is node 2 which has the highest rank. It so happens because node 2 influences a page with a lot of influence itself, hence it's score goes up. The rankings for this sample are:


page  |  rank
----- | -----
0| 4
1 | 2
2 | 1
3 | 3
4 | 5

## Problem Definition
- **P** set of pages
- |**P**| = n
- i,j ranges from [0, n-1]
- **p**[i] is the ith page on **P**
- initially **pageRank**(**p**[i]) = 1/n
- [damping factor](https://en.wikipedia.org/wiki/PageRank#Damping_factor) **d** is set to 0.85
- **In**(p[i]) every page with a link to i
- **Out**(p[i]) every page that i connects to
- convergence is the stopping criteria
- in case of ties, output the smallest node label first

### PageRank formula

![PageRank Formula](https://raw.githubusercontent.com/NatanGarcias/Page_Rank/master/assets/pgRank.png?token=ADE4HTTGCOOUA5L4WVWJOW26YMSQM)


## Project authors
* [Natan Garcias](https://github.com/NatanGarcias) 
* [Lucas Reis](https://github.com/lucas-t-reis)
