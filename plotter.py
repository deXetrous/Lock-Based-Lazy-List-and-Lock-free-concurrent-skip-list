import pandas as pd
import matplotlib.pyplot as plt
df = pd.read_csv("times1.txt", delimiter = ' ', header=None)
x_axis = ['2','4','6','8','10','12','14','16']
plt.plot(x_axis, df[1],label="Lazy SkipList")
plt.plot(x_axis, df[0],label="Lock Free SkipList")
plt.xlabel("Number of threads")
plt.ylabel("Average time to complete test 1 (in ms)")
plt.title("LSL vs LFSL for test 1")
plt.legend()
plt.savefig("test1.png")
plt.close()
df = pd.read_csv("times2.txt", delimiter = ' ', header=None)
plt.plot(x_axis, df[1],label="Lazy SkipList")
plt.plot(x_axis, df[0],label="Lock Free SkipList")
plt.xlabel("Number of threads")
plt.ylabel("Average time to complete test 2 (in ms)")
plt.title("LSL vs LFSL for test 2")
plt.legend()
plt.savefig("test2.png")
plt.close()
df = pd.read_csv("times3.txt", delimiter = ' ', header=None)
plt.plot(x_axis, df[1],label="Lazy SkipList")
plt.plot(x_axis, df[0],label="Lock Free SkipList")
plt.xlabel("Number of threads")
plt.ylabel("Average time to complete test 3 (in ms)")
plt.title("LSL vs LFSL for test 3")
plt.legend()
plt.savefig("test3.png")
plt.close()