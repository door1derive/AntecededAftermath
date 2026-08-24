# floor.py

#def floor_rectangle(name, width, length, location):

#def floor_circle(name, radius, location):

#def floor_polygon(name, points):

# potential rooms:
# reactor
# laboratory
# office
# hangar
# control room

def main():

    import math
    print()
    # TODO
    dire = 'C:\\Users\\doort\\Documents\\prog\\game\\WormholeAftermath\\Blender\\scenes\\'

    sine = math.sin(math.pi / 3.0)
    scal = 10.0
    offs = 0.25
    # TODO: try 2.0
    wallSpac = 2.5

    # delete default cube
    #Sele("Cube")
    #Dele()

    # get the important offset points for spacing triangles appropriately
    righEdge, leftEdge, bottEdge, bottRigh, top_Midd, top_Righ, righOffs, leftOffs, bottOffs, upri, uple = TriaPoinGet_(sine, scal, offs, wallSpac)

    # lobby
    lobby_path(scal, offs)

    # boiler room
    # get the center of the boiler room relative to the origin
    # TODO: reorder parameters
    cent, nextBran = boiler_center(top_Righ, upri, wallSpac, righEdge, bottEdge, bottOffs, leftEdge, uple, scal)
    boiler_room("path.boiler", cent, nextBran, scal, offs)
    boiler_room_fill(cent)

###############################

def lobby_path(scal, offs):
    add_triangle_beveled_scaled(scal, offs)
    RotaSet_((0.0, 0.0, 180.0))
    Name("path.lobby")

def add_triangle():
    import math
    vertList = []
    vertList.append((math.sin(math.pi / 3.0), -0.5, 0.0))
    vertList.append((0.0, 1.0, 0.0))
    vertList.append((-math.sin(math.pi / 3.0), -0.5, 0.0))
    edgeList = []
    edgeList.append((0, 1))
    edgeList.append((1, 2))
    edgeList.append((2, 0))
    polyList = [[0, 1, 2]]
    Uplo([vertList, edgeList, polyList])

def add_triangle_beveled_scaled(scal, offs):
    add_triangle()
    VertBeve(offs = offs)
    Scal((scal, scal, scal))

def boiler_center(top_Righ, upri, wallSpac, righEdge, bottEdge, bottOffs, leftEdge, uple, scal):
    # vector from origin of an inverted triangle to another triangle center 30 degrees with an offset between
    nextLoca = VectAdd_(VectScal(top_Righ, 2.0), VectScal(VectNorm(upri), wallSpac))
    # next two lines are distance from non-inverted to inverted with an offset between
    nextBran = VectAdd_(nextLoca, VectScal(righEdge, 2.0))
    nextBran = VectAdd_(nextBran, VectScal(VectNorm(upri), wallSpac))
    # distance to top of triangle edge
    cent = VectAdd_(nextBran, VectScal(bottEdge, -1.0))
    # vertical midpoint
    cent = VectAdd_(cent, VectScal(bottOffs, -0.5))
    # path to left vertical midpoint at 150 degrees
    nextLoca = VectAdd_(nextLoca, VectScal(leftEdge, 2.0))
    nextLoca = VectAdd_(nextLoca, VectScal(VectNorm(uple), wallSpac))
    nextLoca = VectAdd_(nextLoca, VectScal(bottEdge, -1.0))
    nextLoca = VectAdd_(nextLoca, VectScal(bottOffs, -0.5))
    # center
    cent = VectAver([cent, nextLoca])
    # keep nextBran so that a vector from center to nextBran can be rotated to get other centers
    return cent, nextBran

def boiler_room(name, cent, firs, scal, offs):
    vect = Vect(cent, firs)
    boilIden = 0
    add_triangle_beveled_scaled(scal, offs)
    RotaSet_((0.0, 0.0, 180.0))
    Loca(firs)
    Name(name)
    boilIden += 1
    add_triangle_beveled_scaled(scal, offs)
    # TODO
    loca = VectRota2d__(vect, 60.0, 0)
    loca = (loca[0][0], loca[1][0], 0.0)
    #LineTo__File([StriTo__(loca)], dire + "prin.txt")
    Loca(VectAdd_(loca, cent))
    Join(name)
    Name(name)
    for a in range(2):
        add_triangle_beveled_scaled(scal, offs)
        RotaSet_((0.0, 0.0, 180.0))
        loca = VectRota2d__(loca, 60.0, 0)
        loca = (loca[0][0], loca[1][0], 0.0)
        Loca(VectAdd_(loca, cent))
        Join(name)
        Name(name)
        add_triangle_beveled_scaled(scal, offs)
        loca = VectRota2d__(loca, 60.0, 0)
        loca = (loca[0][0], loca[1][0], 0.0)
        Loca(VectAdd_(loca, cent))
        Join(name)
        Name(name)
    Orig(cent)
    ApplScal()

def boiler_room_fill(cent):
    Sele("path.boiler")
    vertList = VertList()
    edgeList = EdgeList()
    leng = len(edgeList)
    polyList = PolyList()
    magnList = []
    for a in range(len(vertList)): magnList.append([VectMagn(vertList[a]), a])
    magnList = sorted(magnList)
    magnInsi = magnList[0][0]
    magnMidd = magnList[18][0]
    insiList = []
    middList = []
    for a in range(len(magnList)):
        if Clos(magnList[a][0], magnInsi, tole = 0.1): insiList.append(magnList[a][1])
        if Clos(magnList[a][0], magnMidd, tole = 0.1): middList.append(magnList[a][1])
    for a in range(len(insiList)):
        # get two closest vertices, then get the one thats not connected
        magnList = []
        for b in range(len(insiList)):
            if a != b: magnList.append([VectMagn(Vect(vertList[insiList[a]], vertList[insiList[b]])), insiList[b]])
        magnList = sorted(magnList)
        ver0 = magnList[0][1]
        if ((insiList[a], ver0) in edgeList) == True or ((ver0, insiList[a]) in edgeList) == True: ver0 = magnList[1][1]
        if ((insiList[a], ver0) in edgeList) == False and ((ver0, insiList[a]) in edgeList) == False: edgeList.append((insiList[a], ver0))
    for a in range(len(middList)):
        # get the closest vertex
        magnList = []
        for b in range(len(middList)):
            if a != b: magnList.append([VectMagn(Vect(vertList[middList[a]], vertList[middList[b]])), middList[b]])
        magnList = sorted(magnList)
        ver0 = magnList[0][1]
        edgeList.append((middList[a], ver0))
    # add new polygons
    for a in range(leng, len(edgeList) - 1):
        for b in range(a + 1, len(edgeList)):
            ver0 = edgeList[a][0]
            ver1 = edgeList[a][1]
            ver2 = edgeList[b][0]
            ver3 = edgeList[b][1]
            ver_List = [ver0, ver1, ver2, ver3]
            # TODO: are the same edges getting picked
            con0 = False
            con1 = False
            if ((ver0, ver2) in edgeList) or ((ver2, ver0) in edgeList) or ((ver0, ver3) in edgeList) or ((ver3, ver0) in edgeList): con0 = True
            if ((ver1, ver2) in edgeList) or ((ver2, ver1) in edgeList) or ((ver1, ver3) in edgeList) or ((ver3, ver1) in edgeList): con1 = True
            if con0 and con1:
                # TODO: make a sort poly function
                vectList = []
                for c in range(len(ver_List)): vectList.append(vertList[ver_List[c]])
                cen_ = VectAver(vectList)
                for c in range(len(ver_List)): vectList[c] = Vect(cen_, vertList[ver_List[c]])
                # TODO: use signed angle
                axis = VectCros3d__(VectNorm(vectList[1]), VectNorm(vectList[0]))
                axis = Quat3d__(vectList[0], 90.0, VectNorm(axis))
                for c in range(1, len(vectList)):
                    angl = VectAngl(vectList[0], vectList[c])
                    if type(angl) != float:
                        print(angl)
                        quit()
                    dot_ = VectDot_(vectList[c], axis)
                    if dot_ < 0.0: angl = -1.0 * angl
                    vectList[c] = [angl, ver_List[c]]
                vectList[0] = [0.0, ver_List[0]]
                if dot_ < 0.0: angl *= -1.0
                vectList = sorted(vectList)
                poly = []
                for c in range(len(vectList)): poly.append(vectList[c][1])
                polyList.append(poly)
                # TODO: are there extra polygons? check for duplicates, and different orders
                #if (poly in polyList): LineTo__File(["duplicate"], dire + "prin.txt")
                break
    Dele()
    Uplo([vertList, edgeList, polyList])
    Loca(cent)

# get the important offset points for spacing triangles appropriately
def TriaPoinGet_(sine, scal, offs, wallSpac):
    # get the three midpoints of an equilateral triangle
    righEdge = TriaRighEdgeCent(sine, scal)
    leftEdge = TriaRighEdgeCent(sine, scal)
    leftEdge = (-1.0 * leftEdge[0], leftEdge[1], leftEdge[2])
    bottEdge = (0.0, -1.0 * scal / 2.0, 0.0)
    # get the three midpoints of each beveled edge of an equilateral triangle
    bottRigh = TriaBeveBottRighMid_(sine, offs, scal)
    top_Midd = TriaBeveTop_Mid_(sine, offs, scal)
    top_Righ = TriaBeveBottRighMid_(sine, offs, scal)
    top_Righ = (top_Righ[0], -1.0 * top_Righ[1], top_Righ[2])
    # get the space between walls for each direction
    righOffs = VectScal(VectNorm(righEdge), wallSpac)
    leftOffs = VectScal(VectNorm(leftEdge), wallSpac)
    bottOffs = VectScal(VectNorm(bottEdge), wallSpac)
    upri = (sine, 0.5, 0.0)
    uple = (-1.0 * sine, 0.5, 0.0)
    # TODO: down?
    return righEdge, leftEdge, bottEdge, bottRigh, top_Midd, top_Righ, righOffs, leftOffs, bottOffs, upri, uple

#def TriaBeveBottRighMid_(sine, offs, scal, loca):
def TriaBeveBottRighMid_(sine, offs, scal):
    # first point
    vectRighEdge = (sine, -1.5, 0.0)
    vectRighEdge = VectScal(vectRighEdge, scal)
    triaPoinTop_ = (0.0, scal, 0.0)
    vectRighEdgeCorn = VectAdd_(vectRighEdge, triaPoinTop_)
    vectRighEdgeNorm = VectNorm(vectRighEdge)
    offsVect = VectScal(vectRighEdgeNorm, -1.0 * offs)
    vectRighEdgeInte = VectAdd_(vectRighEdgeCorn, VectScal(offsVect, scal))
    # second point
    vectBottEdge = (sine, 0.0, 0.0)
    vectBottEdge = VectScal(vectBottEdge, scal)
    # use this if the triangle corner hasn't been found
    #triaPoinBott = (0.0, -5.0, 0.0)
    #vectBottEdgeCorn = VectAdd_(vectBottEdge, triaPoinBott)
    vectBottEdgeNorm = VectNorm(vectBottEdge)
    offsVect = VectScal(vectBottEdgeNorm, -1.0 * offs)
    # use this if the triangle corner hasn't been found
    #vectBottEdgeInte = VectAdd_(vectBottEdgeCorn, VectScal(offsVect, scal))
    vectBottEdgeInte = VectAdd_(vectRighEdgeCorn, VectScal(offsVect, scal))
    # average two points    
    retu = VectAver([vectRighEdgeInte, vectBottEdgeInte])
    #retu = VectAdd_(retu, loca)
    return retu

def TriaBeveTop_Mid_(sine, offs, scal):
    triaPoinTop_ = (0.0, 1.0, 0.0)
    triaPoinTop_ = VectScal(triaPoinTop_, scal)
    triaPoinTop_Inte = VectAdd_(triaPoinTop_, VectScal(VectNorm((sine, -1.5, 0.0)), scal * offs))
    triaPoinTop_Inte = (0.0, triaPoinTop_Inte[1], 0.0)
    return triaPoinTop_Inte

def TriaRighEdgeCent(sine, scal):
    vectRighEdge = (sine, -1.5, 0.0)
    vectRighEdge = VectScal(vectRighEdge, scal)
    triaPoinTop_ = (0.0, scal, 0.0)
    vectRighEdgeCorn = VectAdd_(vectRighEdge, triaPoinTop_)
    retu = VectAver([vectRighEdgeCorn, triaPoinTop_])
    return retu

##################################

def LineTo__File(line, filePath, mode = "w"):
    fileObje = open(filePath, mode = mode)
    writ = ""
    a = 0
    while a < len(line):
        writ += line[a] + "\n"
        a += 1
    fileObje.write(writ)
    fileObje.close()

def StriTo__(vari):
    retu = ""
    if type(vari) == list or type(vari) == tuple:
        retu = "("
        for a in range(len(vari)):
            retu += str(vari[a])
            if a < len(vari) - 1: retu += ", "
        retu += ")"
    else:
        retu = str(vari)
    return retu

####################################

def Curs(loca):
    import bpy
    bpy.context.scene.cursor.location = loca

def OrigCurs():
    import bpy
    bpy.ops.object.origin_set(type = 'ORIGIN_CURSOR')

def OrigGeom():
    import bpy
    bpy.ops.object.origin_set(type = 'ORIGIN_GEOMETRY')

def Orig(loca):
    Curs(loca)
    OrigCurs()

def Objects():
    import bpy
    return bpy.context.scene.objects

def Sele(name, dese = True):
    import bpy
    scen = bpy.context.scene
    retu = None
    if name in Objects():
        retu = scen.objects[name]
        if dese == True:
            bpy.ops.object.select_all(action = 'DESELECT')
            bpy.context.view_layer.objects.active = scen.objects[name]
            bpy.data.objects[name].select_set(True)
    else:
        print(name, "object not found")
    return retu

def Dele():
    import bpy
    bpy.ops.object.delete()

def Name(name):
    import bpy
    bpy.context.object.name = name

def Join(name):
    import bpy
    bpy.data.objects[name].select_set(True)
    bpy.ops.object.join()

#####################################

def Loca(loca, worl = True):
    import bpy
    if type(loca) == str:
        if worl:
            cont = bpy.context.object.name
            Sele(loca)
        loca = LocaRead(worl = worl)
        if worl: Sele(cont)
    bpy.context.object.location = loca

def LocaRead(worl = False):
    import bpy
    if worl == False:
        retu = tuple(bpy.context.object.location)
    else:
        # TODO: scale
        name = bpy.context.object.name
        loca = tuple(bpy.context.object.location)
        pare = bpy.context.object.parent
        while pare != None:
            Sele(pare.name)
            rota = RotaRead()
            rota = VectDegr(rota)
            pareLoca = tuple(pare.location)
            vect = Vect(pareLoca, loca)
            vect = VectRota3d__(vect, rota)
            loca = VectAdd_(vect, pareLoca)
            pare = bpy.context.object.parent
        retu = loca
        Sele(name)
    return retu

def RotaRead():
    import bpy
    retu = tuple(bpy.context.object.rotation_euler)
    return VectDegr(retu)

def Scal(scal):
    import bpy
    value = (scal[0], scal[1], scal[2])
    constraint_axis = (scal[0] != 1.0, scal[1] != 1.0, scal[2] != 1.0)
    bpy.ops.transform.resize(value = value, constraint_axis = constraint_axis, orient_type = 'GLOBAL')

def RotaSet_(rota):
    import bpy
    import math
    bpy.context.object.rotation_euler = (rota[0] * math.pi / 180.0, rota[1] * math.pi / 180.0, rota[2] * math.pi / 180.0)

def ApplScal():
    import bpy
    bpy.ops.object.transform_apply(location = False, rotation = False, scale = True)

#####################################

def Edit():
    import bpy
    bpy.ops.object.editmode_toggle()

def VertBeve(offs = 0.5):
    import bpy
    Edit()
    bpy.ops.mesh.bevel(offset = offs, offset_pct=0, affect='VERTICES')
    Edit()

#####################################

def VertList():
    import bpy
    retu = []
    a = 0
    while a < len(bpy.context.object.data.vertices):
        vert = bpy.context.object.data.vertices[a]
        retu.append((vert.co.x, vert.co.y, vert.co.z))
        a += 1
    return retu

def EdgeList():
    import bpy
    retu = []
    a = 0
    while a < len(bpy.context.object.data.edges):
        edge = bpy.context.object.data.edges[a]
        retu.append((edge.key[0], edge.key[1]))
        a += 1
    return retu

def PolyList():
    import bpy
    retu = []
    a = 0
    while a < len(bpy.context.object.data.polygons):
        retu.append(PolyVert(a))
        a += 1
    return retu

def PolyVert(inde):
    import bpy
    retu = []
    poly = bpy.context.object.data.polygons[inde]
    a = 0
    while a < len(poly.vertices):
        vert = poly.vertices[a]
        retu.append(vert)
        a += 1
    return retu

def Uplo(objeGeom, name = "obje"):
    import bpy
    bpy.ops.object.add(location = (0.0, 0.0, 0.0), type = 'MESH')
    bpy.context.object.name = name
    bpy.context.object.data.from_pydata(objeGeom[0], objeGeom[1], objeGeom[2])
    bpy.context.object.data.validate()

######################################

def Clos(clos, valu, tole = 0.00001):
    import math
    retu = False
    if math.fabs(clos - valu) <= tole: retu = True
    return retu

######################################

def VectDot_(vec0, vec1):
    retu = 0.0
    a = 0
    while a < len(vec0):
        retu += vec0[a] * vec1[a]
        a += 1
    return retu

def VectMagn(vect):
    return VectDot_(vect, vect) ** 0.5

def Vect(vec0, vec1):
    retu = []
    a = 0
    while a < len(vec0):
        retu.append(vec1[a] - vec0[a])
        a += 1
    return tuple(retu)

def VectAdd_(vec0, vec1):
    retu = []
    a = 0
    while a < len(vec0):
        retu.append(vec0[a] + vec1[a])
        a += 1
    return tuple(retu)

def VectScal(vect, scal):
    retu = []
    a = 0
    while a < len(vect):
        retu.append(vect[a] * scal)
        a += 1
    return tuple(retu)

def VectNorm(vect):
    import math
    tole = 0.0001
    magn = VectMagn(vect)
    retu = []
    a = 0
    while a < len(vect):
        if math.fabs(magn) >= tole:
            retu.append(vect[a] / magn)
        else:
            retu.append(0.0)
        a += 1
    return tuple(retu)

def VectAngl(vec0, vec1, tole = 0.0001, prin = False):
    import math
    retu = 0.0
    deno = VectMagn(vec0) * VectMagn(vec1)
    if math.fabs(deno) >= tole:
        argu = VectDot_(vec0, vec1) / deno
        if math.fabs(argu) <= 1.0 + tole:
            if argu > 1.0: argu = 1.0
            if argu < -1.0: argu = -1.0
            retu = math.degrees(math.acos(argu))
        else:
            if prin: print("math domain error", argu)
    else:
        if prin: print("approaching divide by 0", deno)
    return retu

def VectCros3d__(vec0, vec1):
    a = vec0[1] * vec1[2] - vec0[2] * vec1[1]
    b = vec0[0] * vec1[2] - vec0[2] * vec1[0]
    c = vec0[0] * vec1[1] - vec0[1] * vec1[0]
    return (a, -b, c)

#######################################

# decompose matrix. copy rows to columns
def MatrDeco(matr):
    retu = []
    leng = -1
    if len(matr) > 0:
        leng = len(matr[0])
    for a in range(leng):
        row_ = []
        for b in range(len(matr)):
            row_.append(matr[b][a])
        retu.append(row_)
    return retu

# multiply two matrices
def Matr(mat1 = [], mat2 = [], deco = True):
    retu = []
    leng = -1
    if len(mat1) > 0:
        leng = len(mat1[0])
    # check if the column count of the first matrix matches the row count of the second matrix
    if len(mat2) == leng:
        if deco == True:
            mat2 = MatrDeco(mat2)
        for a in range(len(mat1)):
            row_ = []
            for b in range(len(mat2)):
                vec1 = mat1[a]
                vec2 = mat2[b]
                row_.append(VectDot_(vec1, vec2))
            retu.append(row_)
    else:
        print("matrix sizes do not match")
    return retu

# rotate a two-dimensional vector
# TODO: returns an improperly formatted matrix
def VectRota2d__(vect, rota, axis):
    import math
    if axis == None:
        axis = 0
    mat1 = []
    if axis == 0 or axis == 2:
        mat1.append([math.cos(math.radians(rota)), -math.sin(math.radians(rota))])
        mat1.append([math.sin(math.radians(rota)), math.cos(math.radians(rota))])
    if axis == 1:
        mat1.append([math.cos(math.radians(rota)), math.sin(math.radians(rota))])
        mat1.append([-math.sin(math.radians(rota)), math.cos(math.radians(rota))])
    mat2 = []
    mat2.append([vect[0]])
    mat2.append([vect[1]])
    retu = Matr(mat1, mat2)
    return retu

# rotate a three-dimensional vector
def VectRota3d__(vect, eule):
    vecx = VectRota2d__((vect[1], vect[2]), eule[0], 0) # y z
    vecy = VectRota2d__((vect[0], vecx[1][0]), eule[1], 1) # x z
    vecz = VectRota2d__((vecy[0][0], vecx[0][0]), eule[2], 2) # x y
    return (vecz[0][0], vecz[1][0], vecy[1][0])

# rotate a 3d point by a certain angle using a normalized axis
def Quat3d__(poin, angl, axis):
    import math
    p = poin[0]
    q = poin[1]
    r = poin[2]
    t = math.radians(angl)
    t /= 2.0
    a = math.cos(t)
    b = math.sin(t)
    x = axis[0]
    y = axis[1]
    z = axis[2]
    i = r*(2*b*b*x*z+2*a*b*y)+b*b*p*x*x+2*b*b*q*x*y-b*b*p*y*y-b*b*p*z*z-2*a*b*q*z+a*a*p
    j = r*(2*b*b*y*z-2*a*b*x)-b*b*q*x*x+2*b*b*p*x*y+b*b*q*y*y-b*b*q*z*z+2*a*b*p*z+a*a*q
    k = r*(-b*b*x*x-b*b*y*y+b*b*z*z+a*a)+x*(2*b*b*p*z+2*a*b*q)+y*(2*b*b*q*z-2*a*b*p)
    return (i, j, k)

#######################################

def VectDegr(vect):
    import math
    retu = []
    a = 0
    while a < len(vect):
        retu.append(math.degrees(vect[a]))
        a += 1
    return tuple(retu)

#######################################

def VectAver(vectList):
    retu = None
    if len(vectList) > 0:
        retu = list(vectList[0])
        for a in range(len(retu)):
            retu[a] = 0.0
    a = 0
    for vect in vectList:
        for b in range(len(vect)):
            retu[b] += vect[b]
        a += 1
    if a != 0:
        retu = VectScal(retu, 1.0 / float(a))
        retu = tuple(retu)
    return retu

def VectCent(vectList):
    vecx = []
    vecy = []
    vecz = []
    for a in range(len(vectList)):
        vecx.append(vectList[a][0])
        vecy.append(vectList[a][1])
        vecz.append(vectList[a][2])
    vecx = sorted(vecx)
    vecy = sorted(vecy)
    vecz = sorted(vecz)
    minx = vecx[0]
    maxx = vecx[len(vecx) - 1]
    miny = vecy[0]
    maxy = vecy[len(vecy) - 1]
    minz = vecz[0]
    maxz = vecz[len(vecz) - 1]
    return (minx + (maxx - minx) / 2.0, miny + (maxy - miny) / 2.0, minz + (maxz - minz) / 2.0)

main()