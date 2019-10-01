import os

out = open('performance.csv', 'w')
for nserver in os.listdir("ptest"):
	wt = 0
	rt = 0
	wl = 0
	rl = 0
	correct = True
	for i, fname in enumerate(os.listdir("ptest/"+nserver+"/")):
		f = open("ptest/"+nserver+"/"+fname)
		for line in f:
			print("{}\n\n".format(line))
			tokens = line.split(':')
			if tokens[0] == "Correctness":
				correct = int(tokens[1].split("of")[0].strip()) == 1000
			elif tokens[0] == "Throughput":
				if "write" in tokens[1]:
					wt += float(tokens[1].split("qps")[0].strip())
				elif "read" in tokens[1]:
					rt += float(tokens[1].split("qps")[0].strip())
			elif tokens[0] == "Latency":
				if "write" in tokens[1]:
					wl += float(tokens[1].split("ms")[0].strip())
				elif "read" in tokens[1]:
					rl += float(tokens[1].split("ms")[0].strip())
	rl = rl / float(i+1)
	wl = wl / float(i+1)
	out.write("%s,%.6f,%.6f,%.6f,%.6f\n"%(nserver,rt,wt,rl,wl))
out.close()
