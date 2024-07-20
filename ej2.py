import numpy as np
import glm
import lookuptable 

def marching_cubes(data, objname="marching_cubes.obj"):
    # Paso 1: Inicialización
    # Creamos listas vacías para almacenar vértices y caras
    vertices = []
    faces = []
  
    # Paso 2: División del espacio
    for j in range(yDim):
        for i in range(xDim):
            for k in range(zDim):
                # Paso 3: Procesamiento de cada cubo
                cube = [
                    data[i][j][k], data[i+1][j][k], data[i+1][j][k+1], data[i][j][k+1],
                    data[i][j+1][k], data[i+1][j+1][k], data[i+1][j+1][k+1], data[i][j+1][k+1]
                ]
                
                # Generar el índice de 8 bits
                index = sum([int(cube[i] > iso_value) << i for i in range(8)])
               
                # Paso 4: Determinación de intersecciones
                if index == 0 or index == 255:
                    continue  # No hay intersección o el cubo está completamente dentro

                # Definir las posiciones de las 12 aristas del cubo
                edge_vertices = [
                    glm.vec3(i+0.5, j, k), glm.vec3(i+1, j, k+0.5), glm.vec3(i+0.5, j, k+1), glm.vec3(i, j, k+0.5),
                    glm.vec3(i+0.5, j+1, k), glm.vec3(i+1, j+1, k+0.5), glm.vec3(i+0.5, j+1, k+1), glm.vec3(i, j+1, k+0.5),
                    glm.vec3(i, j+0.5, k), glm.vec3(i+1, j+0.5, k), glm.vec3(i+1, j+0.5, k+1), glm.vec3(i, j+0.5, k+1)
                ]
                
                # Paso 5: Cálculo de puntos de intersección con interpolación lineal
                vertex_list = {}
                for face in lookuptable.lookuptable[index]:
                    for edge in face:
                        if edge not in vertex_list:
                            v0 = cube[lookuptable.edge_to_vertices[edge][0]]
                            v1 = cube[lookuptable.edge_to_vertices[edge][1]]
                            t = (iso_value - v0) / (v1 - v0)
                            vertex = edge_vertices[edge] * (1-t) + edge_vertices[edge] * t
                            vertex_list[edge] = len(vertices)
                            vertices.append(vertex)
                
                # Paso 6: Generación de triángulos
                for face in lookuptable.lookuptable[index]:
                    faces.append(glm.vec3(vertex_list[face[0]], vertex_list[face[1]], vertex_list[face[2]]))

    # Paso 7: Cálculo de normales
    vertex_normals = [glm.vec3(0,0,0) for _ in range(len(vertices))]
    for face in faces:
        v0, v1, v2 = vertices[int(face.x)], vertices[int(face.y)], vertices[int(face.z)]
        normal = glm.normalize(glm.cross(v1 - v0, v2 - v0))
        vertex_normals[int(face.x)] += normal
        vertex_normals[int(face.y)] += normal
        vertex_normals[int(face.z)] += normal
    
    normals = [glm.normalize(n) for n in vertex_normals]

    # Paso 8: Optimización
    # Simplificación de malla (eliminación de vértices duplicados)
    unique_vertices = {}
    new_vertices = []
    new_faces = []
    for i, vertex in enumerate(vertices):
        key = tuple(round(c, 4) for c in vertex)  # Redondear para agrupar vértices cercanos
        if key not in unique_vertices:
            unique_vertices[key] = len(new_vertices)
            new_vertices.append(vertex)
        new_index = unique_vertices[key]
        for face in faces:
            if int(face.x) == i or int(face.y) == i or int(face.z) == i:
                new_face = glm.vec3(
                    unique_vertices[tuple(round(c, 4) for c in vertices[int(face.x)])],
                    unique_vertices[tuple(round(c, 4) for c in vertices[int(face.y)])],
                    unique_vertices[tuple(round(c, 4) for c in vertices[int(face.z)])]
                )
                if new_face.x != new_face.y and new_face.y != new_face.z and new_face.z != new_face.x:
                    new_faces.append(new_face)

    # Paso 9: Renderización (escritura a archivo OBJ)
    with open(objname, 'w') as of:
        for v in new_vertices:
            of.write(f'v {v.x} {v.y} {v.z}\n')
        for n in normals:
            of.write(f'vn {n.x} {n.y} {n.z}\n')
        for f in new_faces:
            of.write(f'f {int(f.x)+1}//{int(f.x)+1} {int(f.y)+1}//{int(f.y)+1} {int(f.z)+1}//{int(f.z)+1}\n')
