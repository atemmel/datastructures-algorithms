#!/bin/python
import matplotlib
import matplotlib.pyplot as plt
import scipy.optimize as opt
import numpy as np
import math

matplotlib.use('Qt5Agg')

def quad(x, a, b, c):
     return np.log(x) * a + b * x + c

def display(plot, string, func, fitstr):
    data = np.genfromtxt(string, delimiter=',')[:,:-1]
    x = data[0]
    y = data[1:]

    fitData = np.mean(y.transpose(1,0), axis=0)

    for v in y:
        stdev = np.std(np.array(v) )
        print("Std dev:", stdev)

    optimizedParameters, pcov = opt.curve_fit(func, x, fitData);

    plot.plot(x, y, 'ro')
    plot.plot(x, quad(x, *optimizedParameters), label=fitstr, color="blue");

    plot.xlabel('Antal element (N)')
    plot.ylabel('Antal lyckade sökningar (M)')
    plot.legend()


plt.subplot(2,2,1)
plt.title("Linear")
print("Linear");
display(plt, "linear.csv", quad, "fit")
print();

plt.subplot(2,2,2)
plt.title("Binary")
print("Binary");
display(plt, "binary.csv", quad, "fit")
print();

plt.subplot(2,2,3)
plt.title("BST")
print("BST");
display(plt, "bst.csv", quad, "fit")
print();

plt.subplot(2,2,4)
plt.title("Hash")
print("Hash");
display(plt, "hash.csv", quad, "fit")
print();

plt.show()
