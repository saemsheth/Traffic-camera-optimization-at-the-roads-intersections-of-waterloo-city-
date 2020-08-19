import sys
import re

S = {}
Vert_dict={}
Verts = []
IN =[]
line_with_intersect=[]
points=[]
sorted_points=[]
E = []
SE=[]
count = 0

def main():
    while True:
        i1 = sys.stdin.readline()
        i1 = i1.replace('\n','')
        if i1 == '':
            break
        # print('read a line:', i1)
        c = i1[0]
        if (c == 'a' or c == 'r' or c == 'c'):
            if i1.count('"') != 2:
                print('Error: Invalid input')
                continue
            x = i1.split('"')
            if (len(x[1])<1):
                print("Error: Invalid Street name")
                continue
            flag = 0
            for i in range(len(x[1])):
                if (x[1][i].isalpha()) | (x[1][i].isspace()) == False:
                    print("Error: Invalid Street name")
                    flag = 1
                    break
            if flag == 1:
                continue
            cor = re.compile('[@"!#$%^&*<>?/\|}{~:.;]')
            if cor.search(x[1]) != None:
                print("Error: Invalid Street name")
                continue

            S_name = x[1].upper()
            # print(S_name)

            if c == 'a':

                if x[2].count("(") != x[2].count(")"):
                    print("Error: Invalid Street Coordinates")
                    continue

                if x[2].count("(") != x[2].count(","):
                    print("Error: Invalid Street Coordinates")
                    continue


                cor1 = re.compile('[a-zA-z@"!#$%^&*<>?/\|}{~:.;]')
                if cor1.search(x[2]) != None:
                    print("Error: Invalid Street coordinates")
                    continue

                if S_name not in S:
                    add_S(S_name,x[2])
                else:
                    print("Error: This Street already exist")
                    continue

            if c == 'r':
                if S_name in S:
                    del S[S_name]
                else:
                    print("Error: This street does not exist")
                    continue

            if c == 'c':
                if S_name in S:
                    del S[S_name]
                    add_S(S_name,x[2])
                else:
                    print("Error: This Street does not exist")
                    continue

        elif c == 'g':
            i1 = i1.rstrip()
            if (len(i1)) > 1:
                print("Error: Invalid Command")
                continue
            else:
                Vert_dict.clear()
                del Verts[:]
                del E[:]
                Create_Verts()
                Inter()
                for i in range(0, len(Verts)):
                    Vert_dict.update({i + 1: Verts[i]})
                # print("V:", Vert_dict)
                # sys.stdout.write("Number of vertices are %d\n" % len(Verts))
                sys.stdout.write("V = {")
                sys.stdout.write("\n")
                for k1, value in Vert_dict.items():
                    sys.stdout.write("%d: " % k1)
                    sys.stdout.write("(")
                    sys.stdout.write("%.2f" % value[0])
                    sys.stdout.write(",%.2f" % value[1])
                    sys.stdout.write(")")
                    sys.stdout.write("\n")
                sys.stdout.write("}")
                sys.stdout.write("\n")

                sys.stdout.write ("E = {  \n")
                for i in range(0, len(E)):
                    for k1, value in Vert_dict.items()	:
                        if E[i][0] == value:
                            for k2, value in Vert_dict.items():
                                if E[i][1] == value:
                                    if i == len(E) - 1:
                                        sys.stdout.write ("<%d" %k1)
                                        sys.stdout.write (",%d" %k2)
                                        if(i != len(E)-1):
                                            sys.stdout.write( ">,")
                                            sys.stdout.write("\n")
                                        else:
                                            sys.stdout.write( ">")
                                            sys.stdout.write("\n")

                                    else:
                                        sys.stdout.write("<%d" % k1)
                                        sys.stdout.write(",%d" % k2)
                                        if (i != len(E) - 1):
                                            sys.stdout.write(">,")
                                            sys.stdout.write("\n")
                                        else:
                                            sys.stdout.write( ">")
                                            sys.stdout.write("\n")


                sys.stdout.write("}\n")
                sys.stdout.flush()
        else:
            print("Error: Invalid input")
            continue
    # print("Finished reading input")
    sys.exit(0)


def Find_Inte(m, s):


    p1 = S[s]["Sections"][m][0]
    p2 = S[s]["Sections"][m][1]
    x1, y1 = p1[0], p1[1]
    x2, y2 = p2[0], p2[1]

    for st in S:
        for line_2 in range(0, len(S[st]["Sections"])):
            if s != st:
                p3 = S[st]["Sections"][line_2][0]
                p4 = S[st]["Sections"][line_2][1]
                x3, y3 = p3[0], p3[1]
                x4, y4 = p4[0], p4[1]

                xnum = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4))
                xden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
                ynum = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)
                yden = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)

                if xnum == 0 and ynum == 0 and xden == 0 and yden == 0:
                    if (x1, y1) not in Verts:    Verts.append((x1, y1))
                    if (x1, y1) not in SE:    SE.append((x1, y1))
                    if (x2, y2) not in Verts:    Verts.append((x2, y2))
                    if (x2, y2) not in SE:    SE.append((x2, y2))
                    if (x3, y3) not in Verts:    Verts.append((x3, y3))
                    if (x3, y3) not in SE:    SE.append((x3, y3))
                    if (x4, y4) not in Verts:    Verts.append((x4, y4))
                    if (x4, y4) not in SE:    SE.append((x4, y4))
                    sorted_sedge = sorted(SE, key=lambda k: [k[0], k[1]])
                    if (sorted_sedge[0], sorted_sedge[1]) not in E:
                        E.append((sorted_sedge[0], sorted_sedge[1]))
                    # if (sorted_sedge[1], sorted_sedge[2]) not in E:
                    #     E.append((sorted_sedge[1], sorted_sedge[2]))
                    del SE[:]
                if xden == 0 or yden == 0:
                    continue
                elif (max(x1, x2) < min(x3, x4)):
                    continue
                elif (max(y1, y2) < min(y3, y4)):
                    continue
                else:
                    xc = xnum / xden
                    yc = ynum / yden
                    if (xc < max(min(x1, x2), min(x3, x4))) or (xc > min(max(x1, x2), max(x3, x4))):
                        continue
                    elif (yc < max(min(y1, y2), min(y3, y4))) or (yc > min(max(y1, y2), max(y3, y4))):
                        continue
                    else:
                        if (xc, yc) not in Verts: Verts.append((xc, yc))
                        if (xc, yc) not in IN: IN.append((xc, yc))
                        if (x1, y1) not in Verts:    Verts.append((x1, y1))
                        if (x2, y2) not in Verts:    Verts.append((x2, y2))
                        if (x3, y3) not in Verts:    Verts.append((x3, y3))
                        if (x4, y4) not in Verts:    Verts.append((x4, y4))
                        if ((x1, y1), (x2, y2)) not in line_with_intersect: line_with_intersect.append(((x1, y1), (x2, y2)))
                        if ((x3, y3), (x4, y4)) not in line_with_intersect: line_with_intersect.append(((x3, y3), (x4, y4)))

                        if ((x1, y1), (xc, yc)) not in E: E.append(((x1, y1), (xc, yc)))
                        if ((x2, y2), (xc, yc)) not in E: E.append(((x2, y2), (xc, yc)))
                        if ((x3, y3), (xc, yc)) not in E: E.append(((x3, y3), (xc, yc)))
                        if ((x4, y4), (xc, yc)) not in E:
                            E.append(((x4, y4), (xc, yc)))
                        else:
                            continue
            else:
                continue
    return

def Create_Verts():
    for st in S:
        for line in range(0,len(S[st]["Sections"])):
            Find_Inte(line,st)
    return

def add_S(Sname,C):
    S[Sname] = {}
    S[Sname]["Points"] = []
    C = C.lstrip()
    C1 = re.findall(r'\(-?\d+\,-?\d+\)', C)

    for i in range (len(C1)):
        C1[i] = C1[i].replace("(", "").replace(")", "")
        x = float(C1[i].split(",")[0])
        y = float(C1[i].split(",")[1])
        xy = (x,y)
        S[Sname]["Points"].append(xy)


    for Sname in S:
        S[Sname]["Sections"] = []
        for i in range(0, len(S[Sname]["Points"]) - 1):
            S[Sname]["Sections"].append([S[Sname]["Points"][i], S[Sname]["Points"][i + 1]])


def Inter():
    for i in range(0, len(line_with_intersect)):
        del points[:]
        x1 = line_with_intersect[i][0][0]
        y1 = line_with_intersect[i][0][1]
        x2 = line_with_intersect[i][1][0]
        y2 = line_with_intersect[i][1][1]
        count = 0
        for w in range(0, len(Verts)):
            x = Verts[w][0]
            y = Verts[w][1]
            if (x2 <= x <= x1) or (x1 <= x <= x2):
                l = int((y - y1) * (x2 - x1))
                r = int((y2 - y1) * (x - x1))

                if (l == r):
                    count += 1
                    if (x1, y1) not in points: points.append((x1, y1))
                    if (x2, y2) not in points: points.append((x2, y2))
                    if (x, y) not in points: points.append((x, y))
                elif (l != r):
                    continue
            else:
                continue
        if count >= 3:
            sorted_points = sorted(points, key=lambda k: [k[0], k[1]])
            for o in range(0, len(sorted_points) - 1):
                x1 = sorted_points[o][0]
                y1 = sorted_points[o][1]
                x2 = sorted_points[o + 1][0]
                y2 = sorted_points[o + 1][1]
                if ((x1, y1), (x2, y2)) not in E:
                    if ((x2, y2), (x1, y1)) not in E:
                        E.append(((x1, y1), (x2, y2)))
            for z in range(0, len(sorted_points) - 2):
                x1 = sorted_points[z][0]
                y1 = sorted_points[z][1]
                for u in range(0, len(sorted_points) - 2 - z):
                    x2 = sorted_points[z + u + 2][0]
                    y2 = sorted_points[z + u + 2][1]
                    f = (x1, y1), (x2, y2)
                    g = (x2, y2), (x1, y1)
                    if ((x1, y1), (x2, y2)) in E:
                        E.remove(f)
                    elif ((x2, y2), (x1, y1)) in E:
                        E.remove(g)
            for (x, y) in E:
                if x == y:
                    E.remove((x, y))
    return


if __name__ == "__main__":
	main()