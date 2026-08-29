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

	print()

	name = "door"
	#name = "occl"

	vert = True
	vert = False
	if vert:
		# read vert list
		line = Pyth.FileTo__Line("vert_list")
		lin_List = []
		# TODO: misses last semicolon
		# TODO: make this into a find and replace function
		inde = 0
		for a in range(len(line)):
			lin_ = ""
			find = 0
			prev = -2
			while True:
				find = Pyth.Find(line[a], "[", find)
				next = Pyth.Find(line[a], "]", find)
				if find == -1: break
				lin_ += line[a][prev + 2: find + 2] + str(inde)
				ind_ = line[a][find + 2 : next + 1]
				prev = find + len(ind_)
				find += 1
				inde += 1
			lin_ += line[a][prev + 2: len(line[a]) - 1]
			lin_List.append(lin_)
		Pyth.LineTo__File(lin_List, "ver__list")

	ver_ = True
	#ver_ = False
	if ver_:
		import bpy
		import math
		uv__List = []
		inde = 0
		for a in range(len(bpy.context.object.data.polygons)):
			poly = bpy.context.object.data.polygons[a]
			for b in range(len(poly.vertices)):
				co = bpy.context.object.data.vertices[poly.vertices[b]].co
				uv__List.append("\t" + name + "VertList[" + str(inde) + "] = " + str(co.x) + "; " + name + "VertList[" + str(inde + 1) + "] = " + str(co.y) + "; " + name + "VertList[" + str(inde + 2) + "] = " + str(co.z) + ";")
				inde += 3
		#uv__List.insert(0, len(bpy.context.object.data.vertices))
		#Pyth.LineTo__File(uv__List, "obje/0/uv__")
		for a in range(len(uv__List)):
			print(uv__List[a])

	norm = True
	#norm = False
	if norm:
		import bpy
		import math
		uv__List = []
		inde = 0
		for a in range(len(bpy.context.object.data.polygons)):
			poly = bpy.context.object.data.polygons[a]
			#for b in range(len(poly.vertices)):
			#co = bpy.context.object.data.vertices[poly.vertices[b]].co
			uv__List.append("\t" + name + "NormList[" + str(inde) + "] = " + str(poly.normal[0]) + "; " + name + "NormList[" + str(inde + 1) + "] = " + str(poly.normal[1]) + "; " + name + "NormList[" + str(inde + 2) + "] = " + str(poly.normal[2]) + ";")
			inde += 3
		#uv__List.insert(0, len(bpy.context.object.data.vertices))
		#Pyth.LineTo__File(uv__List, "obje/0/uv__")
		for a in range(len(uv__List)):
			print(uv__List[a])

	# TODO
	colo = True
	#colo = False
	if colo:
		import bpy
		import math
		uv__List = []
		inde = 0
		for a in range(len(bpy.context.object.data.polygons)):
			poly = bpy.context.object.data.polygons[a]
			for b in range(len(poly.vertices)):
				uv__List.append("\t" + name + "ColoList[" + str(inde) + "] = " + str(1.0) + "; " + name + "ColoList[" + str(inde + 1) + "] = " + str(0.0) + "; " + name + "ColoList[" + str(inde + 2) + "] = " + str(0.0) + ";")
				inde += 3
		for a in range(len(uv__List)):
			print(uv__List[a])

	uv__ = True
	#uv__ = False
	if uv__:
		import bpy
		import math
		uv__List = []
		inde = 0
		#loca = Blen.LocaRead()
		for a in range(len(bpy.context.object.data.polygons)):
			poly = bpy.context.object.data.polygons[a]
			for b in range(len(poly.vertices)):
				co = bpy.context.object.data.vertices[poly.vertices[b]].co
				#widt = 1.0
				#widt = 4.0
				widt = 6.0
				cenu = 0.0
				# map y to uv
				#maxu = 5.0
				# TODO: depends on orientation of door
				# TODO: incorportate orientation, cenu, and origin
				u = math.fabs((co.y + (widt / 2.0)) / widt)
				# map v to z, starting at 0 to -4
				# TODO: test switch origin from top left to bottom left and pull out complement
				#maxi = -4.0
				maxi = -6.0
				cenv = 3.0
				#v = math.fabs(co.z / maxi)
				###########
				# shifting door origin to 2 in z
				v = co.z
				v += math.fabs(cenv)
				#v += math.fabs(maxi / 2.0)
				v /= maxi
				#v += math.fabs(cenv / maxi)
				#v = math.fabs(v / maxi)
				v = math.fabs(v)
				#v = math.fabs(1.0 - co.z / maxi)
				uv__List.append("\t" + name + "Uv__List[" + str(inde) + "] = " + str(u) + "; " + name + "Uv__List[" + str(inde + 1) + "] = " + str(v) + ";")
				inde += 2
				# uv to color (red and green)
				#uv__List.append("\t" + name + "ColoList[" + str(inde) + "] = " + str(u) + "; " + name + "ColoList[" + str(inde + 1) + "] = " + str(v) + "; " + name + "ColoList[" + str(inde + 2) + "] = 0.0;")
				#inde += 3

		#uv__List.insert(0, len(bpy.context.object.data.vertices))
		#Pyth.LineTo__File(uv__List, "obje/0/uv__")
		for a in range(len(uv__List)):
			print(uv__List[a])

main()

