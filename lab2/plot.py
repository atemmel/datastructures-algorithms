#!/bin/python
import matplotlib
import matplotlib.pyplot as plt
import scipy.optimize as opt
import numpy as np

matplotlib.use('Qt5Agg')

def quad(x, a, b, c):
     return x ** 2 * a + b * x + c

def display(plot, string, func, fitstr):
    data = np.genfromtxt(string, delimiter=',')[:,:-1]
    x = data[0]
    y = data[1:]

    fitData = np.mean(y.transpose(1,0), axis=0)

    optimizedParameters, pcov = opt.curve_fit(func, x, fitData);

    plot.plot(x, y, 'ro')
    plot.plot(x, quad(x, *optimizedParameters), label=fitstr, color="blue");

    plot.xlabel('Element att sortera (N)')
    plot.ylabel('Tid (S)')
    plot.legend()


plt.subplot(2,2,1)
plt.title("Insertion Random")
display(plt, "insertion_random.csv", quad, "N^2")

plt.subplot(2,2,2)
plt.title("Insertion Random")
display(plt, "insertion_random.csv", quad, "N^2")

plt.show()
