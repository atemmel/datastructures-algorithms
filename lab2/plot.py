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

    for v in y:
        stdev = np.std(np.array(v) )
        print("Std dev:", stdev)

    optimizedParameters, pcov = opt.curve_fit(func, x, fitData);

    plot.plot(x, y, 'ro')
    plot.plot(x, quad(x, *optimizedParameters), label=fitstr, color="blue");

    plot.xlabel('Element att sortera (N)')
    plot.ylabel('Tid (S)')
    plot.legend()


plt.subplot(2,2,1)
plt.title("Insertion Random")
print("Insertion random:");
display(plt, "insertion_random.csv", quad, "fit")
print();

plt.subplot(2,2,2)
plt.title("Insertion Growing")
print("Insertion Growing:");
display(plt, "insertion_monogrowing.csv", quad, "fit")
print();

plt.subplot(2,2,3)
plt.title("Insertion Falling")
print("Insertion Falling:");
display(plt, "insertion_monofalling.csv", quad, "fit")
print();

plt.subplot(2,2,4)
plt.title("Insertion constant")
print("Insertion constant:");
display(plt, "insertion_constant.csv", quad, "fit")
print();

plt.show()

plt.subplot(2,2,1)
plt.title("Selection Random")
print("Selection random:");
display(plt, "selection_random.csv", quad, "fit")
print();

plt.subplot(2,2,2)
plt.title("Selection Growing")
print("Selection Growing:");
display(plt, "selection_monogrowing.csv", quad, "fit")
print();

plt.subplot(2,2,3)
plt.title("Selection Falling")
print("Selection Falling:");
display(plt, "selection_monofalling.csv", quad, "fit")
print();

plt.subplot(2,2,4)
plt.title("Selection constant")
print("Selection constant:");
display(plt, "selection_constant.csv", quad, "fit")
print();

plt.show()

plt.subplot(2,2,1)
plt.title("Quick Random")
print("Quick random:");
display(plt, "partition_random.csv", quad, "fit")
print();

plt.subplot(2,2,2)
plt.title("Quick Growing")
print("Quick Growing:");
display(plt, "partition_monogrowing.csv", quad, "fit")
print();

plt.subplot(2,2,3)
plt.title("Quick Falling")
print("Quick Falling:");
display(plt, "partition_monofalling.csv", quad, "fit")
print();

plt.subplot(2,2,4)
#plt.title("Quick constant")
#print("Quick constant:");
#display(plt, "partition_constant.csv", quad, "fit")
#print();

plt.show()

plt.subplot(2,2,1)
plt.title("Quick Median Random")
print("Quick Median random:");
display(plt, "medianPartition_random.csv", quad, "fit")
print();

plt.subplot(2,2,2)
plt.title("Quick Median Growing")
print("Quick Growing:");
display(plt, "medianPartition_monogrowing.csv", quad, "fit")
print();

plt.subplot(2,2,3)
plt.title("Quick Median Falling")
print("Quick Falling:");
display(plt, "medianPartition_monofalling.csv", quad, "fit")
print();

plt.show()

plt.subplot(2,2,1)
plt.title("std::sort Random")
print("std::sort random:");
display(plt, "stdsort_random.csv", quad, "fit")
print();

plt.subplot(2,2,2)
plt.title("std::sort Growing")
print("std::sort Growing:");
display(plt, "stdsort_monogrowing.csv", quad, "fit")
print();

plt.subplot(2,2,3)
plt.title("std::sort Falling")
print("std::sort Falling:");
display(plt, "stdsort_monofalling.csv", quad, "fit")
print();

plt.subplot(2,2,4)
plt.title("std::sort constant")
print("std::sort constant:");
display(plt, "stdsort_constant.csv", quad, "fit")
print();

plt.show()
