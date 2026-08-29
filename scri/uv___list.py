import importlib.util
import os

spec = importlib.util.spec_from_file_location("Modu", os.path.expanduser("~") + os.sep + "Documents" + os.sep + "prog" + os.sep + "Pyth" + os.sep + "Modu" + os.sep + "Modu.py")
Modu = importlib.util.module_from_spec(spec)
spec.loader.exec_module(Modu)

Pyth = Modu.Pyth
Math = Modu.Math
Blen = Modu.Blen
BlenGame = Modu.BlenGame
Gene = Modu.Gene
Node = Modu.Node

def main():

	line = Pyth.FileTo__Line("obje/0/uv__")
	#print(line)
	lin_ = 0
	for a in range(len(line)):
		valu = line[a]
		valu = valu.split(" ")
		if len(valu) == 2:
			print("doorUv__List[" + str(int(lin_ * 2)) + "] = " + valu[0] + "; doorUv__List[" + str(int(lin_ * 2 + 1)) + "] = " + valu[1] + ";")
			lin_ += 1

main()

