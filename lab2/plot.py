#!/bin/python
import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import scipy.optimize as opt

matplotlib.use('Qt5Agg')

def func(x, a, b, c):
     return x ** 2 * a + b * x + c

data = np.genfromtxt("insertion_random_old.csv", delimiter=',')[:,:-1]
x = data[0]
y = data[1:]

optimizedParameters, pcov = opt.curve_fit(func, x, y[0], p0=[0.1,0,5]);

print(optimizedParameters)

plt.plot(x, y, 'ro', label="data")
plt.plot(x, func(x, *optimizedParameters), label="fit", color="blue");

plt.xlabel('Element att sortera (N)')
plt.ylabel('Tid (S)')
plt.legend()
plt.show()
