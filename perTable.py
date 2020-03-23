import random

f = open ('table.txt','w')
array = []
f.write(str(32))
for x in range(32):
    array.append(x + 1) 
random.shuffle(array)
for x in range(32):
    f.write('\n' + str(array[x]))
    
f.close()