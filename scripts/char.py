
def MirrName(name):
    name = name.split(".")
    nameNew_ = ""
    for a in range(len(name) - 1): nameNew_ += name[a] + "."
    if name[len(name) - 1] == "l":
        nameNew_ += "r"
    elif name[len(name) - 1] == "r":
        nameNew_ += "l"
    return nameNew_

def main():

    import bpy

    char = "adam"

    Sele(char)
    objeList = [char]
    chilList = bpy.context.object.children_recursive
    for obje in chilList:
        if obje.type == 'MESH':
            objeList.append(obje.name)

    for obje in objeList:
        side = obje.split(".")
        if side[len(side) - 1] == "l":
            Sele(obje)
            # TODO: changes position
            bpy.context.object.modifiers.remove(bpy.context.object.modifiers['Mirror'])
            Dupl()
            bpy.context.object.location = (bpy.context.object.location[0], -bpy.context.object.location[1], bpy.context.object.location[2])
            Scal((1.0, -1.0, 1.0))
            Flip()
            ApplScal()
            nameNew_ = MirrName(obje)
            Name(nameNew_)
            if bpy.context.object.parent != None:
                pare = bpy.context.object.parent.name
                pare = MirrName(pare)
                Pare(pare)
        else:
            bpy.ops.object.modifier_apply(modifier="Mirror")

# select an object by NAME
def Sele(name, dese = True):
	import bpy
	scen = bpy.context.scene
	retu = None
	if name in Objects():
		retu = scen.objects[name]
		if dese == True:
			bpy.ops.object.select_all(action='DESELECT')
		# TODO: is this the right cutoff
		vers = bpy.app.version
		if vers[0] >= 3 or (vers[0] == 2 and vers[1] >= 80):
			bpy.context.view_layer.objects.active = scen.objects[name]
			bpy.data.objects[name].select_set(True)
		else:
			scen.objects.active = scen.objects[name]
			scen.objects[name].select = True
	else:
		print("warning: " + name + " object not found")
	return retu

# duplicate the selected object
def Dupl():
	import bpy
	bpy.ops.object.duplicate_move()

# scale selected object by SCAL
def Scal(scal, proportional_size = 1.0):
	import bpy
	value = (scal[0], scal[1], scal[2])
	constraint_axis = (scal[0] != 1.0, scal[1] != 1.0, scal[2] != 1.0)
	vers = bpy.app.version
	if vers[0] >= 3 or (vers[0] == 2 and vers[1] >= 80):
		bpy.ops.transform.resize(value = value, constraint_axis = constraint_axis, orient_type = 'GLOBAL')
	else:
		bpy.ops.transform.resize(value = value, constraint_axis = constraint_axis, constraint_orientation = 'GLOBAL')

# flip all normals of the selected object
def Flip():
	import bpy
	VertSeleAll_()
	Edit()
	bpy.ops.mesh.flip_normals()
	Edit()

# switch to edit mode
def Edit():
	import bpy
	bpy.ops.object.editmode_toggle()

def ApplScal():
	import bpy
	bpy.ops.object.transform_apply(location=False, rotation=False, scale=True)

# set the NAME of the selected object
def Name(name):
	import bpy
	bpy.context.object.name = name

# select child, then call, and pass PARENT NAME
def Pare(pare = ""):
	import bpy
	chil = bpy.context.object.name
	Sele(pare)
	# TODO: this should be a function
	vers = bpy.app.version
	if vers[0] >= 3 or (vers[0] == 2 and vers[1] >= 80):
		bpy.data.objects[chil].select_set(True)
	else:
		bpy.data.objects[chil].select = True
	bpy.ops.object.parent_set(type = 'OBJECT', xmirror = False, keep_transform = True)
	Sele(chil)

def Objects():
	import bpy
	return bpy.context.scene.objects

# selected all the vertices of a selected object
def VertSeleAll_():
	import bpy
	Edit()
	bpy.ops.mesh.select_all(action = 'SELECT')
	Edit()

main()

