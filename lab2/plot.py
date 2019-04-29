#!/bin/python
import matplotlib
import matplotlib.pyplot as plt
import scipy.optimize as opt
import numpy as np

matplotlib.use('Qt5Agg')

def func(x, a, b, c):
     return x ** 2 * a + b * x + c

data = np.genfromtxt("insertion_random.csv", delimiter=',')[:,:-1]
x = data[0]
y = data[1:]

print(y.transpose(1,0)[0])

fitData = y.transpose(1,0)[0]

optimizedParameters, pcov = opt.curve_fit(func, x, fitData);

plt.plot(x, y, 'ro', label="data")
plt.plot(x, func(x, *optimizedParameters), label="fit", color="blue");

plt.xlabel('Element att sortera (N)')
plt.ylabel('Tid (S)')
plt.legend()
plt.show()
