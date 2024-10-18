import csv
import random
from collections import deque

random.seed(42)
edges = []
sub_edges = []
adj = {}
nodes = set()

def get_max_deg_node(t = 1):
    if t:
        mx = 0
        node = 0
        for x, v in adj.items():
            if len(v) > mx:
                mx = len(v)
                node = x
        return node
    else:
        return random.randint(1, len(adj))

def get_sub_graph(H = 2, Max_node = 1e9, t = 1):
    global edges
    global adj
    global nodes
    global sub_edges
    st = get_max_deg_node(t)
    dis = {}
    dis[st] = 0
    queue = deque([st])
    vis = set()
    vis.add(st)

    while queue:
        x = queue.popleft()
        nodes.add(x)
        if len(nodes) >= Max_node:
            break
        for v in adj[x]:
            if v not in vis:
                dis[v] = dis[x] + 1
                if dis[v] > H:
                    continue
                vis.add(v)
                queue.append(v)
    
    for x, y in edges:
        if x in nodes and y in nodes:
            sub_edges.append((x,y))



# with open('musae_facebook_edges.csv', mode='r', newline='') as f:
#     data = csv.reader(f)
#     next(data, None)
#     for row in data:
#         x = int(row[0]) + 1
#         y = int(row[1]) + 1
#         edges.append((x, y))
#         if x not in adj:
#             adj[x] = []
#         if y not in adj:
#             adj[y] = []

#         adj[x].append(y)
#         adj[y].append(x)


with open('com-youtube.ungraph.txt', mode='r', newline='') as f:
    lines =  f.readlines()
    for line in lines:
        x, y = map(int, line.split())
        edges.append((x, y))
        if x not in adj:
            adj[x] = []
        if y not in adj:
            adj[y] = []

        adj[x].append(y)
        adj[y].append(x)



get_sub_graph(H = 2, Max_node = 100, t = 1)
mapping = {}
tot = 1
for x in nodes:
    mapping[x] = tot
    tot += 1


with open('input5.txt', mode='w', encoding='utf-8') as f:
    f.write(str(len(nodes)) + ' ' + str(len(sub_edges)) + '\n')
    f.write(' '.join(map(str, [random.randint(1, 100) for _ in range(len(nodes))])) + '\n')
    random_integers =  [random.randint(1, 10) for _ in range(len(nodes))]
    f.write(' '.join(map(str, random_integers)) + '\n')
    f.write(' '.join(map(str, [random.randint(0, min(x, 2)) for x in random_integers])) + '\n')
    for x, y in sub_edges:
        f.write(str(mapping[x]) + ' ' + str(mapping[y]) + ' 1\n')