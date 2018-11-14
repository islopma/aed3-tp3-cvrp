def swap(points, i, j):
    while j > i:
        points[i], points[j] = points[j], points[i]
        i+=1
        j-=1

def ds(points, i, j):
    return abs(points[i][0]-points[j][0]+1j*(points[i][1]-points[j][1]))

def fix_one(points):
    n = len(points)
    for i in range(n):
        for j in range(i):
            old_d = ds(points, i, (i+1)%n) + ds(points, j, (j-1)%n)
            new_d = ds(points, j, (i+1)%n) + ds(points, i, (j-1)%n)
            if old_d > new_d:
                swap(points, j, i)
                return True
    return False