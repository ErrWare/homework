# exploring non-unique sums of 2 ints each raised to the same power
# couldn't find any for dimension greater than 4


def f(d): return lambda x, y: x**d + y**d


max_dim = 5
max_int = 5000
for d in range(1, max_dim+1):
    dic = {}
    breakD = False
    for x in range(1, max_int+1):
        for y in range(x+1, max_int+1):
            v = f(d)(x, y)
            if v not in dic:
                dic[v] = []
            dic[v].append((x, y))
            if len(dic[v]) > 1:
                breakD = True
                print(d, ',', v, ':\t', dic[v])
                break
        if breakD:
            break
