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

class Obje:
	name = ""
	iden = -1
	# the start / end index in the list of all polygon vertices
	indeStar = -1
	indeEnd_ = -1
	rota = (0.0, 0.0, 0.0)
	scal = (1.0, 1.0, 1.0)
	loca = (0.0, 0.0, 0.0)
	# is this object stationary (never moves) or dynamic
	stat = False
	# 0: flat shader. 1: normal shader. 2: image texture.
	shad = 0
	# index of texture in imagList
	imagInde = 0
	# TODO: use key / value
	def Writ(self, dataDire, stat):
		retu = []
		# if it doesnt exist, make a new directory for the object, named for its identification number
		if os.path.exists(dataDire + str(self.iden)) == False:
			os.makedirs(dataDire + os.sep + str(self.iden))
		retu.append(self.name)
		retu.append(str(self.iden))
		retu.append(str(self.indeStar))
		retu.append(str(self.indeEnd_))
		retu.append(str(self.rota[0]) + " " + str(self.rota[1]) + " " + str(self.rota[2]))
		retu.append(str(self.scal[0]) + " " + str(self.scal[1]) + " " + str(self.scal[2]))
		retu.append(str(self.loca[0]) + " " + str(self.loca[1]) + " " + str(self.loca[2]))
		retu.append(str(stat))
		retu.append(str(self.shad))
		retu.append(str(self.imagInde))
		Pyth.LineTo__File(retu, dataDire + str(self.iden) + os.sep + "obje")
	def Read(self, dataDire, iden):
		line = Pyth.FileTo__Line(dataDire + str(iden) + os.sep + "obje")
		self.name = line[0]
		self.iden = int(line[1])
		self.indeStar = int(line[2])
		self.indeEnd_ = int(line[3])
		line[4] = line[4].split(" ")
		self.rota = (float(line[4][0]), float(line[4][1]), float(line[4][2]))
		line[5] = line[5].split(" ")
		self.scal = (float(line[5][0]), float(line[5][1]), float(line[5][2]))
		line[6] = line[6].split(" ")
		self.loca = (float(line[6][0]), float(line[6][1]), float(line[6][2]))
		if line[7] == "False":
			self.stat = False
		if line[7] == "True":
			self.stat = True
		self.shad = int(line[8])
		self.imagInde = int(line[9])

# slot: pointer to object.data.material_slots
def ShadGet_(objeDire, iden, slot, lengPoly):
	retu = -1
	if len(slot) > 0:
		mate = slot[0].material
		# TODO: version
		#if slot[0].material.texture_slots[0] != None:
		if 1 == 0:
			retu = 2
		else:
			name = slot[0].material.name
			name = name.split(".")
			if len(name) > 0 and name[0] == "norm":
				retu = 1
			else:
				retu = 0
	lengColo = lengUv__ = 0
	if retu == 0 or retu == 1:
		lengColo = lengPoly
	uv__List = []
	if retu == 2:
		filePath = objeDire + str(iden) + os.sep + "uv__"
		if os.path.exists(filePath):
			uv__List = Pyth.FileTo__Line(filePath)
			if len(uv__List) > 0:
				uv__Leng = int(uv__List[0])
				uv__List.pop(0)
			for a in range(len(uv__List)):
				uv__ = uv__List[a]
				uv__ = uv__.split(" ")
				uv__List[a] = (float(uv__[0]), float(uv__[1]))
	return retu, lengColo, lengUv__, uv__List

def DataGet_(obje, shad):
	import bpy
	# get data
	vertList = Blen.VertList()
	polyList = Blen.PolyList()
	normList = Blen.NormList()
	coloList = []
	if shad == 0 or shad == 1:
		for poly in obje.data.polygons:
			mate_inde = poly.material_index
			if mate_inde >= 0 and len(obje.material_slots) > mate_inde:
				coloList.append(tuple(obje.material_slots[mate_inde].material.diffuse_color))
				coloList.append(tuple(obje.material_slots[mate_inde].material.diffuse_color))
				coloList.append(tuple(obje.material_slots[mate_inde].material.diffuse_color))
	chilList = []
	for chil in obje.children:
		chilList.append(chil.name)
	return vertList, polyList, normList, coloList, chilList

def DataWrit(vertList, polyList, normList, coloList, uv__List, chilList, dataDire, iden, shad):
	# write data
	line = []
	for vert in vertList:
		line.append(str(vert[0]) + " " + str(vert[1]) + " " + str(vert[2]))
	Pyth.LineTo__File(line, dataDire + str(iden) + os.sep + "vert")
	line = []
	for poly in polyList:
		stri = ""
		for a in range(len(poly)):
			stri += str(poly[a])
			if a < len(poly) - 1:
				stri += " "
		line.append(stri)
	Pyth.LineTo__File(line, dataDire + str(iden) + os.sep + "poly")
	line = []
	for norm in normList:
		line.append(str(norm[0]) + " " + str(norm[1]) + " " + str(norm[2]))
	Pyth.LineTo__File(line, dataDire + str(iden) + os.sep + "norm")
	line = []
	for colo in coloList:
		line.append(str(colo[0]) + " " + str(colo[1]) + " " + str(colo[2]))
	Pyth.LineTo__File(line, dataDire + str(iden) + os.sep + "colo")
	line = []
	# TODO:
	for uv__ in uv__List:
		stri = ""
		for a in range(len(uv__)):
			stri += str(uv__[a])
			if a < len(uv__) - 1:
				stri += " "
		line.append(stri)
	Pyth.LineTo__File(line, dataDire + str(iden) + os.sep + "uv__")
	Pyth.LineTo__File(chilList, dataDire + str(iden) + os.sep + "chil")

def DataRead(dataDire, iden, shad):
	# TODO: make reference functions that specify file names, formats, order, etc
	vertList = Pyth.FileTo__Line(dataDire + str(iden) + os.sep + "vert")
	for a in range(len(vertList)):
		vertList[a] = vertList[a].split(" ")
		vertList[a] = (float(vertList[a][0]), float(vertList[a][1]), float(vertList[a][2]))
	polyList = Pyth.FileTo__Line(dataDire + str(iden) + os.sep + "poly")
	for a in range(len(polyList)):
		poly = polyList[a].split(" ")
		polyList[a] = []
		for b in range(len(poly)):
			polyList[a].append(int(poly[b]))
		polyList[a] = tuple(polyList[a])
	normList = Pyth.FileTo__Line(dataDire + str(iden) + os.sep + "norm")
	for a in range(len(normList)):
		normList[a] = normList[a].split(" ")
		normList[a] = (float(normList[a][0]), float(normList[a][1]), float(normList[a][2]))
	coloList = []
	if shad == 0 or shad == 1:
		coloList = Pyth.FileTo__Line(dataDire + str(iden) + os.sep + "colo")
		for a in range(len(coloList)):
			coloList[a] = coloList[a].split(" ")
			coloList[a] = (float(coloList[a][0]), float(coloList[a][1]), float(coloList[a][2]))
	# TODO:
	uv__List = []
	if shad == 2:
		uv__List = Pyth.FileTo__Line(dataDire + str(iden) + os.sep + "uv__")
		for a in range(len(uv__List)):
			uv__ = uv__List[a].split(" ")
			uv__List[a] = []
			for b in range(len(uv__)):
				uv__List[a].append(float(uv__[b]))
			uv__List[a] = tuple(uv__List[a])
	chilList = Pyth.FileTo__Line(dataDire + str(iden) + os.sep + "chil")
	return vertList, polyList, normList, coloList, uv__List, chilList

def IdenRead(dataDire):
	retu = Pyth.FileTo__Line(dataDire + "iden")
	retu = retu[0]
	retu = int(retu)
	return retu

def CounRead(dataDire):
	retu = Pyth.FileTo__Line(dataDire + "coun")
	vert = retu[0]
	poly = retu[1]
	colo = retu[2]
	uv__ = retu[3]
	imag = retu[4]
	return int(vert), int(poly), int(colo), int(uv__), int(imag)

def CounWrit(dataDire, objeDire, iden, shad, vertCoun, polyCoun, lengPoly, coloCoun, lengColo, uv__Coun, lengUv__, imagCoun):
	lengColo = lengUv__ = 0
	if shad == 0 or shad == 1:
		lengColo = lengPoly
	if shad == 2:
		lengUv__ = Pyth.FileTo__Line(objeDire + str(iden) + os.sep + "uv__")
		lengUv__ = lengUv__[0]
		lengUv__ = int(lengUv__)
	Pyth.LineTo__File([str(vertCoun + lengPoly * 9), str(polyCoun + lengPoly), str(coloCoun + lengColo), str(uv__Coun + lengUv__), str(imagCoun)], dataDire + "coun")

def ObjeRead(name, iden, vertCoun, lengPoly, rota, scal, loca, stat, shad, imagCoun):
	retu = Obje()
	retu.name = name
	retu.iden = iden
	retu.indeStar = vertCoun
	retu.indeEnd_ = vertCoun + lengPoly * 9
	retu.rota = rota
	retu.scal = scal
	retu.loca = loca
	retu.stat = stat
	retu.shad = shad
	if shad == 2:
		retu.imagInde = imagCoun
	return retu

# TODO: some artifacts. lat / lon lines. polygon gradients at the bottom
def VertNorm(vect, vertList, polyList, normList, coloList):
	vertNorm = []
	tole = 0.001
	for a in range(len(vertList)):
		nor_List = []
		for b in range(len(polyList)):
			ver1 = polyList[b][0]
			ver2 = polyList[b][1]
			ver3 = polyList[b][2]
			if a == ver1 or a == ver2 or a == ver3:
				exis = False
				for c in range(len(nor_List)):
					if Math.VectDot_(nor_List[c], normList[b]) > 1.0 - tole:
						exis = True
						break
				if exis == False:
					nor_List.append(normList[b])
		norm = Math.VectAver(nor_List)
		#print("nor1", norm)
		if type(norm) == tuple: norm = Math.VectNorm(norm)
		#print("nor2", norm)
		if type(norm) == tuple: norm = Math.VectDot_(vect, norm)
		#print("nor3", norm)
		if type(norm) == float: norm = (norm + 1.0) / 2.0
		#print("nor4", norm)
		if type(norm) == float:
			vertNorm.append(norm)
		else:
			print(a, norm)
	for a in range(len(polyList)):
		ver1 = polyList[a][0]
		ver2 = polyList[a][1]
		ver3 = polyList[a][2]
		ind1 = a * 3 + 0
		ind2 = a * 3 + 1
		ind3 = a * 3 + 2
		coloList[ind1] = (coloList[ind1][0] * vertNorm[ver1], coloList[ind1][1] * vertNorm[ver1], coloList[ind1][2] * vertNorm[ver1])
		coloList[ind2] = (coloList[ind2][0] * vertNorm[ver2], coloList[ind2][1] * vertNorm[ver2], coloList[ind2][2] * vertNorm[ver2])
		coloList[ind3] = (coloList[ind3][0] * vertNorm[ver3], coloList[ind3][1] * vertNorm[ver3], coloList[ind3][2] * vertNorm[ver3])
	return coloList

