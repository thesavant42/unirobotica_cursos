import math
from collections import namedtuple
import numpy as np
import json
from commons.math import distance_between_points as distance

Obstacle = namedtuple("obstacle", "center radius margin eradius")

def angle_between(p1, p2):
    y = p2[1] - p1[1]
    x = p2[0] - p1[0]
    ang = np.arctan2(y, x)

    return ang

def reduce_angle(ang):
    while ang > math.pi:
        ang -= 2 * math.pi
    while ang < -math.pi:
        ang += 2 * math.pi
    return ang

np_reduce_angle = np.vectorize(reduce_angle)

class UnivectorField:
    """
    An implementation of the uni vector field path planning algotithm

    The UnivectorField will generate vector that guides a given point to the target so that it gets there with an angle
    directed at the guiding point. This algorithm is also capable of contouring obstacles and generating a rectangle
    behind the target where the vectors have the save directions as the target-guide vector.
    """

    def __init__(self, n, rect_size, plot=False, path=''):
        """Inits UnivectorField class.

            Parameters
            ----------
                n: Constant the related to how much the path will avoid hitting the target from the wrong direction
                rect_size: the base side size of the rectangle behind the target where all the vectors are the same
                           angle of the target-guide line
        """
        self.obstacles = []
        self.N = n
        self.delta_g = rect_size

        self.plot = plot
        self.path = path

    def set_target(self, g, r):
        """
        Defines the target position and a guiding point

            Parameters
            ----------
                g (tuple[float, float]): target x and y coordinates
                r (tuple[float, float]): guide point x and y coordinates
        """

        self.g = g
        self.r = r

    def add_obstacle(self, p, r0, m):
        """
        Add one obstacle

            Parameters
            ----------
                p (tuple[float, float]): obstacle center x and y coordinates
                r0 (float): obstacle radius
                m (float): obstacle margin (distance the path will avoid)

            Return
            ----------
                obstacle (obstacle): the obstacle object
        """

        self.obstacles.append(Obstacle(p, r0, m, m + r0))
        return self.obstacles[-1]

    def del_obstacle(self, *args, all=False):
        """
        Delete any amount of obstacles

            Parameters
            ----------
                *args (list[obstacles]): one or more obstacle to be deleted
                all (bool): whether to delete all obstacles
        """
        if all:
            self.obstacles = []
            return

        for obstacle in args:
            self.obstacles.pop(obstacle)

    def save(self):
        if not self.plot:
            return

        out = {
            "n": self.N,
            "g": list(self.g),
            "r": list(self.r),
            "num_obstacles": len(self.obstacles),
            "obstacles": [
                {'center': list(obstacle.center), 'r0': obstacle.radius, 'm': obstacle.margin} for obstacle in
                self.obstacles
            ]
        }

        with open(self.path, 'w') as file:
            json.dump(out, file)

    def __call__(self, p):
        return self.compute(p)

    def compute(self, p):
        """
        Calculate the angle for the given position

            Parameters
            ----------
                p (tuple[float, float]): the position for which the angle will be calculated

            Return
            ----------
                angle (float): the angle of the vector in the field at the given position
        """
        behind_angle = None

        ang_pr = angle_between(p, self.r)
        ang_pg = angle_between(p, self.g)
        ang_rg = angle_between(self.g, self.r)

        phi = ang_pr - ang_pg
        phi = reduce_angle(phi)
        angle_f_p = ang_pg - self.N * phi
        angle_f_p = reduce_angle(angle_f_p)

        # check if the position is inside the rectangle behind obstacle
        j = ang_rg + math.pi * .5
        g1 = (self.r[0] + self.delta_g * .5 * math.cos(j), self.r[1] + self.delta_g * 0.5 * math.sin(j))
        g2 = (self.r[0] - self.delta_g * .5 * math.cos(j), self.r[1] - self.delta_g * 0.5 * math.sin(j))

        r_g1 = {'a': math.tan(ang_rg),
                'b': -1,
                'c': g1[1] - math.tan(ang_rg) * g1[0]}
        r_g2 =  {'a': math.tan(ang_rg),
                'b': -1,
                'c': g2[1] - math.tan(ang_rg) * g2[0]}
        d_pg1 = abs(r_g1['a']*p[0] + r_g1['b']*p[1] + r_g1['c']) / (r_g1['a']**2 + r_g1['b']**2)**.5
        d_pg2 = abs(r_g2['a']*p[0] + r_g2['b']*p[1] + r_g2['c']) / (r_g2['a']**2 + r_g2['b']**2)**.5

        if d_pg1 < self.delta_g and d_pg2 < self.delta_g:
            if distance(self.r, p) >= distance(p, self.g):
                angle_f_p = ang_rg

        for obstacle in self.obstacles:

            # check if the position is inside the margin of the obstacle
            if obstacle.margin + obstacle.radius >= distance(obstacle.center, p):
                margin_ang = (2 * angle_between(obstacle.center, p) + angle_f_p) / 3
                margin_ang = reduce_angle(margin_ang)
                if abs(angle_between(obstacle.center, p) - angle_f_p) > math.pi:
                    margin_ang = reduce_angle(margin_ang + math.pi)
                return margin_ang

            # check if the line pg is secant to the obstacle
            a = p[1] - self.g[1]
            b = self.g[0] - p[0]
            c = p[0] * self.g[1] - p[1] * self.g[0]
            if obstacle.eradius >= np.abs(a * obstacle.center[0] + b * obstacle.center[1] + c) / distance((a, b),
                                                                                                          (0, 0)):

                # check if p is behind the obstacle
                if distance(self.g, obstacle.center) <= distance(self.g, p) and distance(self.g, p) > distance(p,
                                                                                                               obstacle.center):
                    # check if the obstacle is in the way of f(p)
                    ang_t1 = angle_between(p, obstacle.center) + np.arctan(
                        obstacle.eradius / distance(p, obstacle.center))
                    ang_t2 = angle_between(p, obstacle.center) - np.arctan(
                        obstacle.eradius / distance(p, obstacle.center))

                    if ang_t1 > angle_f_p > ang_t2:
                        if angle_f_p > angle_between(p, obstacle.center):
                            behind_angle = ang_t1
                            pass
                        else:
                            behind_angle = ang_t2

        if behind_angle is not None:
            return behind_angle
            pass
        angle_f_p = np_reduce_angle(angle_f_p)
        return angle_f_p

    @classmethod
    def from_file(cls, path):
        data = None
        while data is None:
            with open(path, 'r') as file:
                try:
                    data = json.load(file)
                except:
                    pass

        uvf = cls(data['n'], .07)
        uvf.set_target(data['g'], data['r'])
        for obstacle in data['obstacles']:
            uvf.add_obstacle(obstacle['center'], obstacle['r0'], obstacle['m'])

        return uvf

if __name__ == "__main__":
    import matplotlib.pyplot as plt

    uvf = UnivectorField(n=4, rect_size=(delta:=2), plot=True, path='../../uvf_plot.json')

    uvf.add_obstacle((1, 1), 0.075 * 1.4 * 0.5, 0.075 * 1.4 * 0.25)
    uvf.add_obstacle((.7, .65), 0.075 * 1.4 * 0.5, 0.075 * 1.4 * 0.25)
    uvf.add_obstacle((.4, .8), 0.075 * 1.4 * 0.5, 0.075 * 1.4 * 0.25)
    uvf.add_obstacle((1, 1.2), 0.075 * 1.4 * 0.5, 0.075 * 1.4 * 0.25)

    g = np.array([.8, 0.65])
    r = np.array((1.5, 1.3))
    uvf.set_target(g, r)

    xs, ys = np.meshgrid(np.linspace(-5, 5, 53), np.linspace(-5, 5, 53))
    us, vs = [], []

    for x, y in zip(np.nditer(xs), np.nditer(ys)):
        ang = uvf((x, y))

        u = math.cos(ang)
        v = math.sin(ang)

        us.append(u)
        vs.append(v)
    us = np.array(us).reshape(xs.shape)
    vs = np.array(vs).reshape(ys.shape)

    uvf.save()

    fig, ax = plt.subplots()
    plt.quiver(xs, ys, us, vs)
    for obstacle in uvf.obstacles:
        ax.add_patch(plt.Circle(obstacle.center, obstacle.radius, color='r', fill=True))
        ax.add_patch(plt.Circle(obstacle.center, obstacle.eradius, color='r', fill=False))

    plt.plot(*g, 'rx')
    plt.plot(*r, 'bo')
    j = (ang_rg:=angle_between(g, r)) + math.pi * .5
    g1 = (g[0] + delta * .5 * math.cos(j), g[1] + delta * 0.5 * math.sin(j))
    g2 = (g[0] - delta * .5 * math.cos(j), g[1] - delta * 0.5 * math.sin(j))

    r_g1 = {'a': math.tan(ang_rg),
            'b': -1,
            'c': g1[1] - math.tan(ang_rg) * g1[0]}
    r_g2 = {'a': math.tan(ang_rg),
            'b': -1,
            'c': g2[1] - math.tan(ang_rg) * g2[0]}

    print(r_g1)

    x = np.linspace(-5, 5, 2)
    print(x)
    y_g1 = -(r_g1['a']*x+r_g1['c'])/r_g1['b']
    y_g2 = -(r_g2['a']*x+r_g2['c'])/r_g2['b']
    print(y_g2)

    plt.plot(x, y_g2)
    plt.plot(x, y_g1)

    plt.plot(*g1, 'go')
    plt.plot(*g2, 'go')
    ax.set_box_aspect(1)
    plt.show()