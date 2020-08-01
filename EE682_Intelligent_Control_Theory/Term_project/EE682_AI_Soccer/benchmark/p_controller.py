
import math

def calculate_input(xRef,yRef,x,y,th):
    xError = xRef-x
    yError = yRef-y
    thRef = math.atan2(yError,xError)
    thError = thRef-th
    distError = math.sqrt(math.pow(xRef-x,2)+math.pow(yRef-y, 2))

    Kth = 0.2
    Kdist = 0.5

    vl = Kdist*distError-Kth*thError
    vr = Kdist*distError+Kth*thError

    return vl,vr,thError
