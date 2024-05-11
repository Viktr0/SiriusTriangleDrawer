# Sirius Triangles

<p align="center">
  <img src="https://github.com/Viktr0/SiriusTriangleDrawer/assets/47856193/f02f4326-aa94-4e6c-86ca-b1453eb03c7c" alt="animated" width="200%" />
</p>

## Task description

A Szíriusz csillagképből érkező földönkívülieknek megtetszett a Word, Powerpoint, stb. 2D-s rajzoló programja, de azt közvetlenül nem használhatják, ugyanis ők más geometriával dolgoznak. Önt bízták meg a program adaptálásával arra az egyszerű esetre, amikor három pontra egy háromszöget kell illeszteni. Szerencsére adtak egy szótárat, ami a szíriuszi geometriai fogalmakat megfelelteti az általunk használt euklideszi geometriai fogalmaknak: 
* A Szíriusz sík az euklideszi sík egység sugarú köre, amit alapkörnek neveznek.
* Miközben az euklideszi síkot a komplex számokkal lehet számszerűsíteni, a Szíriusz síkot az egységnél kisebb abszolút értékű komplex számokkal. Amíg az euklideszi sík metrikája |dz|=sqrt(dx^2+dy^2) a Szíriusz síké |dz|/(1-|z|^2). 
* A Szíriusz egyenes egy olyan euklideszi körív, amely az alapkörre merőlegesen érkezik.

A feladat három egér klikk után a három pont által definiált háromszöget a háttértől eltérő színnel kitölteni, az éleket ezektől eltérő színnel felrajzolni, és a szabványos kimenetre a háromszög szögeit és oldalainak hosszát kiírni.

### Section

Kényszerek, amiből c és r számítható:
* p1 a körön:|p1-c|= r
* p2 a körön: |p2-c|= r
* Pitagórasz: r^2+1=c^2
* A kör középpontja a p1,p2 szakaszfelező merőlegesén:
* (c-(p1+p2)/2)*(p1-p2) = 0
* Az alapkört merőlegesen metsző kör alapkörre vett inverzív képe önmaga (bizonyítás?)
* pi inverze: pi*= pi / (pi * pi)

![image](https://github.com/Viktr0/SiriusTriangleDrawer/assets/47856193/c37a3abe-d51a-4f3a-a833-41f552dcca49)

* A két végpont polárszögének meghatározása (atan2), majd azon különbség számítása, amely 180 foknál nem nagyobb. A körív vektorizációja a kör parametrikus egyenletéből.
* A két végpont által definiált euklideszi szakasz parametrikus egyenlete (a két végpont konvex kombinációja), a belső pontok előállítása és körre vetítése.

### Sirius triangle

* = körívekkel határolt euklideszi tartomány 
* ~ konkáv poligon 
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
