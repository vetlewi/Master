from numpy import *

class StoppingPowerFile:
    def __init__(self, filename):
        infile = open(filename, 'r')
        lines = infile.readlines()
        infile.close()
        data = []
        for line in lines: data.append(line.split())

        self.x = array([float(data[i][0]) for i in range(len(data))])
        self.y = array([float(data[i][1]) for i in range(len(data))])
        
    def __call__(self, E, factor):
        return factor*interp(E, self.x, self.y, left=1e16, right=self.y[-1])

    def Loss(self, E, width, n):
        try:
            if width == 0: return copy(E)
        
            Eafter = copy(E)
            dx = width/float(n-1)
            for i in range(0, n):
                R1 = dx*self(Eafter, -1)
                R2 = dx*self(Eafter + 0.5*R1, -1)
                R3 = dx*self(Eafter + 0.5*R2, -1)
                R4 = dx*self(Eafter + R3, -1)
                Eafter += (R1 + 2*R2 + 2*R3 + R4)/6.0
            Eafter[Eafter<0] = 0.
            return Eafter
        except:
            if width == 0: E

            Eafter = E
            dx = width/float(n-1)
            for i in range(0, n):
                R1 = dx*self(Eafter, -1)
                R2 = dx*self(Eafter + 0.5*R1, -1)
                R3 = dx*self(Eafter + 0.5*R2, -1)
                R4 = dx*self(Eafter + R3, -1)
                Eafter += (R1 + 2*R2 + 2*R3 + R4)/6.0
            if Eafter < 0:
                return 0.
            return Eafter
    def Gain(self, E, width, n):
        try:
            if width == 0: return copy(E)        

            Eafter = copy(E)
            dx = width/float(n-1)
            for i in range(0, n):
                R1 = dx*self(Eafter, 1)
                R2 = dx*self(Eafter + 0.5*R1, 1)
                R3 = dx*self(Eafter + 0.5*R2, 1)
                R4 = dx*self(Eafter + R3, 1)
                Eafter += (R1 + 2*R2 + 2*R3 + R4)/6.0
            Eafter[Eafter<0] = 0.
            return Eafter
        except:
            if width == 0: E
        
            Eafter = E
            dx = width/float(n-1)
            for i in range(0, n):
                R1 = dx*self(Eafter, 1)
                R2 = dx*self(Eafter + 0.5*R1, 1)
                R3 = dx*self(Eafter + 0.5*R2, 1)
                R4 = dx*self(Eafter + R3, 1)
                Eafter += (R1 + 2*R2 + 2*R3 + R4)/6.0

            if Eafter < 0:
                return 0.
            return Eafter


class Excitation:
    def __init__(self, m1, m2, m3, m4):

        self.m1 = m1
        self.m2 = m2
        self.m3 = m3
        self.m4 = m4

        self.s0 = (m1 + m2)**2

    def __call__(self, Tk, T3, theta3):
        
        m1 = self.m1
        m2 = self.m2
        m3 = self.m3
        m4 = self.m4
        s = self.s0 + 2*m1*Tk
        
        pcm = sqrt(((s - m1**2 - m2**2)**2 - (2*m1*m2)**2)/(4*s))
        chi = log((pcm + sqrt(pcm**2 + m1**2))/m1)

        p3 = sqrt(T3**2 + 2*m3*T3)
        
        ex = s + m3**2
        ex -= 2*sqrt(s)*(sqrt(m3**2 + p3**2)*cosh(chi) - p3*cos(theta3)*sinh(chi))
        ex = sqrt(ex) - m4
        return ex
    
    def FindT3(self, Tk, Ex, theta3):
        
        m4ex = self.m4 + Ex
        s = self.s0 + 2*self.m1*Tk
        
        pcm = sqrt(((s - self.m1**2 - self.m2**2)**2 - (2*self.m1*self.m2)**2)/(4*s))
        chi = log((pcm + sqrt(pcm**2 + self.m1**2))/self.m1)
        
        pcmPrime = sqrt(((s - self.m3**2 - m4ex**2)**2 - (2*self.m3*m4ex)**2)/(4*s))
        
        p3 = sqrt(pcmPrime**2 + self.m3**2)*cos(theta3)*sinh(chi)
        p3 += sqrt(pcmPrime**2 - (self.m3*sin(theta3)*sinh(chi))**2)*cosh(chi)
        p3 /= 1 + (sin(theta3)*sinh(chi))**2
        
        return sqrt(p3**2 + self.m3**2) - self.m3
    
    def maxEx(self, Tk):

        m1 = self.m1
        m2 = self.m2
        m3 = self.m3
        m4 = self.m4
        s = self.s0 + 2*m1*Tk
        
        pcm = sqrt(((s - m1**2 - m2**2)**2 - (2*m1*m2)**2)/(4*s))
        chi = log((pcm + sqrt(pcm**2 + m1**2))/m1)
        
        return sqrt(s - 2*sqrt(s)*m3 + m3**2) - m4


    def Uncertainty(self, T2, T3, theta3, dT2, dT3, dtheta3):
        
        m1 = self.m1
        m2 = self.m2
        m3 = self.m3
        s = self.s0 + 2*m1*T2
        
        pcm = sqrt(((s - m1**2 - m2**2)**2 - (2*m1*m2)**2)/(4*s))
        chi = log((pcm + sqrt(m1**2 + pcm**2))/m1)
        
        p2 = sqrt(T2**2 + 2*m2*T2)
        p3 = sqrt(T3**2 + 2*m3*T3)
        
        E3cm = sqrt(p3**2 + m3**2)*cosh(chi) - p3*cos(theta3)*sinh(chi) 
        
        M4 = sqrt(s - 2*sqrt(s)*E3cm + m3**2)
        
        dET2 = (m1 - T3 - m3 + (T2 + m2)*p3*cos(theta3)/p2)/M4
        
        dET3 = ((T3 + m3)*p2*cos(theta3)/p3 - T2 - m1 - m2)/M4
        
        dETh = p2*p3*sin(theta3)/M4
        
        dEx2 = (dET2*dT2)**2 + (dET3*dT3)**2 + (dETh*dtheta3)**2
        
        return sqrt(dEx2)

class Telescope:
    def __init__(self, m1, m2, m3, m4, T2, tWidth, tStopP, tStopF, aWidth, aStop):

        self.m1 = m1
        self.m2 = m2
        self.m3 = m3
        self.m4 = m4
        self.T2 = T2
        self.tWidth = tWidth
        self.aWidth = aWidth
        self.T2 = T2
        
        self.stopPT = StoppingPowerFile(tStopP)
        self.stopFT = StoppingPowerFile(tStopF)
        self.stopA = StoppingPowerFile(aStop)
        
        self.Tk = self.stopPT.Loss(self.T2, self.tWidth/2., 1001)
        self.dTk = 0.5*(self.T2 - self.stopPT.Loss(self.T2, self.tWidth, 1001))
        
        self.excitation = Excitation(self.m1, self.m2, self.m3, self.m4)
        
        self.ExMax = self.excitation.maxEx(self.Tk)
    
    def theta3(self, i, j):
        x = (6.0 - (i-17./2.)*0.3)/sqrt(2)
        y = (6.0 + (i-17./2.)*0.3)/sqrt(2)
        z = (j - 17./2.)*0.3
        return arccos(y/sqrt(x**2 + y**2 + z**2))
    
    def dtheta3(self, i, j):
        
        x1 = (6.0 - (i - 9.)*0.3)/sqrt(2)
        y1 = (6.0 + (i - 9.)*0.3)/sqrt(2)
        z1 = (j - 9)*0.3    
        x2 = (6.0 - (i - 8)*0.3)/sqrt(2)
        y2 = (6.0 + (i - 8)*0.3)/sqrt(2)
        z2 = (j - 8)*0.3
        
        thMax, thMin = 0., 0.

        if j <= 8:
            thMax = arccos(y1/sqrt(x1**2 + y1**2 + z1**2))
            thMin = arccos(y1/sqrt(x1**2 + y1**2 + z2**2))
        else:
            thMax = arccos(y1/sqrt(x1**2 + y1**2 + z2**2))
            thMin = arccos(y2/sqrt(x2**2 + y2**2 + z1**2))
        
        dtheta = 0.5*(thMax - thMin)
        
        return dtheta
    
    def phi3(self, i, j):
        x = (6.0 - (i-17./2.)*0.3)/sqrt(2)
        y = (6.0 + (i-17./2.)*0.3)/sqrt(2)
        z = (j - 17./2.)*0.3
        return arccos((x+y)/sqrt(2*x**2 + 2*y**2 + 2*z**2))
    
    def Fwd_method(self, i, j):

        th3 = self.theta3(i,j)
        dth3 = self.dtheta3(i,j)
        ph3 = self.phi3(i,j)
        
        ex = linspace(0, self.ExMax, 1001)
        
        T3 = self.excitation.FindT3(self.Tk, ex, th3)
        
        dex = self.excitation.Uncertainty(self.Tk, T3, th3, self.dTk, 0.05*T3, dth3)        
        T3t = self.stopFT.Loss(T3, 0.5*self.tWidth/cos(th3), 1001)
        
        Tfinal = T3t
        if self.aWidth > 0:
            T3a = self.stopA.Loss(T3t, self.aWidth/cos(ph3), 1001)
            Tfinal = T3a
        
        Tfinal = Tfinal[dex==dex]
        ex = ex[dex==dex]
        dex = dex[dex==dex]
        
        return Tfinal, ex, dex
    
    def Bwd_method(self, i, j):
        th3 = self.theta3(i,j)
        dth3 = self.dtheta3(i,j)
        ph3 = self.phi3(i,j)
        
        T3max = self.excitation.FindT3(self.Tk, 0, th3)        
        T3max = self.stopFT.Loss(T3max, 0.5*self.tWidth/cos(th3), 1001)
        
        if self.aWidth > 0:
            T3max = self.stopA.Loss(T3max, self.aWidth/cos(ph3), 1001)
        
        T3a = linspace(0.2, T3max, 1001)        
        T3t = copy(T3a)

        if self.aWidth > 0:
            T3t = self.stopA.Gain(T3t, self.aWidth/cos(ph3), 1001)
        
        T3min = copy(T3t)
        
        T3 = self.stopFT.Gain(T3t, 0.5*self.tWidth/cos(th3), 1001)
        
        T3max = self.stopFT.Gain(T3t, self.tWidth/cos(th3), 1001)
        
        dT3 = 0.5*(T3max - T3min) + 0.05*T3
                
        ex = self.excitation(self.Tk, T3, th3)
        dex = self.excitation.Uncertainty(self.Tk, T3, th3, self.dTk, dT3, dth3)
        
        return T3a, ex, dex

    def MakeCoeff(self, i, j, Emin=5.0):

        T3, ex, dex = self.Fwd_method(i,j)
        
        T3 = T3[ex >= 0]
        dex = dex[ex >= 0]
        ex = ex[ex >= 0]
        
        ex = ex[T3>=Emin]
        dex = dex[T3>=Emin]
        T3 = T3[T3>=Emin]
        
        p = polyfit(T3, ex, 2, w=1/dex)
        
        return p[2], p[1], p[0]


Mp = 1.007276466879*931.494 # Mass proton [MeV/c^2]
Md = 2.01410178*931.494 # Mass deuteron [MeV/c^2]
M86Kr = 85.91061073*931.494 # Mass Krypton-86 [MeV/c^2]
M87Kr = 86.91335486*931.494 # Mass Krypton-87 [MeV/c^2]

T = 300. # Beam energy [MeV]

wT = 250. # Width of target [ug/cm^2]
wA = 52. # Width of absorber [mg/cm^2]

tel = Telescope(Md, M86Kr, Mp, M87Kr, T, wT, "StopKr_C2D4_ugcm2.txt", "StopP_C2D4_ugcm2.txt", wA, "StopP_Ta_mgcm2.txt")

outfile = open("output.txt", "w")

for i in range(1, 17):
    for j in range(1,17):
        a, b, c, chiSq = tel.MakeCoeff(i,j)
        outfile.write("%s %s %s %s %s\n" %(i, j, a, b, c) )

outfile.close()