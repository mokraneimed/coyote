list1 = [1,2,3]
list2 = [3,4,5]


def recursive_sum(vals):
    if len(vals) == 1:
        return vals[0]
    mid = len(vals) // 2
    return recursive_sum(vals[:mid]) + recursive_sum(vals[mid:])

print(recursive_sum([(x - y) ** 2 for x, y in zip(list1, list2)]))