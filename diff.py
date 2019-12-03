import sys
import subprocess
import difflib
import itertools

configfile = sys.argv[1]
seed = sys.argv[2]
times = 1

def diff(a, b):
	print('{} => {}'.format(a,b))  
	for i,s in enumerate(difflib.ndiff(a, b)):
		if s[0]==' ': continue
		elif s[0]=='-':
			print(u'Delete "{}" from position {}'.format(s[-1],i))
		elif s[0]=='+':
			print(u'Add "{}" to position {}'.format(s[-1],i))    
	print()   

resultsMap = {}

resList = []
correctList = []
for i in range(0, times):
	correctRes = subprocess.run(["./soda_correct", configfile, seed], stdout=subprocess.PIPE).stdout.decode('utf-8')
	correctRes = "\n".join(correctRes.split('\n')[1:])

	# if correctRes in resultsMap:
	# 	resultsMap[correctRes][1] += 1
	# else:
	# 	resultsMap[correctRes] = [0, 1]

	myRes = subprocess.run(["./soda", configfile, seed], stdout=subprocess.PIPE).stdout.decode('utf-8')
	# if myRes in resultsMap:
	# 	resultsMap[myRes][0] += 1
	# else:
	# 	resultsMap[myRes] = [1,0]

	if correctRes != myRes:
		for correct, mine in itertools.zip_longest(correctRes.split("\n"), myRes.split("\n"), fillvalue=""):
			for i, v in enumerate(correct.split("\t")):
				if len(v) == 0:
					continue

				while i > len(correctList)-1:
					correctList.append([])
				correctList[i].append(v)

			for i, v in enumerate(mine.split("\t")):
				if len(v) == 0:
					continue
				while i > len(resList)-1:
					resList.append([])
				resList[i].append(v)
			print("correct:\t", correct)
			print("output :\t", mine)
			print("\n\n")

	for correct, mine in itertools.zip_longest(correctList, resList, fillvalue=""):
		print("correct:\t", correct)
		print("output :\t",mine)
		print("\n")
	# diff(correctRes, myRes)


# for key in resultsMap:
	# print(key)
	# print(resultsMap[key])