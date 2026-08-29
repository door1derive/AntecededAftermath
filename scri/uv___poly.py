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

	import bpy

	print()

	iden = 0
	objeDire = "obje" + os.sep
	writ = True
	#writ = False
	obje = bpy.context.object
	uv__List = []
	# TODO: why like this
	"""
	leng = len(obje.data.vertices)
	for a in range(leng):
		brea = False
		for poly in obje.data.polygons:
			for b in range(len(poly.vertices)):
				if poly.vertices[b] == a:
					uvco = obje.data.uv_layers.active.data[poly.loop_indices[b]].uv
					uv__List.append(str(uvco[0]) + " " + str(uvco[1]))
					brea = True
					break
			if brea:
				break
	"""
	for a in range(len(obje.data.polygons)):
		poly = obje.data.polygons[a]
		for b in range(len(poly.vertices)):
			uvco = obje.data.uv_layers.active.data[poly.loop_indices[b]].uv
			uv__List.append(str(uvco[0]) + " " + str(uvco[1]))
	if writ:
		uv__List.insert(0, str(len(uv__List)))
		Pyth.LineTo__File(uv__List, objeDire + str(iden) + os.sep + "uv__")

main()
