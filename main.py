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

	# TODO
	# vertex normal calculation failing
	# parenting problem
	# better organize data offset programming
	# apply all geometry modifiers on an object
	# textures
	# object cant have multiple colors. need to read material id of specific polygon
	# maybe it would be better to associate inteList with an obje object, or a dictionary or something
	# sky box is hard-coded in c
	# maybe format as pipeline

	from scri import Game

	print()

	### SCRIPT OPTIONS

	# scenes to export
	#statScenList = ["scen_text.blend", "scen_stat.blend"]
	#statScenList = ["scen_stat.blend"]
	statScenList = ["helios_floor.blend"]
	#dynaScenList = ["scen_dyna.blend"]
	dynaScenList = []
	docuPath = os.path.expanduser("~") + os.sep + "Documents" + os.sep
	# TODO: maybe define game title
	#gamePath = "game" + os.sep + "node_gl__" + os.sep
	gamePath = "game" + os.sep + "WormholeAftermath" + os.sep
	blenPath = "blen" + os.sep
	objeDire = "obje" + os.sep
	dataDire = "obje" + os.sep + "data" + os.sep
	# TODO: maybe a combine files script should be made
	scenDire = "scen" + os.sep
	#scenDire = docuPath + blenPath + gamePath + "scen" + os.sep
	# vector used for normal shader
	ligh = ((2.0 ** 0.5) / 2.0, 0.0, (2.0 ** 0.5) / 2.0)
	# how many data elements should be written to a file before it's split into a different file
	inte = 1000
	# write files to disk
	writ = True
	#writ = False

	### GAME OPTIONS

	# window resolution
	widt = 640
	heig = 640
	# camera movement speed
	cameSpee = 0.2
	# camera starting location
	cameLoca = (0.0, -5.0, 0.0)
	# background color
	skyr = 0.6
	skyg = 0.8
	skyb = 1.0
	skya = 1.0
	# use a skybox. requires six images in obje + os.sep + sky_: down.bmp east.bmp nort.bmp sout.bmp up__.bmp west.bmp. 
	sky_ = True
	sky_ = False
	# use fullscreen
	fullScre = True
	fullScre = False
	# lock the mouse position to the screen center
	warpPoin = True
	#warpPoin = False
	# hide the mouse cursor
	hideCurs = True
	#hideCurs = False
	# print statements to console
	prin = True
	prin = False
	# time of one frame in billionths of a second
	timeFram = 16666667
	
	##########################

	# create general directories and files

	if writ:
		# make object directory if it doesnt exist
		if os.path.exists(objeDire) == False: os.makedirs(objeDire)
		# make data directory if it doesnt exist
		if os.path.exists(dataDire) == False: os.makedirs(dataDire)
		Pyth.LineTo__File([str(cameLoca[0]) + " " + str(cameLoca[1]) + " " + str(cameLoca[2])], dataDire + "came_loca")
		Pyth.LineTo__File([str(0)], dataDire + "iden")
		Pyth.LineTo__File([str(0), str(0), str(0), str(0), str(0)], dataDire + "coun")

	##########################

	# make scen list

	scenList = []
	for scen in statScenList: scenList.append(scen)
	scenList.append("")
	for scen in dynaScenList: scenList.append(scen)

	##########################

	# write object properties and data to a file
	blenComm = Blen.BlenCommLink()
	stat = True
	for scen in scenList:
		if scen != "":
			# make temporary script
			varsExtr = "objeDire = \"" + objeDire + "\"\n"
			varsExtr += "dataDire = \"" + dataDire + "\"\n"
			varsExtr += "stat = " + str(stat) + "\n"
			varsExtr = Pyth.EscaPath(varsExtr)
			fileObje = open("scri" + os.sep + "expo.py", mode = "r")
			stri = fileObje.read()
			fileObje.close()
			scriStri = varsExtr + stri
			fileObje = open("scri" + os.sep + "temp_scri.py", mode = "w")
			if writ: fileObje.write(scriStri)
			fileObje.close()
			# execute the command-line string and run script
			if writ: os.system(blenComm + " -b " + scenDire + scen + " --python scri" + os.sep + "temp_scri.py")
		else: stat = False

	##########################

	# data counts
	# inteList counts the total number of data files for each object
	# TODO: maybe it would be better to associate inteList with an obje object, or a dictionary or something
	inteList = []
	totaVert = 0
	totaNorm = 0
	totaColo = 0
	totaUv__ = 0
	typeList = []
	# recent dataDire: "obje" + os.sep + "data" + os.sep
	# TODO: change leng to objeCoun, here and in idenread. and maybe change file too, and everything that references it.
	# IdenRead: reads "iden" file where object count was previously written. returns object count. should also correspond to the number of folders in dataDire.
	leng = Game.IdenRead(dataDire)
	vertCoun, polyCoun, coloCoun, uv__Coun, imagCoun = Game.CounRead(dataDire)
	#print("pc", polyCoun)

	##########################

	# convert parent name list to index list
	chilList = []
	statCoun = 0
	# leng: object count
	for a in range(leng):
		line = Pyth.FileTo__Line(dataDire + str(a) + os.sep + "obje")
		chil = [line[0]]
		if line[7] == "True": statCoun += 1
		line = Pyth.FileTo__Line(dataDire + str(a) + os.sep + "chil")
		for chi_ in line: chil.append(chi_)
		chilList.append(chil)
	indeList = []
	# leng: object count
	for a in range(leng):
		indeList.append([])
		for b in range(1, len(chilList[a])):
			for c in range(len(chilList)):
				if chilList[a][b] == chilList[c][0]:
					indeList[a].append(c)

	##########################

	# skybox

	if sky_:
		indeList.append([])
		# faces of a 2 x 2 x 2 cube (quads)
		sky_Vert = [(1.0, 1.0, 1.0), (1.0, 1.0, -1.0), (-1.0, 1.0, -1.0), (-1.0, 1.0, 1.0), (1.0, -1.0, 1.0), (1.0, -1.0, -1.0), (-1.0, -1.0, -1.0), (-1.0, -1.0, 1.0), (1.0, 1.0, 1.0), (1.0, 1.0, -1.0), (1.0, -1.0, -1.0), (1.0, -1.0, 1.0), (-1.0, 1.0, 1.0), (-1.0, 1.0, -1.0), (-1.0, -1.0, -1.0), (-1.0, -1.0, 1.0), (1.0, 1.0, 1.0), (1.0, -1.0, 1.0), (-1.0, -1.0, 1.0), (-1.0, 1.0, 1.0), (1.0, 1.0, -1.0), (1.0, -1.0, -1.0), (-1.0, -1.0, -1.0), (-1.0, 1.0, -1.0)]
		sky_Poly = [(0, 1, 2, 3), (4, 5, 6, 7), (8, 9, 10, 11), (12, 13, 14, 15), (16, 17, 18, 19), (20, 21, 22, 23)]
		sky_Norm = [(0.0, 1.0, 0.0), (0.0, -1.0, 0.0), (1.0, 0.0, 0.0), (-1.0, 0.0, 0.0), (0.0, 0.0, 1.0), (0.0, 0.0, -1.0)]
		sky_Colo = []
		sky_Chil = []
		# skybox texture coordinates
		# TODO: up might be wrong
		sky_Uv__ = [(1.0, 1.0, 1.0), (1.0, 1.0, -1.0), (-1.0, 1.0, -1.0), (-1.0, 1.0, 1.0), (-1.0, -1.0, -1.0), (-1.0, -1.0, 1.0), (1.0, -1.0, 1.0), (1.0, -1.0, -1.0), (1.0, -1.0, 1.0), (1.0, 1.0, 1.0), (1.0, 1.0, -1.0), (1.0, -1.0, -1.0), (-1.0, -1.0, 1.0), (-1.0, 1.0, 1.0), (-1.0, 1.0, -1.0), (-1.0, -1.0, -1.0), (-1.0, -1.0, 1.0), (-1.0, 1.0, 1.0), (1.0, 1.0, 1.0), (1.0, -1.0, 1.0), (-1.0, -1.0, -1.0), (-1.0, 1.0, -1.0), (1.0, 1.0, -1.0), (1.0, -1.0, -1.0)]
		sky_Obje = Game.Obje()
		sky_Obje.name = "sky_"
		sky_Obje.iden = leng
		sky_Obje.indeStar = vertCoun
		#vertCoun += len(sky_Vert) * 3
		sky_Obje.indeEnd_ = vertCoun
		sky_Obje.stat = True
		# TODO
		sky_Obje.shad = -1
		#if os.path.exists(dataDire + str(leng)) == False:
		#	os.makedirs(dataDire + os.sep + str(leng))
		#sky_Obje.Writ(dataDire, True)
		#Game.DataWrit(sky_Vert, sky_Poly, sky_Norm, sky_Colo, sky_Uv__, sky_Chil, dataDire, leng, 2)
		#Game.CounWrit(dataDire, objeDire, leng, -1, vertCoun, polyCoun, len(sky_Poly), coloCoun, 0, uv__Coun, len(sky_Uv__), imagCoun)
		#leng += 1
		# TODO
		#uv__Coun += len(sky_Uv__ * 3)
		#polyCoun += len(sky_Poly * 4)
		#imagCoun += 6

	##########################

	# get depth of data directory
	sep_Coun = 0
	for a in range(len(dataDire)):
		if dataDire[a] == os.sep: sep_Coun += 1

	##########################

	# write cpp object data files

	objeImagList = []
	# leng: object count
	for a in range(leng):
		# b: the file number. if an object has fewer than 1000 of any one element, the object will have 5 data files, 0 - 4. for each data type that exceeds 1000 elements, another file is added and b increases by 1. 0 contains general object data. then vertices are exported. then normals are exported. then vertex colors are exported. then uv data is exported.
		b = 0
		inteList.append(b)
		# TODO: this might be better if inteList doesnt actually refer to files
		#inteList.append(0)
		typeList.append([])
		typeList[a].append(-1)

		obje = Game.Obje()
		obje.Read(dataDire, a)
		dire = dataDire
		# TODO: textures
		# TODO: object cant have multiple colors. need to read material id of specific polygon
		# if a texture on material 0 slot 0 exists, shad is set to 2
		shad = obje.shad
		####
		#obje.shad = 1
		#shad = 1
		####
		#if sky_ and a == leng - 1:
		#	shad = 2
		vertList, polyList, normList, coloList, uv__List, chilList = Game.DataRead(dataDire, a, shad)
		if shad == 2: objeImagList.append(a)
		#print(a, "shad", shad)
		# TODO
		while len(coloList) / 3 < len(polyList): coloList.append((0.0, 0.0, 0.0))
		while len(uv__List) / 6 < len(vertList): uv__List.append((0.0, 0.0))
		# TODO: normList needs to be modified if norm calculation fails
		# TODO: why is norm calculation failing
		# ^^^^ tattoo
		#print(obje.name)
		if obje.shad == 1: coloList = Game.VertNorm(ligh, vertList, polyList, normList, coloList)
		# TODO
		name = obje.name
		rota = obje.rota
		scal = obje.scal
		loca = obje.loca
		if obje.stat: stat = "true"
		else: stat = "false"
		fileList = [""]
		stri = "#include \".." + os.sep
		for d in range(sep_Coun): stri += ".." + os.sep
		# TODO
		#stri += "code" + os.sep + "obje.h\""
		stri += "src" + os.sep + "obje.h\""
		fileList.append(stri)
		fileList.append("")
		fileList.append("void Load_" + str(a) + "_" + str(b) + "(std::vector<ObjeInit> &objeInitList) {")
		fileList.append("")
		fileList.append("\tobjeInitList.push_back(ObjeInit());")
		fileList.append("\tobjeInitList[objeInitList.size() - 1].iden = " + str(a) + ";")
		fileList.append("\tobjeInitList[objeInitList.size() - 1].name = \"" + name + "\";")
		fileList.append("\tobjeInitList[objeInitList.size() - 1].rota.x = " + str(rota[0]) + ";")
		fileList.append("\tobjeInitList[objeInitList.size() - 1].rota.y = " + str(rota[1]) + ";")
		fileList.append("\tobjeInitList[objeInitList.size() - 1].rota.z = " + str(rota[2]) + ";")
		fileList.append("\tobjeInitList[objeInitList.size() - 1].scal.x = " + str(scal[0]) + ";")
		fileList.append("\tobjeInitList[objeInitList.size() - 1].scal.y = " + str(scal[1]) + ";")
		fileList.append("\tobjeInitList[objeInitList.size() - 1].scal.z = " + str(scal[2]) + ";")
		fileList.append("\tobjeInitList[objeInitList.size() - 1].loca.x = " + str(loca[0]) + ";")
		fileList.append("\tobjeInitList[objeInitList.size() - 1].loca.y = " + str(loca[1]) + ";")
		fileList.append("\tobjeInitList[objeInitList.size() - 1].loca.z = " + str(loca[2]) + ";")
		fileList.append("\tobjeInitList[objeInitList.size() - 1].indeStar = " + str(obje.indeStar) + ";")
		fileList.append("\tobjeInitList[objeInitList.size() - 1].indeEnd_ = " + str(obje.indeEnd_) + ";")
		fileList.append("\tobjeInitList[objeInitList.size() - 1].imagInde = " + str(obje.imagInde) + ";")
		fileList.append("\tobjeInitList[objeInitList.size() - 1].shad = " + str(obje.shad) + ";")
		fileList.append("\tobjeInitList[objeInitList.size() - 1].stat = " + stat + ";")
		# TODO: think this is a mistake
		#for b in range(len(indeList[a])):
		for c in range(len(indeList[a])):
			fileList.append("\tobjeInitList[objeInitList.size() - 1].chilList.push_back(" + str(indeList[a][c] - statCoun) + ");")
		fileList.append("")
		fileList.append("}")
		fileList.append("")
		b += 1
		if writ: Pyth.LineTo__File(fileList, dataDire + str(a) + os.sep + "data_" + str(b - 1) + ".cpp")
		def LoopEnd_(stri, fileList, coun, inteCoun, inte, leng, b, c, d, inteList, typeList):
			brea = False
			breaOne_ = False
			fileList.append(stri)
			coun += 1
			inteCoun += 1
			if d == leng - 1:
				inteCoun = inte
				b += 1
				inteList = b
				typeList.append(c)
				c += 1
				coun = 0
				brea = True
			if brea == False and inteCoun >= inte:
				inteCoun = 0
				b += 1
				inteList = b
				typeList.append(c)
				breaOne_ = True
			return fileList, coun, inteCoun, b, c, breaOne_, brea, inteList, typeList
		# c: type
		# 0 - vertex
		# 1 - normal
		# 2 - color
		# 3 - uv
		c = 0
		while c < 4:
			inteCoun = 0
			coun = 0
			brea = False
			empt = False
			#if a == 4: print("bb", b)
			while inteCoun < inte:
				fileList = [""]
				stri = "#include \".." + os.sep
				for d in range(sep_Coun): stri += ".." + os.sep
				# TODO
				#stri += "code" + os.sep + "obje.h\""
				stri += "src" + os.sep + "obje.h\""
				fileList.append(stri)
				fileList.append("")
				if c == 0:
					if len(polyList) == 0:
						empt = True
						inteCoun = inte
						c += 1
					else:
						fileList.append("void Load_" + str(a) + "_" + str(b) + "(GLfloat vertList[" + str(vertCoun + 0) + "]) {")
						fileList.append("")
						for d in range(coun, len(polyList)):
							stri = "\t"
							for e in range(len(polyList[d])):
								vert = vertList[polyList[d][e]]
								for ver_ in vert:
									stri += "vertList[" + str(totaVert) + "] = " + str(ver_) + "; "
									totaVert += 1
							fileList, coun, inteCoun, b, c, breaOne_, brea, inteList[a], typeList[a] = LoopEnd_(stri, fileList, coun, inteCoun, inte, len(polyList), b, c, d, inteList[a], typeList[a])
							if brea or breaOne_: break
						#if a == 4: print("b", b, "c", c)
				elif c == 1:
					if len(normList) == 0:
						empt = True
						inteCoun = inte
						c += 1
					else:
						fileList.append("void Load_" + str(a) + "_" + str(b) + "(GLfloat normList[" + str(polyCoun * 3 + 0) + "]) {")
						fileList.append("")
						for d in range(coun, len(normList)):
							stri = "\t"
							for e in range(len(normList[d])):
								stri += "normList[" + str(totaNorm) + "] = " + str(normList[d][e]) + "; "
								totaNorm += 1
							fileList, coun, inteCoun, b, c, breaOne_, brea, inteList[a], typeList[a] = LoopEnd_(stri, fileList, coun, inteCoun, inte, len(normList), b, c, d, inteList[a], typeList[a])
							if brea or breaOne_: break
				elif c == 2:
					if len(coloList) == 0:
						empt = True
						inteCoun = inte
						c += 1
					else:
						fileList.append("void Load_" + str(a) + "_" + str(b) + "(GLfloat coloList[" + str(coloCoun * 9 + 0) + "]) {")
						fileList.append("")
						for d in range(coun, len(coloList)):
							stri = "\t"
							for e in range(len(coloList[d])):
								stri += "coloList[" + str(totaColo) + "] = " + str(coloList[d][e]) + "; "
								totaColo += 1
							fileList, coun, inteCoun, b, c, breaOne_, brea, inteList[a], typeList[a] = LoopEnd_(stri, fileList, coun, inteCoun, inte, len(coloList), b, c, d, inteList[a], typeList[a])
							if brea or breaOne_: break
				elif c == 3:
					if len(uv__List) == 0:
						empt = True
						inteCoun = inte
						c += 1
					else:
						# TODO: uv__Coun was supposed to be a running total and totaUv__ was supposed to be the total
						#fileList.append("void Load_" + str(a) + "_" + str(b) + "(GLfloat uv__List[" + str(uv__Coun) + "]) {")
						# TODO: dont use polyCoun
						fileList.append("void Load_" + str(a) + "_" + str(b) + "(GLfloat uv__List[" + str(polyCoun * 3 * 2) + "]) {")
						#print(a, b, totaUv__)
						#print(a, b, polyCoun)
						#fileList.append("void Load_" + str(a) + "_" + str(b) + "(GLfloat uv__List[" + str(totaUv__) + "]) {")
						fileList.append("")
						for d in range(coun, len(polyList)):
							stri = "\t"
							for e in range(len(polyList[d])):
								uv__ = uv__List[polyList[d][e]]
								for u___ in uv__:
									stri += "uv__List[" + str(totaUv__) + "] = " + str(u___) + "; "
									totaUv__ += 1
							fileList, coun, inteCoun, b, c, breaOne_, brea, inteList[a], typeList[a] = LoopEnd_(stri, fileList, coun, inteCoun, inte, len(polyList), b, c, d, inteList[a], typeList[a])
							if brea or breaOne_: break
				elif c == 4: break
				fileList.append("")
				fileList.append("}")
				if writ:
					if empt == False: Pyth.LineTo__File(fileList, dataDire + str(a) + os.sep + "data_" + str(b - 1) + ".cpp")
				if brea: break
				#if a == 4: print("b", b)

	##########################

	# write main data file

	# TODO: does this work? make a real fix
	if totaVert == 0: totaVert = 1
	if totaNorm == 0: totaNorm = 1
	if totaColo == 0: totaColo = 1
	if totaUv__ == 0: totaUv__ = 1
	if imagCoun == 0: imagCoun = 1

	mainFileList = [""]
	mainFileList.append("#pragma once")
	mainFileList.append("")
	mainFileList.append("#include <vector>")
	stri = "#include \""
	for d in range(sep_Coun):
		stri += ".." + os.sep
	# TODO: all paths need to be defined above
	#stri += "code" + os.sep + "obje.h\""
	#stri += "src" + os.sep + "obje.h\""
	stri += "src" + "/" + "obje.h\""
	mainFileList.append(stri)
	mainFileList.append("")
	mainFileList.append("GLfloat vertList[" + str(totaVert + 0) + "];")
	mainFileList.append("GLfloat normList[" + str(totaNorm + 0) + "];")
	mainFileList.append("GLfloat coloList[" + str(totaColo + 0) + "];")
	mainFileList.append("GLfloat uv__List[" + str(totaUv__ + 0) + "];")
	mainFileList.append("GLuint imagList[" + str(imagCoun) + "];")
	mainFileList.append("GLuint sky_Imag[6];")
	mainFileList.append("")
	for a in range(leng):
		#for b in range(inteList[a]): mainFileList.append("#include \"" + str(a) + os.sep + "data_" + str(b) + ".cpp\"")
		for b in range(inteList[a]): mainFileList.append("#include \"" + str(a) + "/" + "data_" + str(b) + ".cpp\"")
	mainFileList.append("")
	#mainFileList.append("void Load(std::vector<ObjeInit> &objeInitList, std::vector<ObjeDyna> &objeDynaList, std::vector<ObjeStat> &objeStatList, unsigned int *widt, unsigned int *heig, float *cameSpee, Came *came, float *skyr, float *skyg, float *skyb, float *skya, bool *sky_, bool *fullScre, bool *warpPoin, bool *hideCurs, bool *prin, int *timeFram, std::vector<unsigned int> &objeImagList) {")
	mainFileList.append("void Load(std::vector<ObjeInit> &objeInitList, std::vector<ObjeDyna> &objeDynaList, std::vector<ObjeStat> &objeStatList, int *widt, int *heig, float *cameSpee, Came *came, float *skyr, float *skyg, float *skyb, float *skya, bool *sky_, bool *fullScre, bool *warpPoin, bool *hideCurs, bool *prin, int *timeFram, std::vector<unsigned int> &objeImagList) {")
	mainFileList.append("")
	mainFileList.append("\t*widt = " + str(widt) + ";")
	mainFileList.append("\t*heig = " + str(heig) + ";")
	mainFileList.append("\t*cameSpee = " + str(cameSpee) + "f;")
	mainFileList.append("\tcame->loca = {" + str(cameLoca[0]) + ", " + str(cameLoca[1]) + ", " + str(cameLoca[2]) + "};")
	mainFileList.append("\t*skyr = " + str(skyr) + "f;")
	mainFileList.append("\t*skyg = " + str(skyg) + "f;")
	mainFileList.append("\t*skyb = " + str(skyb) + "f;")
	mainFileList.append("\t*skya = " + str(skya) + "f;")
	if sky_: mainFileList.append("\t*sky_ = true;")
	else: mainFileList.append("\t*sky_ = false;")
	if fullScre: mainFileList.append("\t*fullScre = true;")
	else: mainFileList.append("\t*fullScre = false;")
	if warpPoin: mainFileList.append("\t*warpPoin = true;")
	else: mainFileList.append("\t*warpPoin = false;")
	if hideCurs: mainFileList.append("\t*hideCurs = true;")
	else: mainFileList.append("\t*hideCurs = false;")
	if prin: mainFileList.append("\t*prin = true;")
	else: mainFileList.append("\t*prin = false;")
	mainFileList.append("\t*timeFram = " + str(timeFram) + ";")
	stri = "\t"
	for a in range(len(objeImagList)):
		stri += "objeImagList.push_back(" + str(objeImagList[a]) + "); "
	mainFileList.append(stri)
	#mainFileList.append("\t*sky_Reso = " + str(sky_Reso) + ";")
	mainFileList.append("")
	for a in range(leng):
		for b in range(inteList[a]):
			# TODO: out of range
			# 4 77
			# 91
			# 77
			#print(a, b)
			#print(inteList[a])
			#print(len(typeList[a]))
			if typeList[a][b] == -1: mainFileList.append("\tvoid Load_" + str(a) + "_" + str(b) + "(std::vector<ObjeInit> &objeInitList);")
			if typeList[a][b] == 0: mainFileList.append("\tvoid Load_" + str(a) + "_" + str(b) + "(GLfloat vertList[" + str(totaVert + 0) + "]);")
			if typeList[a][b] == 1: mainFileList.append("\tvoid Load_" + str(a) + "_" + str(b) + "(GLfloat normList[" + str(totaNorm + 0) + "]);")
			if typeList[a][b] == 2 and totaColo > 0: mainFileList.append("\tvoid Load_" + str(a) + "_" + str(b) + "(GLfloat coloList[" + str(totaColo + 0) + "]);")
			#print(a, b, totaUv__)
			if typeList[a][b] == 3 and totaUv__ > 0: mainFileList.append("\tvoid Load_" + str(a) + "_" + str(b) + "(GLfloat uv__List[" + str(totaUv__ + 0) + "]);")
	mainFileList.append("")
	for a in range(leng):
		b = 0
		for b in range(inteList[a]):
			if typeList[a][b] == -1: mainFileList.append("\tLoad_" + str(a) + "_" + str(b) + "(objeInitList);")
			if typeList[a][b] == 0: mainFileList.append("\tLoad_" + str(a) + "_" + str(b) + "(vertList);")
			if typeList[a][b] == 1: mainFileList.append("\tLoad_" + str(a) + "_" + str(b) + "(normList);")
			if typeList[a][b] == 2 and totaColo > 0: mainFileList.append("\tLoad_" + str(a) + "_" + str(b) + "(coloList);")
			if typeList[a][b] == 3 and totaUv__ > 0: mainFileList.append("\tLoad_" + str(a) + "_" + str(b) + "(uv__List);")
	mainFileList.append("")
	mainFileList.append("\tfor (unsigned int a = 0; a < objeInitList.size(); a++) {")
	mainFileList.append("\t\tif (objeInitList[a].stat == true) {")
	mainFileList.append("\t\t\tObjeStat objeStat;")
	mainFileList.append("\t\t\tobjeStat = objeInitList[a];")
	mainFileList.append("\t\t\tobjeStatList.push_back(objeStat);")
	mainFileList.append("\t\t\tfor (unsigned int b = objeInitList[a].indeStar; b < objeInitList[a].indeEnd_; b += 3) {")
	mainFileList.append("\t\t\t\tvertList[b + 0] += objeInitList[a].loca.x;")
	mainFileList.append("\t\t\t\tvertList[b + 1] += objeInitList[a].loca.y;")
	mainFileList.append("\t\t\t\tvertList[b + 2] += objeInitList[a].loca.z;")
	mainFileList.append("\t\t\t}")
	mainFileList.append("\t\t}")
	mainFileList.append("\t\telse if (objeInitList[a].stat == false) {")
	mainFileList.append("\t\t\tObjeDyna objeDyna;")
	mainFileList.append("\t\t\tobjeDyna = objeInitList[a];")
	mainFileList.append("\t\t\tobjeDynaList.push_back(objeDyna);")
	mainFileList.append("\t\t}")
	mainFileList.append("\t}")
	mainFileList.append("")
	mainFileList.append("}")
	if writ: Pyth.LineTo__File(mainFileList, dataDire + "data.cpp")

	##########################

main()
