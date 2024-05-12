# Sirius Triangles

<p align="center">
  <img src="https://github.com/Viktr0/SiriusTriangleDrawer/assets/47856193/f02f4326-aa94-4e6c-86ca-b1453eb03c7c" alt="animated" width="200%" />
</p>

## Task description

Aliens coming from the Sirius which is part of the constallation Canis Major are really like 2D drawing programs of the Word and PowerPoint, but they can not use it directly because their concept of geometry is different.
It is your job to adapt a program for the simple case of fitting a triangle to three points.

Fortunately, they provided a dictionary that corresponds the Sirius geometric concepts to the Euclidean geometric concepts we use:
* The Sirius plane is a circle of unit radius of the Euclidean plane, which is called the base circle.
* The Euclidean plane can be quantified with complex numbers, and the Sirius plane with complex numbers with an absolute value smaller than unity.
* While the metric of the Euclidean plane is **|dz| = sqrt(dx^2 + dy^2)**, the Sirius plane is **|dz| / (1 - |z|^2)**.
* The Sirius line is a Euclidean circular arc that comes perpendicular to the base circle.

After three mouse clicks, the task is to fill the triangle defined by the three points with a different color from the background, draw the edges with a different color, and write the angles and the lengths of the sides of the triangle to the standard output.

### Section

Constraints from which **c** and **r** can be calculated:
* **p1** on the circle: **|p1 - c| = r**
* **p2** on the circle: **|p2 - c| = r**
* Pythagoras: **r^2 + 1 = c^2**
* The center of the circle is on the perpendicular bisector **p1**, **p2**: **(c - (p1 + p2) / 2) * ( p1 - p2) = 0**
* The inverse image of a circle intersecting the base circle at right angles to the base circle is itself (proof?)
* Inverse of **pi**: **pi' = pi / (pi * pi)**

<p align="center">
  <img src="https://github.com/Viktr0/SiriusTriangleDrawer/assets/47856193/c37a3abe-d51a-4f3a-a833-41f552dcca49" width="50%" />
</p>

* Determining the polar angle of the two endpoints (atan2), then calculating the difference, which is not greater than 180 degrees. Vectorization of the circular arc from the parametric equation of the circle.
* The parametric equation of the Euclidean section defined by the two endpoints (the convex combination of the two endpoints), the production of the internal points and the projection onto a circle.

### Sirius triangle

* = an Euclidean domain bounded by circular arcs
* ~ concave polygon 
* = háromszög lista (pl. fülvágás): GL_TRIANGLES
* + él rajzolás: GL_LINE_LOOP

### Angle and distance

* Szög: a körívek találkozási pontjához a középpontokból húzott egyenesek szöge: Vigyázat a belső szögek kellenek!
* Hossz: a vektorizált körívek kis szakaszaira a végpontok ds Szíriusz-távolságait kell számítani, és azt összegezni. 
* Az (x,y) és (x+dx, y+dy) Szíriusz-távolsága: ds = sqrt(dx*dx+dy*dy)/(1–x*x–y*y)

## Installation guide

dlls next to .exe

freeglut folder with GL and lib inside of it

_CRT_SECURE_NO_WARNINGS

Project properties: Linker, C/C++, VC++ Directories

## Solution

Osztályok

Felépítés

Fileok

Működés (több szín)
