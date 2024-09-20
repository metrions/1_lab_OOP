import matplotlib.pyplot as plt

f = open("data.txt")
lines = f.read().split('\n')
lines = list(map(lambda x: [float(x.split(' ')[0]), float(x.split(' ')[1])], lines))
# lines = sorted(lines, key=lambda x: x[0])

x = list(map(lambda x: x[0], lines))
y = list(map(lambda x: x[1], lines))

plt.scatter(x, y, 2, color="green")

pr = open("practic.txt")
practic = pr.read().split('\n')
practic = list(map(lambda x: [float(x.split(' ')[0]), float(x.split(' ')[1])], practic))
# practic = sorted(practic, key=lambda x: x[0])

m = list(map(lambda x: x[0], practic))
n = list(map(lambda x: x[1], practic))

plt.scatter(m, n, 2, color="blue")

plt.show()