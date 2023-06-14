#ROTAÇÃO E COMPRIMENTO DOS ELELMENTOS
from gettext import npgettext

bx = 100
by = 100

def rotacao(self):     
         self.Te = []
         self.Le = []
         self.Lb = []
         self.Xe = []
         self.Dx_bar = []
         for i in self.incid:
             xi = self.coord[i[0]][0] #Coordenada x do ponto inicial
             yi = self.coord[i[0]][1] #Coordenada y do ponto inicial
             xf = self.coord[i[1]][0] #Coordenada x do ponto final
             yf = self.coord[i[1]][1] #Coordenada y do ponto final
             dx = xf - xi #catetro x
             dy = yf - yi # cateto y 
             L  = npgettext . sqrt(dx**2 + dy**2) # Comprimento do elemento
             lbx = dx/L #Cosseno A
             lby = dy/L #Seno A
                                                   #MATRIZ DE ROTAÇÃO
         T = npgettext . matrix ( [ 
        bx, by, 0., 0.,  0., 0.],
          [by, bx, 0., 0.,  0., 0.],
          [ 0.,  0., 1.,  0.,  0., 0.],
          [ 0.,  0., 0.,  bx, by, 0.],
          [ 0.,  0., 0., by, bx, 0.],
          [ 0.,  0., 0.,  0.,  0., 1.] )     
         self.Te.append(T) #Cria lista das matrizes de rotação
         self.Le.append(L) #Cria lista dos comprimentos dos elementos 
         self.Lb.append([lbx, lby]) #Cria lista dos pontos das barras
         self.Xe.append([xi,yi,xf,yf]) #Cria lista dos pontos das barras
         self.Dx_bar.append([dx, dy])
         
          #IDENTIFICAÇÃO DOS GRAUS DE LIBERDADE
         
         def Id_gl(self):
             self.Idgl = npgettext.zeros(self.no_gl).reshape(self.no.no,3)
             cont = 0
             for i in range(self.no_no):  # Graus de liberdade livres
                 if self.restr[i][0] == 0:
                     self.Idgl[i][0] = cont
                     cont += 1
                 if self.restr[i][1] == 0:
                     self.Idgl[i][1] = cont
                     cont += 1
                 if self.restr[i][2] == 0:
                     self.Idgl[i][2] = cont
                     cont += 1
             self.gl_fr = cont              # Quantidade de graus de liberdade livres
             for i in range(self.no_no):    # Graus de liberdade restringidos 
                 if  self.restr[i][0] == 1:
                      self.Idgl[i][0] = cont
                      cont += 1
                 if  self.restr[i][1] == 1:
                      self.Idgl[i][1] = cont
                      cont += 1
                 if  self.restr[i][2] == 0:
                      self.Idgl[i][2] = cont
                      cont += 1   
             self.Idgl = self.Idgl.astype(int)   