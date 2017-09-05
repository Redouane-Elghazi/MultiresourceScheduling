#!/usr/bin/env python
from __future__ import print_function
import sys, os
from subprocess import *
from random import *
from math import *
from io import *
from numpy import mean
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

#call(["make", "all"])


s = randrange(10**9)

i = 1
newplot = False
while i<len(sys.argv):
	if sys.argv[i]=="-n":
		newplot = True
	elif sys.argv[i]=="-s":
		i+=1
		s = int(sys.argv[i])
	i+=1

seed(s)

samplesize = 2
#maxn_4 = 10
x = []
ymean = [[],[],[],[]]
tmean = [[],[],[],[]]
ymax = [[],[],[],[]]
tmax = [[],[],[],[]]

start = 0
step = 10
nbsteps= 7
stop = start+nbsteps*step+1
d = 2
n_4 = 50
P = 30
P_max = 900

if not(newplot):
	try:
		mem = open("log.txt", "r+")
		val = mem.readline()
		while val!='':
			val = [float(v) for v in val.split()]
			x+=[int(val[0])]
			start = x[-1]
			for i in range(4):
				ymean[i] += [val[1+4*i]]
				ymax[i] += [val[2+4*i]]
				tmean[i] += [val[3+4*i]]
				tmax[i] += [val[4+4*i]]
			val = mem.readline()
	except:
		print("Nothing to recover")
		mem = open("log.txt", "w")
else:
	mem = open("log.txt", "w")

for P in range(start+step, stop, step):
	print(" "*30,end="\r")
	print(P, "/", stop-1,end="\r")
	sampley=[[],[],[],[]]
	samplet=[[],[],[],[]]
	for _ in range(samplesize):
		#f = open("temp.in", "w")
		print("\t\t"," "*30,end="\r")
		print("\t\t",_+1,"/",samplesize,end="\r")
		print("\t\t\t\tcreating instance",end="\r")
		sys.stdout.flush()
		n_ahmdalmax = n_4
		n_ahmdalsum = n_4
		n_alphamax = n_4
		n_alphaprod = n_4
		n = n_ahmdalmax + n_ahmdalsum + n_alphamax + n_alphaprod
		inputheader = [str(n), str(d)]

		for i in range(d):
			inputheader += [str(P+i)]#int(P_max**(1./d)))]#randrange(1, int(P_max**(1./d))))]
		I=[str(s) + "\n" + " ".join(inputheader)+"\n"]
		#f.write(unicode(str(s) + "\n" + " ".join(inputheader)+"\n"))
		#f.flush()
		#os.fsync(f)

		for j in range(n_ahmdalsum):
			W = [str(random()/(d+1))]
			for i in range(d):
				W += [str(random()/(d+1))]
			#Popen(["./tasks/ahmdalsum"] + inputheader[1:] + W, stdout=f)
			#f.flush()
			#os.fsync(f)
			I+=[check_output(["./tasks/ahmdalsum"] + inputheader[1:] + W)]

		for j in range(n_ahmdalmax):
			W = [str(random()/2)]
			for i in range(d):
				W += [str(random()/2)]
			#Popen(["./tasks/ahmdalmax"] + inputheader[1:] + W, stdout=f)
			#f.flush()
			#os.fsync(f)
			I+=[check_output(["./tasks/ahmdalmax"] + inputheader[1:] + W)]

		for j in range(n_alphaprod):
			W = [str(random())]
			alpha = []
			for i in range(d):
				alpha += [str(random())]
			#Popen(["./tasks/alphaprod"] + inputheader[1:] + W + alpha, stdout=f)
			#f.flush()
			#os.fsync(f)
			I+=[check_output(["./tasks/alphaprod"] + inputheader[1:] + W + alpha)]

		for j in range(n_alphamax):
			W = []
			alpha = []
			for i in range(d):
				W += [str(random())]
				alpha += [str(random())]
			#Popen(["./tasks/alphamax"] + inputheader[1:] + W + alpha, stdout=f)
			#f.flush()
			#os.fsync(f)
			I+=[check_output(["./tasks/alphamax"] + inputheader[1:] + W + alpha)]

		#f.close()
		print("\t\t\t\t                 ",end="\r")
		print("\t\t\t\trunning",end="\r")
		sys.stdout.flush()
		#res = check_output(["./simulator", "-i", "temp.in", "-u"])
		simu = Popen(["./schedule_sim", "-i", "cin", "-u"], stdin=PIPE, stdout=PIPE)
		res = simu.communicate("".join(I))[0]
		print("\t\t\t\t                 ",end="\r")
		print("\t\t\t\tdone",end="\r")
		res = [float(line.split(" ")[0]) for line in res.split("\n")[:10]]
		sampley[0] += [res[2]/res[0]]
		sampley[1] += [res[4]/res[0]]
		sampley[2] += [res[6]/res[0]]
		sampley[3] += [res[8]/res[0]]
		samplet[0] += [res[3]]
		samplet[1] += [res[5]]
		samplet[2] += [res[7]]
		samplet[3] += [res[9]]
	x += [P]
	toMem = str(x[-1])
	for i in range(4):
		ymean[i] += [mean(sampley[i])]
		ymax[i] += [max(sampley[i])]
		tmean[i] += [mean(samplet[i])]
		tmax[i] += [max(samplet[i])]
		toMem += " " + str(ymean[i][-1]) +" " + str(ymax[i][-1]) +" " + str(tmean[i][-1]) + " " + str(tmax[i][-1])
	mem.write(unicode(toMem+"\n"))
	mem.flush()

print()
mem.close()
		
fig = plt.figure()
p = fig.add_subplot(111)

p.plot(x, ymean[0], 'r', label="List")
p.plot(x, ymean[1], 'b', label="Pack")
p.plot(x, ymean[2], 'g', label="Transfolist")
p.plot(x, ymean[3], 'y', label="Transfopack")
p.legend()
fig.savefig('makespanmean.png')

fig = plt.figure()
p = fig.add_subplot(111)

p.plot(x, ymax[0], 'r', label="List")
p.plot(x, ymax[1], 'b', label="Pack")
p.plot(x, ymax[2], 'g', label="Transfolist")
p.plot(x, ymax[3], 'y', label="Transfopack")
p.legend()
fig.savefig('makespanmax.png')

fig = plt.figure()
p = fig.add_subplot(111)
p.plot(x, tmean[0], 'r', label="List")
p.plot(x, tmean[1], 'b', label="Pack")
p.plot(x, tmean[2], 'g', label="Transfolist")
p.plot(x, tmean[3], 'y', label="Transfopack")
p.legend()
fig.savefig('exectimemean.png')

fig = plt.figure()
p = fig.add_subplot(111)
p.plot(x, tmax[0], 'r', label="List")
p.plot(x, tmax[1], 'b', label="Pack")
p.plot(x, tmax[2], 'g', label="Transfolist")
p.plot(x, tmax[3], 'y', label="Transfopack")
p.legend()
fig.savefig('exectimemax.png')
