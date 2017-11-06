#!/usr/bin/python

import re
import itertools
import sys
import string
import copy

__author__ = 'mengdongqi'

minimal_value = 0.000001


class StreetCameraError(RuntimeError):
    def __init__(self, args):
        self.args = 'Error: ' + args

    def printError(self):
        sys.stderr.write(string.join(self.args, '') + '\n')


class StreetMap:
    streetDict = {}
    street_name_list = []
    # output graph related
    all_street_intersection_list = {}  # The list storage the intersection what we want
    node_list = []  # The list storage all the node and intersection (some node may repeat)
    street_edge_intersection_dict = {}  # The edge for intersection
    edge_index_dict = {}
    intersection_street_name_list = []  # The street name list for which have intersection
    countError=0

    def __init__(self):
        pass

    def addStreet(self, streetName, streetNode):
        "Add street. str: street info. containing street name and node list."
        if streetName in self.street_name_list:
            raise StreetCameraError("The street you add is already exist,you could change it by command 'c'")
        else:
            streetIntNode = []
            for p in streetNode:
                streetIntNode.append(((float)(p[0]), (float)(p[1])))
            self.streetDict[streetName] = streetIntNode
            self.street_name_list.append(streetName)
            # judge if we have overlapping with existing street
            self.judgeOverlapping(streetName)

    def changeStreet(self, streetName, streetNode):
        "Update street. str: street info. containing street name and node list."
        if streetName in self.street_name_list:
            pass
        else:
            raise StreetCameraError("The street you change does not exist,please check your street name")
        streetIntNode = []
        for p in streetNode:
            streetIntNode.append(((float)(p[0]), (float)(p[1])))
        self.streetDict[streetName] = streetIntNode
        # judge if we have overlapping with existing street
        self.judgeOverlapping(streetName)

    def judgeOverlapping(self, currentStreetName):
        segmentsDict = self.getStreetsSegmentsDict()
        # 1. street self-overlap
        if self.judgeSegmentListHasOverlap(segmentsDict[currentStreetName]):
            #self.countError++
            self.removeStreet(currentStreetName)
            raise StreetCameraError("The [ %s ] street has overlap segments! Removed This street!" % (currentStreetName))
        # 2. street overlap with other street
        for streetName in segmentsDict.keys():
            if streetName != currentStreetName:
                twoStreetSegmentList = segmentsDict[currentStreetName]
                for segment in segmentsDict[streetName]:
                    list.append(twoStreetSegmentList, segment)
                if self.judgeSegmentListHasOverlap(twoStreetSegmentList):
                    #self.countError++
                    self.removeStreet(currentStreetName)
                    raise StreetCameraError("The [ %s ] street has overlap segments with [ %s ]!  Removed [ %s ] street!" % (currentStreetName, streetName, currentStreetName))

    def judgeSegmentListHasOverlap(self, segmentList):
        segmentsCompare = itertools.combinations(segmentList, 2)
        for pair in segmentsCompare:
            segment1 = pair[0]
            segment2 = pair[1]
            if self.isTwoSegmentsOverlapping(segment1[0], segment1[1], segment2[0], segment2[1]):
                return True
        return False

    def getStreetsSegmentsDict(self):
        streetSegments = {}
        for streetName in self.streetDict.keys():
            streetSegments[streetName] = self.getOneStreetSegmentsList(self.streetDict[streetName])
        return streetSegments

    def getOneStreetSegmentsList(self, nodeList):
        segmentList = []
        for i in xrange(0, len(nodeList) - 1):
            segmentList.append((nodeList[i], nodeList[i + 1]))
        return segmentList

    def removeStreet(self, streetName):
        "Remove street. Str: street info, containing street name"
        if streetName in self.street_name_list:
            pass
        else:
            raise StreetCameraError("The street you remove is not exist,please check your street name")
        del self.streetDict[streetName]
        self.street_name_list.remove(streetName)

    def getAllStreetintersection_edge(self):
        street_compare = list(itertools.combinations(self.street_name_list, 2))
        for i in xrange(0, len(street_compare)):
            self.getTwoStreetIntersection(street_compare[i][0], street_compare[i][1])

    def getTwoStreetIntersection(self, streetNameA, streetNameB):
        nodeListA = self.streetDict[streetNameA]
        nodeListB = self.streetDict[streetNameB]
        for i in xrange(0, len(nodeListA) - 1):
            for j in xrange(0, len(nodeListB) - 1):
                if self.isTwoSegmentHaveIntersection(nodeListA[i][0], nodeListA[i][1], nodeListA[i + 1][0],
                                                     nodeListA[i + 1][1], nodeListB[j][0], nodeListB[j][1],
                                                     nodeListB[j + 1][0], nodeListB[j + 1][1]):
                    self.all_street_intersection_list[nodeListA[i]] = 0
                    self.all_street_intersection_list[nodeListA[i + 1]] = 0
                    self.all_street_intersection_list[nodeListB[j]] = 0
                    self.all_street_intersection_list[nodeListB[j + 1]] = 0
                    self.node_list.append(nodeListA[i])
                    self.node_list.append(nodeListA[i + 1])
                    self.node_list.append(nodeListB[j])
                    self.node_list.append(nodeListB[j + 1])
                    intersection_x, intersection_y = self.getIntersection(nodeListA[i][0], nodeListA[i][1],
                                                                          nodeListA[i + 1][0],
                                                                          nodeListA[i + 1][1], nodeListB[j][0],
                                                                          nodeListB[j][1], nodeListB[j + 1][0],
                                                                          nodeListB[j + 1][1])
                    temp_tuple = (intersection_x, intersection_y)
                    if streetNameA in self.street_edge_intersection_dict.keys():
                        isEdgeRepeat = 0
                        repeatNodeIndex = ""
                        for eachStreetNodeListIndex in xrange(0, len(self.street_edge_intersection_dict[streetNameA])):
                            eachStreetNodeList = self.street_edge_intersection_dict[streetNameA][
                                eachStreetNodeListIndex]
                            for e in xrange(0, len(eachStreetNodeList) - 1):
                                if eachStreetNodeList[e][0] == nodeListA[i][0] and eachStreetNodeList[e][1] == \
                                        nodeListA[i][1] and eachStreetNodeList[e + 1][0] == nodeListA[i + 1][0] and \
                                                eachStreetNodeList[e + 1][1] == nodeListA[i + 1][1]:
                                    isEdgeRepeat = 1
                                    repeatNodeIndex = eachStreetNodeListIndex
                                else:
                                    pass
                        if isEdgeRepeat == 1:
                            isIntersectionRepeat = 0
                            for en in xrange(0, len(self.street_edge_intersection_dict[streetNameA][repeatNodeIndex])):
                                temp_node = self.street_edge_intersection_dict[streetNameA][repeatNodeIndex][en]
                                if temp_tuple[0] == temp_node[0] and temp_tuple[1] == temp_node[1]:
                                    isIntersectionRepeat = 1
                                else:
                                    pass
                            if isIntersectionRepeat == 1:
                                pass
                            else:
                                self.street_edge_intersection_dict[streetNameA][repeatNodeIndex].append(temp_tuple)
                        else:
                            if temp_tuple == nodeListA[i]:
                                self.street_edge_intersection_dict[streetNameA].append([nodeListA[i], nodeListA[i + 1]])
                            elif temp_tuple == nodeListA[i + 1]:
                                self.street_edge_intersection_dict[streetNameA].append([nodeListA[i], nodeListA[i + 1]])
                            else:
                                self.street_edge_intersection_dict[streetNameA].append(
                                    [nodeListA[i], nodeListA[i + 1], temp_tuple])
                    else:
                        self.intersection_street_name_list.append(streetNameA)
                        self.street_edge_intersection_dict[streetNameA] = [[nodeListA[i], nodeListA[i + 1]]]
                        if self.isNodeAlreadyInNodeList(temp_tuple, self.street_edge_intersection_dict[streetNameA][0]):
                            pass
                        else:
                            self.street_edge_intersection_dict[streetNameA][0].append(temp_tuple)
                    if streetNameB in self.street_edge_intersection_dict.keys():
                        isEdgeRepeat = 0
                        repeatNodeIndex = ""
                        for eachStreetNodeListIndex in xrange(0, len(self.street_edge_intersection_dict[streetNameB])):
                            eachStreetNodeList = self.street_edge_intersection_dict[streetNameB][
                                eachStreetNodeListIndex]
                            for e in xrange(0, len(eachStreetNodeList) - 1):
                                if eachStreetNodeList[e][0] == nodeListB[j][0] and eachStreetNodeList[e][1] == \
                                        nodeListB[j][1] and eachStreetNodeList[e + 1][0] == nodeListB[j + 1][0] and \
                                                eachStreetNodeList[e + 1][1] == nodeListB[j + 1][1]:
                                    isEdgeRepeat = 1
                                    repeatNodeIndex = eachStreetNodeListIndex
                                else:
                                    pass
                        if isEdgeRepeat == 1:
                            isIntersectionRepeat = 0
                            for en in xrange(2, len(self.street_edge_intersection_dict[streetNameB][repeatNodeIndex])):
                                temp_node = self.street_edge_intersection_dict[streetNameB][repeatNodeIndex][en]
                                if temp_tuple[0] == temp_node[0] and temp_tuple[1] == temp_node[1]:
                                    isIntersectionRepeat = 1
                                else:
                                    pass
                            if isIntersectionRepeat == 1:
                                pass
                            else:
                                self.street_edge_intersection_dict[streetNameB][repeatNodeIndex].append(temp_tuple)
                        else:
                            if temp_tuple == nodeListB[j]:
                                self.street_edge_intersection_dict[streetNameB].append([nodeListB[j], nodeListB[j + 1]])
                            elif temp_tuple == nodeListB[j + 1]:
                                self.street_edge_intersection_dict[streetNameB].append([nodeListB[j], nodeListB[j + 1]])
                            else:
                                self.street_edge_intersection_dict[streetNameB].append(
                                    [nodeListB[j], nodeListB[j + 1], temp_tuple])
                    else:
                        self.intersection_street_name_list.append(streetNameB)
                        self.street_edge_intersection_dict[streetNameB] = [[nodeListB[j], nodeListB[j + 1]]]
                        if self.isNodeAlreadyInNodeList(temp_tuple, self.street_edge_intersection_dict[streetNameB][0]):
                            pass
                        else:
                            self.street_edge_intersection_dict[streetNameB][0].append(temp_tuple)
                    self.all_street_intersection_list[temp_tuple] = 0
                    if not self.isNodeAlreadyInNodeList(temp_tuple, self.node_list):
                        self.node_list.append(temp_tuple)

    def isNodeAlreadyInNodeList(self, node, nodeList):
        for n in nodeList:
            if twoFloatsEqual(n[0], node[0]) and twoFloatsEqual(n[1], node[1]):
                return True
        return False

    def isTwoSegmentHaveIntersection(self, street1_nodeA_x, street1_nodeA_y, street1_nodeB_x, street1_nodeB_y,
                                     street2_nodeC_x,
                                     street2_nodeC_y, street2_nodeD_x, street2_nodeD_y):
        "use vector to judge if two segment have intersection"
        vectorAB_x = street1_nodeB_x - street1_nodeA_x
        vectorAB_y = street1_nodeB_y - street1_nodeA_y
        vectorAC_x = street2_nodeC_x - street1_nodeA_x
        vectorAC_y = street2_nodeC_y - street1_nodeA_y
        vectorAD_x = street2_nodeD_x - street1_nodeA_x
        vectorAD_y = street2_nodeD_y - street1_nodeA_y
        vectorCD_x = street2_nodeD_x - street2_nodeC_x
        vectorCD_y = street2_nodeD_y - street2_nodeC_y
        vectorCA_x = street1_nodeA_x - street2_nodeC_x
        vectorCA_y = street1_nodeA_y - street2_nodeC_y
        vectorCB_x = street1_nodeB_x - street2_nodeC_x
        vectorCB_y = street1_nodeB_y - street2_nodeC_y
        resultABACABAD = (vectorAB_x * vectorAC_y - vectorAC_x * vectorAB_y) * (
            vectorAB_x * vectorAD_y - vectorAD_x * vectorAB_y)
        resultCDCACDCB = (vectorCD_x * vectorCA_y - vectorCA_x * vectorCD_y) * (
            vectorCD_x * vectorCB_y - vectorCB_x * vectorCD_y)
        if resultABACABAD <= 0:
            if resultCDCACDCB <= 0:
                return True
        else:
            return False

    def getIntersection(self, street1_nodeA_x, street1_nodeA_y, street1_nodeB_x, street1_nodeB_y, street2_nodeC_x,
                        street2_nodeC_y, street2_nodeD_x, street2_nodeD_y):
        "Get intersection vertex of two segments, return the five Nodes (intesection, street_1_A, street_1_B, street_2_C, street_2_D )"
        if street1_nodeA_x == street1_nodeB_x and street2_nodeC_x != street2_nodeD_x:
            intersection_x = street1_nodeA_x
            intersection_y = ((street2_nodeD_y - street2_nodeC_y) / (
                street2_nodeD_x - street2_nodeC_x)) * street1_nodeA_x + (
                                                                            street2_nodeC_y * street2_nodeD_x - street2_nodeC_x * street2_nodeD_y) / (
                                                                            street2_nodeD_x - street2_nodeC_x)
        elif street2_nodeC_x == street2_nodeD_x and street1_nodeA_x != street1_nodeB_x:
            intersection_x = street2_nodeC_x
            intersection_y = ((street1_nodeB_y - street1_nodeA_y) / (
                street1_nodeB_x - street1_nodeA_x)) * street2_nodeC_x + (
                                                                            street1_nodeA_y * street1_nodeB_x - street1_nodeA_x * street1_nodeB_y) / (
                                                                            street1_nodeB_x - street1_nodeA_x)
        elif street1_nodeA_x == street1_nodeB_x and street2_nodeC_x == street2_nodeD_x:
            if street1_nodeA_x == street2_nodeC_x and street1_nodeA_y == street2_nodeC_y:
                intersection_x = street1_nodeA_x
                intersection_y = street1_nodeA_y
            elif street1_nodeA_x == street2_nodeD_x and street1_nodeA_y == street2_nodeD_y:
                intersection_x = street1_nodeA_x
                intersection_y = street1_nodeA_y
            else:
                intersection_x = street1_nodeB_x
                intersection_y = street1_nodeB_y
        else:
            a1 = (street1_nodeB_y - street1_nodeA_y) / (street1_nodeB_x - street1_nodeA_x)
            c1 = (street1_nodeA_y * street1_nodeB_x - street1_nodeA_x * street1_nodeB_y) / (
                street1_nodeB_x - street1_nodeA_x)
            a2 = (street2_nodeD_y - street2_nodeC_y) / (street2_nodeD_x - street2_nodeC_x)
            c2 = (street2_nodeC_y * street2_nodeD_x - street2_nodeC_x * street2_nodeD_y) / (
                street2_nodeD_x - street2_nodeC_x)
            if a1 == a2:
                if street1_nodeA_x == street2_nodeC_x and street1_nodeA_y == street2_nodeC_y:
                    intersection_x = street1_nodeA_x
                    intersection_y = street1_nodeA_y
                elif street1_nodeA_x == street2_nodeD_x and street1_nodeA_y == street2_nodeD_y:
                    intersection_x = street1_nodeA_x
                    intersection_y = street1_nodeA_y
                else:
                    intersection_x = street1_nodeB_x
                    intersection_y = street1_nodeB_y
            else:
                intersection_x = (c2 - c1) / (a1 - a2)
                intersection_y = (a1 * c2 - a2 * c1) / (a1 - a2)

        return round(intersection_x, 2), round(intersection_y, 2)

    def storageEdgeForOneStreet(self, streetName):
        "Order nodes in every street, and the street are divided into different segment"
        street_node_list = self.street_edge_intersection_dict[streetName]
        for i in xrange(0, len(street_node_list)):
            for j in xrange(1, len(street_node_list[i])):
                for x in xrange(0, len(street_node_list[i]) - j):
                    temp = street_node_list[i][x]
                    if street_node_list[i][x][0] > street_node_list[i][x + 1][0]:
                        street_node_list[i][x] = street_node_list[i][x + 1]
                        street_node_list[i][x + 1] = temp
                    elif street_node_list[i][x][0] == street_node_list[i][x + 1][0]:
                        if street_node_list[i][x][1] > street_node_list[i][x + 1][1]:
                            street_node_list[i][x] = street_node_list[i][x + 1]
                            street_node_list[i][x + 1] = temp
                        else:
                            pass
                    else:
                        pass
        self.street_edge_intersection_dict[streetName] = street_node_list

    def generateStreetNode_edge(self):
        self.all_street_intersection_list.clear()
        self.node_list = []
        self.street_edge_intersection_dict.clear()
        self.edge_index_dict.clear()
        self.intersection_street_name_list = []
        count_repeat_node = 0
        self.getAllStreetintersection_edge()
        for i in xrange(0, len(self.node_list)):
            if self.all_street_intersection_list[self.node_list[i]] == 1:
                self.node_list[i] = " "
                count_repeat_node = count_repeat_node + 1
            else:
                self.all_street_intersection_list[self.node_list[i]] = 1
        for j in xrange(0, count_repeat_node):
            self.node_list.remove(" ")
        for streetName in self.intersection_street_name_list:
            self.storageEdgeForOneStreet(streetName)
        self.edge_index_dict = copy.deepcopy(self.street_edge_intersection_dict)
        for streetName in self.edge_index_dict:
            temp_node_list = self.edge_index_dict[streetName]
            for eachList in temp_node_list:
                for y in xrange(0, len(eachList)):
                    eachList[y] = self.node_list.index(eachList[y])
            self.edge_index_dict[streetName] = temp_node_list
        #if countError > 0 :
            #print countError
        else :
            pass
        print "V",
        """for i in xrange(0, len(self.node_list)):
            //v = self.node_list[i]
            //print "  %d:\t(%.2f,%.2f)" % (i + 1, round(v[0], 2), round(v[1], 2))
        //print "}" """
        print len(self.node_list)

        print "E {",
        for streetName in self.intersection_street_name_list:
            temp_node_list = self.edge_index_dict[streetName]
            for eachNodeList in temp_node_list:
                for z in xrange(0, len(eachNodeList) - 1):
                    if self.intersection_street_name_list.index(streetName) == len(self.intersection_street_name_list) - 1 and \
                                    list.index(temp_node_list, eachNodeList) == len(temp_node_list) - 1 and \
                                    z == len(eachNodeList) - 2:
                        sys.stdout.write('<%d,%d>' % (eachNodeList[z], eachNodeList[z + 1]))
                        sys.stdout.flush()
                    else:
                        sys.stdout.write('<%d,%d>,' % (eachNodeList[z], eachNodeList[z + 1]))
                        sys.stdout.flush()
        print "}"

    def isTwoSegmentsOverlapping(self, A, B, C, D):
        if abs((B[0] - A[0]) * (D[1] - C[1]) - (B[1] - A[1]) * (D[0] - C[0])) > minimal_value:  # different slope
            return False
        if self.threeDotsInOneLine(A, C, B):  # C is in the middle of A-B
            return True
        if self.threeDotsInOneLine(A, D, B):  # D is in the middle of A-B
            return True
        if self.threeDotsInOneLine(C, A, D):  # A is in the middle of C-D
            return True
        if self.threeDotsInOneLine(C, B, D):  # B is in the middle of C-D
            return True
        if twoFloatsEqual(A[0], C[0]) and twoFloatsEqual(A[1], C[1]) \
                and twoFloatsEqual(B[0], D[0]) and twoFloatsEqual(B[1], D[1]):
            return True
        if twoFloatsEqual(A[0], D[0]) and twoFloatsEqual(A[1], D[1]) \
                and twoFloatsEqual(B[0], C[0]) and twoFloatsEqual(B[1], C[1]):
            return True
        return False

    def threeDotsInOneLine(self, A, B, C):
        "Judge if A-B-C dots in one line, return true or false, while B is the middle one."
        # make sure A-B-C in one line
        in_one_line = twoFloatsEqual((A[1] - B[1]) * (A[0] - C[0]), (A[1] - C[1]) * (A[0] - B[0]))

        # make sure B is in the mddile, we should say B is in the rectangle which is constructed by AC vertices.
        B_in_AC_rectangle = (min(A[0], C[0]) <= B[0] and B[0] <= max(A[0], C[0]) \
                             and min(A[1], C[1]) < B[1] and B[1] < max(A[1], C[1])) \
                            or (min(A[0], C[0]) < B[0] and B[0] < max(A[0], C[0]) \
                                and min(A[1], C[1]) <= B[1] and B[1] <= max(A[1], C[1]))
        return in_one_line and B_in_AC_rectangle


def extractCommand(string):
    "Get Command from input string.return a/c/g/r, e for errors. and Street Info string(containing street name and Node list)."
    patten1 = r'^([ac]{1}\s*)(\"[^\"]+\")(.*)'
    patten2 = r'(^g$)'
    patten3 = r'^([r]{1}\s*)(\"[^\"]+\"$)'
    obj = re.match(r'^([acrg]{1})(.*)', string)
    if obj:
        if obj.group(1) == "g":
            formatString2 = re.match(patten2, string)
            if formatString2:
                return formatString2.group(1).strip()
            else:
                raise StreetCameraError("error format, command 'g' can not followed by other things.")
        elif obj.group(1) in ["a", "c"]:
            formatString1 = re.match(patten1, string)
            if formatString1:
                return formatString1.group(1).strip(), formatString1.group(2), formatString1.group(3).strip()
            else:
                raise StreetCameraError(
                    "error format, the format of command 'c' or 'a' is : a/c \"name\" (number,number) ... (number,number)")
        elif obj.group(1) == "r":
            formatString3 = re.match(patten3, string)
            if formatString3:
                return formatString3.group(1).strip(), formatString3.group(2)
            else:
                raise StreetCameraError("error format, the format of command 'r' is : r \"name\"")
    else:
        raise StreetCameraError("error command, command should be one of a/c/g/r.")


def extractStreetName(string):
    "Extract street name from input string."
    obj = re.match(r'"([^\"]+)"', string)
    if obj:
        return obj.group(1)
    raise StreetCameraError("error extracting street name, please check street name. Name format is : \"name\"")


def extractStreetNodes(streetName, Node_list_str):
    "Extract street Node list from string"
    raw_Node_list_str = Node_list_str
    illegal_pattern = r'(\(\s*\-?\d+\s*,\s*\-?\d+\s*\)\s*[^(\s]+\s*)'  # ie. a "w1" (1,2)(3,4) a
    obj = re.search(illegal_pattern, Node_list_str)
    if obj:
        raise StreetCameraError(
            "error extracting street nodes for street [" + streetName + "], please check your street nodes ie: a/c \"name\" (1,2)...(3,4).")
    Node_list_pattern = r'((\(\s*\-?\d+\s*,\s*\-?\d+\s*\)[\s]*){2,})'
    obj = re.search(Node_list_pattern, Node_list_str)
    if obj:
        Node_list_str = obj.group(1)  # incase of (1,2),(3,4)
    else:
        raise StreetCameraError(
            "error extracting street nodes for street [" + streetName + "], every street must have two nodes.please check your street nodes. ie: a/c \"name\" (1,2)...(3,4).")
    Node_pattern = r'\(\s*(\-?\d+)\s*,\s*(\-?\d+)\s*\)'
    Node_lists = re.findall(Node_pattern, Node_list_str)
    correct_brackets = raw_Node_list_str.count('(') == raw_Node_list_str.count(')') == len(Node_lists)
    if len(Node_lists) > 0 and correct_brackets:
        return Node_lists
    raise StreetCameraError("error extracting street Nodes for street [" + streetName + "], please check Node list.ie: a/c \"name\" (1,2)...(3,4).")


def twoFloatsEqual(a, b):
    if abs(a - b) <= minimal_value:
        return True
    return False


def getKey_x(item):
    return item[0]


def getKey_y(item):
    return item[1]

######### Start of the app
if __name__ == '__main__':

    sm = StreetMap()
    while True:
        try:
            user_input = raw_input().strip()
            cmd = ""
            streetName = ""
            streetNodes = []
            temp = re.match(r'^\s*(\w?)(.*)', user_input).group(1)
            if temp:
                if temp == "a":
                    cmd, streetName, streetNodes = extractCommand(user_input)
                    streetName = extractStreetName(streetName)
                    streetNodes = extractStreetNodes(streetName, streetNodes)
                elif temp == "c":
                    cmd, streetName, streetNodes = extractCommand(user_input)
                    streetName = extractStreetName(streetName)
                    streetNodes = extractStreetNodes(streetName, streetNodes)
                elif temp == "r":
                    cmd, streetName = extractCommand(user_input)
                    streetName = extractStreetName(streetName)
                elif temp == "g":
                    cmd = extractCommand(user_input)
                if cmd == 'a':
                    sm.addStreet(streetName, streetNodes)
                elif cmd == 'r':
                    sm.removeStreet(streetName)
                elif cmd == "c":
                    sm.changeStreet(streetName, streetNodes)
                elif cmd == 'g':
                    sm.generateStreetNode_edge()
                else:
                    raise StreetCameraError("error extracting command, you could only use command a/c/g/r")
            else:
                if re.match(r'^[^\s]*', user_input):  # accept for white spaces
                    if re.match(r'^[^\s]*', user_input).group(0) == "":
                        pass
                    else:
                        raise StreetCameraError("error extracting command, you could only use command a/c/g/r")
        except StreetCameraError, args:
            args.printError()
        except EOFError, args:
            break
