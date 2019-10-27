#use : $python cal_line.py {filename}

from sys import argv

def cal_line( arguments ) :
	f = open( arguments[1] , "r" )
	count = 0
	while f.readline() :
		count = count + 1
		if str == '' :
			break
	print( count )
	f.close()

if __name__ == '__main__' :
	arguments = argv
	cal_line( arguments )



"""
f = open("pileupweights_65740.csv","r")
count = 0
while True :
	str = f.readline()
	count = count + 1	
	if str == '' :
		break
	
print(count)
f.close()


f1 = open("pileupweights_69200.csv","r")
count1 = 0
while True :
	str = f1.readline()
	count1 = count1 + 1	
	if str == '' :
		break
print(count1)
f1.close()


f2 = open("pileupweights_72660.csv","r")
count2 = 0
while True :
	str = f2.readline()
	count2 = count2 + 1	
	if str == '' :
		break
print(count2)
f2.close()
"""
