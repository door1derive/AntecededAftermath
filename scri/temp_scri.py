objeDire = "obje\\"
dataDire = "obje\\data\\"
stat = True
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
	spec = importlib.util.spec_from_file_location("Game", os.path.abspath(os.curdir) + os.sep + "scri" + os.sep + "Game.py")
	Game = importlib.util.module_from_spec(spec)
	spec.loader.exec_module(Game)

	for obje in bpy.context.scene.objects:
		if obje.type == 'MESH':
			name = obje.name
			Blen.Sele(name)
			Blen.Tria()
			Blen.VertDoub()
			iden = Game.IdenRead(dataDire)
			vertCoun, polyCoun, coloCoun, uv__Coun, imagCoun = Game.CounRead(dataDire)
			lengPoly = len(Blen.Polygons())
			shad, lengColo, lengUv__, uv__List = Game.ShadGet_(objeDire, iden, obje.material_slots, lengPoly)
			new_Obje = Game.ObjeRead(name, iden, vertCoun, lengPoly, Blen.RotaRead(), Blen.ScalRead(), Blen.LocaRead(), stat, shad, imagCoun)
			# write object properties
			new_Obje.Writ(dataDire, stat)
			# get obje data from blender
			vertList, polyList, normList, coloList, chilList = Game.DataGet_(obje, new_Obje.shad)
			# write obje data
			Game.DataWrit(vertList, polyList, normList, coloList, uv__List, chilList, dataDire, iden, new_Obje.shad)
			# write object count to a file
			Pyth.LineTo__File([str(iden + 1)], dataDire + "iden")
			# update count
			if shad == 2: imagCoun += 1
			Game.CounWrit(dataDire, objeDire, new_Obje.iden, new_Obje.shad, vertCoun, polyCoun, lengPoly, coloCoun, lengColo, uv__Coun, lengUv__, imagCoun)			

main()
