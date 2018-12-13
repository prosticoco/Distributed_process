print("hello")

import os

# creating dictionary of dependencies
depend = {}
with open("membership") as f:
    i = 1
    for _ in range(6):
        next(f)
    for line in f:
        print(line)
        key = i
        val = [int(l) for l in line.split()]
        depend[key] = val
        i += 1


Mapping = {}

for file in os.listdir("./out"):
    process_n = int(file[-5])
    print(process_n)
    S = []
    f = open("./out/" + file, "r")
    for line in f:
        if line[0] == 'd':
            action, sender, m_nr = line.split()

            if(int(sender) in depend[process_n]):
                S.append((int(sender), int(m_nr)))

        elif line[0] == 'b':
            action, m_nr = line.split()
            sender = process_n
            Mapping[(sender, int(m_nr))] = S[:]

print("created mapping for all messages + length is " + str(len(Mapping)))

delivered = [0,0,0,0,0]
test = True
for file in os.listdir("./out"):
    process_n = int(file[-5])
    print(process_n)
    f = open("./out/" + file, "r")
    for line in f:
        if line[0] == 'd':
            action, sender, m_nr = line.split()
            did = Mapping.get((int(sender), int(m_nr)))

            for a in did:
                if(a[1] > delivered[a[0]-1]):
                    test = False
                    print("FAIL!!! " + str(a[0]) + " " + str(a[1]) + " was not performed by process: " + str(process_n) + " before delivering " + m_nr + " from " + sender)
                    break
            delivered[int(sender)-1] = int(m_nr)
print(depend)
print(test)


        
