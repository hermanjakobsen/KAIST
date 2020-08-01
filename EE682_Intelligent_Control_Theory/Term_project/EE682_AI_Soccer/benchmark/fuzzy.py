
import math
import helper

# Will base the membership functions and rule base on paper https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=870287&tag=1

numRule = 49    # Number of rules for each wheel

ruleR = [None for _ in range(numRule)]
ruleL = [None for _ in range(numRule)]
outR = [None for _ in range(numRule)]
outL = [None for _ in range(numRule)]

# Membership functions for distance error
NBd = (-math.inf, 0, 0.1)
NMd = (0, 0.1, 0.2)
NSd = (0.1, 0.2, 0.3)
ZOd = (0.2, 0.3, 0.4)
PSd = (0.3, 0.4, 0.5)
PMd = (0.4, 0.5, 0.6)
PBd = (0.5, 0.6, math.inf)

# Membership functions for angle error
NBth = (-math.inf, math.radians(-120), math.radians(-80))
NMth = (math.radians(-120), math.radians(-80), math.radians(-40))
NSth = (math.radians(-80), math.radians(-40), 0)
ZOth = (math.radians(-40), 0, math.radians(40))
PSth = (0, math.radians(40), math.radians(80))
PMth = (math.radians(40), math.radians(80), math.radians(120))
PBth = (math.radians(80), math.radians(120), math.inf)


def membership(x, A):
    if (A[0] == -math.inf and x < A[1]):
        return 1
    if (A[2] == math.inf and x >= A[1]):
        return 1
    if (x >= A[0] and x < A[1]):
        a = 1 / (A[1] - A[0])
        b = -A[0] / (A[1] - A[0])
        return a * x + b
    if (x >= A[1] and x < A[2]):
        a = -1 / (A[2] - A[1])
        b = A[2] / (A[2] - A[1])
        return a * x + b
    return 0


def strength(x1, A1, x2, A2):
    return min(membership(x1, A1), membership(x2, A2))


def defuzzy(rule, out):
    tmp1 = 0
    tmp2 = 0

    for i in range(len(rule)):
        tmp1+=rule[i]*out[i]
        tmp2+=rule[i]
    if (tmp2==0):
        return 0
    return tmp1/tmp2


def FLC(xRef=None, yRef=None, thRef=None, x=None, y=None, th=None, mode=None):
    Ku = 12

    if (mode == 'position'):
        xError = xRef-x
        yError = yRef-y
        THRef = math.atan2(yError,xError)

        thError = helper.trim_radian(THRef-th)
        distError = math.sqrt(math.pow(xRef-x,2)+math.pow(yRef-y, 2))

        # Stop giving robot input if it's pretty close to the target
        if (abs(distError) <= 0.05):
            return 0, 0, thError, distError

    if (mode == 'angle'):
        distError = 0
        thError = helper.trim_radian(thRef-th)

        # If the angle is close to enough, stop the robot (or else it would drive slightly forward for some reason)
        #if (abs(thError)< 0.05):
         #   return 0, 0, thError, distError

    cmToM = pow(10, -3)

    # Rule base for right wheel 
    ruleR[0] = strength(distError, NBd, thError, NBth) 
    outR[0] = -35 * cmToM
    ruleR[1] = strength(distError, NBd, thError, NMth)
    outR[1] = -25 * cmToM
    ruleR[2] = strength(distError, NBd, thError, NSth)
    outR[2] = -15 * cmToM
    ruleR[3] = strength(distError, NBd, thError, ZOth)
    outR[3] = 30 * cmToM
    ruleR[4] = strength(distError, NBd, thError, PSth)
    outR[4] = 15 * cmToM
    ruleR[5] = strength(distError, NBd, thError, PMth)
    outR[5] = 25 * cmToM
    ruleR[6] = strength(distError, NBd, thError, PBth)
    outR[6] = 35 * cmToM
    ruleR[7] = strength(distError, NMd, thError, NBth)
    outR[7] = -27 * cmToM
    ruleR[8] = strength(distError, NMd, thError, NMth)
    outR[8] = 8 * cmToM
    ruleR[9] = strength(distError, NMd, thError, NSth)
    outR[9] = 15 * cmToM
    ruleR[10]= strength(distError, NMd, thError, ZOth)
    outR[10]= 30 * cmToM
    ruleR[11]= strength(distError, NMd, thError, PSth)
    outR[11]= 40 * cmToM
    ruleR[12]= strength(distError, NMd, thError, PMth)
    outR[12]= 51 * cmToM
    ruleR[13]= strength(distError, NMd, thError, PBth)
    outR[13]= 63 * cmToM
    ruleR[14]= strength(distError, NSd, thError, NBth)
    outR[14]= -27 * cmToM
    ruleR[15]= strength(distError, NSd, thError, NMth)
    outR[15]= 8 * cmToM
    ruleR[16]= strength(distError, NSd, thError, NSth)
    outR[16]= 22 * cmToM
    ruleR[17]= strength(distError, NSd, thError, ZOth)
    outR[17]= 50 * cmToM
    ruleR[18]= strength(distError, NSd, thError, PSth)
    outR[18]= 44 * cmToM
    ruleR[19]= strength(distError, NSd, thError, PMth)
    outR[19]= 51 * cmToM
    ruleR[20]= strength(distError, NSd, thError, PBth)
    outR[20]= 63 * cmToM
    ruleR[21]= strength(distError, ZOd, thError, NBth)
    outR[21]= -3 * cmToM
    ruleR[22]= strength(distError, ZOd, thError, NMth)
    outR[22]= 18 * cmToM
    ruleR[23]= strength(distError, ZOd, thError, NSth)
    outR[23]= 35 * cmToM
    ruleR[24]= strength(distError, ZOd, thError, ZOth)
    outR[24]= 60 * cmToM
    ruleR[25]= strength(distError, ZOd, thError, PSth)
    outR[25]= 65 * cmToM
    ruleR[26]= strength(distError, ZOd, thError, PMth)
    outR[26]= 61 * cmToM
    ruleR[27]= strength(distError, ZOd, thError, PBth)
    outR[27]= 67 * cmToM
    ruleR[28]= strength(distError, PSd, thError, NBth)
    outR[28]= -3 * cmToM
    ruleR[29]= strength(distError, PSd, thError, NMth)
    outR[29]= 31 * cmToM
    ruleR[30]= strength(distError, PSd, thError, NSth)
    outR[30]= 57 * cmToM
    ruleR[31]= strength(distError, PSd, thError, ZOth)
    outR[31]= 90 * cmToM
    ruleR[32]= strength(distError, PSd, thError, PSth)
    outR[32]= 82 * cmToM
    ruleR[33]= strength(distError, PSd, thError, PMth)
    outR[33]= 68 * cmToM
    ruleR[34]= strength(distError, PSd, thError, PBth)
    outR[34]= 67 * cmToM
    ruleR[35]= strength(distError, PMd, thError, NBth)
    outR[35]= -3 * cmToM
    ruleR[36]= strength(distError, PMd, thError, NMth)
    outR[36]= 31 * cmToM
    ruleR[37]= strength(distError, PMd, thError, NSth)
    outR[37]= 67 * cmToM
    ruleR[38]= strength(distError, PMd, thError, ZOth)
    outR[38]= 100 * cmToM
    ruleR[39]= strength(distError, PMd, thError, PSth)
    outR[39]= 92 * cmToM
    ruleR[40]= strength(distError, PMd, thError, PMth)
    outR[40]= 68 * cmToM
    ruleR[41]= strength(distError, PMd, thError, PBth)
    outR[41]= 67 * cmToM
    ruleR[42]= strength(distError, PBd, thError, NBth)
    outR[42]= -3 * cmToM
    ruleR[43]= strength(distError, PBd, thError, NMth)
    outR[43]= 42 * cmToM
    ruleR[44]= strength(distError, PBd, thError, NSth)
    outR[44]= 67 * cmToM
    ruleR[45]= strength(distError, PBd, thError, ZOth)
    outR[45]= 100 * cmToM
    ruleR[46]= strength(distError, PBd, thError, PSth)
    outR[46]= 92 * cmToM
    ruleR[47]= strength(distError, PBd, thError, PMth)
    outR[47]= 77 * cmToM
    ruleR[48]= strength(distError, PBd, thError, PBth)
    outR[48]= 67 * cmToM 

    # Rule base for left wheel (equal to flipped rule base of right wheel for thError = 0)
    ruleL[0] = strength(distError, NBd, thError, NBth) 
    outL[0] = 35 * cmToM
    ruleL[1] = strength(distError, NBd, thError, NMth)
    outL[1] = 25 * cmToM
    ruleL[2] = strength(distError, NBd, thError, NSth)
    outL[2] = 15 * cmToM
    ruleL[3] = strength(distError, NBd, thError, ZOth)
    outL[3] = 30 * cmToM
    ruleL[4] = strength(distError, NBd, thError, PSth)
    outL[4] = -15 * cmToM
    ruleL[5] = strength(distError, NBd, thError, PMth)
    outL[5] = -25 * cmToM
    ruleL[6] = strength(distError, NBd, thError, PBth)
    outL[6] = -35 * cmToM
    ruleL[7] = strength(distError, NMd, thError, NBth)
    outL[7] = 63 * cmToM
    ruleL[8] = strength(distError, NMd, thError, NMth)
    outL[8] = 51 * cmToM
    ruleL[9] = strength(distError, NMd, thError, NSth)
    outL[9] = 40 * cmToM
    ruleL[10]= strength(distError, NMd, thError, ZOth)
    outL[10]= 30 * cmToM
    ruleL[11]= strength(distError, NMd, thError, PSth)
    outL[11]= 15 * cmToM
    ruleL[12]= strength(distError, NMd, thError, PMth)
    outL[12]=  8 * cmToM
    ruleL[13]= strength(distError, NMd, thError, PBth)
    outL[13]= -27 * cmToM
    ruleL[14]= strength(distError, NSd, thError, NBth)
    outL[14]= 63 * cmToM
    ruleL[15]= strength(distError, NSd, thError, NMth)
    outL[15]= 51 * cmToM
    ruleL[16]= strength(distError, NSd, thError, NSth)
    outL[16]= 44 * cmToM
    ruleL[17]= strength(distError, NSd, thError, ZOth)
    outL[17]= 50 * cmToM
    ruleL[18]= strength(distError, NSd, thError, PSth)
    outL[18]= 22 * cmToM
    ruleL[19]= strength(distError, NSd, thError, PMth)
    outL[19]= 8 * cmToM
    ruleL[20]= strength(distError, NSd, thError, PBth)
    outL[20]= -27 * cmToM
    ruleL[21]= strength(distError, ZOd, thError, NBth)
    outL[21]= 67 * cmToM
    ruleL[22]= strength(distError, ZOd, thError, NMth)
    outL[22]= 61 * cmToM
    ruleL[23]= strength(distError, ZOd, thError, NSth)
    outL[23]= 65 * cmToM
    ruleL[24]= strength(distError, ZOd, thError, ZOth)
    outL[24]= 60 * cmToM
    ruleL[25]= strength(distError, ZOd, thError, PSth)
    outL[25]= 35 * cmToM
    ruleL[26]= strength(distError, ZOd, thError, PMth)
    outL[26]= 18 * cmToM
    ruleL[27]= strength(distError, ZOd, thError, PBth)
    outL[27]= -3 * cmToM
    ruleL[28]= strength(distError, PSd, thError, NBth)
    outL[28]= 67 * cmToM
    ruleL[29]= strength(distError, PSd, thError, NMth)
    outL[29]= 68 * cmToM
    ruleL[30]= strength(distError, PSd, thError, NSth)
    outL[30]= 82 * cmToM
    ruleL[31]= strength(distError, PSd, thError, ZOth)
    outL[31]= 90 * cmToM
    ruleL[32]= strength(distError, PSd, thError, PSth)
    outL[32]= 57 * cmToM
    ruleL[33]= strength(distError, PSd, thError, PMth)
    outL[33]= 31 * cmToM
    ruleL[34]= strength(distError, PSd, thError, PBth)
    outL[34]= -3 * cmToM
    ruleL[35]= strength(distError, PMd, thError, NBth)
    outL[35]= 67 * cmToM
    ruleL[36]= strength(distError, PMd, thError, NMth)
    outL[36]= 68 * cmToM
    ruleL[37]= strength(distError, PMd, thError, NSth)
    outL[37]= 92 * cmToM
    ruleL[38]= strength(distError, PMd, thError, ZOth)
    outL[38]= 100 * cmToM
    ruleL[39]= strength(distError, PMd, thError, PSth)
    outL[39]= 67 * cmToM
    ruleL[40]= strength(distError, PMd, thError, PMth)
    outL[40]= 31 * cmToM
    ruleL[41]= strength(distError, PMd, thError, PBth)
    outL[41]= -3 * cmToM
    ruleL[42]= strength(distError, PBd, thError, NBth)
    outL[42]= 67 * cmToM
    ruleL[43]= strength(distError, PBd, thError, NMth)
    outL[43]= 77 * cmToM
    ruleL[44]= strength(distError, PBd, thError, NSth)
    outL[44]= 92 * cmToM
    ruleL[45]= strength(distError, PBd, thError, ZOth)
    outL[45]= 100 * cmToM
    ruleL[46]= strength(distError, PBd, thError, PSth)
    outL[46]= 67 * cmToM
    ruleL[47]= strength(distError, PBd, thError, PMth)
    outL[47]= 42 * cmToM
    ruleL[48]= strength(distError, PBd, thError, PBth)
    outL[48]= -3 * cmToM 

    return Ku*defuzzy(ruleL, outL), Ku*defuzzy(ruleR, outR), thError, distError
